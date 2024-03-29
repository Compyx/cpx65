/** \file   mem.c
 * \brief   Memory handling
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
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "mem.h"


/** \brief  Allocate \a size bytes of memory
 *
 * Basic xmalloc implementation -> simply exit on alloc failure.
 *
 * \param[in]   size    number of bytes to allocate
 *
 * \return  pointer to allocated memory
 */
void *base_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "%s: failed to allocate %zu bytes, exiting.\n",
                __func__, size);
        exit(EXIT_FAILURE);
    }
    return ptr;
}


/** \brief  Allocate and initialize \a nelem elements of \a elsize
 *
 * \param[in]   nelem   number of elements
 * \param[in]   elsize  element size
 *
 * \return  pointer to allocated memory
 */
void *base_calloc(size_t nelem, size_t elsize)
{
    void *ptr = calloc(nelem, elsize);
    if (ptr == NULL) {
        fprintf(stderr, "%s: failed to allocate %zu bytes, exiting.\n",
                __func__, nelem * elsize);
        exit(EXIT_FAILURE);
    }
    return ptr;
}


/** \brief  Reallocate memory allocated at \a ptr to \a size bytes
 *
 * \param[in]   ptr     memory to reallocate
 * \param[in]   size    new size in bytes
 *
 * \return  pointer to resized memory, NULL when \a size is 0 and \a ptr != NULL
 *
 * \note    calls lib_malloc() when \a ptr == NULL
 * \note    calls lib_free() when \a ptr != NULL && \a size == 0
 */
void *base_realloc(void *ptr, size_t size)
{
    void *tmp;

    /* ptr == NULL means call malloc(3) */
    if (ptr == NULL) {
        return base_malloc(size);
    }

    /* ptr != NULL && size == 0 call free(3) */
    if (size == 0) {
        base_free(ptr);
        return NULL;
    }

    /* call realloc(3) */
    tmp = realloc(ptr, size);
    if (tmp == NULL) {
        /* since we don't know the previous size, we'll have to barf on
         * attempting to reallocate to a smaller size. May have to fix this.
         */
        fprintf(stderr, "%s: failed to reallocate %zu bytes, exiting.\n",
                __func__, size);
        exit(EXIT_FAILURE);
    }
    return tmp;
}


/** \brief  Deallocate \a ptr
 *
 * \param[in]   ptr pointer to memory to deallocate
 */
void base_free(void *ptr)
{
    free(ptr);
}


/** \brief  Create heap-allocated copy of string \a s
 *
 * \param[in]   s   string to copy
 *
 * \return  heap-allocated copy of \a s
 *
 * \note    returns "'\0'" when \a s is `NULL`
 */
char *base_strdup(const char *s)
{
    char *t;
    size_t len;

    if (s == NULL) {
        len = 0;
    } else {
        len = strlen(s);
    }

    t = base_malloc(len + 1);
    if (len > 0) {
        memcpy(t, s, len + 1);
    } else {
        *t = '\0';
    }
    return t;
}


/** \brief  Determine if \a n is a power of two
 *
 * \param[in]   n   number to check
 *
 * \return  bool
 */
bool base_ispow2(size_t n)
{
    if (n == 0) {
        return false;
    }
    return (n & (n - 1)) == 0;
}


/** \brief  Get the next power of two for \a n
 *
 * \param[in]   n   number to promote to the next power of two
 *
 * \return  next power of two following number \a n
 */
size_t base_nextpow2(size_t n)
{
    size_t b;

    if (n == 0) {
        return 0;
    }

    /* already power of two? */
    if (base_ispow2(n)) {
        /* next power of two */
        return n << 1LU;
    }

    /* iterate bits in n starting at the MSB, and stop when found */
    b = (sizeof(size_t) / sizeof(char)) * CHAR_BIT - 1;
    while ((n & (1LU << b)) == 0) {
        b--;
    }
    /* we have the highest bit set in n, now return the value for 1<<(n+1),
     * which is the next power of two
     */
    return 1LU << (b + 1LU);
}


/** \brief  Compare strings \a s1 and \a s2 in case-insensitive manner
 *
 * Implementation of POSIX.1-2001/4.4BSD strcasecmp()
 *
 * \param[in]   s1  first string
 * \param[in]   s2  second string
 *
 *
 * \return  -1 when s1 < s2, 0 if s1 == s2, 1 if s2 > s1
 */
int base_strcasecmp(const char *s1, const char *s2)
{
    while (*s1 != '\0' && *s2 != '\0') {
        int c1 = tolower(*s1);
        int c2 = tolower(*s2);

        if (c1 < c2) {
            return -1;
        } else if (c1 > c2) {
            return 1;
        }
        s1++;
        s2++;
    }
    if (*s1 == '\0' && *s2 == '\0') {
        return 0;
    } else if (*s1 == '\0') {
        return -1;
    } else {
        return 1;
    }
}
