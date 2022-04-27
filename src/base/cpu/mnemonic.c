/** \file   mnemonic.c
 * \brief   Mnemonics
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#include "../debug.h"

#include "mnemonic.h"

/** \brief  Mnemonics
 *
 * Huc6280 specific:
 *
 *  brs     BRanch to Subroutine
 *  cla     CLear Accu
 *  clx     CLear X
 *  cly     CLear Y
 *  csh     Chabge Speed High
 *  csl     Change Speed Low
 *  sax     Swap A and X (different from 6502's illegal opcode)
 *  say     Swap A and Y
 *  st0     STore vdc 0
 *  st1     STore vcd 1
 *  st2     STore vdc 2
 *  sxy     Swap X and Y
 *  tai     Transfer Alternate Increment
 *  tam     Transfer Accu rto MPRs
 *  tdd     Transfer Decrement Decrement
 *  tia     Transfer Increment Alternate
 *  tii     Transfer Increment Increment
 *  tin     Transfer INcrement
 *  tma     Transfer MPR to Accu
 *  tst     TeST
 */
static const char *mnemonic_text[] = {
    /* A */
    "adc", "alr", "anc", "and", "ane", "arr", "asl",
    /* B */
    "bbr", "bbr0", "bbr1", "bbr2", "bbr3", "bbr4", "bbr5", "bbr6", "bbr7",
    "bbs", "bbs0", "bbs1", "bbs2", "bbs3", "bbs4", "bbs5", "bbs6", "bbs7",
    "bcc", "bcs", "beq", "bit", "bmi", "bne", "bpl", "bra", "brk", "bsr",
    "bvc", "bvs",
    /* C */
    "cla", "clc", "cld", "cli", "clv", "clx", "cly", "cmp", "csh", "csl",
    "cpx", "cpy",
    /* D */
    "dcp", "dea", "dec", "dex", "dey",
    /* E */
    "eor",
    /* I */
    "ina", "inc", "inx", "iny", "isc",
    /* J */
    "jam", "jmp", "jsr",
    /* L */
    "las", "lax", "lda", "ldx", "ldy", "lsr",
    /* N */
    "nop",
    /* O */
    "ora",
    /* P */
    "pha", "php", "phx", "phy", "pla", "plp", "plx", "ply",
    /* R */
    "rla", "rmb", "rmb0", "rmb1", "rmb2", "rmb3", "rmb4", "rmb5", "rmb6",
    "rmb7", "rol", "ror", "rra", "rti", "rts",
    /* S */
    "sax", "sax", "sbc", "sbx", "sec", "sed", "sei", "set", "sha", "shx", "shy",
    "slo", "smb", "smb0", "smb1", "smb2", "smb3", "smb4", "smb5", "smb6",
    "smb7", "sre",
    "st0", "st1", "st2",    /* Huc6270 VDC control */
    "sta", "stp", "stx", "sty", "stz", "sxy",
    /* T */
    "tai", "tam", "tas", "tax", "tay", "tdd", "tia", "tii", "tin", "tma",
    "trb", "tsb", "tst", "tsx", "txa", "txs", "tya",
    /* W */
    "wai"
};


/** \brief  Get mnemonic text for mnemonic ID
 *
 * \param[in]   id  mnemonic ID
 *
 * \return  mnemonic text (lower case)
 *
 * \note    returns '???' for \a id out of bounds, may have to change that
 */
const char *mnemonic_get_text(mnemonic_id_t id)
{
    if (id < 0 || id >= MNEMONIC_COUNT) {
        return "???";
    }
    return mnemonic_text[id];
}


/** \brief  bsearch 'compar' function for mnemonic_get_id()
 *
 * \param[in]   p1  key
 * \param[in]   p2  current element
 *
 * \return  0 if equal, <1 if a<b, >2 if a>b
 */
static int mne_compar(const void *p1, const void *p2)
{
    const char *s1 = p1;
    const char * const *s2 = p2;
    return strcmp(s1, *s2);
}

/** \brief  Get Mnemonic ID for \a text
 *
 * \param[in]   text    mnemonic text
 *
 * \return  mnemonic ID or -1 on error
 */
mnemonic_id_t mnemonic_get_id(const char *text)
{
    char mne[MNEMONIC_MAX_LEN + 1];
    int i;
    void *result;

    printf("sizeof(mnemonic_id_t) = %zu\n", sizeof(mnemonic_id_t));
    printf("sizeof(ptrdiff_t) = %zu\n", sizeof(ptrdiff_t));

    /* convert to lower case */
    for (i = 0; *text != '\0' && i < MNEMONIC_MAX_LEN; i++) {
        mne[i] = (char)tolower((int)(text[i]));
    }
    mne[i] = '\0';

    /* use bsearch to find mnemonic */
    result = bsearch(
            (const void *)mne,
            (const void *)mnemonic_text,
            sizeof mnemonic_text / sizeof mnemonic_text[0],
            sizeof mnemonic_text[0],
            mne_compar);
    if (result == NULL) {
        return -1;
    }
    return (const char **)result - mnemonic_text;
}
