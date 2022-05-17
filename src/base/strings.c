/** \file   strings.c
 * \brief   Low-level string handling
 * \author  Bas Wassink <b.wassink@ziggo.nl>
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


#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#include "error.h"

#include "strings.h"


/** \brief  Determine if string could contain a numeric literal
 *
 * Check string \a s for a valid numeric literal character.
 *
 * Currently valid are 0-9, % and $.
 *
 * \param[in]   s   string to check
 */
bool str_maybe_int_literal(const char *s)
{
    return (*s == STR_PREFIX_BIN ||
            *s == STR_PREFIX_HEX ||
            isdigit((int)(*s)))
        ? true : false;
}


/** \brief  Parse a string for an integer literal
 *
 * \param[in]   nptr    string to parse
 * \param[out]  endptr  pointer to first non-literal character (optional, use
 *                      `NULL` to ignore)
 * \param[out]  result  resulting integer value
 *
 * \return  `true` on success
 * \throw   BASE_ERR_EMPTY  not a single valid character was encountered
 *                          (besides a possible prefix)
 * \throw   BASE_ERR_RANGE  result is out of range
 */
bool str_parse_int(const char *nptr, char **endptr, int32_t *result)
{
    int base = 10;
    long long res;

    if (*nptr == STR_PREFIX_BIN) {
        base = 2;
        nptr++;
    } else if (*nptr == STR_PREFIX_HEX) {
        base = 16;
        nptr++;
    }

    errno = 0;
    res = strtoll(nptr, endptr, base);
    if ((const char *)(*endptr) == nptr) {
        base_errno = BASE_ERR_EMPTY;
        return false;
    }
    if (errno == ERANGE) {
        base_errno = BASE_ERR_RANGE;
        return false;
    }
    if (res < INT32_MIN || res > INT32_MAX) {
        base_errno = BASE_ERR_RANGE;
        return false;
    }
    *result = (int32_t)res;
    return true;
}



/** \brief  Parse string for a floating point literal
 *
 * \param[in]   nptr    string to parse
 * \param[out]  endptr  pointer to first invalid character
 * \param[out]  result  resulting floating point value
 *
 * \return  `true` on success
 * \throw   BASE_ERR_EMPTY  not a single valid character was encountered
 * \throw   BASE_ERR_RANGE  result is out of range
 */
bool str_parse_double(const char *nptr, char **endptr, double *result)
{
    errno = 0;
    *result = strtod(nptr, endptr);
    if ((const char *)(*endptr) == nptr) {
        base_errno = BASE_ERR_EMPTY;
        return false;
    }
    if (errno == ERANGE) {
        base_errno = BASE_ERR_RANGE;
        return false;
    }
    return true;
}


/** \brief  Find closing quote in string
 *
 * Find closing quote matching s[0], handling escaped characters ('\\').
 *
 * \param[in]   s   string to scan, the first character is used as the closing
 *                  quote character
 *
 * \return  pointer to closing quote or `NULL` when not found
 */
char *str_find_closing_quote(char *s)
{
    int qchar = *s;
    bool escaped = false;

    while (*s != '\0') {
        if (escaped) {
            escaped = false;
        } else if (*s == '\\') {
            escaped = true;
        } else if (*s == qchar) {
            return s;
        }
        s++;
    }
    return NULL;
}







