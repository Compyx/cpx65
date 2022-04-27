/** \file   mnemonic.h
 * \brief   Mnemonics - header
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

#ifndef BASE_CPU_MNEMONIC_H
#define BASE_CPU_MNEMONIC_H


/** \brief  Mnemonic IDs
 */
typedef enum mnemonic_id_e {
    MNE_ILL = -1,    /**< ID for illegal mnemonic */
    /* A */
    MNE_ADC, MNE_ALR, MNE_ANC, MNE_AND, MNE_ANE, MNE_ARR, MNE_ASL,
    /* B */
    MNE_BBR, MNE_BBR0, MNE_BBR1, MNE_BBR2, MNE_BBR3, MNE_BBR4, MNE_BBR5,
    MNE_BBR6, MNE_BBR7, MNE_BBS, MNE_BBS0, MNE_BBS1, MNE_BBS2, MNE_BBS3,
    MNE_BBS4, MNE_BBS5, MNE_BBS6, MNE_BBS7, MNE_BCC, MNE_BCS, MNE_BEQ, MNE_BIT,
    MNE_BMI, MNE_BNE, MNE_BPL, MNE_BRA, MNE_BRK, MNE_BSR, MNE_BVC, MNE_BVS,
    /* C */
    MNE_CLA, MNE_CLC, MNE_CLD, MNE_CLI, MNE_CLV, MNE_CLX, MNE_CLY, MNE_CMP,
    MNE_CPX, MNE_CPY, MNE_CSH, MNE_CSL,
    /* D */
    MNE_DCP, MNE_DEA, MNE_DEC, MNE_DEX, MNE_DEY,
    /* E */
    MNE_EOR,
    /* I */
    MNE_INA, MNE_INC, MNE_INX, MNE_INY, MNE_ISC,
    /* J */
    MNE_JAM, MNE_JMP, MNE_JSR,
    /* L */
    MNE_LAS, MNE_LAX, MNE_LDA, MNE_LDX, MNE_LDY, MNE_LSR,
    /* N */
    MNE_NOP,
    /* O */
    MNE_ORA,
    /* P */
    MNE_PHA, MNE_PHP, MNE_PHX, MNE_PHY, MNE_PLA, MNE_PLP, MNE_PLX, MNE_PLY,
    /* R */
    MNE_RLA, MNE_RMB, MNE_RMB0, MNE_RMB1, MNE_RMB2, MNE_RMB3, MNE_RMB4,
    MNE_RMB5, MNE_RMB6, MNE_RMB7, MNE_ROL, MNE_ROR, MNE_RRA, MNE_RTI, MNE_RTS,
    /* S */
    MNE_SAX,    /* Different meaning on 65xx and Huc65280) */
    MNE_SAY, MNE_SBC, MNE_SBX, MNE_SEC, MNE_SED, MNE_SEI, MNE_SET, MNE_SHA,
    MNE_SHX, MNE_SHY, MNE_SLO, MNE_SMB, MNE_SMB0, MNE_SMB1, MNE_SMB2, MNE_SMB3,
    MNE_SMB4, MNE_SMB5, MNE_SMB6, MNE_SMB7, MNE_SRE, MNE_ST0, MNE_ST1, MNE_ST2,
    MNE_STA, MNE_STP, MNE_STX, MNE_STY, MNE_STZ, MNE_SXY,
    /* T */
    MNE_TAI, MNE_TAM, MNE_TAS, MNE_TAX, MNE_TAY, MNE_TDD, MNE_TIA, MNE_TII,
    MNE_TIN, MNE_TMA, MNE_TRB, MNE_TSB, MNE_TST, MNE_TSX, MNE_TXA, MNE_TXS,
    MNE_TYA,
    /* W */
    MNE_WAI

} mnemonic_id_t;


/** \brief  Number of mnemonic IDs
 *
 * This *excludes* the MNE_ILL constant.
 */
#define MNEMONIC_COUNT   (MNE_WAI + 1)


/** \brief  Maximum length of a mnemonic
 */
#define MNEMONIC_MAX_LEN    3


const char *mnemonic_get_text(mnemonic_id_t id);

mnemonic_id_t mnemonic_get_id(const char *text);

#endif
