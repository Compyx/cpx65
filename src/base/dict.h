/** \file   dict.h
 * \brief   Simple dictionary implementation - header
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

#ifndef BASE_DICT_H
#define BASE_DICT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** \brief  Dict type enum
 */
typedef enum dict_type_e {
    DICT_ITEM_ERR = -1, /**< error code */
    DICT_ITEM_INT,      /**< signed integer value */
    DICT_ITEM_STR,      /**< string (allocated/freed by dict */
    DICT_ITEM_PTR,      /**< pointer value */
} dict_type_t;


/** \brief  Dict value type */
typedef void* dict_value_t;

#define DICT_INT_TO_VALUE(x)    (dict_value_t)(intptr_t)(x)
#define DICT_STR_TO_VALUE(x)    (dict_value_t)(x)
#define DICT_PTR_TO_VALUE(x)    (x)

#define DICT_VALUE_TO_INT(x) (int)(intptr_t)(x)
#define DICT_VALUE_TO_STR(x) (const char *)(x)
#define DICT_VALUE_TO_PTR(x) (x)


typedef struct dict_item_s {
    char *key;                  /**< key */
    dict_type_t type;           /**< value type */
    dict_value_t value;         /**< value */
    struct dict_item_s *next;   /**< next item in the list */
    struct dict_item_s *prev;   /**< previous item in the list */
} dict_item_t;



typedef struct dict_s {
    dict_item_t **  items;      /**< hash map with linked lists */
    size_t          size;       /**< size of hash map in entries */
    uint32_t        bits;       /**< size of hash in bits */
    size_t          count;      /**< number of items in dict */
    size_t          collisions; /**< number of hash collisions */
} dict_t;


const char *    dict_type_name(dict_type_t type);

dict_t *        dict_new    (void);

void            dict_free   (dict_t *dict);

bool            dict_set    (dict_t *dict,
                             const char *key,
                             dict_value_t value,
                             dict_type_t type);

bool            dict_get    (const dict_t *dict,
                             const char *key,
                             dict_value_t *value,
                             dict_type_t *type);

bool            dict_del    (dict_t *dict, const char *key);

bool            dict_has_key(const dict_t *dict, const char *key);

const char **   dict_keys   (const dict_t *dict);

#if 0
bool            dict_set_int(dict_t *dict, const char *key, int value);

bool            dict_get_int(const dict_t *dict, const char *key, int *value);
#endif

#endif
