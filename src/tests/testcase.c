/** \file   testcase.c
 * \brief   Simple unit test module
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "../base/mem.h"

#include "testcase.h"


/* ANSI escape sequences for colors
 *
 * Colors are prefixed with 'L' for 'light' instead of 'B'/BR' for bright so
 * I can use 'B' for bold, should I require that.
 *
 * Use testcase_enable_ansi() to enable/disable colors, which changes the below
 * pointers. This way the format strings for terminal output can always refer
 * to colors without having to resort to testing of a variable.
 */

/** \brief  ANSI sequence for reset */
static const char *RESET = "";
/** \brief  ANSI sequence for black */
static const char *BLACK = "";
/** \brief  ANSI sequence for red */
static const char *RED = "";
/** \brief  ANSI sequence for green */
static const char *GREEN = "";
/** \brief  ANSI sequence for yellow */
static const char *YELLOW = "";
/** \brief  ANSI sequence for blue */
static const char *BLUE = "";
/** \brief  ANSI sequence for magenta */
static const char *MAGENTA = "";
/** \brief  ANSI sequence for cyan */
static const char *CYAN = "";
/** \brief  ANSI sequence for white */
static const char *WHITE = "";
/** \brief  ANSI sequence for bright black */
static const char *LBLACK = "";
/** \brief  ANSI sequence for bright red */
static const char *LRED = "";
/** \brief  ANSI sequence for bright green */
static const char *LGREEN = "";
/** \brief  ANSI sequence for bright yellow */
static const char *LYELLOW = "";
/** \brief  ANSI sequence for bright blue */
static const char *LBLUE = "";
/** \brief  ANSI sequence for bright magenta */
static const char *LMAGENTA = "";
/** \brief  ANSI sequence for bright cyan */
static const char *LCYAN = "";
/** \brief  ANSI sequence for bright white */
static const char *LWHITE = "";



/** \brief  Enable/disable ANSI color codes
 *
 * Enable output of ANSI escape sequences for terminal colors.
 *
 * \param[in]   enable  use ANSI colors
 */
void testcase_enable_ansi(bool enable)
{
    if (enable) {
        RESET    = "\033[0m";
        BLACK    = "\033[30m";
        RED      = "\033[31m";
        GREEN    = "\033[32m";
        YELLOW   = "\033[33m";
        BLUE     = "\033[34m";
        MAGENTA  = "\033[35m";
        CYAN     = "\033[36m";
        WHITE    = "\033[37m";
        LBLACK   = "\033[90m";
        LRED     = "\033[91m";
        LGREEN   = "\033[92m";
        LYELLOW  = "\033[93m";
        LBLUE    = "\033[94m";
        LMAGENTA = "\033[95m";
        LCYAN    = "\033[96m";
        LWHITE   = "\033[97m";
    } else {
        RESET    = "";
        BLACK    = "";
        RED      = "";
        GREEN    = "";
        YELLOW   = "";
        BLUE     = "";
        MAGENTA  = "";
        CYAN     = "";
        WHITE    = "";
        LBLACK   = "";
        LRED     = "";
        LGREEN   = "";
        LYELLOW  = "";
        LBLUE    = "";
        LMAGENTA = "";
        LCYAN    = "";
        LWHITE   = "";
    }
}


/** \brief  Create new test case
 *
 * \param[in]   name        test name
 * \param[in]   desc        test description
 * \param[in]   count       number of tests that will run
 * \param[in]   execute     function that executes the test(s)
 * \param[in]   setup       setup function, called before calling \a execute
 * \param[in]   teardown    teardown function, called after calling \a execute
 *
 * \return  new test case
 */
testcase_t *testcase_new(const char *name,
                         const char *desc,
                         int count,
                         execute_t execute,
                         setup_t setup,
                         teardown_t teardown)
{
    testcase_t *test = base_malloc(sizeof *test);

    test->name = base_strdup(name);
    test->desc = base_strdup(desc);
    test->count = count;
    test->current = 1;
    test->passed = 0;
    test->setup = setup;
    test->execute = execute;
    test->teardown = teardown;

    return test;
}


/** \brief  Free memory used by test case
 *
 * \param[in]   test    test case
 */
