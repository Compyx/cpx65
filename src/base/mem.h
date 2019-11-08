/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   mem.h
 * \brief   Memory handling - header
 * \ingroup base
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


#endif
