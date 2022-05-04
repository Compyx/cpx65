/** \file   test_base_operators.
 * \brief   Unit tests for base/operators.c
 *
 * Some simple unit tests for operator handling.
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
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "../base/mem.h"
#include "../base/operators.h"
#include "testcase.h"

#include "test_base_operators.h"


/** \brief  Object for getter tests
 */
typedef struct info_test_s {
    operator_id_t       id;     /**< operator ID */
    const char *        text;   /**< expected text */
    operator_arity_t    arity;  /**< expected arity */
    operator_assoc_t    assoc;  /**< expected associativity */
} info_test_t;


/** \brief  Table of tests for id, text, associativity and 'arity' getters
 */
static const info_test_t info_tests[] = {
    { OPR_ID_ADD,           "+",    OPR_LTR,        OPR_BINARY },
    { OPR_ID_LSB,           "<",    OPR_RTL,        OPR_UNARY },
    { OPR_ID_LT,            "<",    OPR_LTR,        OPR_BINARY },
    { OPR_ID_NOTEQUAL,      "!=",   OPR_LTR,        OPR_BINARY },
    { OPR_ID_ASSIGN_RSHIFT, ">>=",  OPR_RTL,        OPR_BINARY },
    { OPR_ID_RSHIFT,        ">>",   OPR_LTR,        OPR_BINARY },
    { OPR_ID_COMMA,         ",",    OPR_LTR,        OPR_BINARY },
    /* need separate tests for other IDs than -1 */
    { -1,                   NULL,   OPR_ASSOC_ERR,  OPR_ARITY_ERR }
};


/** \brief  Test #operator_text()
 *
 * \param[in]   self    test case
 *
 * \return  false on fatal error
 */
static bool test_operator_text(testcase_t *self)
{
    bool pass = true;

    for (size_t i = 0; i < base_array_len(info_tests); i++) {
        const char *text;
        const char *expected = info_tests[i].text;
        operator_id_t id = info_tests[i].id;
        bool result;

        text = operator_text(id);
        printf("... operator_text(%d) = ", id);
        if (text != NULL) {
            printf("'%s'", text);
        } else {
            printf("NULL");
        }
        printf(", expected ");
        if (expected != NULL) {
            printf("'%s': ", expected);
        } else {
            printf("NULL: ");
        }
        /* check result */
        if (text == NULL && expected == NULL) {
            result = true;
        } else if (text == NULL || expected == NULL) {
            result = false;
        } else {
            result = strcmp(text, expected) == 0 ? true : false;
        }
        printf("%s\n", result ? "ok" : "failed");
        if (!result) {
            pass = false;
        }
    }

    testcase_assert_true(self, pass);
    return true;
}


/** \brief  Create test group 'base/operators'
 *
 * \return  test group
 */
testgroup_t *get_base_operators_tests(void)
{
    testgroup_t *group;
    testcase_t *test;

    group = testgroup_new("base/operators",
                          "Test the base/operators module",
                          NULL, NULL);

    test = testcase_new("text",
                        "Test retrieving operator string literals",
                        1, test_operator_text, NULL, NULL);
    testgroup_add_case(group, test);

    return group;
}
