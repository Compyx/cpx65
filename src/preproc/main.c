/** \file   main.c
 * \brief   Preprocessor driver
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

#include "../base/cmdline.h"
#include "../base/strlist.h"


static strlist_t *defines = NULL;
static strlist_t *includes = NULL;
static strlist_t *libraries = NULL;


/** \brief  Command line options
 */
static const cmdline_option_t options[] = {
    { 'D', NULL, "SYMBOL", CMDLINE_TYPE_ARR,
        &defines, NULL,
        "Define SYMBOL" },
    { 'I', NULL, "PATH", CMDLINE_TYPE_ARR,
        &includes, NULL,
        "Add PATH to the search path for source files" },
    { 'L', NULL, "PATH", CMDLINE_TYPE_ARR,
        &libraries, NULL,
        "Add PATH to the search path for the linker" },

    CMDLINE_OPTION_TERMINATOR
};


/** \brief  Program driver
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  `EXIT_SUCCESS` or `EXIT_FAILURE`
 */
int main(int argc, char *argv[])
{
    int result;
    strlist_t *args = NULL;

    cmdline_init("cpx65pp", "0.1");
    cmdline_add_options(options);

    if (argc < 2) {
        cmdline_show_help();
        cmdline_exit();
        return EXIT_FAILURE;
    }

    result = cmdline_parse(argc, argv, &args);
    if (result == CMDLINE_EXIT_ERROR) {
        cmdline_exit();
        return EXIT_FAILURE;
    } else if (result == CMDLINE_EXIT_HELP || result == CMDLINE_EXIT_VERSION) {
        cmdline_exit();
        return EXIT_SUCCESS;
    }

    if (defines != NULL) {
        printf("List of symbols (-D):\n");
        strlist_dump(defines);
    }
    if (includes != NULL) {
        printf("List of search paths for source files (-I):\n");
        strlist_dump(includes);
    }
    if (libraries != NULL) {
        printf("List of search paths for the linker (-L):\n");
        strlist_dump(libraries);
    }


    /* string lists are freed by the cmdline parser */
    cmdline_exit();
    return EXIT_SUCCESS;
}
