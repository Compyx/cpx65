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
#include "../base/error.h"

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
#define TESTKEY1    "foo"
#define TESTVAL1    42
#define TESTKEY2    "bar"
#define TESTVAL2    69

    bool result;
    dict_value_t value;
    dict_type_t type;

    /* test #1: set an item */
    printf("... setting item ('%s' -> %d)\n", TESTKEY1, TESTVAL1);
    result = dict_set(dict, TESTKEY1, DICT_INT_TO_VALUE(TESTVAL1), DICT_ITEM_INT);
    testcase_assert_true(self, result);

    /* test #2; get the item back */
    printf("... getting item ('%s') ..\n", TESTKEY1);
    result = dict_get(dict, TESTKEY1, &value, &type);
    printf("... value = %d, type = %s\n",
           DICT_VALUE_TO_INT(value),
           dict_type_name(type));
    testcase_assert_true(self, DICT_VALUE_TO_INT(value) == TESTVAL1);

    /*test #3: try setting an item with an empty string as key */
    base_errno = 0;
    printf("... setting item with empty (\"\") key (should fail)\n");
    result = dict_set(dict, "", NULL, DICT_ITEM_PTR);
    printf("... result = %s\n", result ? "true" : "false");
    testcase_assert_false(self, result);
    /* test #4: make sure base_errno is set correctly */
    printf("... checking error code: got %d, expected %d); error msg = '%s'\n",
           base_errno, BASE_ERR_KEY, base_strerror(base_errno));
    testcase_assert_equal(self, base_errno, BASE_ERR_KEY);

    /*test #5: try setting an item with NULL as key */
    base_errno = 0;
    printf("... setting item with NULL as key (should fail)\n");
    result = dict_set(dict, NULL, NULL, DICT_ITEM_PTR);
    printf("... result = %s\n", result ? "true" : "false");
    testcase_assert_false(self, result);
    /* test #6: make sure base_errno is set correctly */
    printf("... checking error code: got %d ('%s'), expected %d ('%s')\n",
           base_errno, base_strerror(base_errno),
           BASE_ERR_KEY, base_strerror(BASE_ERR_KEY));
    testcase_assert_equal(self, base_errno, BASE_ERR_KEY);

    /* test #7: set item with invalid type */
    base_errno = 0;
    printf("... setting item with invalid type (1234, should fail)\n");
    result = dict_set(dict, TESTKEY2, DICT_INT_TO_VALUE(TESTVAL2), 1234);
    printf("... result = %s\n", result ? "true" : "false");
    testcase_assert_false(self, result);
    /* test #8: check base_errno */
    printf("... checking error code: got %d ('%s'), expected %d ('%s')\n",
           base_errno, base_strerror(base_errno),
           BASE_ERR_ENUM, base_strerror(BASE_ERR_ENUM));
    testcase_assert_equal(self, base_errno, BASE_ERR_ENUM);

#undef TESTKEY1
#undef TESTVAL1
#undef TESTKEY2
#undef TESTVAL2

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
                        6, test_set, setup, teardown);
    testgroup_add_case(group, test);

    return group;
}
