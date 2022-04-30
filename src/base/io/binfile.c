/** \file   binfile.c
 * \brief   Raw binary file handling
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
#include <stdint.h>
#include <string.h>

#include "../debug.h"
#include "../mem.h"
#include "../error.h"

#include "binfile.h"


/** \brief  Initial size of buffer
 */
#define INITIAL_BUFSIZE     65536


/** \brief  Size of chunks to read
 */
#define CHUNK_SIZE          INITIAL_BUFSIZE


/** \brief  Read binary file while allocating memory
 *
 * \param[in]   path    path to file
 * \param[out]  dest    object to store pointer to data of \a path
 *
 * \return  number of bytes read, or -1 on error
 *
 * \throw   BASE_ERR_IO I/O error
 *
 * \note    Tries to adjust memory used to the number of bytes read with
 *          realloc(3), but this is not guaranteed to work according to the C
 *          standard, so the memory used at \a dest might exceed the number of
 *          bytes returned.
 */
long base_binfile_read(const char *path, uint8_t **dest)
{
    FILE *fp;
    uint8_t *data;
    size_t pos;
    size_t bufsize;
    long size;
    size_t len;

    /* attempt to open file */
    fp = fopen(path, "rb");
    if (fp == NULL) {
        base_errno = BASE_ERR_IO;
        return -1;
    }

    /* allocate initial buffer and set initial state */
    data = base_malloc(INITIAL_BUFSIZE);
    pos = 0;
    bufsize = INITIAL_BUFSIZE;

    /* keep reading data until exhausted */
    do {
#if 0
        printf("pos = %zu.\n", pos);
#endif
        /* do we need to resize the buffer? */
        if (pos == bufsize) {
#if 0
            printf("resizing array to %zu bytes.\n", bufsize *= 2);
#endif
            data = base_realloc(data, bufsize * 2);
            bufsize *= 2;
        }

        /* read chunk */
        len = fread(data + pos, 1U, CHUNK_SIZE, fp);
        if (len != CHUNK_SIZE) {
            /* possible read error */
            if (feof(fp)) {
                /* OK */
                size = (long)(pos + len);
            } else {
                /* I/O error */
                base_errno = BASE_ERR_IO;
                size = -1;
                base_free(data);
                data = NULL;
            }
            break;
        }

        pos += len;

    } while (true);

    /* resize data to its minimum size with realloc(3)
     *
     * We can't use base_realloc() here since it'll barf on a failed attempt at
     * resizing to a smaller size. */
    if (data != NULL) {
        uint8_t *tmp = realloc(data, (size_t)size);
        if (tmp != NULL) {
            data = tmp;
        }
    }
    *dest = data;
    fclose(fp);
    return size;
}


/** \brief  Write binary data to file
 *
 * \param[in]   path    file to write \a data to
 * \param[in]   data    data to write to file
 * \param[in]   size    number of bytes of \a data to write
 *
 * \return  boolean
 *
 * \throw   BASE_ERR_IO I/O error
 */
bool base_binfile_write(const char *path, const uint8_t *data, size_t size)
{
    FILE *fp;
    size_t result;

    /* attempt to open file for writing */
    fp = fopen(path, "wb");
    if (fp == NULL) {
        base_errno = BASE_ERR_IO;
        return false;
    }

    /* try to write to file */
    result = fwrite(data, 1U, size, fp);
    if (result != size) {
        /* oops */
        base_errno = BASE_ERR_IO;
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}
