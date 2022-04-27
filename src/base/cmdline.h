/** \file   cmdline.h
 * \brief   Command line handling - header
 */

#ifndef CMDLINE_H
#define CMDLINE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "strlist.h"

/** \brief  Command line option types
 */
typedef enum cmdline_option_type_e {
    CMDLINE_TYPE_BOOL,  /**< boolean */
    CMDLINE_TYPE_INT,   /**< integer */
    CMDLINE_TYPE_STR,   /**< string */
    CMDLINE_TYPE_ARR    /**< array */
} cmdline_option_type_t;


/** \brief  Command line parser exit codes
 */
enum {
    CMDLINE_EXIT_ERROR = -1,    /**< an error occured during parsing */
    CMDLINE_EXIT_OK = 0,        /**< OK */
    CMDLINE_EXIT_HELP,          /**< --help requested and shown */
    CMDLINE_EXIT_VERSION        /**< --version requested and shown */
};


/** \brief  Command line option object
 *
 * Contains data on a single command line option.
 *
 * For short options use 0 to indicate there is none, for long options use
 * `NULL` to indicate there is none. Do not use '-' or '--' for these, the
 * parser will add them.
 *
 */
typedef struct cmdline_option_t {
    int                     short_opt;      /**< short option name */
    const char *            long_opt;       /**< long option name */
    const char *            arg_desc;       /**< argument description (used
                                                 for non-boolean options) */
    cmdline_option_type_t   type;           /**< option type */
    void *                  target;         /**< option value target */
    void *                  factory;        /**< factory option value */
    const char *            description;    /**< option description for help */
} cmdline_option_t;


/** \brief  Terminator for a list of cmd_line_option_t's
 */
#define CMDLINE_OPTION_TERMINATOR { 0, NULL, NULL, 0, NULL, NULL, NULL }


void cmdline_init(const char *name, const char *version);
void cmdline_exit(void);

void cmdline_set_prologue(void (*func)(void));
void cmdline_set_epilogue(void (*func)(void));

void cmdline_show_help(void);
void cmdline_show_version(void);

bool cmdline_add_options(const cmdline_option_t *options);
void cmdline_list_options(void);

int  cmdline_parse(int argc, char *argv[], strlist_t **list);

#endif
