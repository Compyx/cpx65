/** \file   txtfile.c
 * \brief   Text file handling
 * \ingroup base
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
#include <stdbool.h>
#include <errno.h>

#include "../base.h"

#include "txtfile.h"


/** \brief  Initial size of the buffer to read lines from a file
 */
#define INITIAL_BUFSIZE 1024


/** \brief  Initialize \a handle
 *
 * Set default values and allocate an initial buffer
 *
 * \param[in,out]   handle  text file handle
 */
static void txtfile_handle_init(txtfile_t *handle)
{
    handle->fp = NULL;
    handle->path = NULL;
    handle->buffer = base_calloc(INITIAL_BUFSIZE, 1UL);
    handle->bufsize = INITIAL_BUFSIZE;
    handle->bufused = 0;
    handle->filepos = -1;
    handle->linenum = 0;
}


/** \brief  Free memory used by the member of \a handle
 *
 * \param[in,out]   handle  text file handle
 */
static void txtfile_handle_cleanup(txtfile_t *handle)
{
    fclose(handle->fp);
    base_free(handle->path);
    base_free(handle->buffer);
}


/** \brief  Open text file \a path
 *
 * \param[in,out]   handle  text file handle
 * \param[in]       path    path to file to open
 *
 * \return  boolean
 *
 * \throw   BASE_ERR_IO failed to open file
 */
bool txtfile_open(txtfile_t *handle, const char *path)
{
    FILE *fp;

    base_debug("Attempting to open '%s':", path);
    fp = fopen(path, "rb");
    if (fp == NULL) {
        base_debug(".. Failed!");
        base_errno = BASE_ERR_IO;
        return false;
    }
    base_debug(".. OK.");
    txtfile_handle_init(handle);
    handle->fp = fp;
    handle->path = base_strdup(path);
    return true;
}


/** \brief  Close text file
 *
 * Frees memory used by the member of \a handle and closes the file.
 *
 * \param[in,out]   handle  text file handle
 */
void txtfile_close(txtfile_t *handle)
{
    txtfile_handle_cleanup(handle);
}


/** \brief  Read a line of text from \a handle
 *
 * \return  line of text or `NULL` on error
 *
 * \param[in,out]   handle  text file handle
 *
 * \return  pointer to buffer or `NULL` on failure
 *
 * \throw   BASE_ERR_IO file I/O error
 */
const char *txtfile_readline(txtfile_t *handle)
{
    /* TODO: actually read a line of text from the file */
    size_t index = 0;
    int ch;

    errno = 0;

    while ((ch = fgetc(handle->fp)) != EOF)  {

        /* resize buffer? */
        if (index == handle->bufsize) {
            handle->buffer = base_realloc(handle->buffer, handle->bufsize * 2);
            handle->bufsize *= 2;
        }

        /* store char */

        /* check for Windows CR */
        if (ch == '\r') {
            /* skip */
            ch = fgetc(handle->fp);
            if (ch == EOF) {
                break;
            }
        }

        /* check for end-of-line */
        if (ch == '\n') {
            /* end-of-line */
            handle->buffer[index] = '\0';
            handle->bufused = index + 1;
            handle->linenum++;
            /* store current pos */
            handle->filepos = ftell(handle->fp);
            break;
        }

        handle->buffer[index++] = (char)ch;
    }

    if (feof(handle->fp)) {
        if (errno != 0) {
            base_errno = BASE_ERR_IO;
        }
        return NULL;
    }

    return handle->buffer;
}


/** \brief  Get EOF status
 *
 * Tests \a handle for end-of-file
 *
 * \param[in]   handle  text file handle
 *
 * \return  EOF status
 */
bool txtfile_get_eof(const txtfile_t *handle)
{
    return feof(handle->fp);
}


/** \brief  Get current line number in file
 *
 * \param[in]   handle  text file handle
 */
long txtfile_get_linenum(const txtfile_t *handle)
{
    return handle->linenum;
}


/** \brief  Get length of string in buffer, excluding 0-terminator
 *
 * \param[in]   handle  text file handle
 *
 * \return  length of string in buffer excluding 0-terminator
 */
size_t txtfile_get_linelen(const txtfile_t *handle)
{
    return handle->bufused - 1LU;
}


/** \brief  Get path to file
 *
 * \param[in]   handle  text file handle
 *
 * \return  path to file
 */
const char *txtfile_get_path(const txtfile_t *handle)
{
    return handle->path;
}


/** \brief  Get current text in buffer
 *
 * \param[in]   handle  text file handle
 *
 * \return  current text in buffer
 */
const char *txtfile_get_text(const txtfile_t *handle)
{
    return handle->buffer;
}
