#include <asm/irq.h>
#include <asm/except.h>

/******************************************************************************/
/*                                                                            */
/* Macros                                                                     */
/*                                                                            */
/******************************************************************************/

.macro SAVE_ALL
	pushl %ebp
	pushl %esi
	pushl %edi
	pushl %eax
	pushl %ebx
	pushl %ecx
	pushl %edx
.endm

.macro RESTORE_ALL
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	popl %edi
	popl %esi
	popl %ebp
.endm

// Low level default exception handler (NOERR)
.macro except_default_noerr name
	pushl $0
	pushl $default_except_handler
	jmp except_common
.endm

// Low level default exception handler (ERR)
.macro except_default_err
	pushl $default_except_handler
	jmp except_common
.endm

// Low level default exception handler (RESERVED)
.macro except_default_res name=0
except_\name:
	pushl $0
	pushl $default_except_handler
	jmp except_common
.endm

.macro interrupt_stub vector=0
__irq\vector:
	pushl $\vector
	pushl $0xDEADBEEF // FIXME: unused push
	jmp interrupt_common
.endm

/******************************************************************************/
/*                                                                            */
/* Code                                                                       */
/*                                                                            */
/******************************************************************************/

.section .text
.extern default_except_handler
.extern do_irq

/******************************************************************************/
/*                                                                            */
/* Exceptions handler definitions                                             */
/*                                                                            */
/******************************************************************************/

except_common:
	cli
	SAVE_ALL
	movl 28(%esp), %eax // 28 == 7 * 4
	call *%eax
	RESTORE_ALL
	addl $8, %esp
	sti
	iret

.global do_irq
interrupt_common:
	SAVE_ALL
	cld
	call do_irq
	RESTORE_ALL
	addl $8, %esp
	iret


/* TODO: remove except_default*** replace by the real excpetions handler */

except_de: // Divide error
	except_default_noerr
except_db: // Debug exception
	except_default_noerr
except_nmi: // NMI interrupt
	except_default_noerr
except_bp: // Breakpoint
	except_default_noerr
except_of: // Overflow
	except_default_noerr
except_br: // BOUND range exceeded
	except_default_noerr
except_ud: // Invalid opcode
	except_default_noerr
except_nm: // Device note available (no math coprocessor)
	except_default_noerr
except_df: // Double fault
	except_default_err
except_default_res 9
except_ts: // Invalid TSS
	except_default_err
except_np: // Segment not present
	except_default_err
except_ss: // Stack segment fault
	except_default_err
except_gp: // General protection
	except_default_err
except_pf: // Page fault
	except_default_err
except_default_res 15
except_mf: // x87 FPU Floating-point error
	except_default_noerr
except_ac: // Alignment check
	except_default_err
except_mc: // Machine check
	except_default_noerr
except_xm: // SIMD Floating-point exception
	except_default_noerr
except_ve: // Virtualization exception
	except_default_noerr
	except_default_res 21
	except_default_res 22
	except_default_res 23
	except_default_res 24
	except_default_res 25
	except_default_res 26
	except_default_res 27
	except_default_res 28
	except_default_res 29
	except_default_res 30
	except_default_res 31

.rept IRQS_NR
interrupt_stub \+
.endr

/******************************************************************************/
/*                                                                            */
/* Data                                                                       */
/*                                                                            */
/******************************************************************************/

.section .data
.global except_handler_table
.global interrupt_stub_table

.align 16
except_handler_table:
	.long except_de		// Divide error
	.long except_db		// Debug exception
	.long except_nmi	// NMI interrupt
	.long except_bp		// Breakpoint
	.long except_of		// Overflow
	.long except_br		// BOUND range exceeded
	.long except_ud		// Invalid opcode
	.long except_nm		// Device note available (no math coprocessor)
	.long except_df		// Double fault
	.long except_9		// Coprocessor segment overrune (reserved)
	.long except_ts		// Invalid TSS
	.long except_np		// Segment not present
	.long except_ss		// Stack segment fault
	.long except_gp		// General protection
	.long except_pf		// Page fault
	.long except_15		// Reserved
	.long except_mf		// x87 FPU Floating-point error
	.long except_ac		// Alignment check
	.long except_mc		// Machine check
	.long except_xm		// SIMD Floating-point exception
	.long except_ve		// Virtualization exception
	.long except_21		// Reserved
	.long except_22		// Reserved
	.long except_23		// Reserved
	.long except_24		// Reserved
	.long except_25		// Reserved
	.long except_26		// Reserved
	.long except_27		// Reserved
	.long except_28		// Reserved
	.long except_29		// Reserved
	.long except_30		// Reserved
	.long except_31		// Reserved
interrupt_stub_table:
.rept IRQS_NR
	.long __irq\+
.endr
interrupt_stub_table_end:
