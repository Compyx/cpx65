/** \file   strlist.c
 * \brief   Heap-allocated list of strings
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

#include "debug.h"
#include "mem.h"

#include "strlist.h"


/** \brief  Initial size of a string list
 */
#define STRLIST_INITIAL_SIZE    4


/** \brief  Create a string list
 *
 * \return  new string list
 */
strlist_t *strlist_init(void)
{
    strlist_t *slist = base_malloc(sizeof *slist);
    slist->items = base_malloc(STRLIST_INITIAL_SIZE * sizeof *(slist->items));
    slist->items_max = STRLIST_INITIAL_SIZE;
    slist->items_used = 0;

    for (size_t i = 0; i < STRLIST_INITIAL_SIZE; i++) {
        slist->items[i] = NULL;
    }
    return slist;
}


/** \brief  Clean up memory used by \a slist
 *
 * \param[in,out]   slist   string list
 */
void strlist_free(strlist_t *slist)
{
    for (size_t i = 0; i < slist->items_used; i++) {
        base_free(slist->items[i]);
    }
    base_free(slist->items);
    base_free(slist);
}


/** \brief  Get number of items in \a slist
 *
 * \param[in]   slist   string list
 *
 * \return  number of items in the list
 */
size_t strlist_len(const strlist_t *slist)
{
    return slist->items_used;
}


/** \brief  Add string \a s to string list \a slist
 *
 * \param[in,out]   slist   string list
 * \param[in]       s       string to add
 */
void strlist_add(strlist_t *slist, const char *s)
{
    if (s == NULL) {
        base_debug("got NULL, ignoring.");
        return;
    }
    if (*s == '\0') {
        base_debug("got empty string, ignoring.");
        return;
    }

    if (slist->items_max == slist->items_used) {
        slist->items_max *= 2;
        base_debug("Resizing string list to %zu items",
                slist->items_max);
        slist->items = base_realloc(slist->items,
                                    slist->items_max * sizeof *(slist->items));
    }
    base_debug("adding item '%s' at index %zu.\n",
            s, slist->items_used);
    slist->items[slist->items_used++] = base_strdup(s);
}


/** \brief  Debug hook: dump contents of \a slist on stdout
 *
 * \param[in]   slist   string list
 */
void strlist_dump(const strlist_t *slist)
{
    for (size_t i = 0; i < slist->items_used; i++) {
        printf("[%zu] = '%s'\n", i, slist->items[i]);
    }
}


/** \brief  Create a deep copy of \a slist
 *
 * \param[in]   slist   string list
 *
 * \return  heap-allocated deep copy of \a slist
 */
strlist_t *strlist_dup(const strlist_t *slist)
{
    strlist_t *newlist = base_malloc(sizeof *newlist);
    newlist->items = base_malloc(slist->items_max * sizeof *(newlist->items));
    newlist->items_max = slist->items_max;
    newlist->items_used = slist->items_used;

    for (size_t i = 0; i < newlist->items_used; i++) {
        newlist->items[i] = base_strdup(slist->items[i]);
    }
    return newlist;
}


/** \brief  Get item in strlist by index
 *
 * \param[in]   slist   string list
 * \param[in]   index   index in \a slist
 *
 * \return  item or `NULL` on when \a index is out of bounds
 */
const char *strlist_item(const strlist_t *slist, size_t index)
{
    if (index >= slist->items_used) {
        return NULL;
    }
    return slist->items[index];
}

