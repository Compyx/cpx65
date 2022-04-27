/** \file   binfile.h
 * \brief   Raw binary file handling - header
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

#ifndef BASE_IO_BINFILE_H
#define BASE_IO_BINFILE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

long base_binfile_read(const char *path, uint8_t **dest);
bool base_binfile_write(const char *path, const uint8_t *data, size_t size);

#endif
