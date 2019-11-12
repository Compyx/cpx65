/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   test_io.c
 * \brief   Unit tests for base/io
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "unit.h"
#include "../base/base.h"

#include "test_io.h"

#define TEXT_TEST_FILE  "src/base/io/txtfile.c"



/*
 * Forward declarations
 */
static bool test_binfile(int *total, int *passed);
static bool test_txtfile(int *total, int *passed);


/** \brief  List of subtests
 */
static unit_test_t tests[] = {
    { "binfile", "Test raw binary file handling", test_binfile, false },
    { "txtfile", "Test text file handling", test_txtfile, false },
    { NULL, NULL, NULL, false }
};


/** \brief  Public unit test module
 */
unit_module_t io_module = {
    "io",
    "Test base/io",
    NULL, NULL,
    0, 0,
    tests
};


/** \brief  Test the binary file I/O
 *
 * \param[out]  total   total number of tests
 * \param[out]  passed  number of passed tests
 *
 * \return  bool
 */
static bool test_binfile(int *total, int *passed)
{
    uint8_t *data;
    long result;

    printf("..... testing base_binfile_read() with non-existant file ... ");
    (*total) += 1;
    result = base_binfile_read("foo.bar.huppel", &data);
    if (result < 0) {
        printf("pass.\n");
        (*passed)++;
    } else {
        printf("failed.\n");
    }


    printf("...... testing base_binfile_read(\"unit_tests\" ... ");
    (*total)++;
    result = base_binfile_read("unit_tests", &data);
     if (result > 0) {
        printf("pass: %ld bytes.\n", result);
        (*passed)++;
        base_free(data);
    } else {
        printf("failed.\n");
    }
    return true;
}


/** \brief  Test the text file I/O
 *
 * \param[out]  total   total number of tests
 * \param[out]  passed  number of passed tests
 *
 * \return  bool
 */
static bool test_txtfile(int *total, int *passed)
{
    txtfile_t handle;

    /* Test failure to open file */
    printf("..... testing base_txtfile_read() with non-existant file ... ");
    (*total) += 1;
    if (txtfile_open(&handle, "foo-bar-huppel-appel-meloen")) {
        printf("OK (shouldn't happen!)\n");
        return false;
    }
    printf("Failed -> OK\n");
    (*passed)++;

    /* Test opening existing file */
    printf("..... testing base_txtfile_read(\"%s\") ... ", TEXT_TEST_FILE);
    (*total) += 1;
    if (txtfile_open(&handle, TEXT_TEST_FILE)) {
        printf("OK.\n");
        (*passed)++;
    } else {
        printf("Failed\n");
        return false;
    }


    /* test reading lines */
    (*total)++;

    do {
        const char *line = txtfile_readline(&handle);
        if (line == NULL) {
            bool eof = txtfile_get_eof(&handle);
            printf("Got NULL, checking for eof: %s\n",
                    eof ? "YES" : "NO");
            if (eof) {
                (*passed)++;
            }
            break;
        }
        printf("%04ld: %04zu: '%s'\n",
                txtfile_get_linenum(&handle),
                txtfile_get_linelen(&handle),
                line);
    } while (true);


    /* clean up */
    txtfile_close(&handle);

    return true;

}


