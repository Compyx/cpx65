/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   objpool.h
 * \brief   Module to reuse heap-allocated objects - header
 */

#ifndef BASE_OBJPOOL_H
#define BASE_OBJPOOL_H


#include <stdlib.h>
#include <stdbool.h>


/** \brief  Object pool object
 *
 */
typedef struct objpool_s {
    void **obj_active_list;     /**< list of active objects */
    size_t obj_active_size;     /**< size of active objects list */
    size_t obj_active_used;     /**< number items in active objects list */

    void **obj_inactive_list;   /**< list of active objects */
    size_t obj_inactive_size;   /**< size of active objects list */
    size_t obj_inactive_used;   /**< number items in active objects list */

    /*
     * Callbacks to object handling functions
     */

    /** \brief  Function called to create the requested object
     *
     * Called when the inactive objects list is empty or no suitably sized
     * object could be found.
     */
    void *  (*obj_alloc_cb)(void *param);

    /** \brief  Function called to reuse an object
     *
     * Called when reusing an object from the inactive objects list.
     *
     * The \a param argument is an optional initializer, for example a string
     * when creating a string pool.
     */
    void *  (*obj_reuse_cb)(void *obj, void *param);

    /** \brief  Function called when removing \a obj from the inactive list
     */
    void    (*obj_free_cb)(void *obj);

    /** \brief  Function called to get the size of \a obj
     *
     * This is an optional function, useful when an object has a variable size,
     * such as a string object, or an array of other objects.
     */
    size_t  (*obj_size_cb)(void *obj);

    /*
     * Statistics used for profiling/debugging
     */
    size_t requests_total;      /**< total number of objects requested */
    size_t requests_from_pool;  /**< total number of requests for objects
                                     served from the inactive objects */
    size_t requests_resizes;    /**< number of times the active objects list
                                     got resized */
    size_t requests_frees;      /**< number of times an object actually had to
                                     be freed due to the free list being full */
} objpool_t;



void    objpool_init(objpool_t *pool);
void    objpool_free(objpool_t *pool);

void *  objpool_request(objpool_t *pool, size_t size, void *data);
void    objpool_release(objpool_t *pool, void *obj);
void    objpool_dump_stats(const objpool_t *pool);
#endif
