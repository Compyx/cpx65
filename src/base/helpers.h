/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   helpers.h
 * \brief   Helper macros
 */

#ifndef BASE_HELPERS_H
#define BASE_HELPERS_H


/** \brief  Determine number of elements is \a ARR
 *
 * \param[in]   ARR array
 */
#define base_arrsize(ARR) (sizeof ARR / sizeof ARR[0])

#endif


