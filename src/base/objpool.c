/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   objpool.c
 * \brief   Module to reuse heap-allocated objects
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


/** \brief  Initialize \a pool for use
 *
 * Initializes \a pool by allocating the active and inactive objects list and
 * setting members to their proper values.
 *
 * \param[in,out]   pool    memory bool
 */
void objpool_init(objpool_t *pool)
{
    base_debug("Called.");
    assert(pool != NULL);
    assert(pool->obj_active_size > 0);
    assert(pool->obj_inactive_size > 0);
    assert(pool->obj_alloc_cb != NULL);
    assert(pool->obj_reuse_cb != NULL);
    assert(pool->obj_free_cb != NULL);

    /* initialize fields */
    pool->obj_active_used = 0;
    pool->obj_inactive_used = 0;
    pool->requests_total = 0;
    pool->requests_from_pool = 0;
    pool->requests_resizes = 0;
    pool->requests_frees = 0;

    /* allocate and initialize arrays */
    pool->obj_active_list = base_malloc(pool->obj_active_size *
            sizeof *(pool->obj_active_list));
    for (size_t i = 0; i < pool->obj_active_size; i++) {
        pool->obj_active_list[i] = NULL;
    }
    pool->obj_inactive_list = base_malloc(pool->obj_inactive_size *
            sizeof *(pool->obj_inactive_list));
    for (size_t i = 0; i < pool->obj_inactive_size; i++) {
        pool->obj_inactive_list[i] = NULL;
    }
}


/** \brief  Clean up \a pool
 *
 * Frees memory used by members of \a pool and the objects in both the active
 * in inactive objects list.
 *
 * \param[in,out]   pool    object pool
 */
void objpool_free(objpool_t *pool)
{
    base_debug("Called.");

    /* free active object list and its objects */
    base_debug("Freeing active object list:");
    for (size_t i = 0; i < pool->obj_active_used; i++) {
        base_debug("Calling obj_free(%zu).", i);
        pool->obj_free_cb(pool->obj_active_list[i]);
    }
    base_free(pool->obj_active_list);

    /* free inactive object list and its objects */
    base_debug("Freeing inactive object list:");
    for (size_t i = 0; i < pool->obj_inactive_used; i++) {
        base_debug("Calling obj_free(%zu).", i);
        pool->obj_free_cb(pool->obj_inactive_list[i]);
    }
    base_free(pool->obj_inactive_list);

}


/** \brief  Add \a obj to active objects in \a pool
 *
 * \param[in,out]   pool    object pool
 * \param[in]       obj     object to add to active list
 *
 * \return  \a obj
 */
static void *objpool_add_active(objpool_t *pool, void *obj)
{
    base_debug("Called:");

    /* do we need to resize the array? */
    if (pool->obj_active_used == pool->obj_active_size) {
        /* resize array */
        base_debug("Resizing list to %lu items.",
                pool->obj_active_size * 2);
        pool->obj_active_list = base_realloc(
                pool->obj_active_list,
                pool->obj_active_size * 2 * sizeof *(pool->obj_active_list));
        pool->obj_active_size *= 2;
        pool->requests_resizes++;
    }
    pool->obj_active_list[pool->obj_active_used++] = obj;
    return obj;
}



/** \brief  Request a suitable object from the \a pool
 *
 * Scans the inactive object list for a suitable object.
 *
 * If \a size is 0, it'll return the last object in the inactive objects list,
 * if size > 0 it will use the registered 'obj_size_cb' to find the first object
 * that satisfies the \a size requirement.
 *
 * \param[in,out]   pool    object pool
 * \param[in,out]   size    option size request
 * \param[in]       param   parameter to pass to the \a object constructor
 */
void *objpool_request(objpool_t *pool, size_t size, void *param)
{
    void *obj = NULL;

    (void)(size);

    pool->requests_total++;

    base_debug("New object requested with size %zu:", size);
    if (pool->obj_inactive_used == 0) {
        base_debug("No inactive object, allocate new object:");
        obj = pool->obj_alloc_cb(param);
        objpool_add_active(pool, obj);
    } else {
        base_debug("Checking inactive objects list for suitable object:");
        obj = pool->obj_inactive_list[--(pool->obj_inactive_used)];
        pool->obj_reuse_cb(obj, param);
    }
    return obj;
}


/** \brief  Release \a obj
 *
 * Release \a obj either back into the pool as a reusable object, or frees
 * it entirely when the free list is full.
 *
 * \param[in,out]   pool    object pool
 * \param[in,out]   obj     object
 */
void objpool_release(objpool_t *pool, void *obj)
{
    base_debug("Called.");


    if (pool->obj_inactive_size == pool->obj_inactive_used) {
        base_debug("Free list full\n");
        /* TODO: check size and free the smallest item */
        pool->obj_free_cb(obj);
        pool->requests_frees++;
    } else {
        base_debug("Scanning free list:\n");
        pool->obj_inactive_list[pool->obj_inactive_used++] = obj;
    }
}


/** \brief  Dump statistics on \a pool on stdout
 *
 * \param[in]   pool    object pool
 */
void objpool_dump_stats(const objpool_t *pool)
{
    printf("active objects: %zu/%zu (%.2f)\n",
            pool->obj_active_used,
            pool->obj_active_size,
            (float)(pool->obj_active_used) / (float)(pool->obj_active_size)
            * 100.0);
    printf("active ojects array resize count: %lu\n", pool->requests_resizes);
    printf("inactive objects: %zu/%zu (%.2f)\n",
            pool->obj_inactive_used,
            pool->obj_inactive_size,
            (float)(pool->obj_inactive_used) / (float)(pool->obj_inactive_size)
            * 100.0);

}