void testcase_free(testcase_t *test)
{
    base_free(test->name);
    base_free(test->desc);
    base_free(test);
}


/** \brief  Mark current test passed
 *
 * Marks current test in case passed and increments current test number.
 *
 * \param[in]   test    test case
 */
void testcase_pass(testcase_t *test)
{
    printf("... test %d of %d: %spass%s.\n",
           test->current, test->count, LGREEN, RESET);
    test->passed++;
    test->current++;
}


/** \brief  Mark current test failed
 *
 * Marks current test in case failed and increments current test number.
 *
 * \param[in]   test    test case
 */
void testcase_fail(testcase_t *test)
{
    printf("... test %d of %d: %sfail%s.\n",
           test->current, test->count, LRED, RESET);
    test->current++;
}


/** \brief  Assert if \a condition is true
 *
 * \param[in]   test        test case
 * \param[in]   condition   condition to test, boolean
 */
void testcase_assert_true(testcase_t *test, int condition)
{
    if (condition) {
        testcase_pass(test);
    } else {
        testcase_fail(test);
    }
}


/** \brief  Assert if \a condition is false
 *
 * \param[in]   test        test case
 * \param[in]   condition   condition to test, boolean
 */
void testcase_assert_false(testcase_t *test, int condition)
{
    if (condition) {
        testcase_fail(test);
    } else {
        testcase_pass(test);
    }
}


/** \brief  Assert if \a expected equals \a result
 *
 * Prints \a expected and \a result in case of failure, providing a little more info
 * than just 'fail'.
 *
 * \param[in]   test        test case
 * \param[in]   expected    expected value
 * \param[in]   result      resulting value
 */
void testcase_assert_equal(testcase_t *test, int expected, int result)
{
    if (expected == result) {
        testcase_pass(test);
    } else {
        testcase_fail(test);
        printf("..... assert_equal: result %d differs from expected value %d.\n",
               expected, result);
    }
}


/** \brief  Assert if \a ptr is NULL
 *
 * \param[in]   test    test case
 * \param[in]   ptr     pointer
 */
void testcase_assert_null(testcase_t *test, const void *ptr)
{
    if (ptr == NULL) {
        testcase_pass(test);
    } else {
        testcase_fail(test);
        printf("..... assert_null: value is not NULL.\n");
    }
}


/** \brief  Assert if \a ptr is not NULL
 *
 * \param[in]   test    test case
 * \param[in]   ptr     pointer
 */
void testcase_assert_not_null(testcase_t *test, const void *ptr)
{
    if (ptr != NULL) {
        testcase_pass(test);
    } else {
        testcase_fail(test);
        printf("..... assert_not_null: value is NULL.\n");
    }
}


/** \brief  Print formatted message
 *
 * Print formatted message, prefixed with "[\<testnum\>]".
 *
 * \param[in]   test    test case
 * \param[in]   fmt     format string
 */
void testcase_printf(testcase_t *test, const char *fmt, ...)
{
    va_list args;

    printf("... [%d] ", test->current);
    va_start(args, fmt);
    vprintf(fmt, args);
    fflush(stdout);
    va_end(args);
}


/** \brief  Execute tests in case
 *
 * First runs setup(), if available, then execute() and finally teardown(),
 * if available.
 * The case should only return `false` if it runs into an unexpected condition,
 * test failures should be marked with testcase_pass() and the after all tests
 * the execute() function should return `true` to continue running other test
 * cases.
 *
 * \param[in]   test    test case
 *
 * \return  `false` on an unexpected failure to stop running further tests
 */
bool testcase_exec(testcase_t *test)
{
    bool pass_full;
    double pass_perc;

    printf(". running case '%s%s%s' (%d %s):\n",
            LCYAN, test->name, RESET,
            test->count, test->count == 1 ? "test" : "tests");

    if (test->setup != NULL) {
        printf("... calling setup().\n");
        if (!test->setup()) {
            printf("... fatal error during setup, aborting.\n");
            return false;
        }
    }

    if (!test->execute(test)) {
        printf(". fatal error during test, aborting.\n");
        return false;
    }

    if (test->teardown != NULL) {
        printf("... calling teardown().\n");
        if (!test->teardown()) {
            printf("... fatal error during teardown, aborting.\n");
            return false;
        }
    }

    pass_full = test->passed == test->count;
    pass_perc = (double)test->passed / (double)test->count * 100.0;
    printf("... %d of %d %s passed: %s%.0f%%%s.\n",
            test->passed, test->count, test->count == 1 ? "test" : "tests",
            pass_full ? LGREEN : LRED, pass_perc, RESET);

    return true;
}


