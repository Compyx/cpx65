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
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "debug.h"
#include "error.h"
#include "hash.h"
#include "mem.h"

#include "dict.h"

/** \brief  Hashmap size in bits
 *
 * For now we'll use < 16 and the fnv-1a-tiny hash function.
 */
#define HASHMAP_SIZE_BITS  10u



static const char *type_names[] = {
    "integer",
    "string",
    "pointer"
};


static uint32_t calc_hash(const dict_t *dict, const char *key)
{
    return hash_fnv1_tiny((const uint8_t *)key, strlen(key), dict->bits);
}


/** \brief  Allocate and initialize new dict item
 *
 * Allocate memory for item, copy key name and set item type to 'undefined'.
 *
 * \param[in]   key     item key
 * \param[in]   value   item value
 * \param[in]   type    item type
 *
 * \return  new item or NULL on failure
 *
 * \throw   BASE_ERR_KEY    \a key is NULL or empty
 */
static dict_item_t *dict_item_new(const char *key, dict_value_t value, int type)
{
    dict_item_t *item;

    if (key == NULL || *key == '\0') {
        base_errno = BASE_ERR_KEY;
        return NULL;
    }

    item = base_malloc(sizeof *item);

    item->key = base_strdup(key);
    item->type = type;
    if (type == DICT_ITEM_STR) {
        item->value = base_strdup((const char *)value);
    } else {
        item->value = value;
    }
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
        if (item->type == DICT_ITEM_STR && item->value != NULL) {
            base_free(item->value);
        }
        next = item->next;
        base_free(item);
        item = next;
    }
}


/** \brief  Get dict value type name
 *
 * \param[in]   type    dict value type
 *
 * \return  name, or NULL on invalid \a type
 *
 * \throw   BASE_ERR_ENUM   \a type is invalid
 */
const char *dict_type_name(dict_type_t type)
{
    if (type < 0 || type >= (dict_type_t)base_array_len(type_names)) {
        base_errno = BASE_ERR_ENUM;
        return NULL;
    }
    return type_names[type];
}


/** \brief  Find item by key
 *
 * Find item in \a dict at \a key, optionally storing the hash of the key at
 * \a hash_result.
 * The \a hash_result argument can be used to avoid having to calculate the hash
 * of a key twice (as in dict_remove()), use `NULL` to ignore the hash result.
 *
 * \param[in]   dict        dict
 * \param[in]   key         item key
 * \param[out]  hash_result hash result (optional)
 *
 * \return  item or NULL when not found
 *
 * \throw   BASE_ERR_KEY    \a key is NULL or empty
 */
static dict_item_t *find_item(const dict_t *dict,
                              const char *key,
                              uint32_t *hash_result)
{
    dict_item_t *item;
    uint32_t hash;

    if (key == NULL || *key == '\0') {
        base_errno = BASE_ERR_KEY;
        return NULL;
    }

    hash = calc_hash(dict, key);
    if (hash_result != NULL) {
        *hash_result = hash;
    }

    item = dict->items[hash];
    while (item != NULL) {
        if (strcmp(key, item->key) == 0) {
            break;
        }
        item = item->next;
    }
    return item;
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
    dict->count = 0;
    dict->collisions = 0;

    dict->items = base_malloc(sizeof *(dict->items) * dict->size);
    for (size_t i = 0; i < dict->size; i++) {
        dict->items[i] = NULL;
    }

    return dict;
}


/** \brief  Free dict
 *
 * Free dict, and if any value is a string, free the value as well.
 *
 * \param[in]   dict    dict
 */
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


/** \brief  Set dict item
 *
 * Set dict item \a key to \a value, overwriting the previous value if present.
 *
 * If \a key exists and the item's value is a string the string be deallocated.
 * If \a value is a string a copy will be allocated in the \a dict.
 *
 * \param[in]   dict    dict
 * \param[in]   key     item key
 * \param[in]   value   item value
 * \param[in]   type    item type
 *
 * \return  true on success
 *
 * \throw   BASE_ERR_KEY    \a key is NULL or empty
 * \throw   BASE_ERR_ENUM   \a type is invalid
 */
