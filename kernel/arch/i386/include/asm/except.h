/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:05:29 by Antoine Mas       #+#    #+#             */
/*   Updated: 2025/03/12 17:00:05 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_AXCEPT_H
# define ASM_AXCEPT_H

# include <asm/memory.h>
# include <asm/types.h>

/*       NAME        |  VEC NUM  |  DESCRIPTION                               */
# define EXCEPT_DE_VEC        0  /* Divide error                              */
# define EXCEPT_DB_VEC        1  /* Debug exception                           */
# define EXCEPT_NMI_VEC       2  /* NMI interrupt                             */
# define EXCEPT_BP_VEC        3  /* Breakpoint                                */
# define EXCEPT_OF_VEC        4  /* Overflow                                  */
# define EXCEPT_BR_VEC        5  /* BOUND range exceeded                      */
# define EXCEPT_UD_VEC        6  /* Invalid opcode                            */
# define EXCEPT_NM_VEC        7  /* Device note available (no math coproc)    */
# define EXCEPT_DF_VEC        8  /* Double fault                              */
/* # define EXCEPT_VEC        9     Coprocessor segment overrune (reserved)   */
# define EXCEPT_TS_VEC       10  /* Invalid TSS                               */
# define EXCEPT_NP_VEC       11  /* Segment not present                       */
# define EXCEPT_SS_VEC       12  /* Stack segment fault                       */
# define EXCEPT_GP_VEC       13  /* General protection                        */
# define EXCEPT_PF_VEC       14  /* Page fault                                */
/* # define EXCEPT_VEC       15     Intel reserved                            */
# define EXCEPT_AC_VEC       17  /* Alignment check                           */
# define EXCEPT_MF_VEC       16  /* x87 FPU Floating-point error              */
# define EXCEPT_MC_VEC       18  /* Machine check                             */
# define EXCEPT_XM_VEC       19  /* SIMD Floating-point exception             */
# define EXCEPT_VE_VEC       20  /* Virtualization exception                  */
/*                           21     include is intel reserved                 */
/*                           32     user defined                              */

# ifndef __ASSEMBLY__

#  define PIC1						(0x20)	/* IO base address for master PIC */
#  define PIC2						(0xA0)	/* IO base address for slave PIC  */

#  define PIC1_COMMAND				PIC1
#  define PIC1_DATA					(PIC1+1)
#  define PIC2_COMMAND				PIC2
#  define PIC2_DATA					(PIC2+1)

#  define PIC_MASTER_VECTOR_OFFSET	(0x20)
#  define PIC_SLAVE_VECTOR_OFFSET	(0xA0)

#  define ICW1_ICW4					(0x01)	/* ICW4 will be present           */
#  define ICW1_SINGLE				(0x02)	/* Single (cascade) mode          */
#  define ICW1_INTERVAL4			(0x04)	/* Call address interval 4 (8)    */
#  define ICW1_LEVEL				(0x08)	/* Level triggered (edge) mode    */
#  define ICW1_INIT					(0x10)	/* Initialization - required!     */

#  define ICW4_8086					(0x01)	/* 8086/88 (MCS-80/85) mode       */
#  define ICW4_AUTO					(0x02)	/* Auto (normal) EOI              */
#  define ICW4_BUF_SLAVE			(0x08)	/* Buffered mode/slave            */
#  define ICW4_BUF_MASTER			(0x0C)	/* Buffered mode/master           */
#  define ICW4_SFNM					(0x10)	/* Special fully nested (not)     */

#  define HALT() for (;;) __asm__ volatile ("hlt")

#  define DUMP_STACK_FRAME(sf)								\
	{														\
		interrupt_stack_frame_t	*__asm__ptr__s__ = &(sf);	\
		__asm__ volatile (									\
			"pushl %%eax;"									\
			"movl %%edi, %%eax;"							\
			"movl %0, %%edi;"								\
			"movl %%eax, 24(%%edi);"						\
			"movl %%cr0, %%eax;"							\
			"movl %%eax,   (%%edi);"						\
			"movl %%cr3, %%eax;"							\
			"movl %%eax,  4(%%edi);"						\
			"popl %%eax;"									\
			"movl %%edx,  8(%%edi);"						\
			"movl %%ecx, 12(%%edi);"						\
			"movl %%ebx, 16(%%edi);"						\
			"movl %%eax, 20(%%edi);"						\
			"movl %%esi, 28(%%edi);"						\
			"movl %%ebp, 32(%%edi);"						\
			"movl %%esp, 36(%%edi);"						\
			:												\
			: "r"(__asm__ptr__s__)							\
			: "eax", "edi"									\
		);													\
	}

/* TODO : move this struct into other header */
typedef struct
{
	u32	cr0;
	u32	cr3;
	u32	edx;
	u32	ecx;
	u32	ebx;
	u32	eax;
	u32	edi;
	u32	esi;
	u32	ebp;
	u32	esp;
	u32	intnum;
	u32	errcode;
}	__PACKED__ interrupt_stack_frame_t;

typedef void (*exception_handler_t)(interrupt_stack_frame_t *);

void	irq_install(void);

void	irq_install_handler(int irq, exception_handler_t handler);
void	irq_uninstall_handler(int irq);

void	isr_install_handler(int isr, exception_handler_t handler);
void	isr_uninstall_handler(int isr);

# endif

#endif
