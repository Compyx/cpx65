/** \file   testrunner.c
 * \brief   Run unit tests
 * \author  Bas Wassink <b.wassink@ziggo.nl>
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "../base/cmdline.h"
#include "../base/mem.h"
#include "testcase.h"

#include "test_testcase.h"
#include "test_base_cpu.h"
//#include "test_io.h"
//#include "test_keywords.h"


/** \brief  Command line option: disable colored output (--no-color)
 */
static bool opt_no_color = false;

/** \brief  Command line option: List groups and their cases (--list-all)
 */
static bool opt_list_all = false;

/** \brief  Command line option: List groups (--list-groups)
 */
static bool opt_list_groups = false;

/** \brief  Command line option: List cases of a group (--list-cases \<group\>)
 */
static char *opt_list_cases = NULL;

/** \brief  Command line option: Execute all cases in all groups (-a)
 */
static bool opt_exec_all = false;

/** \brief  Command line option: Execute all cases in a group (-g)
 */
static char *opt_exec_group = NULL;

/** \brief  Command line option: Execute single case in a group (-g -c)
 */
static char *opt_exec_case = NULL;


/** \brief  List of registered test groups
 */
static testgroup_t **group_list = NULL;

/** \brief  Number of registered test groups
 */
static int group_count = 0;


/** \brief  Command line options
 */
static cmdline_option_t options[] = {
    { 0, "no-color", NULL, CMDLINE_TYPE_BOOL,
        &opt_no_color, (void*)false,
        "disable colored output" },
    { 0, "list-all", NULL, CMDLINE_TYPE_BOOL,
        &opt_list_all, (void*)false,
        "list all groups and cases" },
    { 0, "list-groups", NULL, CMDLINE_TYPE_BOOL,
        &opt_list_groups, (void*)false,
        "list available groups" },
    { 0, "list-cases", "GROUP", CMDLINE_TYPE_STR,
        &opt_list_cases, NULL,
        "list cases in a group" },
    { 'a', "all", NULL, CMDLINE_TYPE_BOOL,
        &opt_exec_all, (void*)false,
        "execute all cases in all groups" },
    { 'g', "group", "GROUP", CMDLINE_TYPE_STR,
        &opt_exec_group, NULL,
        "execute all cases in a group" },
    { 'c', "case", "CASE", CMDLINE_TYPE_STR,
        &opt_exec_case, NULL,
        "execute single case (requires --group <GROUP>)" },
    CMDLINE_OPTION_TERMINATOR
};


/** \brief  Register group
 *
 * Add \a group to the list of test groups.
 *
 * \param[in]   group   test group
 */
static void register_group(testgroup_t *group)
{
    group_list = base_realloc(group_list,
                              (size_t)(group_count + 1) * sizeof *group_list);
    group_list[group_count++] = group;
}


/** \brief  List registered groups
 *
 * Prints a list of group names and descriptions on stdout.
 *
 * \param[in]   cases   also list all cases of each group
 */
static void list_groups(bool cases)
{
    for (int g = 0; g < group_count; g++) {
        const testgroup_t *group = group_list[g];

        printf("%-19s %s\n", group->name, group->desc);
        if (cases) {
            testgroup_list_cases(group);
        }
    }
}


/** \brief  Register test groups
 *
 * This function registers all available groups.
 *
 * Whenever writing a new group of tests for a module, add a call to
 * register_group() here, calling a function in the file running the tests that
 * provides the group.
 */
static void register_groups(void)
{
    register_group(get_testcase_tests());
//    register_group(get_io_tests());
 //   register_group(get_keywords_tests());
 //
    register_group(get_base_cpu_tests());
}


/** \brief  Free all memory used by test groups and their cases
 */
static void unregister_groups(void)
{
    for (int g = 0; g < group_count; g++) {
        testgroup_free(group_list[g]);
    }
    base_free(group_list);
    group_list = NULL;
    group_count = 0;
}


/** \brief  Find group by name
 *
 * \param[in]   name    group name
 *
 * \return  group or `NULL` when not found
 */
static testgroup_t *find_group(const char *name)
{

    for (int g = 0; g < group_count; g++) {
        testgroup_t *group = group_list[g];

        if (strcmp(group->name, name) == 0) {
            return group;
        }
    }
    fprintf(stderr, "error: unknown group '%s'.\n", name);
    return NULL;
}


/** \brief  Execute all tests in a group
 *
 * \param[in]   name    group name
 *
 * \return  false on fatal error
 */
static bool execute_group(const char *name)
{
    testgroup_t *group = find_group(name);

    if (group == NULL) {
        return false;
    }
    return testgroup_exec_cases(group);
}


/** \brief  Execute a single case in a single group
 *
 * \param[in]   group_name  group name
 * \param[in]   case_name   case name
 *
 * \return  false on fatal error
 */
static bool execute_case(const char *group_name, const char *case_name)
{
    testgroup_t *group = find_group(group_name);

    if (group == NULL) {
        return false;
    }
    return testgroup_exec_case(group, case_name);
}


/** \brief  List test cases available in a group
 *
 * \param[in]   name    group name
 *
 * \return  false if \a name not found
 */
static bool list_group_cases(const char *name)
{
    testgroup_t *group = find_group(name);

    if (group == NULL) {
        return false;
    }
    testgroup_list_cases(group);
    return true;
}


/** \brief  Execute all cases in all groups
 *
 * Runs all tests available, in the order they were registered.
 *
 * \return  false on fatal error
 */
static bool execute_all(void)
{
    for (int g = 0; g < group_count; g++) {
        testgroup_t *group = group_list[g];

        if (!testgroup_exec_cases(group)) {
            fprintf(stderr, "error: fatal error during test execution, aborting.\n");
            return false;
        }
    }
    /* TODO: list totals! */
    return true;
}


/** \brief  Test driver entry point
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    int optres;
    strlist_t *args = NULL; /* freed by cmdline_exit() */
    bool result = true;

    cmdline_init("testrunner", "0.1");
    cmdline_add_options(options);

    if (argc < 2) {
        cmdline_show_help();
        cmdline_exit();
        return EXIT_FAILURE;
    }

    optres = cmdline_parse(argc, argv, &args);
    if (optres == CMDLINE_EXIT_ERROR) {
        /* error during parsing, already reported */
        cmdline_exit();
        return EXIT_FAILURE;
    } else if (optres == CMDLINE_EXIT_HELP || optres == CMDLINE_EXIT_VERSION) {
        cmdline_exit();
        return EXIT_SUCCESS;
    }

    testcase_enable_ansi(!opt_no_color);
    register_groups();

    if (opt_list_all) {
        list_groups(true);
    } else if (opt_list_groups) {
        list_groups(false);
    } else if (opt_list_cases != NULL) {
        list_group_cases(opt_list_cases);
    } else if (opt_exec_all) {
        printf("running all cases in all groups:\n");
        result = execute_all();
    } else if (opt_exec_case != NULL && opt_exec_group == NULL) {
        fprintf(stderr, "error: `--case` requires `--group <group-name>`.\n");
        result = false;
    } else if (opt_exec_group != NULL) {
        if (opt_exec_case != NULL) {
            result = execute_case(opt_exec_group, opt_exec_case);
        } else {
            result = execute_group(opt_exec_group);
        }
    }

    unregister_groups();
    cmdline_exit();

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
