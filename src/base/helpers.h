/** \file   helpers.h
 * \brief   Helper macros
 */

#ifndef BASE_HELPERS_H
#define BASE_HELPERS_H

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

/** \brief  Determine number of elements in \a ARR
 *
 * Only works on proper arrays, for example:
 * \code
 *  int foo[] = { 0, 1, 2, 3 };
 *  printf("%zu\n", BASE_ARRAY_SIZE(foo));  // prints 4
 * \endcode
 *
 * \param[in]   ARR array
 */
#define BASE_ARRAY_SIZE(ARR) (sizeof ARR / sizeof ARR[0])

#endif
