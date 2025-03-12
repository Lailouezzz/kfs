/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:53:35 by amassias          #+#    #+#             */
/*   Updated: 2025/03/12 18:23:10 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm/except.h>

// Mouse-specific commands
#define MOUSE_SET_SAMPLE (0xF3)
#define MOUSE_SET_RESOLUTION (0xE8)
#define MOUSE_ENABLE_SCALING (0xE7)
#define MOUSE_DISABLE_SCALING (0xE7)

// Bits of the first mouse data packet
#define MOUSE_Y_OVERFLOW (1 << 7)
#define MOUSE_X_OVERFLOW (1 << 6)
#define MOUSE_Y_NEG (1 << 5)
#define MOUSE_X_NEG (1 << 4)
#define MOUSE_ALWAYS_SET (1 << 3)
#define MOUSE_MIDDLE (1 << 2)
#define MOUSE_RIGHT (1 << 1)
#define MOUSE_LEFT (1 << 0)

// Bits of the fourth mouse data packet
#define MOUSE_UNUSED_A (1 << 7)
#define MOUSE_UNUSED_B (1 << 6)

typedef struct
{
	i32		x;
	i32		y;
	int		left_pressed;
	int		right_pressed;
	int		middle_pressed;
}	mouse_t;

typedef void (*mouse_callback_t)(mouse_t);

void	init_mouse(u32 device);
void	mouse_set_callback(mouse_callback_t cb);

int		mouse_get_x(void);
int		mouse_get_y(void);
int		mouse_left_button(void);
int		mouse_right_button(void);
