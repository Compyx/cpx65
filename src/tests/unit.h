/** \file   unit.h
 * \brief   Unit testing module - header
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
