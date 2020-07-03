/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   error.h
 * \brief   Error hanlding/reporting - header
 */

#ifndef BASE_ERROR_H
#define BASE_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/** \brief  Error codes
 */
enum {
    BASE_ERR_UNKNOWN = -1,  /**< unknown error code */
    BASE_ERR_OK = 0,        /**< OK */
    BASE_ERR_ALLOC,         /**< memory allocation error */
    BASE_ERR_IO,            /**< I/O error */
    BASE_ERR_INVALID_SIZE,  /**< invalid size */
};

/** \brief  Print error code and message on stderr
 *
 * Prints message in the form: "<file>:<line>:<func>: <errcode> <errmsg>".
 * Also prints libc's errno and strerror(errno) in cases where libc might set
 * an error code, such as I/O errors.
 */
#define base_perror(void)                               \
    do {                                                \
        fprintf(stderr,                                 \
                "%s:%d:%s(): %d: %s",                   \
                __FILE__, __LINE__, __func__,           \
                base_errno, base_strerror(base_errno)); \
        if (base_errno == BASE_ERR_IO) {                \
            fprintf(stderr,                             \
                    " (%d: %s)",                        \
                    errno, strerror(errno));            \
        }                                               \
        fputc('\n', stderr);                            \
    } while (0);

extern int base_errno;

const char *base_strerror(int err);

#endif

