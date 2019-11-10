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
