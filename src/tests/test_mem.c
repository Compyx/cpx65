/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   test_mem.c
 * \brief   Unit tests for base/mem.c
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../base/debug.h"
#include "unit.h"
#include "../base/mem.h"

#include "test_mem.h"


/*
 * Forward declarations
 */
static bool test_ispow2(int *total, int *passed);
static bool test_nextpow2(int *total, int *passed);


/** \brief  List of subtests
 */
static unit_test_t tests[] = {
    { "ispow2", "Test base_ispow2()", test_ispow2, false },
    { "nextpow2", "Test base_nextpow2()", test_nextpow2, false },
    { NULL, NULL, NULL, false }
};


/** \brief  Public unit test module
 */
unit_module_t mem_module = {
    "mem",
    "Test base/mem",
    NULL, NULL,
    0, 0,
    tests
};


/** \brief  Object containing data to test #base_ispow2
 */
typedef struct ispow2_test_s {
    size_t value;   /**< input */
    bool expected;  /**< expected result */
} ispow2_test_t;


/** \brief  Object containing data to test #base_nextpow2
 */
typedef struct nextpow2_test_s {
    size_t value;       /**< input */
    size_t expected;    /**< expected output */
} nextpow2_test_t;


/** \brief  Test cases for #base_ispow2()
 */
static const ispow2_test_t ispow2_tests[] = {
    { 0, false },
    { 1, true },
    { 2, true },
    { 3, false },
    { 4, true },
    { 256, true },
    { 36233, false },
    { 65536, true }
};


/** \brief  Test cases for #base_nextpow2()
 */
static const nextpow2_test_t nextpow2_tests[] = {
    { 0, 0 },
    { 1, 2 },
    { 7, 8 },
    { 16, 32 },
    { 31, 32 },
    { 42, 64 },
    { SIZE_MAX, 0 }
};


/** \brief  Test the #base_ispow2() function
 *
 * \param[out]  total   total number of tests
 * \param[out]  passed  number of passed tests
 *
 * \return  bool
 */
static bool test_ispow2(int *total, int *passed)
{
    for (size_t i = 0; i < sizeof ispow2_tests / sizeof ispow2_tests[0]; i++) {
        bool func_res = base_ispow2(ispow2_tests[i].value);
        bool final_res = func_res == ispow2_tests[i].expected;

        printf(" .. ispow2(%zx) = %s (expected %s) -> %s\n",
                ispow2_tests[i].value,
                func_res ? "true" : "false",
                ispow2_tests[i].expected ? "true" : "false",
                final_res ? "OK" : "Failed");
        (*total)++;
        (*passed) += final_res ? 1 : 0;
    }
    return true;
}


/** \brief  Test #base_nextpow2()
 *
 * \param[out]  total   total subtests
 * \param[out]  passed  number of passed subtests
 *
 * \return  bool
 */
static bool test_nextpow2(int *total, int *passed)
{
    for (size_t i = 0; i < sizeof nextpow2_tests / sizeof nextpow2_tests[0]; i++) {
        size_t func_res = base_nextpow2(nextpow2_tests[i].value);
        bool final_res = func_res == nextpow2_tests[i].expected;

        printf(" .. nextpow2(%zx) = %lx (expected %zx) -> %s\n",
                nextpow2_tests[i].value,
                func_res,
                nextpow2_tests[i].expected,
                final_res ? "OK" : "Failed");
        (*total)++;
        (*passed) += final_res ? 1 : 0;
    }
    return true;
}

