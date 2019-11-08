/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   unit.h
 * \brief   Unit testing module - header
 */

#ifndef HAVE_UNIT_H
#define HAVE_UNIT_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/** \brief  Unit test case
 */
typedef struct unit_test_s {
    const char *name;                       /**< test name */
    const char *description;                /**< test description */
    bool (*func)(int *total, int *passed);  /**< test function */
    bool keep_going;                        /**< keep going on failure */
} unit_test_t;


/** \brief  Unit test module
 *
 * Contains data on a collection of unit tests
 */
typedef struct unit_module_s {
    const char *name;           /**< module name */
    const char *description;    /**< module description */
    bool (*setup)(void);        /**< module setup function (optional) */
    bool (*teardown)(void);     /**< module teardown function (optional) */
    int tests_total;            /**< number of tests run */
    int tests_passed;           /**< number of succesfull tests */
    unit_test_t *tests;         /**< list of unit tests */
} unit_module_t;


void unit_module_add(unit_module_t *module);
void unit_dump_module_list(void);
void unit_dump_module_test_list(const unit_module_t *module);
void unit_dump_module_test_list_by_name(const char *name);

bool unit_module_init(unit_module_t *module);
bool unit_module_exit(unit_module_t *module);
bool unit_module_exec(unit_module_t *module);
bool unit_module_exec_by_name(const char *name);
bool unit_module_run_all(void);

void unit_dump_results(void);

#endif

