/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   convert.h
 * \brief   Type conversion macros
 * \ingroup base
 */

#ifndef BASE_CONVERT_H
#define BASE_CONVERT_H


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/** \brief  Convert pointer \a P to short
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_short(P) ((short)(intptr_t)(P))


/** \brief  Convert pointer \a P to unsigned short
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_ushort(P) ((unsigned short)(uintptr_t)(P))


/** \brief  Convert pointer \a P to int
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_int(P) ((int)(intptr_t)(P))


/** \brief  Convert pointer \a P to unsigned int
 *
 * \param[in]   P   pointer value
 */
#define base_ptr_to_uint(P) ((unsigned int)(uintptr_t)(P))



#endif
