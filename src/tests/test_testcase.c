/** \file   test_testcase.c
 * \brief   Simple unit test of the unit test itself
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../base/mem.h"
#include "../base/strlist.h"
#include "testcase.h"

#include "test_testcase.h"


/** \brief  testcase::setup() was called
 */
static bool setup_called = false;

/** \brief  testcase::teardown() was called
 */
static bool teardown_called = false;


/** \brief  Setup function
 *
 * \return  true
 */
static bool setup(void)
{
    setup_called = true;
    return true;
}

/** \brief  Teardown function
 *
 * \return  true
 */
static bool teardown(void)
{
    teardown_called = true;
    return true;
}


/** \brief  Test triggering the 'setup' function
 *
 * \param[in]   self    test case
 *
 * \return  true
 */
static bool test_setup(testcase_t *self)
{
    if (setup_called) {
        testcase_pass(self);
    }
    return true;
}

/** \brief  Test triggering the 'teardown' function
 *
 * \param[in]   self    test case
 *
 * \return  true
 *
 * \note    Depends on test_setup() being called before
 */
static bool test_teardown(testcase_t *self)
{
    if (teardown_called) {
        testcase_pass(self);
    }
    return true;
}

/** \brief  Test multiple tests in a single case
 *
 * Runs a few tests to check proper increment of test count and pass count.
 *
 * \param[in]   self    test case
 *
 * \return  true
 */
static bool test_multi(testcase_t *self)
{
    /* mark one test passed: 1 of X */
    testcase_pass(self);
    /* current should be 2 now  */
    testcase_assert_equal(self, self->current, 2);
    /* passed should be 2 now */
    testcase_assert_equal(self, self->passed, 2);
    /* number of tests should be 3 on exit */
    return true;
}


/** \brief  Create all tests
 *
 * \return  group with all tests
 */
testgroup_t *get_testcase_tests(void)
{
    testgroup_t *group;
    testcase_t *test;

    group = testgroup_new("testcase",
                          "Test the unit testing framework itself",
                          NULL, NULL);

    test = testcase_new("setup",
                        "Test calling the testcase::setup() function",
                        1, test_setup, setup, teardown);
    testgroup_add_case(group, test);

    /* only passes when another test was executed before, triggering the
     * teardown() function that sets the `teardown_called` variable */
    test = testcase_new("teardown",
                        "Test calling the testcase::teardown() function",
                        1, test_teardown, setup, teardown);
    testgroup_add_case(group, test);

    test = testcase_new("multi",
                        "Test multiple tests in a case",
                        3, test_multi, NULL, NULL);
    testgroup_add_case(group, test);
    return group;
}

