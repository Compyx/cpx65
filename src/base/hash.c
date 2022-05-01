/** \file   hash.c
 * \brief   various hash functions
 *
 * FNV-1a hash: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a
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

#include <stddef.h>
#include <stdint.h>

#include "hash.h"

/** \brief  Prime for the 32-bit FNV-1(a) hash */
#define FNV1_PRIME_32   ((2u << 24u) + (2u << 8u) + 0x93u)

/** \brief  Offset for the 32-bit FNV-1(a) hash */
#define FNV1_OFFSET_32  2166136261u

/** \brief  Mask for the 16-bit FNV-1(a) hash
 *
 * Mask used to xor-fold a 32-bit hash result into 16 bits
 */
#define FNV1_MASK_16    ((1u << 16u) - 1u)


/** \brief  Calculate 32-bit FNV-1a hash
 *
 * Calculate 32-bit FNV-1a hash of \a size bytes of \a data.
 *
 * \param[in]   data    data to hash
 * \param[in]   size    number of bytes of \a data to process
 *
 * \return  32-bit FNV-1a hash
 */
uint32_t hash_fnv1_32(const uint8_t *data, size_t size)
{
    uint32_t hash = FNV1_OFFSET_32;

    while (size-- > 0) {
        hash ^= *data++;
        hash *= FNV1_PRIME_32;
    }
    return hash;
}


/** \brief  Calculate 16-bit FNV-1a hash
 *
 * Calculate 16-bit FNV-1a hash of \a size bytes of \a data.
 *
 * Uses the 32-bit hash and xor-folds the result into 16 bits.
 *
 * \param[in]   data    data to hash
 * \param[in]   size    number of bytes of \a data to process
 *
 * \return  16-bit FNV-1a hash
 */
uint32_t hash_fnv1_16(const uint8_t *data, size_t size)
{
    uint32_t hash = hash_fnv1_32(data, size);

    return (hash >> 16u) ^ (hash & FNV1_MASK_16);
}


/** \brief  Calculate FNV-1a hash of less than 16 bits
 *
 * Calculate \a bits bit FNV-1a hash of \a size bytes of \a data.
 *
 * Uses the 32-bit hash and xor-folds and 
 *
 * \param[in]   data    data to hash
 * \param[in]   size    number of bytes of \a data to process
 * \param[in]   bits    size of hash in bits (\< 16)
 *
 * \return  FNV-1a hash of \a bits size
 */

uint32_t hash_fnv1_tiny(const uint8_t *data, size_t size, uint32_t bits)
{
    uint32_t hash = hash_fnv1_32(data, size);

    return ((hash >> bits) ^ hash) & ((1u << bits) - 1u);
}
