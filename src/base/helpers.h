/** \file   helpers.h
 * \brief   Helper macros
 */

#ifndef BASE_HELPERS_H
#define BASE_HELPERS_H


/** \brief  Determine number of elements in \a ARR
 *
 * Only works on proper arrays, for example:
 * \code
 *  int foo[] = { 0, 1, 2, 3 };
 *  printf("%zu\n", BASE_ARRAY_SIZE(foo));  // prints 4
 * \endcode
 *
 * \param[in]   ARR array
 */
#define BASE_ARRAY_SIZE(ARR) (sizeof ARR / sizeof ARR[0])

#endif
