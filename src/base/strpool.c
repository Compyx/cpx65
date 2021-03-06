/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   strpool.c
 * \brief   Module to reuse heap-allocated strings
 */

#include <assert.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "debug.h"
#include "error.h"
#include "mem.h"
#include "objpool.h"

#include "strpool.h"

/* Forward declarations of methods
 */
static void *   strpool_obj_alloc(void *param);
static void *   strpool_obj_reuse(void *obj, void *param);
static void     strpool_obj_free(void *obj);
static size_t   strpool_obj_size(void *obj);


/** \brief  Data of the string pool
 */
static objpool_t strpool_data = {
    NULL, 8, 0,
    NULL, 4, 0,
    strpool_obj_alloc,
    strpool_obj_reuse,
    strpool_obj_free,
    strpool_obj_size,
    0, 0, 0, 0
};


/** \brief  Allocate string object
 *
 * \param[in]   param   initial string (const char *)
 *
 * \return  object
 */
static void *strpool_obj_alloc(void *param)
{
    strpool_obj_t *obj;
    size_t len;
    size_t size;

    len = strlen((const char *)param);
    size = base_nextpow2(len + 1);

    base_debug("strlen = %zu, allocated = %zu\n", len, size);

    obj = base_malloc(sizeof *obj);
    obj->text = base_malloc(size);
    memcpy(obj->text, param, len + 1);
    obj->len = len;
    obj->size = size;
    return obj;
}


/** \brief  Free \a obj and its members
 *
 * \param[in,out]   obj strpool object
 */
static void strpool_obj_free(void *obj)
{
    strpool_obj_t *s= obj;

    base_free(s->text);
    base_free(s);
}


/** \brief  Reuse \a obj
 *
 * Copy string \a param into \a obj, updating len.
 *
 * This only works if \a param fits in the already allocated data in \a obj,
 * which it should as long as \c obj_size() function works properly when
 * requesting a new string.
 *
 * \param[in,out]   obj     strpool object
 * \param[in]       param   new string for the object
 *
 * \return  pointer to object
 */
static void *strpool_obj_reuse(void *obj, void *param)
{
    strpool_obj_t *s = obj;
    size_t len = strlen(param);

    s->len = len;
    memcpy(s->text, param, len + 1);
    return s;
}


static size_t strpool_obj_size(void *obj)
{
    return ((strpool_obj_t *)obj)->size;
}


void strpool_init(void)
{
    objpool_init(&strpool_data);
}



void strpool_free(void)
{
    objpool_free(&strpool_data);
}


void *strpool_add(char *text)
{
    void *obj;
    size_t len = strlen(text);

    obj = objpool_request(&strpool_data, len + 1, text);
    return obj;
}
