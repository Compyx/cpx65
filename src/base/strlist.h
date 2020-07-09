/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   strlist.h
 * \brief   Heap-allocated list of strings - header
 * \ingroup base
 */

#ifndef BASE_STRLIST_H
#define BASE_STRLIST_H

#include <stdlib.h>


/** \brief  String list handle
 */
typedef struct strlist_s {
    char **items;       /**< list of strings */
    size_t items_max;   /**< size of the list */
    size_t items_used;  /**< number of items used in the list */
} strlist_t;


strlist_t * strlist_init(void);
void        strlist_free(strlist_t *list);
strlist_t * strlist_dup(const strlist_t *slist);
size_t      strlist_num_items(const strlist_t *slist);
void        strlist_add(strlist_t *slist, const char *s);
void        strlist_dump(const strlist_t *slist);
const char *strlist_get_item_at(const strlist_t *slist, size_t index);

#endif


