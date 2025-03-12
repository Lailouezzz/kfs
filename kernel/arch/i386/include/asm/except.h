/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   except.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amassias <massias.antoine.pro@gmail.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 14:00:07 by ale-boud          #+#    #+#             */
/*   Updated: 2025/03/12 19:35:45 by amassias         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  ASM_EXCEPT_H
# define ASM_EXCEPT_H

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


#endif
