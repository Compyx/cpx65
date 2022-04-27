/** \file   mem.h
 * \brief   Memory handling - header
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

#ifndef BASE_MEM_H
#define BASE_MEM_H

#include <stdlib.h>
#include <stdbool.h>

void *  base_malloc(size_t size);
void *  base_calloc(size_t nelem, size_t elsize);
void *  base_realloc(void *ptr, size_t size);
void    base_free(void *ptr);

char *  base_strdup(const char *s);

bool    base_ispow2(size_t n);
size_t  base_nextpow2(size_t n);

int     base_strcasecmp(const char *s1, const char *s2);

#endif
