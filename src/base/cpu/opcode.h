/** \file   opcode.h
 * \brief   Opcode tables - header
 */

/*
cpx65 - Assembler for MOS 6502 and variants.
Copyright (C) 2019-2022  Bas Wassink.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef BASE_CPU_OPCODE_H
#define BASE_CPU_OPCODE_H


#include <stdbool.h>

#include "addrmode.h"
#include "cputype.h"
#include "mnemonic.h"


/** \brief  Opcode types
 *
 * This enum is used to mark opcodes as normal, documented opcodes, or as
 * undocumented opcodes which are either stable, partially stable or highly
 * unstable.
 */
typedef enum opcode_type_e {
    OPC_TYPE_NORMAL,    /**< normal documented opcode */
    OPC_TYPE_STABLE,    /**< stable undocumented opcode */
    OPC_TYPE_PARTIAL,   /**< partially unstable opcode */
    OPC_TYPE_UNSTABLE   /**< highly unstable opcode */
} opcode_type_t;


/** \brief  Opcode object
 *
 */
typedef struct opcode_s {
    mnemonic_id_t   mnemonic_id;   /**< mnemonic ID */
    addrmode_id_t   addrmode_id;    /**< addressing mode ID */
    opcode_type_t   opcode_type;    /**< opcode type (\see #opcode_type_t) */
    int             cycles;         /**< number of cycles used */
    int             page;           /**< add 1 cycle when crossing page boundary */
    int             branch;         /**< add 1 cycle when branch is taken */
    int             decimal;        /**< add 1 cycle when D=1 */
} opcode_t;


typedef struct opcode_table_s {
    const opcode_t *table;
    int undocumented;
} opcode_table_t;


typedef struct opcode_data_s {
    int opc_type;
    int mne_id;
    const char *mne_text;
    int amd_id;
    int opr_count;
    const int *opr_sizes;
} opcode_data_t;



void opcode_set_cpu_type(cpu_type_t type);
cpu_type_t opcode_get_cpu_type(void);

const char *opcode_get_mnemonic_text(int opc);
int opcode_get_mnemonic_id(int opc);

void opcode_get_data(int opc, opcode_data_t *opc_data);

bool opcode_is_branch(int opc);

bool opcode_dump_table(void);

#endif
