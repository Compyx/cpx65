/** \file   test_cpu.c
 * \brief   Unit tests for base/cpu/
 *
 * Some simple unit tests for the CPU-info code in base/cpu.
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../base/debug.h"
#include "unit.h"
#include "../base/cpu/addrmode.h"
#include "../base/cpu/cputype.h"
#include "../base/cpu/mnemonic.h"
#include "../base/cpu/opcode.h"

#include "test_cpu.h"


static bool test_mnemonic_text(int *total, int *passed);
static bool test_mnemonic_id(int *total, int *passed);
static bool test_opcodes(int *total, int *passed);

static unit_test_t tests[] = {
    { "mne-text", "Mnemonic id-to-text", test_mnemonic_text, false },
    { "mne-id", "Mnemonic lookup ID", test_mnemonic_id, false },
    { "opcode", "Opcode table checks", test_opcodes, false },
    { NULL, NULL, NULL, false }
};


/** \brief  Public unit test module
 */
unit_module_t cpu_module = {
    "cpu",
    "Test base/cpu/*",
    NULL, NULL,
    0, 0,
    tests
};


typedef struct mne_text_test_s {
    mnemonic_id_t   id;         /**< mnemonic ID */
    const char *    expected;   /**< expected mnemonic string */
} mne_text_test_t;

static const mne_text_test_t text_tests[] = {
    { MNE_ADC, "adc" },
    { MNE_ILL, "???" },
    { MNE_TYA, "tya" },
};


/** \brief  Test the #mnemonic_get_text() function
 *
 * \param[out]  total   total number of tests
 * \param[out]  passed  number of passed tests
 *
 * \return  bool
 */
static bool test_mnemonic_text(int *total, int *passed)
{
    for (size_t i = 0; i < sizeof text_tests / sizeof text_tests[0]; i++) {

        mnemonic_id_t id = text_tests[i].id;
        const char *expected = text_tests[i].expected;
        const char *text = mnemonic_get_text(id);
        int result;

        result = strcmp(text, expected);

        printf(".. mnemonic_get_text(%d) = '%s', expected '%s' -> %s\n",
                (int)id, text, expected,
                result == 0 ? "OK" : "failed");

        (*total)++;
        (*passed) += result == 0 ? 1 : 0;
    }
    return true;
}


/** \brief  Test the #mnemonic_get_id() function
 *
 * \param[out]  total   total number of tests
 * \param[out]  passed  number of passed tests
 *
 * \return  bool
 */
static bool test_mnemonic_id(int *total, int *passed)
{
    printf("ID('aDc') = %d\n", mnemonic_get_id("aDc"));
    printf("ID('TyA') = %d\n", mnemonic_get_id("TyA"));

    (*total) += 2;
    (*passed) += 2;
    return true;
}


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
