/** \file   strlist.h
 * \brief   Heap-allocated list of strings - header
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

#ifndef BASE_STRLIST_H
#define BASE_STRLIST_H

#include <stdlib.h>


/** \brief  String list handle
 */
typedef struct strlist_s {
    char **items;       /**< list of strings */
    size_t items_max;   /**< size of the list */
    size_t items_used;  /**< number of items used in the list */
} strlist_t;


strlist_t * strlist_init(void);
void        strlist_free(strlist_t *list);
strlist_t * strlist_dup(const strlist_t *slist);
size_t      strlist_len(const strlist_t *slist);
void        strlist_add(strlist_t *slist, const char *s);
void        strlist_dump(const strlist_t *slist);
const char *strlist_item(const strlist_t *slist, size_t index);

#endif
