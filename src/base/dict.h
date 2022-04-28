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


enum {
    DICT_ITEM_UNDEFINED,    /**< item is not defined */
    DICT_ITEM_INTEGER,      /**< signed integer value */
    DICT_ITEM_DOUBLE,       /**< double value */
    DICT_ITEM_STRING,       /**< string (allocated/freed by dict */
    DICT_ITEM_POINTER,      /**< pointer value */
};


typedef struct dict_item_s {
    char *  key;        /**< key */
    size_t  klen;       /**< length of string \a key */
    int     type;       /**< value type */
    union {
        char *  str;    /**< string, allocated/freed by the dict */
        int     ival;   /**< integer value */
        double  dbl;    /**< double value */
        void *  ptr;    /**< pointer value */
    } value;            /**< value */
    struct dict_item_s *next;   /**< next item in the list */
    struct dict_item_s *prev;   /**< previous item in the list */
} dict_item_t;



typedef struct dict_s {
    dict_item_t **  items;  /**< hash map with linked lists */
    size_t          size;   /**< size of hash map in entries */
    uint32_t        bits;   /**< size of hash in bits */
} dict_t;


dict_t *dict_new    (void);
void    dict_free   (dict_t *dict);

bool    dict_set_int(dict_t *dict, const char *key, int value);
bool    dict_get_int(dict_t *dict, const char *key, int *value);
bool    dict_delete (dict_t *dict, const char *key);


#endif
