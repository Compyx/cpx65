/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   addrmode.c
 * \brief   Addressing modes
 */

#include <stdlib.h>
#include <stdio.h>

#include "../helpers.h"

#include "addrmode.h"


/** \brief  Addressing mode descriptions
 */
static const addrmode_desc_t addrmode_descriptions[] = {
    { "imp", "implied" },
    { "acc", "accumulator" },
    { "imm", "immediate" },
    { "zpg", "zero page" },
    { "zpx", "zero page,X" },
    { "zpy", "zero page,Y" },
    { "izx", "(zero page,X)" },
    { "izy", "(zero page),Y" },
    { "abs", "absolute" },
    { "abx", "absolute,X" },
    { "aby", "absolute,Y" },
    { "iab", "(absolute)" },
    { "rel", "relative" },
    /* 65C02 additions */
    { "izp", "(zero page)" },
    { "iax", "(absolute,X)" },
    { "zpr", "zero page, branch" },
    /* HuC6280 additions */
    { "blt", "block transfer" },
    { "ima", "immediate, absolute" },
    { "imz", "immediate, zero page" }
};


/** \brief  Size of the addressing mode descriptions array
 */
#define ADDRMODE_DESC_ARRSIZE BASE_ARRAY_SIZE(addrmode_descriptions)


/** \brief  Get three-letter addressing mode description for \a amd
 *
 * \param[in]   amd addressing mode ID
 *
 * \return  description, or "ill" when \a amd is out of range
 */
const char *addrmode_get_desc_enum(addrmode_id_t amd)
{
    if (amd < 0 ||
            amd >= (addrmode_id_t)ADDRMODE_DESC_ARRSIZE) {
        return "ill";
    }
    return addrmode_descriptions[amd].desc_enum;
}



/** \brief  Get 'example' addressing mode description for \a amd
 *
 * \param[in]   amd addressing mode ID
 *
 * \return  description, or "illegal" when \a amd is out of range
 */
const char *addrmode_get_desc_example(addrmode_id_t amd)
{
    if (amd < 0 ||
            amd >= (addrmode_id_t)ADDRMODE_DESC_ARRSIZE) {
        return "illegal";
    }
    return addrmode_descriptions[amd].desc_example;
}

