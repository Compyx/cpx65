/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   error.c
 * \brief   Error hanlding/reporting
 * \ingroup base
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "error.h"


/** \brief  List of error messages
 */
static const char *err_msgs[] = {
    "<unknown error>",
    "OK",
    "memory allocation error",
    "I/O error",
    "invalid size"
};


/** \brief  Error code for the various binaries
 */
int base_errno = 0;


/** \brief  Get error message for error code \a err
 *
 * \param[in]   err error code
 * \return  string
 */
const char *base_strerror(int err)
{
    if (err < 0 || err >= (int)(sizeof err_msgs / sizeof err_msgs[0])) {
        return err_msgs[0];
    }
    return err_msgs[err + 1];
}
