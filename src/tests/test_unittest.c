/** \file   test_unittest.c
 * \brief   Test the unit testing itself
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

#include "unit.h"

/*
 * Forward declarations
 */

static bool setup(void);
static bool teardown(void);

static bool test_passing(int *total, int *passed);
static bool test_failing_ok(int *total, int *passed);
static bool test_failing_fatal(int *total, int *passed);


/** \brief  Test cases
 */
static unit_test_t tests[] = {
    { "pass", "Test passing a test", test_passing, true },
    { "fail-ok", "Test failing a test, non-fatal", test_failing_ok, true },
    { "fail-fatal", "Test failing a test, fatal", test_failing_fatal, false },
    { NULL, NULL, NULL, NULL }
};


/** \brief  Module containing tests
 */
unit_module_t unittest_module = {
    "unittest",
    "Simple tests to test the unit test code =)",
    setup, teardown,
    0, 0,
    tests
};


/** \brief  Setup function
 *
 * This is called when initializing the module.
 *
 * Currently doesn't do anything, except report it got called (with --debug)
 * and return `true`.
 */
static bool setup(void)
{
    printf("%s:%d:%s(): called.\n",
            __FILE__, __LINE__, __func__);
    return true;
}


/** \brief  Teardown function
 *
 * This is called when cleaning up any resources set by #setup.
 *
 * Doesn't do anything except reporting it get called (with --debug) and
 * return `true`
 */
static bool teardown(void)
{
    printf("%s:%d:%s(): called.\n",
            __FILE__, __LINE__, __func__);
    return true;
}


/** \brief  Test passing of a unit test
 *
 * \param[out]  total   total number of subtests
 * \param[out]  passed  number of passed subtests
 *
 * \return  bool
 */
static bool test_passing(int *total, int *passed)
{
    *total += 1;
    *passed += 1;
    return true;
}


/** \brief  Test failing of a unit test, non-fatal
 *
 * This tests fails (returns `false`), but is marked 'keep-going', so the test
 * module should keep going.
 *
 * \param[out]  total   total number of subtests
 * \param[out]  passed  number of passed subtests
 *
 * \return  bool
 */
static bool test_failing_ok(int *total, int *passed)
{
    *total += 1;
    (void)passed;
    return false;
}


/** \brief  Test failing of a unit test, fatal
 *
 * This tests fails (returns `false`), and isn't marked 'keep-going', so the
 * test module should fail.
 *
 * \param[out]  total   total number of subtests
 * \param[out]  passed  number of passed subtests
 *
 * \return  bool
 */
static bool test_failing_fatal(int *total, int *passed)
{
    *total += 1;
    (void)passed;
    return false;
}
