/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   binfile.c
 * \brief   Raw binary file handling
 * \ingroup base
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
        printf("pos = %zu.\n", pos);
        /* do we need to resize the buffer? */
        if (pos == bufsize) {
            printf("resizing array to %zu bytes.\n", bufsize *= 2);
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

