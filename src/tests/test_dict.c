/** \file   test_dict.c
 * \brief   Unit tests for base/dict.c
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

#include "../base/debug.h"
#include "unit.h"
#include "../base/dict.h"

#include "test_dict.h"


/*
 * Forward declarations
 */
static bool test_new(int *total, int *passed);


/** \brief  List of subtests
 */
static unit_test_t tests[] = {
    { "new", "Test dict_new()/dict_free()", test_new, false },
    { NULL, NULL, NULL, false }
};


/** \brief  Public unit test module
 */
unit_module_t dict_module = {
    "dict",
    "Test base/dict",
    NULL, NULL,
    0, 0,
    tests
};


static bool test_new(int *total, int *passed)
{
    dict_t *dict;

    printf("..... testing dict_new()\n");
    (*total) += 1;
    dict = dict_new();
    (*passed)++;

    printf("..... testing dict_free()\n");
    (*total) += 1;
    dict_free(dict);
    (*passed)++;

    return true;
}
