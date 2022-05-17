/** \file   strings.h
 * \brief   Low-level string handling - header
 * \author  Bas Wassink <b.wassink@ziggo.nl>
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

#ifndef BASE_STRINGS_H
#define BASE_STRINGS_H

#include <stdbool.h>
#include <stdint.h>

/** \brief  Prefix of a binary integer literal
 */
#define STR_PREFIX_BIN  '%'

/** \brief  Prefix of a hexadecimal integer literal
 */
#define STR_PREFIX_HEX  '$'


bool str_maybe_int_literal(const char *s);
bool str_parse_int(const char *nptr, char **endptr, int32_t *value);
bool str_parse_double(const char *nptr, char **endptr, double *result);
char *str_find_closing_quote(char *s);

#endif
