/** \file   dict.c
 * \brief   Simple dictionary implementation
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
#include <stdbool.h>
#include <string.h>

#include "mem.h"
#include "hash.h"

#include "dict.h"

/** \brief  Hashmap size in bits
 *
 * For now we'll use < 16 and the fnv-1a-tiny hash function.
 */
#define HASHMAP_SIZE_BITS  10u


static dict_item_t *dict_item_new(const char *key)
{
    dict_item_t *item;

    if (key == NULL || *key == '\0') {
        return NULL;
    }

    item = base_malloc(sizeof *item);

    /* TODO: implement base_strndup() to avoid scanning key twice */
    item->key = base_strdup(key);
    item->klen = strlen(key);
    item->type = DICT_ITEM_UNDEFINED;
    item->next = NULL;
    item->prev = NULL;

    return item;
}


static void dict_item_free(dict_item_t *item)
{
    while (item != NULL) {
        dict_item_t *next = item->next;

        if (item->key != NULL) {
            base_free(item->key);
        }
        if (item->type == DICT_ITEM_STRING && item->value.str != NULL) {
            base_free(item->value.str);
        }
        next = item->next;
        base_free(item);
        item = next;
    }
}


/** \brief  Create new empty dict
 *
 * Create an empty dict object and initialize to empty.
 *
 * \todo    Perhaps allow setting a hashmap size?
 *
 * \return  new dict
 */
dict_t *dict_new(void)
{
    dict_t *dict = base_malloc(sizeof *dict);

    dict->bits = HASHMAP_SIZE_BITS;
    dict->size = 1u << dict->bits;

    dict->items = base_malloc(sizeof *(dict->items) * dict->size);
    for (size_t i = 0; i < dict->size; i++) {
        dict->items[i] = NULL;
    }

    return dict;
}


void dict_free(dict_t *dict)
{
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->items[i] != NULL) {
            dict_item_free(dict->items[i]);
        }
    }
    base_free(dict->items);
    base_free(dict);
}
