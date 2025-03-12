/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 18:00:15 by amassias          #+#    #+#             */
/*   Updated: 2025/03/12 19:42:18 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //

#include <asm/ps2.h>
#include <asm/mouse.h>
#include <asm/idt.h>
#include <asm/irq.h>
#include <kfs/kernel.h>

// ************************************************************************** //
// *                                                                        * //
// * Defines                                                                * //
// *                                                                        * //
// ************************************************************************** //

#define LOG(fmt, ...) printk("<PS2 MS DRIVER> " fmt "\n" __VA_OPT__(,) __VA_ARGS__)

// ************************************************************************** //
// *                                                                        * //
// * Helper prototypes                                                      * //
// *                                                                        * //
// ************************************************************************** //

static void	_handle_packet(void);
static void	_handle_interrupt(unsigned int n, int_regs_s *regs);
static void	_set_sample_rate(u8 rate);
static void	_set_resolution(u8 level);
static void	_set_scaling(int enabled);
static void	_enable_scroll_wheel(void);
static void	_enable_five_buttons(void);

// ************************************************************************** //
// *                                                                        * //
// * Globals                                                                * //
// *                                                                        * //
// ************************************************************************** //

u32					current_byte = 0;
u32					bytes_per_packet = 3;
u8					packet[4] = { 0 };
u32					device;
mouse_t				state;
mouse_callback_t	callback;

// ************************************************************************** //
// *                                                                        * //
// * Header definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

void	init_mouse(u32 _device)
{
	LOG("Initializing %d", _device);
	device = _device;
	request_irq(12, _handle_interrupt);

	// Enable features
	_enable_scroll_wheel();
	_enable_five_buttons();

	// Set mouse parameters
	_set_sample_rate(80);
	_set_resolution(0x00); // One unit per mm
	_set_scaling(0);       // Disable acceleration

	// Start receiving IRQ12s
	ps2_write_device(device, PS2_DEV_ENABLE_SCAN);
	ps2_expect_ack();
}

void	mouse_set_callback(mouse_callback_t cb)
{
	callback = cb;
}

int	mouse_get_x(void)
{
	return (state.x);
}
int	mouse_get_y(void)
{
	return (state.y);
}
int	mouse_left_button(void)
{
	return (state.left_pressed);
}
int	mouse_right_button(void)
{
	return (state.right_pressed);
}

// ************************************************************************** //
// *                                                                        * //
// * Helper definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

// Receives one byte from the mouse and packages it in `packet`. Once that
// packet is full, calls `_handle_packet`.
static
void	_handle_interrupt(unsigned int n, int_regs_s *regs)
{
	u8	byte;

	UNUSED(n);
	UNUSED(regs);
	byte = ps2_read(PS2_DATA);

	// Try to stay synchronized by discarding obviously out of place bytes
	if (current_byte == 0 && !(byte & MOUSE_ALWAYS_SET))
		return ;
	packet[current_byte] = byte;
	current_byte = (current_byte + 1) % bytes_per_packet;

	// We've received a full packet
	if (current_byte == 0)
		_handle_packet();
}

static
int	mouse_states_equal(mouse_t* a, mouse_t* b)
{
	return (a->x == b->x && a->y == b->y
		&& a->left_pressed == b->left_pressed
		&& a->middle_pressed == b->middle_pressed
		&& a->right_pressed == b->right_pressed);
}

static
void	_handle_packet(void)
{
	mouse_t	old_state;
	u32		delta_x;
	u32		delta_y;
	u8		flags;
	u8		extra;

	old_state = state;
	flags = packet[0];
	delta_x = (u32)packet[1];
	delta_y = (u32)packet[2];
	extra = 0;

	// Superior mice send four bytes
	if (bytes_per_packet == 4)
	{
		extra = packet[3];
		if (extra & MOUSE_UNUSED_A || extra & MOUSE_UNUSED_B)
			return; // Unused bits are set: beware
	}

	// Packets with X or Y overflow are probably garbage
	if (flags & MOUSE_X_OVERFLOW || flags & MOUSE_Y_OVERFLOW)
		return;

	// Two's complement by hand
	if (flags & MOUSE_X_NEG)
		delta_x |= 0xFFFFFF00;
	if (flags & MOUSE_Y_NEG)
		delta_y |= 0xFFFFFF00;
	state.left_pressed = flags & MOUSE_LEFT;
	state.right_pressed = flags & MOUSE_RIGHT;
	state.middle_pressed = flags & MOUSE_MIDDLE;
	state.x += delta_x;
	state.y -= delta_y; // Point the y-axis downward

	if (!mouse_states_equal(&old_state, &state) && callback)
		callback(state);
}

static
void	_set_sample_rate(u8 rate)
{
	ps2_write_device(device, MOUSE_SET_SAMPLE);
	ps2_expect_ack();
	ps2_write_device(device, rate);
	ps2_expect_ack();
}

static
void	_set_resolution(u8 level)
{
	ps2_write_device(device, MOUSE_SET_RESOLUTION);
	ps2_expect_ack();
	ps2_write_device(device, level);
	ps2_expect_ack();
}

static
void	_set_scaling(int enabled)
{
	u8	cmd;

	cmd = enabled ? MOUSE_ENABLE_SCALING : MOUSE_DISABLE_SCALING;
	ps2_write_device(device, cmd);
	ps2_expect_ack();
}

static
void	_enable_scroll_wheel(void)
{
	u32	type;

	_set_sample_rate(200);
	_set_sample_rate(100);
	_set_sample_rate(80);
	type = ps2_identify_device(device);
	if (type == PS2_MOUSE_SCROLL_WHEEL)
	{
		bytes_per_packet = 4;
		LOG("Enabled scroll wheel");
	}
	else
		LOG("Unable to enable scroll wheel");
}

static
void	_enable_five_buttons(void)
{
	u32	type;

	if (bytes_per_packet != 4)
		return;
	_set_sample_rate(200);
	_set_sample_rate(200);
	_set_sample_rate(80);
	type = ps2_identify_device(device);
	if (type != PS2_MOUSE_FIVE_BUTTONS)
		LOG("Mouse has fewer than five buttons");
	else
		LOG("Five buttons enabled");
}
