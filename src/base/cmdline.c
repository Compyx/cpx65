/** \file   cmdline.c
 * \brief   Commandline handling
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
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "convert.h"
#include "debug.h"
#include "mem.h"
#include "strlist.h"

#include "cmdline.h"


/** \brief  Initial size of the options list array when allocating
 *
 * \note    Currently set to a rediculously small value to test the code
 *          handling the resizing of the list.
 */
#define OPT_LIST_INITIAL_SIZE   4


/*
 * Forward declarations of static functions
 */

static void cmdline_option_list_init(void);
static void cmdline_option_list_free(void);
static void cmdline_option_free(cmdline_option_t *option);
static cmdline_option_t *cmdline_option_init(const cmdline_option_t *option);
static bool cmdline_add_option(const cmdline_option_t *option);


/** \brief  Program name
 *
 * Used for display during --help, --version and error message
 */
static char *prg_name = NULL;


/** \brief  Program version
 *
 * Used for displaying --version
 */
static char *prg_version = NULL;


/** \brief  Prologue function to call for --help (optional)
 */
static void (*prologue)(void) = NULL;


/** \brief  Epilogue function to call for --help (optional)
 *
 * Called after listing the register command line options
 */
static void (*epilogue)(void) = NULL;


/** \brief  List of registered options
 */
static cmdline_option_t **option_list = NULL;


/** \brief  Size of the \c options_list
 */
static size_t option_list_size = 0;


/** \brief  Number of used entries in the \c options_list
 */
static size_t option_list_used = 0;

/** \brief  Pointer to the strlist_t passed to cmdline_parse()
 *
 * Used to free the arguments collected by the parser when calling
 * cmdline_exit(). To keep the argument list, use strlist_dup() (TODO).
 */
static strlist_t **args_list = NULL;


/** \brief  Initialize the command line options list
 *
 * Allocates a list of cmdline_option_t pointers.
 *
 * The list is filled with `NULL` pointers initialy.
 *
 * \see #OPT_LIST_INITIAL_SIZE
 */
static void cmdline_option_list_init(void)
{
    size_t i;

    option_list = base_malloc(OPT_LIST_INITIAL_SIZE * sizeof *option_list);
    option_list_size = OPT_LIST_INITIAL_SIZE;
    option_list_used = 0;
    for (i = 0; i < OPT_LIST_INITIAL_SIZE; i++) {
        option_list[i] = NULL;
    }
}


/** \brief  Free memory used by the command line list and its elements
 *
 * Frees all command line option objects registered and their members and the
 * list itself.
 */
static void cmdline_option_list_free(void)
{
    size_t i;

    for (i = 0; i < option_list_used; i++) {
        cmdline_option_free(option_list[i]);
    }
    base_free(option_list);
}


/** \brief  Add \a option to the command line option list
 *
 * \param[in]   option  command line option
 */
static void cmdline_option_list_add(cmdline_option_t *option)
{
    /* do we need to resize the option list? */
    if (option_list_size == option_list_used) {
        /* yup */
        option_list_size *= 2;
        option_list = base_realloc(option_list,
                option_list_size * sizeof *option_list);
    }
    option_list[option_list_used++] = option;
}


/** \brief  Deallocate \a option and its members
 *
 * \param[in,out]   option  command line option
 */
static void cmdline_option_free(cmdline_option_t *option)
{
    char **tmp;
    strlist_t **lptr;

    switch (option->type) {
        case CMDLINE_TYPE_BOOL: /* fall through */
        case CMDLINE_TYPE_INT:
            /* NOP */
            break;
        case CMDLINE_TYPE_STR:
            tmp = option->target;
            if (*tmp != NULL) {
                base_free(*tmp);
            }
            break;
        case CMDLINE_TYPE_ARR:
            /* free list of strings, if any */
            lptr = option->target;
            if (*lptr != NULL) {
                strlist_free(*lptr);
            }
            break;
        default:
            break;
    }
    base_free(option);
}


/** \brief  Copy \a option and initialize the copy
 *
 * \param[in]   option  command line option
 *
 * \return  initialized copy of \a option, or `NULL` on error
 */
