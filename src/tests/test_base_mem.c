/** \file   test_base_mem.c
 * \brief   Unit tests for base/mem.c
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

#include "testcase.h"
#include "../base/debug.h"
#include "../base/mem.h"

#include "test_base_mem.h"


/** \brief  Object containing data to test #base_ispow2
 */
typedef struct ispow2_test_s {
    size_t value;   /**< input */
    bool expected;  /**< expected result */
} ispow2_test_t;

/** \brief  Test cases for #base_ispow2()
 */
static const ispow2_test_t ispow2_tests[] = {
    {     0, false },
    {     1, true },
    {     2, true },
    {     3, false },
    {     4, true },
    {   256, true },
    { 36233, false },
    { 65536, true }
};


/** \brief  Object containing data to test #base_nextpow2
 */
typedef struct nextpow2_test_s {
    size_t value;       /**< input */
    size_t expected;    /**< expected output */
} nextpow2_test_t;

/** \brief  Test cases for #base_nextpow2()
 */
static const nextpow2_test_t nextpow2_tests[] = {
    {        0,  0 },
    {        1,  2 },
    {        7,  8 },
    {       16, 32 },
    {       31, 32 },
    {       42, 64 },
    { SIZE_MAX,  0 }
};


/** \brief  Test the #base_ispow2() function
 *
 * \param[in]   self    test case
 *
 * \return  bool
 */
static bool test_ispow2(testcase_t *self)
{
    for (size_t i = 0; i < sizeof ispow2_tests / sizeof ispow2_tests[0]; i++) {
        bool result = base_ispow2(ispow2_tests[i].value);
        bool expected = ispow2_tests[i].expected;

        printf("... ispow2(%zx) = %s (expected %s)\n",
               ispow2_tests[i].value,
               result ? "true" : "false",
               expected ? "true" : "false");
        testcase_assert_true(self, result == expected);
    }
    return true;
}


/** \brief  Test #base_nextpow2()
 *
 * \param[in]   self    test case
 *
 * \return  bool
 */
static bool test_nextpow2(testcase_t *self)
{
    for (size_t i = 0; i < sizeof nextpow2_tests / sizeof nextpow2_tests[0]; i++) {
        size_t result = base_nextpow2(nextpow2_tests[i].value);
        size_t expected =  nextpow2_tests[i].expected;

        printf("... nextpow2(%zx) = %lx (expected %zx)\n",
               nextpow2_tests[i].value,
               result,
               expected);
        testcase_assert_true(self, result == expected);
    }
    return true;
}


/** \brief  Create test group 'base/mem'
 *
 * \return  test group
 */
testgroup_t *get_base_mem_tests(void)
{
    testgroup_t *group;
    testcase_t *test;

    group = testgroup_new("base/mem",
                          "Test the base/mem module",
                          NULL, NULL);

    test = testcase_new("ispow2",
                        "Test the base_ispow2() function",
                        (int)(base_array_len(ispow2_tests)),
                        test_ispow2, NULL, NULL);
    testgroup_add_case(group, test);

    test = testcase_new("nextpow2",
                        "Test the base_nextpow2() function",
                        (int)(base_array_len(nextpow2_tests)),
                        test_nextpow2, NULL, NULL);
    testgroup_add_case(group, test);

    return group;
}
