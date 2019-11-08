/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   mem.c
 * \brief   Memory handling
 * \ingroup base
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

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
