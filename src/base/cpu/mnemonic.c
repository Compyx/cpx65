/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   mnemonic.c
 * \brief   Mnemonics
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


static const char *mnemonic_text[] = {
    /* A */
    "adc", "alr", "anc", "and", "ane", "arr", "asl",
    /* B */
    "bbr", "bbr0", "bbr1", "bbr2", "bbr3", "bbr4", "bbr5", "bbr6", "bbr7",
    "bbs", "bbs0", "bbs1", "bbs2", "bbs3", "bbs4", "bbs5", "bbs6", "bbs7",
    "bcc", "bcs", "beq", "bit", "bmi", "bne", "bpl", "bra", "brk", "bvc",
    "bvs",
    /* C */
    "clc", "cld", "cli", "clv", "cmp", "cpx", "cpy",
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
    "sax", "sbc", "sbx", "sec", "sed", "sei", "sha", "shx", "shy", "slo",
    "smb", "smb0", "smb1", "smb2", "smb3", "smb4", "smb5", "smb6", "smb7",
    "sre", "sta", "stp", "stx", "sty", "stz",
    /* T */
    "tas", "tax", "tay", "trb", "tsb", "tsx", "txa", "txs", "tya",
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

