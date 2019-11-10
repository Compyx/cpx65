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
#include "../base/debug.h"
#include "../base/mem.h"
#include "../base/io/binfile.h"


#include "test_io.h"


/*
 * Forward declarations
 */
static bool test_binfile(int *total, int *passed);


/** \brief  List of subtests
 */
static unit_test_t tests[] = {
    { "binfile", "Test raw binary file handling", test_binfile, false },
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
