/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   test_symtab.c
 * \brief   Unit tests for base/symtab.c
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
    symtab_node_t *root = NULL;
    size_t i;

    const char *labels[] = {
        "foo", "bar", "huppel", "compyx", "is", "very", "great", "and", "awesome"
    };

    for (i = 0; i < BASE_ARRAY_SIZE(labels); i++) {
        printf(".. adding '%s'\n", labels[i]);
        root = symtab_node_add(root, labels[i]);
    }
    symtab_node_dump(root);
    symtab_node_free(root);

    (*total)++;
    (*passed)++;
    return true;
}