static cmdline_option_t *cmdline_option_init(const cmdline_option_t *option)
{
    cmdline_option_t *newopt = base_malloc(sizeof *newopt);

    int *iptr;
    char **sptr;
    strlist_t **lptr;

    /* copy members */
    newopt->short_opt = option->short_opt;
    newopt->long_opt = option->long_opt;
    newopt->arg_desc = option->arg_desc;
    newopt->type = option->type;
    newopt->target = option->target;
    newopt->factory = option->factory;
    newopt->description = option->description;

    /* initialize target with factory value */
    switch (option->type) {
        case CMDLINE_TYPE_BOOL:
            iptr =newopt->target;
            *iptr = base_ptr_to_int(newopt->factory);
            break;
        case CMDLINE_TYPE_INT:
            iptr = newopt->target;
            *iptr = base_ptr_to_int(newopt->factory);
            break;
        case CMDLINE_TYPE_STR:
            sptr = newopt->target;
            if (newopt->factory != NULL) {
                *sptr = base_strdup((const char *)(newopt->factory));
            } else {
                *sptr = NULL;
            }
            break;
        case CMDLINE_TYPE_ARR:
            lptr = newopt->target;
            *lptr = strlist_init();
            break;
        default:
            base_debug_cmdline("illegal option type %d'\n", newopt->type);
            cmdline_option_free(newopt);
            newopt = NULL;
    }

    return newopt;
}


/** \brief  Initialize command line parser
 *
 * \param[in]   name    program name
 * \param[in]   version program version
 */
void cmdline_init(const char *name, const char *version)
{
    prg_name = base_strdup(name);
    prg_version = base_strdup(version);

    cmdline_option_list_init();
}


/** \brief  Clean up command line parser
 */
void cmdline_exit(void)
{
    base_free(prg_name);
    base_free(prg_version);

    cmdline_option_list_free();
    if (args_list != NULL && *args_list != NULL) {
        strlist_free(*args_list);
        *args_list = NULL;
    }
}


/** \brief  Set prologue function to call for --help
 *
 * Called in handling of --help after outputting "Usage: <prg_name> ..." but
 * before listing the registers options.
 *
 * \param[in]   func    pointer to prologue function
 */
void cmdline_set_prologue(void (*func)(void))
{
    prologue = func;
}


/** \brief  Set epilogue function to call for --help
 *
 * Called after listing the register command line options.
 *
 * \param[in]   func    pointer to epilogue function
 */
void cmdline_set_epilogue(void (*func)(void))
{
    epilogue = func;
}


/** \brief  Show program name and version on stdout
 */
void cmdline_show_version(void)
{
    printf("%s %s\n", prg_name, prg_version);
}


/** \brief  Add \a option to the command line option list
 *
 * \param[in]   option  command line option
 *
 * \return  `true` on success, `false` when \a option wasn't correct
 */
static bool cmdline_add_option(const cmdline_option_t *option)
{
    cmdline_option_t *newopt = cmdline_option_init(option);

    if (newopt == NULL) {
        return false;
    }
    cmdline_option_list_add(newopt);
    return true;
}


/** \brief  Add a list of command line options to the command line parser
 *
 * \param[in]   options list of command line options
 *
 * \note    The list needs be terminated by #CMDLINE_OPTION_TERMINATOR
 *
 * \return  bool
 */
bool cmdline_add_options(const cmdline_option_t *options)
{
    const cmdline_option_t *opt;

    for (opt = options;
            opt->short_opt > 0
            || (opt->long_opt != NULL && *(opt->long_opt) != '\0');
            opt++)
    {
        base_debug_cmdline("Adding option -%c/--%s (%s)\n",
                opt->short_opt, opt->long_opt, opt->description);
        if (!cmdline_add_option(opt)) {
            return false;
        }
    }
    return true;
}


/** \brief  List registered command line options
 */
