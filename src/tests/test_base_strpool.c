/** \file   test_base_strpool.c
 * \brief   Unit tests for base/strpool.c
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

#include "test_base_strpool.h"



/*
 * Forward declarations
 */

static bool setup(void);
static bool teardown(void);


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

/* FIXME: Write proper test */
static bool test_add(testcase_t *self)
{
    size_t i;
    void *obj;

    for (i = 0; i < sizeof list1 / sizeof list1[0]; i++) {
        printf("... adding '%s':\n", list1[i]);
        obj = strpool_add(list1[i]);
        used_objects_add(obj);
    }

    strpool_dump_stats();

    testcase_pass(self);
    return true;
}



static bool test_del(testcase_t *self)
{
    void *obj;

    obj = used_objects_list[0];
    strpool_del(obj);
    used_objects_list[0] = NULL;

    testcase_pass(self);
    return true;
}


/** \brief  Create test group 'base/strpool'
 *
 * \return  test group
 */
testgroup_t *get_base_strpool_tests(void)
{
    testgroup_t *group;
    testcase_t *test;

    group = testgroup_new("base/strpool",
                          "Test the string pool module",
                          setup, teardown);

    test = testcase_new("add",
                        "Test adding items to a string pool",
                        1, test_add, NULL, NULL);
    testgroup_add_case(group, test);

    test = testcase_new("del",
                        "Test deleting items from a string pool",
                        1, test_del, NULL, NULL);
    testgroup_add_case(group, test);

    return group;
}
