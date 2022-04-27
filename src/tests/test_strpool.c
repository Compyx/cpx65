/** \file   test_strpool.c
 * \brief   Unit tests for strpool.c
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
#include "../base/strpool.h"

#include "test_strpool.h"



/*
 * Forward declarations
 */

static bool setup(void);
static bool teardown(void);

static bool test_add(int *total, int *passed);
static bool test_del(int *total, int *passed);


static char *list1[] = {
    "compyx",
    "fucking",
    "rules",
    "and",
    "that's",
    "true",
    "also large string",
    "more",
    "bla",
    "iweurowieuroiuweorewr",
    "erwerwer"
};



static unit_test_t tests[] = {
    { "add", "Test adding strings", test_add, false },
    { "del", "Test deleting strings", test_del, false },
    { NULL, NULL, NULL, false }
};


unit_module_t strpool_module = {
    "strpool",
    "Test string pool handling",
    setup, teardown,
    0, 0,
    tests
};


#define USED_OBJLIST_SIZE   64

static void *used_objects_list[USED_OBJLIST_SIZE];
static size_t used_objects_num = 0;


static void used_objects_add(void *obj)
{
    if (used_objects_num == USED_OBJLIST_SIZE) {
        fprintf(stderr, "%s:%d:%s(): Error: object list full, aborting!\n",
                __FILE__, __LINE__, __func__);
        abort();
    }

    used_objects_list[used_objects_num++] = obj;
}





static bool setup(void)
{
    strpool_init();
    return true;
}

static bool teardown(void)
{
    strpool_free();
    return true;
}


/*
 * Unit tests
 */

static bool test_add(int *total, int *passed)
{
    size_t i;
    void *obj;

    (*total)++;

    for (i = 0; i < sizeof list1 / sizeof list1[0]; i++) {
        printf(".... adding '%s':\n", list1[i]);
        obj = strpool_add(list1[i]);
        used_objects_add(obj);
    }

    strpool_dump_stats();

    (*passed)++;
    return true;
}



static bool test_del(int *total, int *passed)
{
    void *obj;

    (*total)++;

    obj = used_objects_list[0];
    strpool_del(obj);
    used_objects_list[0] = NULL;


    (*passed)++;
    return true;
}