void cmdline_list_options(void)
{
    size_t i;

    for (i = 0; i < option_list_used; i++) {
        cmdline_option_t *option = option_list[i];
        int sopt = option->short_opt;
        const char *lopt = option->long_opt;
        cmdline_option_type_t type = option->type;
        const char *arg_desc = option->arg_desc;
        int c;


        if (sopt > 0 && lopt != NULL) {
            c = printf("   -%c, --%s", sopt, lopt);
        } else if (sopt > 0 && lopt == NULL) {
            c = printf("   -%c", sopt);
        } else {
            c = printf("       --%s", lopt);
        }
        /* add argument description */
        if (type != CMDLINE_TYPE_BOOL) {
            c += printf(" %s", arg_desc);
        }

        while (c < 40) {
            putchar(' ');
            c++;
        }
        printf("%s\n", option->description);
    }
}


/** \brief  Show help on stdout
 */
void cmdline_show_help(void)
{
    printf("Usage: %s [<options>] [<args>]\n\n", prg_name);
    if (prologue != NULL) {
        prologue();
    }
    cmdline_list_options();
    if (epilogue != NULL) {
        epilogue();
    }
}


/** \brief  Find short option
 *
 * \param[in]   opt         argv element containing option (and perhaps argument)
 * \param[out]  argptr      pointer to argument in \a opt, if present
 * \param[out]  with_arg    \a opt contained argument to option
 *
 * \return  pointer to option list element or NULL on error
 */
static cmdline_option_t *option_find_short(const char *opt,
                                           const char **argptr,
                                           bool *with_arg)
{
    for (size_t i = 0; i < option_list_used; i++) {
        cmdline_option_t *option = option_list[i];

        if (option->short_opt == opt[1]) {
            if (opt[2] != '\0') {
                /* got argument inside same argv element */
                *with_arg = true;
                *argptr = opt + 2;
            } else {
                *with_arg = false;
                *argptr = NULL;
            }
            return option;
        }
    }
    fprintf(stderr, "%s: error: invalid option '-%c'\n", prg_name, opt[1]);
    return NULL;
}


/** \brief  Find long option
 *
 * \param[in]   opt         argv element containing option (and perhaps argument)
 * \param[out]  argptr      pointer to argument in \a opt, if present
 * \param[out]  with_arg    \a opt contained argument to option
 *
 * \return  pointer to option list element or NULL on error
 */
static cmdline_option_t *option_find_long(const char *opt,
                                          const char **argptr,
                                          bool *with_arg)
{
    size_t len;
    const char *p;

    *with_arg = false;
    *argptr = NULL;

    /* find '=', if any */
    for (p = opt + 2; *p != '\0'; p++) {
        if (*p == '=') {
            *with_arg = true;
            *argptr = p + 1;
            break;

        }
    }
    len = (size_t)(p - opt - 2);

    /* reject anything shorter than two characters (also catches '--=') */
    if (len < 2) {
        fprintf(stderr, "%s: error: invalid option %s\n", prg_name, opt);
        return NULL;
    }

    /* find option */
    for (size_t i = 0; i < option_list_used; i++) {
        cmdline_option_t *option = option_list[i];

        if (option->long_opt != NULL &&
                strncmp(option->long_opt, opt + 2, len) == 0) {
            return option;
        }
    }
    fprintf(stderr, "%s: error: invalid option %s\n", prg_name, opt);
    return NULL;
}


/** \brief  Handle \a option
 *
 * \param[in]   opt     argv element with possible option
 * \param[in]   arg     argv element with optional option argument
 *
 * \return  extra offset in argv, or -1 on error
 */
