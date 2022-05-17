/** \file   error.h
 * \brief   Error hanlding/reporting - header
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

#ifndef BASE_ERROR_H
#define BASE_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/** \brief  Error codes
 */
enum {
    BASE_ERR_UNKNOWN = -1,  /**< unknown error code */
    BASE_ERR_OK = 0,        /**< OK */
    BASE_ERR_ALLOC,         /**< memory allocation error */
    BASE_ERR_IO,            /**< I/O error */
    BASE_ERR_INVALID_SIZE,  /**< invalid size */
    BASE_ERR_NULL,          /**< unexpected NULL value */
    BASE_ERR_EMPTY,         /**< something shouldn't be empty */
    BASE_ERR_KEY,           /**< key error */
    BASE_ERR_INDEX,         /**< index error */
    BASE_ERR_ENUM,          /**< value is not valid for enum */
    BASE_ERR_RANGE          /**< out of range */
};

/** \brief  Print error code and message on stderr
 *
 * Prints message in the form: "<file>:<line>:<func>: <errcode> <errmsg>".
 * Also prints libc's errno and strerror(errno) in cases where libc might set
 * an error code, such as I/O errors.
 */
#define base_perror(void)                               \
    do {                                                \
        fprintf(stderr,                                 \
                "%s:%d:%s(): %d: %s",                   \
                __FILE__, __LINE__, __func__,           \
                base_errno, base_strerror(base_errno)); \
        if (base_errno == BASE_ERR_IO) {                \
            fprintf(stderr,                             \
                    " (%d: %s)",                        \
                    errno, strerror(errno));            \
        }                                               \
        fputc('\n', stderr);                            \
    } while (0);

extern int base_errno;

const char *base_strerror(int err);

#endif
