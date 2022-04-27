/** \file   strpool.h
 * \brief   Module to reuse heap-allocated strings - header
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

#ifndef BASE_STRPOOL_H
#define BASE_STRPOOL_H

#include "objpool.h"


/** \brief  String pool object
 *
 * The memory allocated for \c text will be increased to the next power of two
 * when len+1 isn't a power of two.
 */
typedef struct strpool_obj_s {
    OBJ_POOL_OBJ_BASE
    char *  text;   /**< text */
    size_t  size;   /**< number of bytes allocated text */
    size_t  len;    /**< actual string length of text */
} strpool_obj_t;

void strpool_init(void);
void strpool_free(void);
void strpool_dump_stats(void);

void *strpool_add(char *text);
void strpool_del(void *obj);


#endif
