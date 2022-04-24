/** \file   txtfile.h
 * \brief   Text file handling - header
 * \ingroup base
 */

#ifndef BASE_IO_TXT_FILE_H
#define BASE_IO_TXT_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/** \brief  Text file handle
 *
 * Contains information on a text file and can be used in a 'file stack', which
 * is something I still need to implement, to handle #include's.
 */
typedef struct txtfile_s {
    FILE *fp;           /**< file pointer */
    char *path;         /**< path used to open the file (heap-allocated copy) */
    char *buffer;       /**< buffer for a line of text */
    size_t bufsize;     /**< size of the buffer */
    size_t bufused;     /**< number of used bytes in the buffer (string len) */
    long filepos;       /**< position in the file (in "binary" mode) */
    long linenum;       /**< line number */
} txtfile_t;


bool        txtfile_open(txtfile_t *handle, const char *path);
void        txtfile_close(txtfile_t *handle);
const char *txtfile_readline(txtfile_t *handle);

/* Status accessors */
const char *txtfile_get_text(const txtfile_t *handle);
bool        txtfile_get_eof(const txtfile_t *handle);
long        txtfile_get_linenum(const txtfile_t *handle);
size_t      txtfile_get_linelen(const txtfile_t *handle);
const char *txtfile_get_path(const txtfile_t *handle);

#endif
