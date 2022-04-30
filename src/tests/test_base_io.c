/** \file   test_base_io.c
 * \brief   Unit tests for base/io
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
#include "../base/error.h"
#include "../base/io/io.h"
#include "../base/mem.h"

#include "test_base_io.h"


/** \brief  Test file for text file handling
 */
#define TEXT_TEST_FILE  "src/base/io/txtfile.c"


/** \brief  Test the binary file I/O
 *
 * \param[in]   self    test case
 *
 * \return  bool
 */
static bool test_binfile(testcase_t *self)
{
    uint8_t *data = NULL;
    long result;

    /* test #1: proper handling of non-existent file */
    printf("... testing base_binfile_read() with non-existent file 'foo.bar.huppel' ..\n");
    result = base_binfile_read("foo.bar.huppel", &data);
    testcase_assert_true(self, result < 0);
    /* make sure to clean up after a failed test */
    if (result >= 0) {
        printf("failed.\n");
        if (data != NULL) {
            base_free(data);
        }
    }

    /* test #2: reading a file */
    printf("... testing base_binfile_read(\"testrunner\") .. ");
    result = base_binfile_read("testrunner", &data);
    printf("%ld bytes.\n", result);
    testcase_assert_true(self, result > 0);
    if (result >= 0) {
        base_free(data);
    }

    return true;
}


/** \brief  Test the text file I/O
 *
 * \param[in]   self    test case
 *
 * \return  bool
 */
static bool test_txtfile(testcase_t *self)
{
    txtfile_t handle;
    bool result;
    int printlines = 4;

    /* test #1: test failure to open file */
    printf("... testing base_txtfile_read() with non-existant file ..\n");
    base_errno = 0;
    result = txtfile_open(&handle, "foo-bar-huppel-appel-meloen");
    testcase_assert_false(self, result);
    /* test #2: test value of base_errno */
    printf("... checking base_errno for BASE_ERR_IO (%d): got %d (%s)\n",
           BASE_ERR_IO, base_errno, base_strerror(base_errno));
    testcase_assert_equal(self, base_errno, BASE_ERR_IO);
    if (result) {
        /* clean up */
        txtfile_close(&handle);
    }

    /* test #3: test opening existing file */
    printf("... testing base_txtfile_read(\"%s\") ..\n", TEXT_TEST_FILE);
    base_errno = 0;
    result = txtfile_open(&handle, TEXT_TEST_FILE);
    testcase_assert_true(self, result);

    /* test #4: reading line (only works if test #3 passed) */
    if (result) {
        printf("... reading all text until EOF; printing the first %d lines..\n",
               printlines);
        do {
            const char *line = txtfile_readline(&handle);
            if (line == NULL) {
                bool eof = txtfile_get_eof(&handle);
                printf("... got NULL, checking for eof: %s\n", eof ? "YES" : "NO");
                testcase_assert_true(self, eof);
                break;
            }
            if (printlines > 0) {
                printf("..... %04ld: %04zu: '%s'\n",
                       txtfile_get_linenum(&handle),
                       txtfile_get_linelen(&handle),
                       line);
                printlines--;
            }
        } while (true);
        /* clean up */
        txtfile_close(&handle);
    } else {
        /* mark test failed if unable to run */
        testcase_fail(self);
    }

    return true;

}


/** \brief  Create test group 'base/io'
 *
 * \return  test group
 */
testgroup_t *get_base_io_tests(void)
{
    testgroup_t *group;
    testcase_t *test;

    group = testgroup_new("base/io",
                          "Test the base/io module",
                          NULL, NULL);

    test = testcase_new("binfile",
                        "Test binary file handling",
                        2, test_binfile, NULL, NULL);
    testgroup_add_case(group, test);

    test = testcase_new("txtfile",
                        "Test text file handling",
                        4, test_txtfile, NULL, NULL);
    testgroup_add_case(group, test);

    return group;
}
