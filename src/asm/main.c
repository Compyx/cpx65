/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   main.c
 * \brief   Main entry point, currently used for some test code
 */

#include <stdio.h>
#include <stdlib.h>

#include "../base/cmdline.h"
#include "../base/mem.h"
#include "../base/error.h"
#include "../base/strlist.h"
#include "../base/objpool.h"
#include "../base/cpu/mnemonic.h"

/** \brief  Commandline parser boolean test object
 */
static int bool_test;

/** \brief  Commandline parser integer test object
 */
static int int_test;

/** \brief  Commandline parser string test object
 */
static char *str_test;

/** \brief  Commandline parser array test object
 */
static strlist_t *defines;


/** \brief  Command line options for testing the command line parser
 */
static const cmdline_option_t cmdline_tests[] = {
    { 'b', "boolean", NULL, CMDLINE_TYPE_BOOL,
        &bool_test, (void *)0,
        "boolean test" },
    { 'i', "integer", "<integer>", CMDLINE_TYPE_INT,
        &int_test, (void *)256,
        "integer test" },
    { 's', "string", "<string>", CMDLINE_TYPE_STR,
        &str_test, (void*)"Hello World",
        "string test" },
    { 'D', "define", "<define>", CMDLINE_TYPE_ARR,
        &defines, NULL,
        "string list test" },
    CMDLINE_OPTION_TERMINATOR
};


/** \brief  Prologue function for --help
 */
static void prologue(void)
{
    printf("----> Prologue function called <----\n");
    printf("  This binary is a simple test for a few things until I finish"
            " the cmdline and unit test code.\n");
    printf("----> End prologue output <----\n");
}


/** \brief  Prologue function for --help
 */
static void epilogue(void)
{
    printf("----> Epilogue function called <----\n");
}


/** \brief  Main entry point
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    int result;
    strlist_t *args;

    cmdline_init("cpx65", "0.0.1");
    cmdline_set_prologue(prologue);
    cmdline_set_epilogue(epilogue);
    cmdline_add_options(cmdline_tests);
#if 0
    cmdline_show_version();
    cmdline_list_options();
#endif
    printf("\nTesting command line parser:\n");
    result = cmdline_parse(argc, argv, &args);
    printf("parser result = %d\n", result);
    switch (result) {
        case CMDLINE_EXIT_OK:
            printf("boolean = %d\n", bool_test);
            printf("integer = %d\n", int_test);
            printf("string  = '%s'\n", str_test);
            printf("defines = ...\n");
            strlist_dump(defines);

            printf("non-option arguments:\n");
            strlist_dump(args);
            break;
        case CMDLINE_EXIT_HELP: /* fall through */
        case CMDLINE_EXIT_VERSION:
            cmdline_exit();
            return EXIT_SUCCESS;
        default:
            /* error */
            base_perror();
            cmdline_exit();
            return EXIT_FAILURE;
    }

    cmdline_exit();


    printf("\nTesting base_perror():\n");
    base_errno = BASE_ERR_ALLOC;
    base_perror();
    base_errno = BASE_ERR_IO;
    base_perror();

    const char *mne = mnemonic_get_text(MNE_TAX);
    printf("MNE_TAX = '%s'\n", mne);


    return EXIT_SUCCESS;
}

