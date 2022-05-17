/** \file   error.c
 * \brief   Error hanlding/reporting
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "error.h"


/** \brief  List of error messages
 */
static const char *err_msgs[] = {
    "<unknown error>",
    "OK",
    "memory allocation error",
    "I/O error",
    "invalid size",
    "cannot be NULL",
    "cannot be empty",
    "key error",
    "index error",
    "invalid enum value",
    "out of range"
};


/** \brief  Error code for the various binaries
 */
int base_errno = 0;


/** \brief  Get error message for error code \a err
 *
 * \param[in]   err error code
 * \return  string
 */
const char *base_strerror(int err)
{
    if (err < 0 || err >= (int)(sizeof err_msgs / sizeof err_msgs[0])) {
        return err_msgs[0];
    }
    return err_msgs[err + 1];
}
