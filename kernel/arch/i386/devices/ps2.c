/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ps2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 13:26:50 by amassias          #+#    #+#             */
/*   Updated: 2025/03/12 18:09:07 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ************************************************************************** //
// *                                                                        * //
// * Includes                                                               * //
// *                                                                        * //
// ************************************************************************** //

#include <asm/ps2.h>
#include <asm/io.h>
#include <asm/keyboard.h>
#include <asm/mouse.h>
#include <kfs/kernel.h>

// ************************************************************************** //
// *                                                                        * //
// * Defines                                                                * //
// *                                                                        * //
// ************************************************************************** //

#define LOG(fmt, ...) printk("<PS2 DRIVER> "  fmt  "\n" __VA_OPT__(,) __VA_ARGS__)

// ************************************************************************** //
// *                                                                        * //
// * Helper functions                                                       * //
// *                                                                        * //
// ************************************************************************** //

static void	ps2_enable_port(u32 num, int enable);
static void	ps2_wait_ms(u32 ms);

// ************************************************************************** //
// *                                                                        * //
// * Globals                                                                * //
// *                                                                        * //
// ************************************************************************** //

static int	controllers[2] = {1, 1};

// ************************************************************************** //
// *                                                                        * //
// * Header definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

void	init_ps2(void)
{
	int	dual_channel;
	int	driver_called;
	u32	type;
	u8	config;
	u8	ret;
	u8	ret2;

	dual_channel = 1;
	LOG("Initializing PS/2 devices");
	__asm__("cli");

	// Disable both PS/2 device ports
	// Even if only one is present, disabling the second is harmless
	ps2_write(PS2_CMD, PS2_DISABLE_FIRST);
	ps2_write(PS2_CMD, PS2_DISABLE_SECOND);

	// Flush output bufffer: if the controller had anything to say, ignore it
	inb(PS2_DATA);

	// Get the controller configuration byte
	ps2_write(PS2_CMD, PS2_READ_CONFIG);
	config = ps2_read(PS2_DATA);
	config |= PS2_CFG_SYSTEM_FLAG;

	// Check the basics
	if (config & PS2_CFG_MUST_BE_ZERO)
		LOG("Invalid bit set in configuration byte");

	// Disable interrupts and scan code translation, through the configuration
	// byte
	config &= ~(PS2_CFG_FIRST_PORT | PS2_CFG_SECOND_PORT | PS2_CFG_TRANSLATION);
	ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
	ps2_write(PS2_DATA, config);

	// Controller self-test
	ps2_write(PS2_CMD, PS2_SELF_TEST);
	if (ps2_read(PS2_DATA) != PS2_SELF_TEST_OK)
	{
		LOG("Controller failed self-test");
		controllers[0] = 0;
		controllers[1] = 0;
		return;
	}

	// The last write may have reset our controller: better reset our
	// configuration byte just to be sure
	ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
	ps2_write(PS2_DATA, config);

	// Check if we _really_ have two channels
	ps2_write(PS2_CMD, PS2_ENABLE_SECOND);
	ps2_write(PS2_CMD, PS2_READ_CONFIG);
	config = ps2_read(PS2_DATA);
	if (config & PS2_CFG_SECOND_CLOCK)
	{
		LOG("Only one PS/2 controller");
		dual_channel = 0;
		controllers[1] = 0;
	}
	else
	{
		// Re-disable the second controller
		ps2_write(PS2_CMD, PS2_DISABLE_SECOND);
	}

	// Test the controllers
	ps2_write(PS2_CMD, PS2_TEST_FIRST);
	if (ps2_read(PS2_DATA) != PS2_TEST_OK)
	{
		LOG("First PS/2 port failed testing");
		controllers[0] = 0;
	}
	if (dual_channel)
	{
		ps2_write(PS2_CMD, PS2_TEST_SECOND);
		if (ps2_read(PS2_DATA) != PS2_TEST_OK)
		{
			LOG("Second PS/2 port failed testing");
			controllers[1] = 0;
		}
	}

	// Reset devices
	for (u8 i = 0; i < 2; i++)
	{
		if (!controllers[i])
			continue;

		// Enable the relevant port
		ps2_enable_port(i, 1);

		// Perform the reset
		LOG("Resetting device %d...", i);
		ps2_write_device(i, PS2_DEV_RESET);
		ps2_wait_ms(500);
		ps2_expect_ack();
		ps2_wait_ms(100);
		ret = ps2_read(PS2_DATA);

		// Keyboards ACK, mice (usually) ACK and send 0x00
		if (i == 0 && ret != PS2_DEV_RESET_ACK)
		{
			LOG("Keyboard failed to acknowledge reset, sent %x", ret);
			goto error;
		}
		else if (i == 1)
		{
			ret2 = ps2_read(PS2_DATA);

			// Mice are a complete mess, cut them some slack
			if ((ret == PS2_DEV_RESET_ACK && (ret2 == PS2_DEV_ACK || !ret2))
				|| ((ret == PS2_DEV_ACK || !ret) && ret2 == PS2_DEV_RESET_ACK))
			{
				// Wrong if for readability
			}
			else
			{
				LOG("Mice failed to acknowledge reset, sent %x, %x", ret, ret2);
				goto error;
			}
		}

		// Put the keyboard back to sleep so it doesn't send anything while the
		// mouse resets, and flush the input, because keyboards are a mess too.
		if (i == 0)
		{
			ps2_read(PS2_DATA);
			ps2_enable_port(i, 0);
		}
		continue;

	error:
		ps2_enable_port(i, 0);
		controllers[i] = 0;
	}

	// Reenable the keyboard port if it worked in the first place
	if (controllers[0])
		ps2_enable_port(0, 1);

	for (u8 i = 0; i < 2; i++)
	{
		if (!controllers[i])
			continue ;
		type = ps2_identify_device(i);
		driver_called = 1;

		switch (type)
		{
			case PS2_KEYBOARD:
			case PS2_KEYBOARD_TRANSLATED: init_keyboard(i); break;
			case PS2_MOUSE:
			case PS2_MOUSE_SCROLL_WHEEL:
			case PS2_MOUSE_FIVE_BUTTONS: init_mouse(i); break;
			default: driver_called = 0; break;
		}

		// Enable interrupts from the port; we write the new config just below
		if (driver_called)
		{
			config |= i == 0 ? PS2_CFG_FIRST_PORT : PS2_CFG_SECOND_PORT;
			config &= ~(i == 0 ? PS2_CFG_FIRST_CLOCK : PS2_CFG_SECOND_CLOCK);
		}
	}

	ps2_write(PS2_CMD, PS2_WRITE_CONFIG);
	ps2_write(PS2_DATA, config);

	__asm__("sti");
}

int	ps2_wait_read(void)
{
	int timer = PS2_TIMEOUT;
	while (!(inb(PS2_CMD) & 1) && timer-- >= 0)
		__asm__ ("pause");
	return timer != 0;
}

int	ps2_wait_write(void)
{
	int timer = PS2_TIMEOUT;

	while ((inb(PS2_CMD) & 2) && timer-- > 0)
		__asm__ ("pause");
	return (timer != 0);
}

u8	ps2_read(u32 port)
{
	if (ps2_wait_read())
		return (inb(port));
	printk("[PS2] Read failed\n");
	return (-1);
}

int	ps2_write(u32 port, u8 b)
{
	if (ps2_wait_write())
	{
		outb(b, port);
		return (1);
	}
	printk("[PS2] Write failed\n");
	return (0);
}

int	ps2_write_device(u32 device, u8 b)
{
	if (device != 0 && !ps2_write(PS2_CMD, PS2_WRITE_SECOND))
		return (0);
	return (ps2_write(PS2_DATA, b));
}

int	ps2_expect_ack(void)
{
	if (ps2_read(PS2_DATA) == PS2_DEV_ACK)
		return (1);
	printk("[PS2] Device failed to acknowledge command\n");
	return (0);
}

int	ps2_can_read(void)
{
	return (inb(PS2_CMD) & 1);
}

u32	ps2_identity_bytes_to_type(u8 first, u8 second)
{
	if (first == 0x00 || first == 0x03 || first == 0x04)
		return (first); // PS2_MOUSE* match the first byte
	else if (first == 0xAB)
	{
		if (second == 0x41 || second == 0xC1)
			return (PS2_KEYBOARD_TRANSLATED);
		else if (second == 0x83)
			return (PS2_KEYBOARD);
	}
	return (PS2_DEVICE_UNKNOWN);
}

u32	ps2_identify_device(u32 num)
{
	u32	first_id_byte;
	u32	second_id_byte;

	ps2_write_device(num, PS2_DEV_DISABLE_SCAN); // Disables scanning
	ps2_expect_ack();
	ps2_write_device(num, PS2_DEV_IDENTIFY); // Identify
	ps2_expect_ack();
	first_id_byte = ps2_read(PS2_DATA);
	second_id_byte = ps2_read(PS2_DATA);
	return (ps2_identity_bytes_to_type(first_id_byte, second_id_byte));
}

// ************************************************************************** //
// *                                                                        * //
// * Helper definitions                                                     * //
// *                                                                        * //
// ************************************************************************** //

static void	ps2_enable_port(u32 num, int enable)
{
	if (!controllers[num])
		return;
	if (!enable)
		ps2_write(PS2_CMD, num == 0 ? PS2_DISABLE_FIRST : PS2_DISABLE_SECOND);
	else
		ps2_write(PS2_CMD, num == 0 ? PS2_ENABLE_FIRST : PS2_ENABLE_SECOND);

	/* Some keyboards actually send a reply, flush it */
	ps2_read(PS2_DATA);
}

static void	ps2_wait_ms(u32 ms)
{
	ms *= 10000;
	while (--ms)
		__asm__ volatile ("pause");
}
