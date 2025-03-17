/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Antoine Massias <massias.antoine.pro@gm    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 21:56:25 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/17 17:29:13 by Antoine Mas      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <multiboot2.h>

#include <asm/idt.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/keyboard.h>
#include <asm/mouse.h>
#include <asm/ps2.h>
#include <asm/ptrace.h>
#include <kfs/kernel.h>
#include <kfs/shell.h>
#include <kfs/terminal.h>
#include <kfs/vga.h>

extern void	init_gdt(void);

static void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic);

struct multiboot_info *g_mb_info = NULL;

/**
 * @brief The arch main entry in the higher half
 *
 * @param mb_info multiboot2 info
 * @param magic multiboot magic
 */
void	arch_main(struct multiboot_info *mb_info, unsigned int magic)
{
	init_gdt();
	init_idt();
	init_irq();
	init_vga();
	init_terminal();
	check_multiboot(mb_info, magic);
	init_ps2();
	__asm__("sti");
	printk("Welcome to KFS !\n");
	__asm__ volatile (
		"push %eax;"
		"mov $7, %eax;"
		"int $0x80;"
		"pop %eax"
	);
	start_shell();
}

static void	check_multiboot(struct multiboot_info *mb_info, unsigned int magic)
{
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
		panic("No multiboot 2 header present (%x)", magic);
	g_mb_info = mb_info;
}
