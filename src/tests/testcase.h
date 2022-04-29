/** \file   testcase.h
 * \brief   Simple unit test case - header
 * \author  Bas Wassink <b.wassink@ziggo.nl>
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

#ifndef TESTS_TESTCASE_H
#define TESTS_TESTCASE_H

#include <stdbool.h>

struct testcase_s;

/** \brief  setup function prototype
 *
 * \return  false in case of fatal error
 */
typedef bool (*setup_t)(void);

/** \brief  test function prototype
 *
 * \param[in]   self    test case object
 *
 * \return  false in case of fatal error
 */
typedef bool (*execute_t)(struct testcase_s *self);

/** \brief  setup function prototype
 *
 * \return  false in case of fatal error
 */
typedef bool (*teardown_t)(void);


/** \brief  Test case object
 */
typedef struct testcase_s {
    char *name;             /**< test name */
    char *desc;             /**< test description */
    setup_t setup;          /**< setup function (optional) */
    execute_t execute;      /**< function running the test(s) */
    teardown_t teardown;    /**< teardown function (optional) */
    int count;              /**< number of tests in unit */
    int current;            /**< current test */
    int passed;             /**< number of passed tests */
} testcase_t;


/** \brief  Test group object
 *
 * Contains one or more test cases.
 */
typedef struct testgroup_s {
    char *name;             /**< name of group */
    char *desc;             /**< description of group */
    setup_t setup;          /**< setup function (optional) */
    teardown_t teardown;    /**< teardown function (optional) */
    testcase_t **cases;     /**< list of test cases */
    int case_count;         /**< number of test cases */
    int case_current;       /**< current test case */
    int case_passed;        /**< number of passed cases */
    int tests_count;        /**< number of tests in unit */
    int tests_current;      /**< current test */
    int tests_passed;       /**< number of passed tests */
} testgroup_t;


void testcase_enable_ansi(bool enable);

testcase_t *testcase_new(const char *name,
                         const char *desc,
                         int count,
                         execute_t execute,
                         setup_t setup,
                         teardown_t teardown);
void testcase_free(testcase_t *test);
bool testcase_exec(testcase_t *test);
void testcase_pass(testcase_t *test);
void testcase_fail(testcase_t *test);
void testcase_assert_true(testcase_t *test, int condition);
void testcase_assert_false(testcase_t *test, int condition);
void testcase_assert_equal(testcase_t *test, int val1, int val2);
void testcase_assert_null(testcase_t *test, const void *ptr);
void testcase_assert_not_null(testcase_t *test, const void *ptr);
void testcase_printf(testcase_t *test, const char *fmt, ...);

testgroup_t *testgroup_new(const char *name,
                           const char *desc,
                           setup_t setup,
                           teardown_t teardown);
void testgroup_free(testgroup_t *group);
void testgroup_add_case(testgroup_t *group, testcase_t *test);
testcase_t *testgroup_find_case(testgroup_t *group, const char *name);
void testgroup_list_cases(const testgroup_t *group);
bool testgroup_exec_cases(testgroup_t *group);
bool testgroup_exec_case(testgroup_t *group, const char *name);

#endif