/*
 * Test groups API
 */

/** \brief  Create new test group
 *
 * Create a new test group that can contain multiple test cases.
 *
 * The setup and teardown functions are optional, and can be used to set up some
 * state for the cases to work with. For more fine-grained control the test
 * cases also have optional setup/teardown functions should that be required.
 *
 * \param[in]   name        group name
 * \param[in]   desc        group description
 * \param[in]   setup       setup function (optional)
 * \param[in]   teardown    teardown function (optional)
 *
 * \return  new test group
 */
testgroup_t *testgroup_new(const char *name,
                           const char *desc,
                           setup_t setup,
                           teardown_t teardown)
{
    testgroup_t *group = base_malloc(sizeof *group);

    group->name = base_strdup(name);
    group->desc = base_strdup(desc);
    group->setup = setup;
    group->teardown = teardown;
    group->cases = NULL;
    group->case_count = 0;
    group->case_current = 1;
    group->case_passed = 0;
    group->tests_count = 0;
    group->tests_current = 1;
    group->tests_passed = 0;

    return group;
}


/** \brief  Free memory used by \a group and its test cases
 *
 * \param[in]   group   test group
 */
void testgroup_free(testgroup_t *group)
{
    for (int c = 0; c < group->case_count; c++) {
        testcase_free(group->cases[c]);
    }
    if (group->cases != NULL) {
        base_free(group->cases);
    }
    base_free(group->name);
    base_free(group->desc);
    base_free(group);
}


/** \brief  Add test case to test group
 *
 * \param[in]   group   test group
 * \param[in]   test    test case
 */
void testgroup_add_case(testgroup_t *group, testcase_t *test)
{
    group->cases = base_realloc(group->cases,
                               (size_t)(group->case_count + 1) * sizeof *group->cases);
    group->cases[group->case_count++] = test;
}


/** \brief  Find test case by name in test group
 *
 * \param[in]   group   test group
 * \param[in]   name    test case name
 *
 * \return  test case or `NULL` when not found
 */
testcase_t *testgroup_find_case(testgroup_t *group, const char *name)
{
    for (int c = 0; c < group->case_count; c++) {
        testcase_t *test = group->cases[c];

        if (strcmp(test->name, name) == 0) {
            return test;
        }
    }
    return NULL;
}


/** \brief  Print list of test cases on stdout
 *
 * \param[in]   group
 */
void testgroup_list_cases(const testgroup_t *group)
{
    for (int c = 0; c < group->case_count; c++) {
        const testcase_t *test = group->cases[c];

        printf("    %-19s %s\n", test->name, test->desc);
    }
}


/** \brief  Execute all test cases in a test group
 *
 * \param[in]   group   test group
 *
 * \return  false on fatal error
 */
bool testgroup_exec_cases(testgroup_t *group)
{
    printf("running all cases of group '%s%s%s':\n",
           LMAGENTA, group->name, RESET);

    for (int c = 0; c < group->case_count; c++) {
        testcase_t *test = group->cases[c];

        printf(". case %d of %d:\n", group->case_current, group->case_count);
        if (!testcase_exec(test)) {
            return false;
        }
        group->case_current++;
        group->tests_count += test->count;
        group->tests_passed += test->passed;
    }
    printf(". %d of %d tests of '%s%s%s' passed.\n",
           group->tests_count, group->tests_passed,
           LMAGENTA, group->name, RESET);

    return true;
}


/** \brief  Execute a single case in a group
 *
 * Execute test case by \a name in \a group.
 *
 * \param[in]   group   test group
 * \param[in]   name    case name
 *
 * \return  false on fatal error
 */
bool testgroup_exec_case(testgroup_t *group, const char *name)
{
    testcase_t *test = testgroup_find_case(group, name);

    if (test == NULL) {
        fprintf(stderr, "error: unknown case '%s'.\n", name);
        return false;
    }
    return testcase_exec(test);
}
