#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../base/debug.h"
#include "../base/cmdline.h"
#include "../base/mem.h"
#include "../base/strlist.h"
#include "../base/cpu/cputype.h"
#include "../base/cpu/opcode.h"
#include "../base/io/binfile.h"


/* Forward declarations
 */
static long disassemble(const char *path);


/** \brief  Option result for -b/--binary
 *
 * Treat input file as if not having a load address in its first two bytes.
 *
 * Default = false
 */
static int opt_binary_mode = 0;

/** \brief  Option result for -r/--break-return
 *
 * Stop disassembling on RTS/RTI.
 *
 * Default = false
 */
static int opt_break_return = 0;

/** \brief  Option result for -u/--break-undoc
 *
 * Stop disassembling on undocumented opcode (65xx, 2a03/2a07).
 *
 * Default = false
 */
static int opt_break_undoc = 0;

/** \brief  Option result for --break-branch
 *
 * Stop disassembling on branch instruction.
 *
 * Default = false
 */
static int opt_break_branch = 0;

/** \brief  Option result for -s/--skip
 *
 * Skip bytes from binary start to start disassembly.
 *
 * Default = false (-1)
 */
static int opt_skip = -1;

/** \brief  Option result for -n/--number
 *
 * Set number of bytes to disassemble.
 *
 * Default = all (-1)
 */
static int opt_number = -1;

/** \brief  Option result for -a/--address
 *
 * Override disassemble address display.
 *
 * Default = false (-1)
 */
static int opt_address = -1;

/** \brief  Option result for -m/--machine
 *
 * Set CPU/machine type.
 *
 * Default = 65xx (6502 with undocumented opcodes)
 *
 * \todo    Properly support
 */
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
    { 0,  "break-branch", NULL, CMDLINE_TYPE_BOOL,
        &opt_break_branch, (void *)0,
        "Stop disassembly on branch instruction" },
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
                retval = EXIT_FAILURE;
            } else {
                printf(" (id = %d)\n", cpu_id);
            }
            printf("disassembled %ld bytes\n",
                    disassemble(strlist_item(args, 0)));
            break;
        default:
            fprintf(stderr, "%s: unknown cmdline parser exit code %d.\n",
                    argv[0], result);
            retval = EXIT_FAILURE;
    }

    cmdline_exit();
    return retval;
}


/** \brief  Disassemble file \a path
 *
 * \param[in]   path    path to file to disassemble
 *
 * \return  number of bytes processed or -1 on error
 */
static long disassemble(const char *path)
{
    long fsize;
    uint8_t *data = NULL;
    int address;
    int index;
    int count = 0;

    base_debug("Reading '%s'\n", path);
    fsize = base_binfile_read(path, &data);
    base_debug("Bytes read = %ld ($%lx)\n", fsize, fsize);
    if (fsize < 0) {
        return EXIT_FAILURE;
    }
    /* add 8 bytes for overflow */
    data = base_realloc(data, (size_t)(fsize + 8));
    memset(data + fsize, 0, 8);

    /* check skip */
    if (opt_skip > 0) {
        if (opt_skip >= fsize) {
            base_free(data);
            return 0;
        }
    }


    /* determine address */
    if (opt_binary_mode) {
        if (opt_address >= 0) {
            address = opt_address;
        } else {
            address = 0;
        }
        index= 0;
    } else {
        if (opt_address >= 0) {
            address = opt_address;
        } else {
            address = data[0] + data[1] * 256;
        }
        index = 2;
    }

    /* TODO: guard against buffer overflows! */

    while (true) {

        int opc;
        int bytes;
        int b;
        int i;
        uint8_t raw_data[8];
        unsigned int rel_addr;
        opcode_data_t opc_data;

        if (count >= fsize) {
            break;
        }

        if (opt_number > 0 && count >= opt_number) {
            break;
        }

        printf(".%04x  ", (unsigned int)(address));

        /* get opcode */
        opc = data[index];
        memcpy(raw_data, data + index, 8);
        opcode_get_data(opc, &opc_data);

        bytes = 1;
        for (i = 0; i < 4; i++) {
            bytes += opc_data.opr_sizes[i];
        }
        for (b = 0; b < bytes; b++) {
            printf("%02x ", data[index + b]);
        }

        for (i = 0; i < 7 - bytes; i++) {
            printf("   ");
        }


        /* print mnemonic */
        printf("%s ", opc_data.mne_text);

        /* print operands */
        switch (opc_data.amd_id) {
            case AMD_ACC:   /* fall through */
            case AMD_IMP:
                break;
            case AMD_IMM:
                printf("#$%02x", raw_data[1]);
                break;
            case AMD_ZP:
                printf("$%02x", raw_data[1]);
                break;
            case AMD_ZPX:
                printf("$%02x,X", raw_data[1]);
                break;
            case AMD_ZPY:
                printf("$%02x,Y", raw_data[1]);
                break;
            case AMD_IZX:
                printf("($%02x,X)", raw_data[1]);
                break;
            case AMD_IZY:
                printf("($%02x),Y", raw_data[1]);
                break;
            case AMD_ABS:
                printf("$%02x%02x", raw_data[2], raw_data[1]);
                break;
            case AMD_ABX:
                printf("$%02x%02x,X", raw_data[2], raw_data[1]);
                break;
            case AMD_ABY:
                printf("$%02x%02x,Y", raw_data[2], raw_data[1]);
                break;
            case AMD_IAB:
                printf("($%02x%02x)", raw_data[2], raw_data[1]);
                break;
            case AMD_REL:
                if (raw_data[1] >= 0x80) {
                    rel_addr = (unsigned int)(address - (0x100 - raw_data[1] - 2));
                } else {
                    rel_addr = (unsigned int)(address + raw_data[1] + 2);
                }
                printf("$%04x", rel_addr);
                break;

            default:
                printf("Unknow addressing mode %d, aborting.\n",
                        opc_data.amd_id);
                abort();
        }


        putchar('\n');

        /* break on undoc */
        if (opt_break_undoc && opc_data.opc_type != OPC_TYPE_NORMAL) {
            printf("Found undocumented opcode $%02x, breaking\n", opc);
            break;
        }

        /* break on rts/rti */
        if (opt_break_return && (opc == 0x40  || opc == 0x60)) {
            printf("Found RTI/RTS, breaking\n");
            break;
        }

        /* break on branch */
        if (opt_break_branch && opcode_is_branch(opc)) {
            printf("Got branch\n");
            break;
        }


        address += bytes;
        count += bytes;
        index += bytes;
    }

    base_free(data);
    return count;
}
