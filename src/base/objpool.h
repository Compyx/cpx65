/** \file   objpool.h
 * \brief   Module to reuse heap-allocated objects - header
 */

#ifndef BASE_OBJPOOL_H
#define BASE_OBJPOOL_H


#include <stdlib.h>
#include <stdbool.h>


/** \brief  Housekeeping data for each objpool object
 *
 * To speed up code and to simplify the API for modules using an obpool, we
 * use some housekeeping data.
 *
 * * pool:  The pool containing the object
 * * index: Index in the pool of the object
 */
#define OBJ_POOL_OBJ_BASE \
    struct objpool_s *pool; \
    size_t index;


/** \brief  Object pool object
 */
typedef struct objpool_obj_s {
    OBJ_POOL_OBJ_BASE
} objpool_obj_t;


/** \brief  Object pool
 *
 */
typedef struct objpool_s {
    void ** active_list;    /**< list of active objects */
    size_t  active_size;    /**< size of active objects list */
    size_t  active_used;    /**< number items in active objects list */

    void ** inactive_list;  /**< list of active objects */
    size_t  inactive_size;  /**< size of active objects list */
    size_t  inactive_used;  /**< number items in active objects list */

    /*
     * Callbacks to object handling functions
     */

    /** \brief  Function called to create the requested object
     *
     * Called when the inactive objects list is empty or no suitably sized
     * object could be found.
     */
    void *  (*alloc_cb)(void *param);

    /** \brief  Function called to reuse an object
     *
     * Called when reusing an object from the inactive objects list.
     *
     * The \a param argument is an optional initializer, for example a string
     * when creating a string pool.
     */
    void *  (*reuse_cb)(void *obj, void *param);

    /** \brief  Function called when removing \a obj from the inactive list
     */
    void    (*free_cb)(void *obj);

    /** \brief  Function called to get the size of \a obj
     *
     * This is an optional function, useful when an object has a variable size,
     * such as a string object, or an array of other objects.
     */
    size_t  (*size_cb)(void *obj);

    /*
     * Statistics used for profiling/debugging
     */
    size_t  requests_total;     /**< total number of objects requested */
    size_t  requests_from_pool; /**< total number of requests for objects
                                     served from the inactive objects */
    size_t  requests_resizes;   /**< number of times the active objects list
                                     got resized */
    size_t  requests_frees;     /**< number of times an object actually had to
                                     be freed due to the free list being full */
} objpool_t;


void    objpool_init(objpool_t *pool);
void    objpool_free(objpool_t *pool);

void *  objpool_request(objpool_t *pool, size_t size, void *data);
void    objpool_release(objpool_t *pool, void *obj);
void    objpool_dump_stats(const objpool_t *pool);

#endif
