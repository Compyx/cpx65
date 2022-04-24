/** \file   binfile.h
 * \brief   Raw binary file handling - header
 * \ingroup base
 */

#ifndef BASE_IO_BINFILE_H
#define BASE_IO_BINFILE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

long base_binfile_read(const char *path, uint8_t **dest);
bool base_binfile_write(const char *path, const uint8_t *data, size_t size);

#endif
