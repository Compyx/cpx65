/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   strpool.h
 * \brief   Module to reuse heap-allocated strings - header
 */

#ifndef BASE_STRPOOL_H
#define BASE_STRPOOL_H

#include "objpool.h"


/** \brief  String pool object
 *
 * The memory allocated for \c text will be increased to the next power of two
 * when len+1 isn't a power of two.
 */
typedef struct strpool_obj_s {
    char *  text;   /**< text */
    size_t  size;   /**< number of bytes allocated text */
    size_t  len;    /**< actual string length of text */
} strpool_obj_t;

void strpool_init(void);
void strpool_free(void);

void *strpool_add(char *text);


#endif
