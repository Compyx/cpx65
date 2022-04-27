/** \file   convert.h
 * \brief   Type conversion macros
 * \ingroup base
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

#ifndef BASE_CONVERT_H
#define BASE_CONVERT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/** \brief  Convert pointer \a P to short
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_short(P) ((short)(intptr_t)(P))


/** \brief  Convert pointer \a P to unsigned short
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_ushort(P) ((unsigned short)(uintptr_t)(P))


/** \brief  Convert pointer \a P to int
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_int(P) ((int)(intptr_t)(P))


/** \brief  Convert pointer \a P to unsigned int
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_uint(P) ((unsigned int)(uintptr_t)(P))


#endif
