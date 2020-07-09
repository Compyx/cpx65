/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   opcode.c
 * \brief   Opcode tables
 *
 * Opcode tables for supported CPUs.
 *
 * \note    MNE_BRA (BRanch Always) ($80): this instruction always branches and
 *          thus I've set the base cycle count to 3 and the add-cycle-on branch
 *          to 0. Other branch instructions have the usual { 2, 1 }.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "cputype.h"
#include "addrmode.h"
#include "mnemonic.h"
#include "../helpers.h"
#include "../debug.h"

#include "opcode.h"


/* {{{ 65XX opcodes table */
/** \brief  List of opcodes for 6502 derivatives like the 6510
 *
 * The table contains information on undocumented opcodes used in the 'original'
 * 6502 and 6510.
 */
static const opcode_t opcodes_65xx[] = {
    /*
      mne,      addr mode,  opcode type,        C, P, B, D

      Where C = cycles, P = add cycle when crossing page, B = add cycle when
      branch is taken, D = add cycle if using decimal mode (D flag set).
      Opcode type is only used for 6502/6510.
     */
    /* $00-$03 */
    { MNE_BRK,  AMD_IMP,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },  /* immediate */
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_JAM,  AMD_IMP,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_SLO,  AMD_IZX,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $04-$07 */
    { MNE_NOP,  AMD_ZP,     OPC_TYPE_STABLE,    3, 0, 0, 0 },
    { MNE_ORA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ASL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SLO,  AMD_ZP,     OPC_TYPE_STABLE,    5, 0, 0, 0 },
    /* $08-$0b */
    { MNE_PHP,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ORA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ASL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ANC,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    /* $0c-$0f */
    { MNE_NOP,  AMD_ABS,    OPC_TYPE_STABLE,    3, 0, 0, 0 },
    { MNE_ORA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SLO,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    /* $10-$13 */
    { MNE_BPL,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_JAM,  AMD_IMP,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_SLO,  AMD_IZX,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $14-$17 */
    { MNE_NOP,  AMD_ZPX,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    { MNE_ORA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SLO,  AMD_ZPX,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $18-$1b */
    { MNE_CLC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ORA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_SLO,  AMD_ABY,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $1c-$1f */
    { MNE_NOP,  AMD_ABX,    OPC_TYPE_STABLE,    4, 1, 0, 0 },
    { MNE_ORA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ASL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_SLO,  AMD_ABX,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $20-$23 */
    { MNE_JSR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_AND,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_JAM,  AMD_ZP,     OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_RLA,  AMD_IZX,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $24-$27 */
    { MNE_BIT,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_AND,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RLA,  AMD_ZP,     OPC_TYPE_STABLE,    5, 0, 0, 0 },
    /* $28-$2b */
    { MNE_PLP,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ANC,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    /* $2c-$2f */
    { MNE_BIT,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RLA,  AMD_ABS,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $30-$33 */
    { MNE_BMI,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_AND,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_JAM,  AMD_ZPX,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_RLA,  AMD_IZY,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $34-$37 */
    { MNE_NOP,  AMD_ZPX,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RLA,  AMD_ZPX,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $38-$3b */
    { MNE_SEC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_AND,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_RLA,  AMD_ABY,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $3c-$3f */
    { MNE_NOP,  AMD_ABX,    OPC_TYPE_STABLE,    4, 1, 0, 0 },
    { MNE_AND,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_RLA,  AMD_ABX,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $40-$43 */
    { MNE_RTI,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_EOR,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_JAM,  AMD_ZPY,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_SRE,  AMD_IZX,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $44-$47 */
    { MNE_NOP,  AMD_ZP,     OPC_TYPE_STABLE,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LSR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SRE,  AMD_ZP,     OPC_TYPE_STABLE,    5, 0, 0, 0 },
    /* $48-$4b */
    { MNE_PHA,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LSR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ALR,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    /* $4c-$4f */
    { MNE_JMP,  AMD_ABS,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SRE,  AMD_ABS,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $50-$53 */
    { MNE_BVC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_EOR,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_JAM,  AMD_IZX,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_SRE,  AMD_IZY,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    /* $54-$57 */
    { MNE_NOP,  AMD_ZPX,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    { MNE_EOR,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SRE,  AMD_ZPX,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $58-$5b */
    { MNE_CLI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_EOR,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_SRE,  AMD_ABY,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $5c-$5f */
    { MNE_NOP,  AMD_ABX,    OPC_TYPE_STABLE,    4, 1, 0, 0 },
    { MNE_EOR,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LSR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_SRE,  AMD_ABX,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $60-$63 */
    { MNE_RTS,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ADC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_JAM,  AMD_IZX,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_RRA,  AMD_IZX,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $64-$67 */
    { MNE_NOP,  AMD_ZP,     OPC_TYPE_STABLE,    3, 0, 0, 0 },
    { MNE_ADC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROR,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RRA,  AMD_ZP,     OPC_TYPE_STABLE,    5, 0, 0, 0 },
    /* $68-$6b */
    { MNE_PLA,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ARR,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    /* $6c-$6f */
    { MNE_JMP,  AMD_IAB,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ADC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RRA,  AMD_ABS,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $70-$73 */
    { MNE_BVS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ADC,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_JAM,  AMD_ABS,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_RRA,  AMD_IZY,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $74-$77 */
    { MNE_NOP,  AMD_ZPX,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RRA,  AMD_ABX,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $78-$7b */
    { MNE_SEI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ADC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_RRA,  AMD_ABY,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $7c-$7f */
    { MNE_NOP,  AMD_ABX,    OPC_TYPE_STABLE,    4, 1, 0, 0 },
    { MNE_ADC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_RRA,  AMD_ABX,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $80-$83 */
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_STA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_PARTIAL,   2, 0, 0, 0 },
    { MNE_SAX,  AMD_IZX,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $84-$87 */
    { MNE_STY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SAX,  AMD_ZP,     OPC_TYPE_STABLE,    3, 0, 0, 0 },
    /* $88-$8b */
    { MNE_DEY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_TXA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ANE,  AMD_IMM,    OPC_TYPE_UNSTABLE,  2, 0, 0, 0 },
    /* $8c-$8f */
    { MNE_STY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SAX,  AMD_ABS,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    /* $90-$93 */
    { MNE_BCC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_STA,  AMD_IZY,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_ABX,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_SHA,  AMD_IZY,    OPC_TYPE_PARTIAL,   6, 0, 0, 0 },
    /* $94-$97 */
    { MNE_STY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SAX,  AMD_ZPY,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    /* $98-$9b */
    { MNE_TYA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_STA,  AMD_ABY,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TXS,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TAS,  AMD_ABY,    OPC_TYPE_PARTIAL,   5, 0, 0, 0 },
    /* $9c-$9f */
    { MNE_SHY,  AMD_ABX,    OPC_TYPE_PARTIAL,   5, 0, 0, 0 },
    { MNE_STA,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SHX,  AMD_ABY,    OPC_TYPE_PARTIAL,   5, 0, 0, 0 },
    { MNE_SHA,  AMD_ABY,    OPC_TYPE_PARTIAL,   5, 0, 0, 0 },
    /* $a0-$a3 */
    { MNE_LDY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_LDX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LAX,  AMD_IZX,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $a4-$a7 */
    { MNE_LDY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LAX,  AMD_ZP,     OPC_TYPE_STABLE,    3, 0, 0, 0 },
    /* $a8-$ab */
    { MNE_TAY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TAX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LAX,  AMD_IMM,    OPC_TYPE_UNSTABLE,  2, 0, 0, 0 },
    /* $ac-$af */
    { MNE_LDY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LAX,  AMD_ABS,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    /* $b0-$b3 */
    { MNE_BCS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_LDA,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_JAM,  AMD_ABY,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_LAX,  AMD_IZY,    OPC_TYPE_STABLE,    5, 1, 0, 0 },
    /* $b4-$b7 */
    { MNE_LDY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LAX,  AMD_ZPY,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    /* $b8-$bb */
    { MNE_CLV,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_TSX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LAS,  AMD_ABY,    OPC_TYPE_PARTIAL,   4, 1, 0, 0 },
    /* $bc-$bf */
    { MNE_LDY,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDX,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LAX,  AMD_ABY,    OPC_TYPE_STABLE,    4, 1, 0, 0 },
    /* $c0-$c3 */
    { MNE_CPY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_PARTIAL,   2, 0, 0, 0 },
    { MNE_DCP,  AMD_IZX,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $c4-$c7 */
    { MNE_CPY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_CMP,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_DEC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_DCP,  AMD_ZP,     OPC_TYPE_STABLE,    5, 0, 0, 0 },
    /* $c8-$cb */
    { MNE_INY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_DEX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBX,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    /* $cc-$cf */
    { MNE_CPY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_CMP,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_DCP,  AMD_ABS,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $d0-$d3 */
    { MNE_BNE,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_JAM,  AMD_IAB,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_DCP,  AMD_IZY,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $d4-$d7 */
    { MNE_NOP,  AMD_ZPX,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    { MNE_CMP,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_DCP,  AMD_ZPX,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $d8-$db */
    { MNE_CLD,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_DCP,  AMD_ABY,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $dc-$df */
    { MNE_NOP,  AMD_ABX,    OPC_TYPE_STABLE,    4, 1, 0, 0 },
    { MNE_CMP,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEC,  AMD_ABX,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_DCP,  AMD_ABX,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $e0-$e3 */
    { MNE_CPX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_PARTIAL,   2, 0, 0, 0 },
    { MNE_ISC,  AMD_IZX,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $e4-$e7 */
    { MNE_CPX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SBC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_INC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ISC,  AMD_ZP,     OPC_TYPE_STABLE,    5, 0, 0, 0 },
    /* $e8-$eb */
    { MNE_INX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IMM,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    /* $ec-$ef */
    { MNE_CPX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SBC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ISC,  AMD_ABS,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $f0-$f3 */
    { MNE_BEQ,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_JAM,  AMD_REL,    OPC_TYPE_STABLE,    0, 0, 0, 0 },
    { MNE_ISC,  AMD_IZY,    OPC_TYPE_STABLE,    8, 0, 0, 0 },
    /* $f4-$f7 */
    { MNE_NOP,  AMD_ZPX,    OPC_TYPE_STABLE,    4, 0, 0, 0 },
    { MNE_SBC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ISC,  AMD_ZPX,    OPC_TYPE_STABLE,    6, 0, 0, 0 },
    /* $f8-$fb */
    { MNE_SED,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_STABLE,    2, 0, 0, 0 },
    { MNE_ISC,  AMD_ABY,    OPC_TYPE_STABLE,    7, 0, 0, 0 },
    /* $fc-$ff */
    { MNE_NOP,  AMD_ABX,    OPC_TYPE_STABLE,    4, 1, 0, 0 },
    { MNE_SBC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INC,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_ISC,  AMD_ABX,    OPC_TYPE_STABLE,    7, 0, 0, 0 }
};
/* }}} */

/* {{{ 65C02 opcodes table */
/** \brief  List of opcodes for the 'standard' 65C02
 *
 * I used an NCR 65C02 datasheet for this.
 *
 */
static const opcode_t opcodes_65c02[] = {
    /* $00-$03 */
    { MNE_BRK,  AMD_IMP,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },  /* immediate */
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $04-$07 */
    { MNE_TSB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ASL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $08-$0b */
    { MNE_PHP,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ORA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ASL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $0c-$0f */
    { MNE_TSB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $10-$13 */
    { MNE_BPL,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $14-$17 */
    { MNE_TRB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $18-$1b */
    { MNE_CLC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ORA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $1c-$1f */
    { MNE_TRB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_ORA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ASL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $20-$23 */
    { MNE_JSR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_AND,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $24-$27 */
    { MNE_BIT,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_AND,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $28-$2b */
    { MNE_PLP,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $2c-$2f */
    { MNE_BIT,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $30-$33 */
    { MNE_BMI,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_AND,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_AND,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $34-$37 */
    { MNE_BIT,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $38-$3b */
    { MNE_SEC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_AND,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $3c-$3f */
    { MNE_BIT,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_AND,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $40-$43 */
    { MNE_RTI,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_EOR,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $44-$47 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LSR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $48-$4b */
    { MNE_PHA,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LSR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $4c-$4f */
    { MNE_JMP,  AMD_ABS,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $50-$53 */
    { MNE_BVC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_EOR,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_EOR,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $54-$57 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    /* $58-$5b */
    { MNE_CLI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_EOR,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHY,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $5c-$5f */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LSR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $60-$63 */
    { MNE_RTS,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ADC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $64-$67 */
    { MNE_STZ,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ADC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROR,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $68-$6b */
    { MNE_PLA,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $6c-$6f */
    { MNE_JMP,  AMD_IAB,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ADC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $70-$73 */
    { MNE_BVS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ADC,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_ADC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $74-$77 */
    { MNE_STZ,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $78-$7b */
    { MNE_SEI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ADC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLY,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $7c-$7f */
    { MNE_JMP,  AMD_IAX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ADC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $80-$83 */
    { MNE_BRA,  AMD_REL,    OPC_TYPE_NORMAL,    3, 0, 1, 0 },
    { MNE_STA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $84-$87 */
    { MNE_STY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $88-$8b */
    { MNE_DEY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_BIT,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TXA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $8c-$8f */
    { MNE_STY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $90-$93 */
    { MNE_BCC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_STA,  AMD_IZY,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_STA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $94-$97 */
    { MNE_STY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $98-$9b */
    { MNE_TYA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_STA,  AMD_ABY,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TXS,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $9c-$9f */
    { MNE_STZ,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_STZ,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $a0-$a3 */
    { MNE_LDY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_LDX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $a4-$a7 */
    { MNE_LDY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $a8-$ab */
    { MNE_TAY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TAX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ac-$af */
    { MNE_LDY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $b0-$b3 */
    { MNE_BCS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_LDA,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_LDA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $b4-$b7 */
    { MNE_LDY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $b8-$bb */
    { MNE_CLV,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_TSX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $bc-$bf */
    { MNE_LDY,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDX,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $c0-$c3 */
    { MNE_CPY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $c4-$c7 */
    { MNE_CPY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_CMP,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_DEC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $c8-$cb */
    { MNE_INY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_DEX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $cc-$cf */
    { MNE_CPY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_CMP,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $d0-$d3 */
    { MNE_BNE,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_CMP,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $d4-$d7 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_CMP,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $d8-$db */
    { MNE_CLD,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHX,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $dc-$df */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_CMP,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEC,  AMD_ABX,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $e0-$e3 */
    { MNE_CPX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $e4-$e7 */
    { MNE_CPX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SBC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_INC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $e8-$eb */
    { MNE_INX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ec-$ef */
    { MNE_CPX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SBC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $f0-$f3 */
    { MNE_BEQ,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SBC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $f4-$f7 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_SBC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $f8-$fb */
    { MNE_SED,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLX,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $fc-$ff */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_SBC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INC,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 }
};
/* }}} */

/* {{{ R65C02 opcodes table */
/** \brief  List of opcodes for the Rockwell R65C02
 *
 */
static const opcode_t opcodes_r65c02[] = {
    /* $00-$03 */
    { MNE_BRK,  AMD_IMP,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },  /* immediate */
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $04-$07 */
    { MNE_TSB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ASL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB0, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $08-$0b */
    { MNE_PHP,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ORA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ASL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $0c-$0f */
    { MNE_TSB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR0, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $10-$13 */
    { MNE_BPL,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $14-$17 */
    { MNE_TRB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB1, AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $18-$1b */
    { MNE_CLC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ORA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $1c-$1f */
    { MNE_TRB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_ORA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ASL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR1, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $20-$23 */
    { MNE_JSR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_AND,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $24-$27 */
    { MNE_BIT,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_AND,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB2, AMD_ZP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $28-$2b */
    { MNE_PLP,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $2c-$2f */
    { MNE_BIT,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR2, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $30-$33 */
    { MNE_BMI,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_AND,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_AND,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $34-$37 */
    { MNE_BIT,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB3, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $38-$3b */
    { MNE_SEC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_AND,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $3c-$3f */
    { MNE_BIT,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_AND,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR3, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $40-$43 */
    { MNE_RTI,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_EOR,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SAY,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_TMA,  AMD_BLK,    OPC_TYPE_NORMAL,    17, 0, 0, 0 },  /*+ 6 per byte */
    /* $44-$47 */
    { MNE_BSR,  AMD_REL,    OPC_TYPE_NORMAL,    8, 0, 0, 0 },
    { MNE_EOR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LSR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_RMB4, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $48-$4b */
    { MNE_PHA,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LSR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $4c-$4f */
    { MNE_JMP,  AMD_ABS,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBR4, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $50-$53 */
    { MNE_BVC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_EOR,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_EOR,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TAM,  AMD_BLK,    OPC_TYPE_NORMAL,    17, 0, 0, 0 }, /* +6 per byte */
    /* $54-$57 */
    { MNE_CSL,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB5, AMD_ZP,     OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    /* $58-$5b */
    { MNE_CLI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_EOR,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHY,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $5c-$5f */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LSR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR5, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $60-$63 */
    { MNE_RTS,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ADC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_CLA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $64-$67 */
    { MNE_STZ,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ADC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROR,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB6, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $68-$6b */
    { MNE_PLA,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $6c-$6f */
    { MNE_JMP,  AMD_IAB,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ADC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR6, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $70-$73 */
    { MNE_BVS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ADC,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_ADC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_TII,  AMD_BLK,    OPC_TYPE_NORMAL,    1, 0, 0, 0 }, /* +6 per byte */
    /* $74-$77 */
    { MNE_STZ,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB7, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $78-$7b */
    { MNE_SEI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ADC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLY,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $7c-$7f */
    { MNE_JMP,  AMD_IAX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ADC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR7, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },

    /* 2020-07-08 18:10: got to here */

    /* $80-$83 */
    { MNE_BRA,  AMD_REL,    OPC_TYPE_NORMAL,    3, 0, 1, 0 },
    { MNE_STA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_CLX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TST,  AMD_IMZ,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $84-$87 */
    { MNE_STY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SMB0, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $88-$8b */
    { MNE_DEY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_BIT,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TXA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $8c-$8f */
    { MNE_STY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBS0, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $90-$93 */
    { MNE_BCC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_STA,  AMD_IZY,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_STA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TST,  AMD_IMA,    OPC_TYPE_NORMAL,    8, 0, 0, 0 },
    /* $94-$97 */
    { MNE_STY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SMB1, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $98-$9b */
    { MNE_TYA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_STA,  AMD_ABY,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TXS,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $9c-$9f */
    { MNE_STZ,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_STZ,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_BBS1, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $a0-$a3 */
    { MNE_LDY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_LDX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TST,  AMD_IMZX,   OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $a4-$a7 */
    { MNE_LDY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SMB2, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $a8-$ab */
    { MNE_TAY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TAX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ac-$af */
    { MNE_LDY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBS2, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $b0-$b3 */
    { MNE_BCS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_LDA,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_LDA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TST,  AMD_IMAX,   OPC_TYPE_NORMAL,    8, 0, 0, 0 },
    /* $b4-$b7 */
    { MNE_LDY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SMB3, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $b8-$bb */
    { MNE_CLV,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_TSX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $bc-$bf */
    { MNE_LDY,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDX,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_BBS3, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $c0-$c3 */
    { MNE_CPY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_CLY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TDD,  AMD_BLK,    OPC_TYPE_NORMAL,    17, 0, 0, 0 }, /* +6 per byte */
    /* $c4-$c7 */
    { MNE_CPY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_CMP,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_DEC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SMB4, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $c8-$cb */
    { MNE_INY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_DEX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $cc-$cf */
    { MNE_CPY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_CMP,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBS4, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $d0-$d3 */
    { MNE_BNE,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_CMP,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TIN,  AMD_BLK,    OPC_TYPE_NORMAL,    17, 0, 0, 0 }, /* +6 per byte */
    /* $d4-$d7 */
    { MNE_CSH,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_CMP,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SMB5, AMD_IMP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $d8-$db */
    { MNE_CLD,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHX,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $dc-$df */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_CMP,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEC,  AMD_ABX,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_BBS5, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $e0-$e3 */
    { MNE_CPX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_TIA,  AMD_BLK,    OPC_TYPE_NORMAL,    17, 0, 0, 0 }, /* +6 per byte */
    /* $e4-$e7 */
    { MNE_CPX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SBC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_INC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SMB6, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $e8-$eb */
    { MNE_INX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ec-$ef */
    { MNE_CPX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SBC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBS6, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $f0-$f3 */
    { MNE_BEQ,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SBC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_TAI,  AMD_BLK,    OPC_TYPE_NORMAL,    17, 0, 0, 0 }, /* +6 per byte */
    /* $f4-$f7 */
    { MNE_SET,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SMB7, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $f8-$fb */
    { MNE_SED,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLX,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $fc-$ff */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_SBC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INC,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBS7, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 }
};
/* }}} */

/* {{{ W65C02S opcodes table */
/** \brief  List of opcodes for the WDC W65C02S
 *
 * Copied the R652C02 table and added STP and WAI.
 *
 * \todo    Check cycle count for RMBn and SMBn opcodes!
 */
static const opcode_t opcodes_w65c02s[] = {
    /* $00-$03 */
    { MNE_BRK,  AMD_IMP,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },  /* immediate */
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $04-$07 */
    { MNE_TSB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ASL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB0, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $08-$0b */
    { MNE_PHP,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ORA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ASL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $0c-$0f */
    { MNE_TSB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR0, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $10-$13 */
    { MNE_BPL,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $14-$17 */
    { MNE_TRB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB1, AMD_IMP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $18-$1b */
    { MNE_CLC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ORA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $1c-$1f */
    { MNE_TRB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_ORA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ASL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR1, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $20-$23 */
    { MNE_JSR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_AND,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $24-$27 */
    { MNE_BIT,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_AND,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB2, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $28-$2b */
    { MNE_PLP,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $2c-$2f */
    { MNE_BIT,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR2, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $30-$33 */
    { MNE_BMI,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_AND,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_AND,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $34-$37 */
    { MNE_BIT,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB3, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $38-$3b */
    { MNE_SEC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_AND,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $3c-$3f */
    { MNE_BIT,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_AND,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR3, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $40-$43 */
    { MNE_RTI,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_EOR,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $44-$47 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LSR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_RMB4, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $48-$4b */
    { MNE_PHA,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LSR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $4c-$4f */
    { MNE_JMP,  AMD_ABS,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBR4, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $50-$53 */
    { MNE_BVC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_EOR,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_EOR,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $54-$57 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB5, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $58-$5b */
    { MNE_CLI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_EOR,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHY,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $5c-$5f */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LSR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR5, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $60-$63 */
    { MNE_RTS,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ADC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $64-$67 */
    { MNE_STZ,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ADC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROR,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB6, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $68-$6b */
    { MNE_PLA,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $6c-$6f */
    { MNE_JMP,  AMD_IAB,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ADC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR6, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $70-$73 */
    { MNE_BVS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ADC,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_ADC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $74-$77 */
    { MNE_STZ,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB7, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $78-$7b */
    { MNE_SEI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ADC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLY,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $7c-$7f */
    { MNE_JMP,  AMD_IAX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ADC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR7, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $80-$83 */
    { MNE_BRA,  AMD_REL,    OPC_TYPE_NORMAL,    3, 0, 1, 0 },
    { MNE_STA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $84-$87 */
    { MNE_STY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SMB0, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $88-$8b */
    { MNE_DEY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_BIT,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TXA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $8c-$8f */
    { MNE_STY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBS0, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $90-$93 */
    { MNE_BCC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_STA,  AMD_IZY,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_STA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $94-$97 */
    { MNE_STY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SMB1, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $98-$9b */
    { MNE_TYA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_STA,  AMD_ABY,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TXS,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $9c-$9f */
    { MNE_STZ,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_STZ,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_BBS1, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $a0-$a3 */
    { MNE_LDY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_LDX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $a4-$a7 */
    { MNE_LDY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SMB2, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $a8-$ab */
    { MNE_TAY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TAX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ac-$af */
    { MNE_LDY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBS2, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $b0-$b3 */
    { MNE_BCS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_LDA,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_LDA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $b4-$b7 */
    { MNE_LDY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SMB3, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $b8-$bb */
    { MNE_CLV,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_TSX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $bc-$bf */
    { MNE_LDY,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDX,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_BBS3, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $c0-$c3 */
    { MNE_CPY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $c4-$c7 */
    { MNE_CPY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_CMP,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_DEC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SMB4, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $c8-$cb */
    { MNE_INY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_DEX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_WAI,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    /* $cc-$cf */
    { MNE_CPY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_CMP,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBS4, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $d0-$d3 */
    { MNE_BNE,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_CMP,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $d4-$d7 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_CMP,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SMB5, AMD_IMP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $d8-$db */
    { MNE_CLD,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHX,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STP,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    /* $dc-$df */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_CMP,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEC,  AMD_ABX,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_BBS5, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $e0-$e3 */
    { MNE_CPX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $e4-$e7 */
    { MNE_CPX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SBC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_INC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SMB6, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $e8-$eb */
    { MNE_INX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ec-$ef */
    { MNE_CPX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SBC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBS6, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $f0-$f3 */
    { MNE_BEQ,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SBC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $f4-$f7 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_SBC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SMB7, AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $f8-$fb */
    { MNE_SED,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLX,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $fc-$ff */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_SBC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INC,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBS7, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 }
};
/* }}} */

/* {{{ HuC6280 opcodes table */
/** \brief  List of opcodes for the Hudson Soft Huc6280
 *
 * Copied the W65C02S table and replaced 65C02 opcodes with HuC6280 opcodes
 *
 */
static const opcode_t opcodes_huc6280[] = {
    /* $00-$03 */
    { MNE_BRK,  AMD_IMP,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },  /* immediate */
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SXY,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ST0,  AMD_IMM,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $04-$07 */
    { MNE_TSB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ASL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB0, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $08-$0b */
    { MNE_PHP,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ORA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ASL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $0c-$0f */
    { MNE_TSB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR0, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $10-$13 */
    { MNE_BPL,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ORA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ORA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ST1,  AMD_IMM,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $14-$17 */
    { MNE_TRB,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ORA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ASL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB1, AMD_IMP,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $18-$1b */
    { MNE_CLC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ORA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $1c-$1f */
    { MNE_TRB,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_ORA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ASL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR1, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $20-$23 */
    { MNE_JSR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_AND,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SAX,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ST2,  AMD_IMM,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    /* $24-$27 */
    { MNE_BIT,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_AND,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROL,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB2, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $28-$2b */
    { MNE_PLP,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROL,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $2c-$2f */
    { MNE_BIT,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR2, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $30-$33 */
    { MNE_BMI,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_AND,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_AND,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $34-$37 */
    { MNE_BIT,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_AND,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROL,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB3, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $38-$3b */
    { MNE_SEC,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_AND,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEA,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $3c-$3f */
    { MNE_BIT,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_AND,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROL,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR3, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $40-$43 */
    { MNE_RTI,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_EOR,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $44-$47 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LSR,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_RMB4, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $48-$4b */
    { MNE_PHA,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LSR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMM,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $4c-$4f */
    { MNE_JMP,  AMD_ABS,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_EOR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBR4, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $50-$53 */
    { MNE_BVC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_EOR,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_EOR,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $54-$57 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LSR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB5, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $58-$5b */
    { MNE_CLI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_EOR,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHY,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $5c-$5f */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_EOR,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LSR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR5, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $60-$63 */
    { MNE_RTS,  AMD_IMP,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_ADC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $64-$67 */
    { MNE_STZ,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ADC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_ROR,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_RMB6, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $68-$6b */
    { MNE_PLA,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ROR,  AMD_ACC,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $6c-$6f */
    { MNE_JMP,  AMD_IAB,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_ADC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBR6, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $70-$73 */
    { MNE_BVS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_ADC,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_ADC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $74-$77 */
    { MNE_STZ,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ADC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_ROR,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_RMB7, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $78-$7b */
    { MNE_SEI,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_ADC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLY,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $7c-$7f */
    { MNE_JMP,  AMD_IAX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ADC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_ROR,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBR7, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $80-$83 */
    { MNE_BRA,  AMD_REL,    OPC_TYPE_NORMAL,    3, 0, 1, 0 },
    { MNE_STA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $84-$87 */
    { MNE_STY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SMB0, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $88-$8b */
    { MNE_DEY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_BIT,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TXA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $8c-$8f */
    { MNE_STY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBS0, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $90-$93 */
    { MNE_BCC,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_STA,  AMD_IZY,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_STA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $94-$97 */
    { MNE_STY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SMB1, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $98-$9b */
    { MNE_TYA,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_STA,  AMD_ABY,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_TXS,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $9c-$9f */
    { MNE_STZ,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_STA,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_STZ,  AMD_ABX,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_BBS1, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $a0-$a3 */
    { MNE_LDY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_LDX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $a4-$a7 */
    { MNE_LDY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDA,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_LDX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SMB2, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $a8-$ab */
    { MNE_TAY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_TAX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ac-$af */
    { MNE_LDY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_BBS2, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $b0-$b3 */
    { MNE_BCS,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_LDA,  AMD_IZY,    OPC_TYPE_NORMAL,    5, 1, 0, 0 },
    { MNE_LDA,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $b4-$b7 */
    { MNE_LDY,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDA,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_LDX,  AMD_ZPY,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SMB3, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $b8-$bb */
    { MNE_CLV,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_LDA,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_TSX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $bc-$bf */
    { MNE_LDY,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDA,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_LDX,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_BBS3, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $c0-$c3 */
    { MNE_CPY,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $c4-$c7 */
    { MNE_CPY,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_CMP,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_DEC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SMB4, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $c8-$cb */
    { MNE_INY,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_DEX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_WAI,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    /* $cc-$cf */
    { MNE_CPY,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_CMP,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBS4, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $d0-$d3 */
    { MNE_BNE,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_CMP,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_CMP,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $d4-$d7 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_CMP,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_DEC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SMB5, AMD_IMP,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $d8-$db */
    { MNE_CLD,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_CMP,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PHX,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_STP,  AMD_IMP,    OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    /* $dc-$df */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_CMP,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_DEC,  AMD_ABX,    OPC_TYPE_NORMAL,    6, 1, 0, 0 },
    { MNE_BBS5, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $e0-$e3 */
    { MNE_CPX,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $e4-$e7 */
    { MNE_CPX,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_SBC,  AMD_ZP,     OPC_TYPE_NORMAL,    3, 0, 0, 0 },
    { MNE_INC,  AMD_ZP,     OPC_TYPE_NORMAL,    5, 0, 0, 0 },
    { MNE_SMB6, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $e8-$eb */
    { MNE_INX,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_IMM,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $ec-$ef */
    { MNE_CPX,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_SBC,  AMD_ABS,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ABS,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_BBS6, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 },
    /* $f0-$f3 */
    { MNE_BEQ,  AMD_REL,    OPC_TYPE_NORMAL,    2, 1, 1, 0 },
    { MNE_SBC,  AMD_IZX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SBC,  AMD_IZP,    OPC_TYPE_NORMAL,    5, 0, 0, 1 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $f4-$f7 */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_SBC,  AMD_ZPX,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_INC,  AMD_ZPX,    OPC_TYPE_NORMAL,    6, 0, 0, 0 },
    { MNE_SMB7, AMD_ZP,     OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    /* $f8-$fb */
    { MNE_SED,  AMD_IMP,    OPC_TYPE_NORMAL,    2, 0, 0, 0 },
    { MNE_SBC,  AMD_ABY,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_PLX,  AMD_IMP,    OPC_TYPE_NORMAL,    4, 0, 0, 0 },
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    /* $fc-$ff */
    { MNE_NOP,  AMD_IMP,    OPC_TYPE_NORMAL,    1, 0, 0, 0 },
    { MNE_SBC,  AMD_ABX,    OPC_TYPE_NORMAL,    4, 1, 0, 0 },
    { MNE_INC,  AMD_ABX,    OPC_TYPE_NORMAL,    7, 0, 0, 0 },
    { MNE_BBS7, AMD_ZPR,    OPC_TYPE_NORMAL,    5, 1, 1, 0 }
};
/* }}} */


/** \brief  Mapping of opcode tables to CPU types
 */
static const opcode_table_t opcode_tables[]= {
    { opcodes_65xx,     1 },    /* CPU_65xx */
    { opcodes_65xx,     0 },    /* CPU_6502 */
    { opcodes_65c02,    0 },    /* CPU_65C02 */
    { opcodes_r65c02,   0 },    /* CPU_R65C02 */
    { opcodes_w65c02s,  0 },    /* CPU_W65C02S */
    { NULL,             0 },    /* CPU_65CE02 */
    { opcodes_65xx,     1 },    /* CPU_2A03 */
    { opcodes_huc6280,  0 },    /* CPU_HUC6280 */
    { NULL,             0 }     /* CPU_65816 */
};


/** \brief  CPU type
 *
 * CPU type used to access an opcode table.
 *
 * \see opcode_set_cpu_type
 * \see opcode_get_cpu_type
 */
static cpu_type_t opcode_cpu_type = CPU_65XX;


/** \brief  Set CPU type
 *
 * \param[in]   type    CPU type ID
 */
void opcode_set_cpu_type(cpu_type_t type)
{
    opcode_cpu_type = type;
}


/** \brief  Get CPU type
 *
 * \return  CPU type ID
 */
cpu_type_t opcode_get_cpu_type(void)
{
    return opcode_cpu_type;
}


/** \brief  Dump opcode table for \a cpu_type on stdout
 *
 * \param[in]   cpu_type    CPU type ID
 */
bool opcode_dump_table(void)
{
    const opcode_t *table;
    int undoc;
    size_t opc;

    printf("Got CPU type %d (%s)\n",
            opcode_cpu_type, cpu_type_get_desc(opcode_cpu_type));

    table = opcode_tables[opcode_cpu_type].table;
    undoc = opcode_tables[opcode_cpu_type].undocumented;

    if (table == NULL) {
        fprintf(stderr, "Error: no opcode table for CPU %d (%s), bailing.\n",
                opcode_cpu_type, cpu_type_get_desc(opcode_cpu_type));
        return false;
    }

    for (opc = 0; opc < 0x100; opc++) {
        if (table[opc].opcode_type == OPC_TYPE_NORMAL || undoc) {
            printf("%02lx  %s  %s  %d%c%c\n",
                    opc,
                    mnemonic_get_text(table[opc].mnemonic_id),
                    addrmode_get_desc(table[opc].addrmode_id),
                    table[opc].cycles,
                    table[opc].page ? '*' : ' ',
                    table[opc].branch ? '*' : ' ');
        } else {
            printf("%02lx  ???\n", opc);
        }
    }
    return true;
}



const char *opcode_get_mnemonic_text(int opc)
{
    const opcode_t *table = opcode_tables[opcode_cpu_type].table;
    return mnemonic_get_text(table[opc].mnemonic_id);
}


int opcode_get_mnemonic_id(int opc)
{
    const opcode_t *table = opcode_tables[opcode_cpu_type].table;
    return table[opc].mnemonic_id;
}


void opcode_get_data(int opc, opcode_data_t *opc_data)
{
    const opcode_t *table = opcode_tables[opcode_cpu_type].table;

    opc_data->opc_type = table[opc].opcode_type;
    opc_data->mne_id = table[opc].mnemonic_id;
    opc_data->mne_text = mnemonic_get_text(table[opc].mnemonic_id);
    opc_data->amd_id = table[opc].addrmode_id;
    opc_data->opr_count = addrmode_get_opr_count(opc_data->amd_id);
    opc_data->opr_sizes = addrmode_get_opr_sizes(opc_data->amd_id);
}





