/** \file   test_base_symtab.c
 * \brief   Unit tests for base/symtab.c
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

#include "unit.h"
#include "../base/debug.h"
#include "../base/mem.h"
#include "../base/symtab.h"
#include "../base/helpers.h"

#include "test_mem.h"


static bool test_add_symbols(int *total, int *passed);


/** \brief  List of subtests
 */
static unit_test_t tests[] = {
    { "add", "Test adding symbols", test_add_symbols, false },
    { NULL, NULL, NULL, false }
};


/** \brief  Public unit test module
 */
unit_module_t symtab_module = {
    "symtab",
    "Test base/symtab",
    NULL, NULL,
    0, 0,
    tests
};


static bool test_add_symbols(int *total, int *passed)
{
    symtab_t table;
    symtab_node_t *result;
    size_t i;

    const char *labels[] = {
        "foo", "bar", "huppel", "compyx", "is", "very", "great", "and", "awesome"
    };

    symtab_init(&table);

    (*total)++;
    for (i = 0; i < BASE_ARRAY_SIZE(labels); i++) {
        printf("...... adding '%s' .. ", labels[i]);
        result = symtab_add(&table, labels[i]);
        if (result == NULL) {
            /* unexpected failure */
            printf("failed!\n");
            symtab_free(&table);
            return false;
        }
        printf("OK.\n");
    }
    printf("...... dumping symbols in order:\n");
    symtab_dump(&table);
    (*passed)++;

    /* add existing symbol: expected result: fail */
    (*total)++;
    printf("...... adding existing symbol 'compyx' .. ");
    result = symtab_add(&table, "compyx");
    if (result != NULL) {
        printf("OK (unexpected, failure)\n");
    } else {
        printf("failed (expected, OK)\n");
        (*passed)++;
    }

    symtab_free(&table);

    return true;
}
