/* vim: set et ts=4 sts=4 sw=4 syntax=c.doxygen: */

#include <stdio.h>
#include <stdlib.h>

#include "../base/debug.h"
#include "../base/cmdline.h"
#include "../base/strlist.h"
#include "../base/cpu/cputype.h"


/** \brief  Option result for -b/--binary
 *
 * Treat input file as if not having a load address in its first two bytes.
 *
 * Default = false
 */
static int opt_binary_mode = 0;

static int opt_break_return = 0;
static int opt_break_undoc = 0;
static int opt_skip = -1;
static int opt_number = -1;
static int opt_address = -1;
static const char *opt_machine = NULL;


/** \brief  Command line options
 */
static const cmdline_option_t disasm_cmdline_options[] = {
    { 'a', "address", "<INTEGER>", CMDLINE_TYPE_INT,
        &opt_address, (void*)-1,
        "Override/set address" },
    { 'b', "binary", NULL, CMDLINE_TYPE_BOOL,
        &opt_binary_mode, (void *)0,
        "Ignore 2-byte load address" },
    { 'm', "machine", "<machine-name>", CMDLINE_TYPE_STR,
        &opt_machine, "65xx",
        "Set machine type" },
    { 'n', "number", "<INTEGER>", CMDLINE_TYPE_INT,
        &opt_number, (void*)-1,
        "Number of bytes to disassemble" },
    { 'r', "break-return", NULL, CMDLINE_TYPE_BOOL,
        &opt_break_return, (void *)0,
        "Stop disassembly on RTS and RTI" },
    { 's', "skip", "<INTEGER>", CMDLINE_TYPE_INT,
        &opt_skip, (void *)-1,
        "Number of bytes to skip of input" },
    { 'u', "break-undoc", NULL, CMDLINE_TYPE_BOOL,
        &opt_break_undoc, (void *)0,
        "Stop disassembly on undocumented opcode (6502/6510 only)" },
    CMDLINE_OPTION_TERMINATOR
};


/** \brief  Dissambler driver
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  EXIT_SUCCESS on succes, EXIT_FAILURE on failure
 */
int main(int argc, char *argv[])
{
    int retval = EXIT_SUCCESS;
    int result;
    strlist_t *args = NULL;
    int cpu_id;

#ifdef HAVE_DEBUG
    base_debug("argc = %d\nargv = [", argc);
    for (int i = 0; i < argc; i++) {
        printf("    %d: \"%s\"\n", i, argv[i]);
    }
    printf("]\n");
#endif

    cmdline_init("cpx65dis", "0.0.1");
    cmdline_add_options(disasm_cmdline_options);

    result = cmdline_parse(argc, argv, &args);
    base_debug("result = %d", result);

    switch (result) {
        case CMDLINE_EXIT_HELP:     /* fall through */
        case CMDLINE_EXIT_VERSION:
            break;
        case CMDLINE_EXIT_ERROR:
            retval = EXIT_FAILURE;
            break;
        case CMDLINE_EXIT_OK:
            if (opt_address < 0) {
                printf("address      = from binary\n");
            } else {
                printf("address      = %04x\n", opt_address);
            }
            printf("binary       = %s\n", opt_binary_mode ? "true" : "false");
            printf("break-return = %s\n", opt_break_return ? "true" : "false");
            printf("break-undoc  = %s\n", opt_break_undoc ? "true" : "false");
            printf("skip         = %d\n", opt_skip);
            printf("number       = %d\n", opt_number);
            printf("machine      = %s", opt_machine);
            cpu_id = cpu_type_get_id_by_name(opt_machine);
            if (cpu_id < 0) {
                printf(" (unknown cpu)\n");
            } else {
                printf(" (id = %d)\n", cpu_id);
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
