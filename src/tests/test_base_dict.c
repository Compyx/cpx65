/** \file   test_base_dict.c
 * \brief   Unit tests for base/dict.c
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
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "testcase.h"
#include "../base/dict.h"

#include "test_base_dict.h"


/** \brief  Dict object for tests
 *
 * Created in setup() before a test case and freed in teardown() after a test case.
 */
static dict_t *dict = NULL;


static bool setup(void)
{
    dict = dict_new();
    return true;
}

static bool teardown(void)
{
    if (dict != NULL) {
        dict_free(dict);
        dict = NULL;
    }
    return true;
}


/** \brief  Test dict_new()/dict_free()
 *
 * \param[in]   self    test case
 *
 * \return  false on fatal error
 */
static bool test_new_free(testcase_t *self)
{
    dict_t *tmpdict;
    bool result;

    printf("... creating new dict with dict_new()\n");
    tmpdict = dict_new();
    printf("... dict hashmap size = %zu (%"PRIu32" bits)\n",
           tmpdict->size, tmpdict->bits);
    printf("... checking if hashmap size (%zu) matches (1 << hashmap bits)"
           " (%"PRIu32") ..\n",
           tmpdict->size, 1u << tmpdict->bits);
    testcase_assert_equal(self, (int)(tmpdict->size), (int)(1u << tmpdict->bits));
    printf("... checking if all hashmap entries are properly initialized ..\n");
    result = true;
    for (size_t i = 0; i < tmpdict->size; i++) {
        if (tmpdict->items[i] != NULL) {
            result = false;
            break;
        }
    }
    testcase_assert_true(self, result);

    printf("... destroying dict with dict_free()..\n");
    dict_free(tmpdict);
    /* can't check anything since everything is freed, use valgrind for a
     * simple sanity check */
    testcase_pass(self);

    return true;
}


static bool test_set(testcase_t *self)
{
#define TESTKEY "foo"
#define TESTVAL 42

    bool result;
    dict_value_t value;
    dict_type_t type;

    printf("... setting item ('%s' -> %d)\n", TESTKEY, TESTVAL);
    result = dict_set(dict, TESTKEY, DICT_INT_TO_VALUE(TESTVAL), DICT_ITEM_INT);
    testcase_assert_true(self, result);

    printf("... getting item ('%s') ..\n", TESTKEY);
    result = dict_get(dict, TESTKEY, &value, &type);
    printf("... value = %d, type = %s\n",
           DICT_VALUE_TO_INT(value),
           dict_type_name(type));
    testcase_assert_true(self, result);

    return true;
}


/** \brief  Create test group 'base/dict'
 *
 * \return  test group
 */
testgroup_t *get_base_dict_tests(void)
{
    testgroup_t *group;
    testcase_t *test;

    group = testgroup_new("base/dict",
                          "Test the base/dict module",
                          NULL, NULL);

    test = testcase_new("new",
                        "Test dict_new()/dict_free()",
                        3, test_new_free, NULL, NULL);
    testgroup_add_case(group, test);

    test = testcase_new("set",
                        "Test dict_set()",
                        2, test_set, setup, teardown);
    testgroup_add_case(group, test);

    return group;
}
