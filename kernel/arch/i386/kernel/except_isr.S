#include "asm/except.h"

.extern except_handler

.section .text

// Push all registers
.macro pushaq
	pushl %esp
	pushl %ebp
	pushl %esi
	pushl %edi
	pushl %eax
	pushl %ebx
	pushl %ecx
	pushl %edx
.endm

// Pop all registers
.macro popaq
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	popl %edi
	popl %esi
	popl %ebp
	popl %esp
.endm

// Low level ISR routine calling exception handler (NOERR)
.macro except_isr_noerr name
	cli
	pushl $0
	pushl $\name
	pushaq
	// Initial esp is now at esp+10*4

	// Pointer to regs_s struct
	mov %esp, %edi

	cld
	call except_handler

	// Skip int num and err code
	popaq
	add $8, %esp
	sti
	iretl
.endm

// Low level ISR routine calling exception handler (ERR)
.macro except_isr_err name
	cli
	push $\name
	pushaq
	// Initial esp is now at esp+10*4

	// Pointer to interrupt_stack_frame_s struct
	mov %esp, %edi

	cld
	call except_handler

	popaq
	// Skip int num and err code
	add $8, %esp
	sti
	iretl
.endm

// Low level ISR routine calling exception handler (RESERVED)
.macro except_isr_res name=0
except_\name:
	push $0
	push $\name
	cli
	pushaq
	// Initial esp is now at esp+10*4

	// Pointer to interrupt_stack_frame_s struct
	mov %esp, %edi

	cld
	call except_handler

	popaq
	// Skip int num and err code
	add $8, %esp
	sti
	iretl
.endm

except_de: // Divide error
	except_isr_noerr EXCEPT_DE_VEC
except_db: // Debug exception
	except_isr_noerr EXCEPT_DB_VEC
except_nmi: // NMI interrupt
	except_isr_noerr EXCEPT_NMI_VEC
except_bp: // Breakpoint
	except_isr_noerr EXCEPT_BP_VEC
except_of: // Overflow
	except_isr_noerr EXCEPT_OF_VEC
except_br: // BOUND range exceeded
	except_isr_noerr EXCEPT_BR_VEC
except_ud: // Invalid opcode
	except_isr_noerr EXCEPT_UD_VEC
except_nm: // Device note available (no math coprocessor)
	except_isr_noerr EXCEPT_NM_VEC
except_df: // Double fault
	except_isr_err EXCEPT_DF_VEC
except_isr_res  9
except_ts: // Invalid TSS
	except_isr_err EXCEPT_TS_VEC
except_np: // Segment not present
	except_isr_err EXCEPT_NP_VEC
except_ss: // Stack segment fault
	except_isr_err EXCEPT_SS_VEC
except_gp: // General protection
	except_isr_err EXCEPT_GP_VEC
except_pf: // Page fault
	except_isr_err EXCEPT_PF_VEC
except_isr_res 15
except_mf: // x87 FPU Floating-point error
	except_isr_noerr EXCEPT_MF_VEC
except_ac: // Alignment check
	except_isr_err EXCEPT_AC_VEC
except_mc: // Machine check
	except_isr_noerr EXCEPT_MC_VEC
except_xm: // SIMD Floating-point exception
	except_isr_noerr EXCEPT_XM_VEC
except_ve: // Virtualization exception
	except_isr_noerr EXCEPT_VE_VEC
	except_isr_res 21
	except_isr_res 22
	except_isr_res 23
	except_isr_res 24
	except_isr_res 25
	except_isr_res 26
	except_isr_res 27
	except_isr_res 28
	except_isr_res 29
	except_isr_res 30
	except_isr_res 31

.section .data
.global except_isr_table

.align 16
except_isr_table:
	.long except_de		/* Divide error */
	.long except_db		/* Debug exception */
	.long except_nmi	/* NMI interrupt */
	.long except_bp		/* Breakpoint */
	.long except_of		/* Overflow */
	.long except_br		/* BOUND range exceeded */
	.long except_ud		/* Invalid opcode */
	.long except_nm		/* Device note available (no math coprocessor) */
	.long except_df		/* Double fault */
	.long except_9		/* Coprocessor segment overrune (reserved) */
	.long except_ts		/* Invalid TSS */
	.long except_np		/* Segment not present */
	.long except_ss		/* Stack segment fault */
	.long except_gp		/* General protection */
	.long except_pf		/* Page fault */
	.long except_15		/* Reserved */
	.long except_mf		/* x87 FPU Floating-point error */
	.long except_ac		/* Alignment check */
	.long except_mc		/* Machine check */
	.long except_xm		/* SIMD Floating-point exception */
	.long except_ve		/* Virtualization exception */
	.long except_21		/* Reserved */
	.long except_22		/* Reserved */
	.long except_23		/* Reserved */
	.long except_24		/* Reserved */
	.long except_25		/* Reserved */
	.long except_26		/* Reserved */
	.long except_27		/* Reserved */
	.long except_28		/* Reserved */
	.long except_29		/* Reserved */
	.long except_30		/* Reserved */
	.long except_31		/* Reserved */
