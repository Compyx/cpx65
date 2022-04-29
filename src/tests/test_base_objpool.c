/** \file   test_base_objpool.c
 * \brief   Unit tests for base/objpool.c
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../base/debug.h"
#include "unit.h"
#include "../base/mem.h"
#include "../base/strlist.h"
#include "../base/objpool.h"

#include "test_objpool.h"


/** \brief  Minimal amount of memory to allocate
 */
#define MIN_ALLOC   16


/** \brief  Object pool test object
 *
 * A simple 'string' object
 */
typedef struct string_obj_test_s {
    /* required 'base class' members */
    OBJ_POOL_OBJ_BASE
    char *  text;   /**< memory allocated for a string */
    size_t  size;   /**< size of memory allocated for 'text' */
    size_t  len;    /**< actual length of string in 'text' */
} string_obj_test_t;




/*
 * Forward declarations
 */

static bool setup(void);
static bool teardown(void);

static bool test_init(int *total, int *passed);
static bool test_add_item(int *total, int *passed);
static bool test_item_reuse(int *total, int *passed);

/*
 * Object callbacks
 */
static void *   pool_obj_alloc(void *param);
static void *   pool_obj_reuse(void *obj, void *param);
static void     pool_obj_free(void *obj);
static size_t   pool_obj_size(void *obj);


static unit_test_t tests[] = {
    { "init", "Test initializing and freeing an objpool", test_init, false },
    { "add-item", "Test adding item to an empty objpool", test_add_item, false },
    { "item-reuse", "Test reusing items", test_item_reuse, false },
    { NULL, NULL, NULL, false }
};


unit_module_t objpool_module = {
    "objpool",
    "Test object pool handling",
    setup, teardown,
    0, 0,
    tests
};


static objpool_t pool_test = {
    NULL, 8, 0,
    NULL, 4, 0,
    pool_obj_alloc,
    pool_obj_reuse,
    pool_obj_free,
    pool_obj_size,
    0, 0, 0, 0
};


/*
 * Implementation of the test object, a string
 */


static void *pool_obj_alloc(void *param)
{
    string_obj_test_t *obj;
    size_t len;
    size_t size;

    base_debug("Called.");

    len = strlen((const char *)param);

    obj = base_malloc(sizeof *obj);

    if (len + 1 < MIN_ALLOC) {
        size = MIN_ALLOC;
    } else {
        size = base_nextpow2(len + 1);
    }


    obj->text = base_malloc(size);

    memcpy(obj->text, param, len + 1);

    obj->size = size;
    obj->len = len;
    return obj;
}


static void *pool_obj_reuse(void *obj, void *param)
{
    base_debug("Called.");
    (void)obj;
    (void)param;
    return NULL;
}


static void pool_obj_free(void *obj)
{
    string_obj_test_t *s = obj;

    base_debug("Called.");

    if (s->text != NULL) {
        base_free(s->text);
    }
    base_free(obj);
}


static size_t pool_obj_size(void *obj)
{
    base_debug("Called.");
    (void)obj;
    return 0;
}


static void *string_obj_test_alloc(char *text)
{
    size_t len = strlen(text);
    void *obj;

    base_debug("Called with '%s' (len = %zu).", text, len);
    obj = objpool_request(&pool_test, len + 1u, text);

    return obj;
}



static bool setup(void)
{
    objpool_init(&pool_test);
    return true;
}

static bool teardown(void)
{
    objpool_free(&pool_test);
    return true;
}


/*
 * Unit tests
 */

static bool test_init(int *total, int *passed)
{
    *total += 1;
    *passed += 1;
    return true;
}


static bool test_add_item(int *total, int *passed)
{
    bool result = true;
    void *obj;

    *total += 1;

    obj = objpool_request(&pool_test, 0 , "Compyx!");

    printf("Object content: '%s'\n", ((string_obj_test_t *)obj)->text);

    *passed += 1;

    printf("Dumping objpool stats:\n\n");
    objpool_dump_stats(&pool_test);
    return result;
}


static bool test_item_reuse(int *total, int *passed)
{
    void *ptrs[1024];
    int i;
    int k;
    char *strings[]= {
        "compyx", "fucking", "rules", "and", "don't you forget it",
        "you bastardo!", "pummekl", "piemel", NULL
    };


    /* wipe ptrs to avoid weird stuff */
    for (i = 0; i < 1024; i++) {
        ptrs[i] = NULL;
    }

    /* add strings to the pool */
    for (i = 0; strings[i] != NULL; i++) {
        printf(".. adding '%s' to pool\n", strings[i]);
        ptrs[i] = string_obj_test_alloc(strings[i]);
    }

    /* dump strings on stdout
     *
     * format: [index:size:len] "string content"
     */
    for (k = 0; k < i; k++) {
        string_obj_test_t *s = ptrs[k];
        printf(".. [%04d:%04zu:%04zu \"%s\"\n",
                k, s->size, s->len, s->text);
    }

    objpool_dump_stats(&pool_test);

    *total = i;
    *passed = i;
    return true;
}