bool dict_set(dict_t *dict,
              const char *key,
              dict_value_t value,
              dict_type_t type)
{
    uint32_t hash;
    dict_item_t *node;

    if (key == NULL || *key == '\0') {
        base_errno = BASE_ERR_KEY;
        return false;
    }
    if (type < 0 || type > DICT_ITEM_PTR) {
        base_errno = BASE_ERR_ENUM;
        return false;
    }

    hash = calc_hash(dict, key);
    //base_debug("hash of '%s' is 0x%"PRIx32, key, hash);
    node = dict->items[hash];
    if (node == NULL) {
        //base_debug("creating first node of list");
        dict->items[hash] = dict_item_new(key, value, type);
        dict->count++;
    } else {
        /* we've got a hash collision, find tail of linked list to add item or
         * update value of existing item */
        dict_item_t *prev = node->prev;
        base_debug("hash collision, iterating list to add item");
        while (node != NULL) {

            if (strcmp(key, node->key) == 0) {
                /* found key: replace value and update type */
                base_debug("found key, replacing value");

                if (node->type == DICT_ITEM_STR && node->value != NULL) {
                    /* free old string string */
                    base_free(node->value);
                }

                /* set value and type */
                if (type == DICT_ITEM_STR) {
                    node->value = base_strdup(value);
                }
                node->type = type;
                return true;
            }
            node = node->next;
        }

        /* different key but with same hash */
        base_debug("key not found, adding item to list");
        dict->collisions++;
        dict->count++;  /* don't forget this! */

        /* create new node, add to tail of list */
        node = dict_item_new(key, value, type);
        node->prev = prev;
        prev->next = node;

    }

    return true;
}


/** \brief  Retrieve item value from dict
 *
 * Look up \a key in \a dict and set \a value and \a type if found.
 * If \a key isn't found this function returns `false` and \a value is set to
 * `NULL` and \a type is set to `DICT_ITEM_ERR`.
 *
 * \param[in]   dict    dict
 * \param[in]   key     item key
 * \param[out]  value   item value
 * \param[out]  type    item type
 *
 * \return  true if \a key was found
 *
 * \throw   BASE_ERR_KEY    \a key is NULL or empty
 */
bool dict_get(const dict_t *dict,
              const char *key,
              dict_value_t *value,
              dict_type_t *type)
{
    const dict_item_t *item = find_item(dict, key, NULL);

    if (item == NULL) {
        base_errno = BASE_ERR_KEY;
        *type = DICT_ITEM_ERR;
        *value = NULL;
        return false;
    }
    *value = item->value;
    *type = item->type;
    return true;
}


/** \brief  Remove item from dict
 *
 * Remove item at \a key from \a dict, freeing its value if the item's value
 * is a string.
 *
 * \return  true if \a key was found
 *
 * \throw   BASE_ERR_KEY    \a key is NULL or empty
 */
bool dict_remove(dict_t *dict, const char *key)
{
    dict_item_t *item;
    uint32_t hash;

    item = find_item(dict, key, &hash);

    if (item == NULL) {
        return false;
    } else {
        /* unlink item */
        dict_item_t *next = item->next;
        dict_item_t *prev = item->prev;
        dict_item_t *head = dict->items[hash];

        if (prev != NULL) {
            prev->next = next;
        }
        if (next != NULL) {
            next->prev = prev;
        }
        if (item == head) {
            /* item is the head, update head */
            dict->items[hash] = next;
        }

        /* free memory used */
        if (item->type == DICT_ITEM_STR && item->value != NULL) {
            base_free(item->value);
        }
        base_free(item->key);
        base_free(item);

        dict->count--;  /* update item count */
        return true;
    }
}


/** \brief  Determine if a key exists
 *
 * Check if \a key is present in \a dict.
 *
 * \return  bool
 *
 * \throw   BASE_ERR_KEY    \a key is NULL or empty
 */
bool dict_has_key(const dict_t *dict, const char *key)
{
    return find_item(dict, key, NULL) != NULL ? true : false;
}


/** \brief  Get keys in the dict
 *
 * Get an unsorted list of keys in the dict.
 *
 * \note    The list must be freed with base_free() but the keys are owned
 *          by the \a dict and must not be freed.
 *
 * \return  NULL-terminated list of keys in \a dict
 */
const char **dict_keys(const dict_t *dict)
{
    const char **keys;
    size_t index = 0;

    keys = base_malloc(sizeof *keys * (dict->count + 1u));

    for (size_t hash = 0; hash < dict->size; hash++) {
        const dict_item_t *item = dict->items[hash];

        while (item != NULL) {
            keys[index++] = item->key;
            item = item->next;
        }
    }
    keys[index] = NULL;
    return keys;
}
