/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   opcode.h
 * \brief   Opcode tables - header
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


void opcode_set_cpu_type(cpu_type_t type);
cpu_type_t opcode_get_cpu_type(void);

bool opcode_dump_table(void);

#endif
