/** \file   addrmode.c
 * \brief   Addressing modes
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
#include <stdbool.h>

#include "../helpers.h"

#include "addrmode.h"


/** \brief  Addressing mode descriptions
 */
static const addrmode_info_t addrmode_info[] = {
    { "imp",    0,  { 0, 0, 0, 0 }, "implied" },
    { "acc",    0,  { 0, 0, 0, 0 }, "accumulator" },
    { "imm",    1,  { 1, 0, 0, 0 }, "immediate" },
    { "zpg",    1,  { 1, 0, 0, 0 }, "zero page" },
    { "zpx",    1,  { 1, 0, 0, 0 }, "zero page,X" },
    { "zpy",    1,  { 1, 0, 0, 0 }, "zero page,Y" },
    { "izx",    1,  { 1, 0, 0, 0 }, "(zero page,X)" },
    { "izy",    1,  { 1, 0, 0, 0 } ,"(zero page),Y" },
    { "abs",    1,  { 2, 0, 0, 0 }, "absolute" },
    { "abx",    1,  { 2, 0, 0, 0 }, "absolute,X" },
    { "aby",    1,  { 2, 0, 0, 0 }, "absolute,Y" },
    { "iab",    1,  { 2, 0, 0, 0 }, "(absolute)" },
    { "rel",    1,  { 1, 0, 0, 0 }, "relative" },
    /* 65C02 additions */
    { "izp",    1,  { 1, 0, 0, 0 }, "(zero page)" },
    { "iax",    1,  { 2, 0, 0, 0 }, "(absolute,X)" },
    { "zpr",    2,  { 1, 1, 0, 0 }, "zero page, branch" },
    /* HuC6280 additions */
    { "blk",    3,  { 2, 2, 2, 0 }, "block transfer: src, dest, len" },
    { "ima",    2,  { 1, 2, 0, 0 }, "immediate, absolute" },
    { "imax",   2,  { 1 ,2, 0, 0 }, "immediate, absolute,X" },
    { "imz",    2,  { 1, 1, 0, 0 }, "immediate, zero page" },
    { "imzx",   2,  { 1, 1, 0, 0 }, "immediate, zero page,X" }
};


/** \brief  Size of the addressing mode descriptions array
 */
#define ADDRMODE_ARRSIZE BASE_ARRAY_SIZE(addrmode_info)


/** \brief  Determine if \a amd is a valid addressing mode ID
 *
 * \param[in]   amd addressing mode ID
 *
 * \return  bool
 */
bool addrmode_is_valid(addrmode_id_t amd)
{
    return (bool)(amd >= 0 && amd < (addrmode_id_t)ADDRMODE_ARRSIZE);
}


/** \brief  Get three-letter addressing mode description for \a amd
 *
 * \param[in]   amd addressing mode ID
 *
 * \return  description, or "ill" when \a amd is out of range
 */
const char *addrmode_get_desc(addrmode_id_t amd)
{
    if (!addrmode_is_valid(amd)) {
        return "ill";
    }
    return addrmode_info[amd].desc;
}


/** \brief  Get 'example' addressing mode description for \a amd
 *
 * \param[in]   amd addressing mode ID
 *
 * \return  description, or "illegal" when \a amd is out of range
 */
const char *addrmode_get_example(addrmode_id_t amd)
{
    if (!addrmode_is_valid(amd)) {
        return "illegal";
    }
    return addrmode_info[amd].example;
}


/** \brief  Get operand count for addressing mode \a amd
 *
 * \param[in]   amd     addressing mode ID
 *
 * \return  operand count or -1 on error
 */
int addrmode_get_opr_count(addrmode_id_t amd)
{
    if (!addrmode_is_valid(amd)) {
        return -1;
    }
    return addrmode_info[amd].oprcount;
}


/** \brief  Get operand sizes for addressing mode \a amd
 *
 * \param[in]   amd     addressing mode ID
 *
 * \return  pointer to array of sizes or `NULL` on error
 */
const int *addrmode_get_opr_sizes(addrmode_id_t amd)
{
    if (!addrmode_is_valid(amd)) {
        return NULL;
    }
    return addrmode_info[amd].oprsizes;
}