static int option_handle(const char *opt, const char *arg)
{
    cmdline_option_t *option;
    const char *argptr; /* option's argument, either inside opt, or arg itself */
    char *endptr;
    char **sptr;
    long num;
    int delta;      /* extra offset to add to index in argv */
    bool with_arg;  /* option argument is contained in argv element */

    if (opt[1] == '-') {
        /* long option */
        option = option_find_long(opt, &argptr, &with_arg);
    } else {
        /* short option */
        option = option_find_short(opt, &argptr, &with_arg);
    }
    if (option == NULL) {
        /* error has already been reported */
        return -1;
    }
    if (argptr == NULL) {
        /* no argument found inside option */
        argptr = arg;
    }


#ifdef HAVE_DEBUG
    if (option->short_opt > 0 && option->long_opt != NULL) {
        base_debug_cmdline("handling option -%c/--%s",
                option->short_opt, option->long_opt)
    } else if (option->short_opt > 0) {
        base_debug_cmdline("handling option -%c", option->short_opt)
    } else {
        base_debug_cmdline("handling option --%s", option->long_opt);
    }
#endif

    base_debug_cmdline("arg to option: '%s'", argptr);

    /* check for option argument */
    if (option->type != CMDLINE_TYPE_BOOL &&
            (argptr == NULL || *argptr == '\0')) {
        fprintf(stderr, "%s: error: missing argument.\n", prg_name);
        return -1;
    }

    delta = 0;
    switch (option->type) {

        /* boolean */
        case CMDLINE_TYPE_BOOL:
            *((int *)(option->target)) = 1;
            break;

        /* integer */
        case CMDLINE_TYPE_INT:
            num = strtol(argptr, &endptr, 0);
            if (*endptr != '\0') {
                /* failed, report error */
                fprintf(stderr,
                        "%s: error: failed to convert argument '%s' to integer.\n",
                        prg_name, argptr);
                delta = -1;
            } else {
                *((int *)(option->target)) = (int)num;
                if (!with_arg) {
                    delta = 1;
                }
            }
            break;

        /* string */
        case CMDLINE_TYPE_STR:
            /* free factory value, if available */
            sptr = (char **)(option->target);
            if (*sptr != NULL) {
                base_free(*sptr);
            }
            *sptr = base_strdup(argptr);
            if (!with_arg) {
                delta = 1;
            }
            break;

        /* array */
        case CMDLINE_TYPE_ARR:
            strlist_add(*(strlist_t **)(option->target), argptr);
            if (!with_arg) {
                delta = 1;
            }
            break;

        /* default: error out */
        default:
            base_debug_cmdline("Unsupported CMDLINE_TYPE %d", option->type);
            delta = -1;
            break;
    }

    return delta;
}


/** \brief  Parse command line arguments for options and arguments
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument count
 * \param[out]  list    object to store reference to list of non-option args
 *
 * \note    The \a list is freed when calling cmdline_exit(), so if the list
 *          of args needs to be kept after calling cmdline_exit(), use the
 *          strlist_t function strlist_dup().
 *
 * \return  >= 0 on success, -1 on failure
 */
int cmdline_parse(int argc, char *argv[], strlist_t **list)
{
    int i;

#ifdef HAVE_DEBUG
    /* show input */
    base_debug_cmdline("argc = %d\n", argc);
    for (i = 0; i < argc; i++) {
        base_debug_cmdline("argv[%d] = '%s'\n", i, argv[i]);
    }
#endif

    /* store pointer to string list so we can clean it up on calling
     * cmdline_exit()
     */
    args_list = list;
    /* initialize list of non-option args */
    *list = strlist_init();

    /* process argv */
    for (i = 1; i < argc; i++) {
        int delta = 0;  /* extra offset to add to the index */

        base_debug_cmdline("parsing argv[%d]: '%s'", i, argv[i]);
        if (argv[i][0] == '-') {
            /* possible option */
            base_debug_cmdline(".. found possible option '%s'", argv[i]);

            /*
             * check --help and --version first
             */
            if (argv[i][1] == '-') {
                if (strcmp(argv[i], "--help") == 0) {
                    base_debug_cmdline(".... --help requested");
                    cmdline_show_help();
                    return CMDLINE_EXIT_HELP;
                } else if (strcmp(argv[i], "--version") == 0) {
                    base_debug_cmdline(".... --version requested");
                    cmdline_show_version();
                    return CMDLINE_EXIT_VERSION;
                }
            }
            /* handle option */
            delta = option_handle(argv[i], argv[i + 1]);
            if (delta < 0) {
                /* error has been already reported */
                return CMDLINE_EXIT_ERROR;
            }

        } else {
            /* non-option argument */
            base_debug_cmdline(".. adding non-option argument '%s'", argv[i]);
            strlist_add(*list, argv[i]);
        }
        i += delta;
    }

    return CMDLINE_EXIT_OK;
}
