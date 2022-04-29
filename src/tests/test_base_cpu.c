/** \file   test_base_cpu.c
 * \brief   Unit tests for base/cpu/
 *
 * Some simple unit tests for the CPU-info code in base/cpu.
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
#include "../base/mem.h"
#include "../base/cpu/addrmode.h"
#include "../base/cpu/cputype.h"
#include "../base/cpu/mnemonic.h"
#include "../base/cpu/opcode.h"
#include "testcase.h"

#include "test_base_cpu.h"

/** \brief  Data object for the mnemonic_get_text() tests
 */
typedef struct mne_text_test_s {
    mnemonic_id_t   id;     /**< mnemonic ID */
    const char *    text;   /**< expected mnemonic string */
} mne_text_test_t;

/** \brief  Data for for the mnenonic_get_text() tests
 */
static const mne_text_test_t mne_text_tests[] = {
    { MNE_ADC, "adc" },
    { MNE_ILL, "???" },
    { MNE_TYA, "tya" },
};


/** \brief  Data object for the mnemonic_get_id() tests
 */
typedef struct mne_id_test_s {
    const char *    text;   /**< mne string to parse */
    mnemonic_id_t   id;     /**< expected ID */
} mne_id_test_t;

/** \brief  Data for for the mnenonic_get_id() tests
 */
static const mne_id_test_t mne_id_tests[] = {
    { "nop",    MNE_NOP },
    { "NOP",    MNE_NOP },
    { "NoP",    MNE_NOP },
    { "noop",   MNE_ILL },
    { "sbc",    MNE_SBC },
    { "tya",    MNE_TYA },
    { "lsr",    MNE_LSR },
    { "lsr a",  MNE_ILL },
    { "",       MNE_ILL },
    { NULL,     MNE_ILL }
};


/** \brief  Test the #mnemonic_get_text() function
 *
 * \param[in,out]   self    test case
 *
 * \return  bool
 */
static bool test_mnemonic_text(testcase_t *self)
{
    for (size_t i = 0; i < base_array_len(mne_text_tests); i++) {

        mnemonic_id_t id = mne_text_tests[i].id;
        const char *expected = mne_text_tests[i].text;
        const char *text = mnemonic_get_text(id);
        int result;

        result = strcmp(text, expected);

        printf("... mnemonic_get_text(%d) = '%s', expected '%s'\n",
                (int)id, text, expected),
        testcase_assert_true(self, result == 0);
    }
    return true;
}


/** \brief  Test the #mnemonic_get_id() function
 *
 * \param[in,out]   self    test case
 *
 * \return  bool
 */
static bool test_mnemonic_id(testcase_t *self)
{
    for (size_t i = 0; i < base_array_len(mne_id_tests); i++) {

        mnemonic_id_t id;
        mnemonic_id_t result;
        const char *text;

        id = mne_id_tests[i].id;
        text = mne_id_tests[i].text;
        result = mnemonic_get_id(text);
        if (text != NULL) {
            printf("... mnemonic_get_id(\"%s\") = %d (expected %d)\n",
                   text, result, id);
        } else {
            printf("... mnemonic_get_id(NULL) = %d (expected %d)\n",
                   result, id);
        }
        testcase_assert_true(self, result == id);
    }
    return true;
}

#if 0
/** \brief  Test opcodes in base/cpu
 *
 * \param[out]  total   total number of tests
 * \param[out]  passed  number of passed tests
 *
 * \return  bool
 */
static bool test_opcodes(int *total, int *passed)
{
    *total = 1;
    *passed = 0;

    printf("Opcodes test\n");

    printf(".. testing 65xx:\n");
    opcode_set_cpu_type(CPU_65XX);
    if (opcode_dump_table()) {
        (*passed)++;
    }

    return true;
}
#endif


/** \brief  Create test group 'base/cpu'
 *
 * \return  test group
 */
testgroup_t *get_base_cpu_tests(void)
{
    testgroup_t *group;
    testcase_t *test;

    group = testgroup_new("base/cpu",
                          "Test the base/cpu module",
                          NULL, NULL);

    test = testcase_new("mne-text",
                        "Test mnemonic ID-to-text function mnemonic_get_text()",
                        (int)base_array_len(mne_text_tests),
                        test_mnemonic_text, NULL, NULL);
    testgroup_add_case(group, test);

    test = testcase_new("mne-id",
                        "Test mnemonic text-to-ID function mnemonic_get_id()",
                        (int)base_array_len(mne_id_tests),
                        test_mnemonic_id, NULL, NULL);
    testgroup_add_case(group, test);


    return group;
}
