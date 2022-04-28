/** \file   unit_tests.c
 * \brief   Unit testing binary
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

#include "../base/cmdline.h"
#include "../base/debug.h"
#include "../base/strlist.h"
#include "unit.h"

/*
 * test modules
 */
#include "test_unittest.h"
#include "test_objpool.h"
#include "test_mem.h"
#include "test_cpu.h"
#include "test_io.h"
#include "test_symtab.h"
#include "test_strpool.h"
#include "test_dict.h"


/** \brief  Option result object for -m/--list-modules
 */
static int opt_list_modules = 0;

/** \brief  Option result object for -t/--list-tests
 */
static char *opt_list_tests = NULL;

/** \brief  Option result object for -a/--run-all-modules
 */
static int opt_run_all_modules = 0;

/** \brief  Option result object for -r/--run-module
 */
static char *opt_run_module = NULL;



/** \brief  Command line options
 */
static const cmdline_option_t unit_cmdline_options[] = {
    { 'm', "list-modules", NULL, CMDLINE_TYPE_BOOL,
        &opt_list_modules, (void *)0,
        "list all available unit test modules" },
    { 't', "list-tests", "<module-name>", CMDLINE_TYPE_STR,
        &opt_list_tests, NULL,
        "list tests in <module>" },
    { 'a', "run-all-modules", NULL, CMDLINE_TYPE_BOOL,
        &opt_run_all_modules, NULL,
        "run all registered modules in registered order" },
    { 'r', "run-module", "<module-name>", CMDLINE_TYPE_STR,
        &opt_run_module, NULL,
        "run all tests in a specific module" },
    CMDLINE_OPTION_TERMINATOR
};


/** \brief  Register all modules
 */
static void register_modules(void)
{
    printf("Registering modules...\n");
    unit_module_add(&unittest_module);
    unit_module_add(&mem_module);
    unit_module_add(&objpool_module);
    unit_module_add(&cpu_module);
    unit_module_add(&io_module);
    unit_module_add(&symtab_module);
    unit_module_add(&strpool_module);
    unit_module_add(&dict_module);
}


/** \brief  Entry point
 *
 * Run unit tests
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    int i;
    strlist_t *args = NULL;
    int result;
    int retval = EXIT_SUCCESS;


    register_modules();

    cmdline_init("unit_tests", "0.0.1");
    cmdline_add_options(unit_cmdline_options);

    printf("argc = %d, argv= [\n", argc);
    for (i = 0; i < argc; i++) {
        printf("    '%s'\n", argv[i]);
    }
    printf("]\n");

    result = cmdline_parse(argc, argv, &args);
    base_debug("cmdline_parse() result = %d\n", result);

    switch (result) {
        case CMDLINE_EXIT_HELP: /* fall through */
        case CMDLINE_EXIT_VERSION:
            break;
        case CMDLINE_EXIT_ERROR:
            retval = EXIT_FAILURE;
            break;
        case CMDLINE_EXIT_OK:
            /* Handle option(s) and arg(s) */
            if (opt_run_module != NULL && *opt_run_module != '\0') {
                if (!unit_module_exec_by_name(opt_run_module)) {
                    unit_dump_results();
                    retval = EXIT_FAILURE;
                }
            } else if (opt_run_all_modules) {
                base_debug("Running all unit modules (TODO)\n");
                if (!unit_module_run_all()) {
                    retval = EXIT_FAILURE;
                    break;
                }
            } else if (opt_list_modules) {
                unit_dump_module_list();
            } else if (opt_list_tests != NULL && *opt_list_tests != '\0') {
                unit_dump_module_test_list_by_name(opt_list_tests);
            }
            break;
        default:
            fprintf(stderr, "%s: unknown cmdline parser exit code %d.\n",
                    argv[0], result);
            retval = EXIT_FAILURE;
    }

    cmdline_exit();
    return retval;
}
