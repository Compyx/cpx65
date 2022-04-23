/** \file   debug.h
 * \brief   Debug messages
 * \ingroup base
 */

#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H


#ifdef HAVE_DEBUG

# include <stdio.h>
# include <stdlib.h>
# include <inttypes.h>
# include <stdbool.h>

/** \brief  Print debug message stdout when --enable-debug is given
 *
 * Prints '[debug] file:lineno:func(): message' with a newline after message.
 * Works like printf()
 */
# define base_debug(...) \
    fprintf(stderr, \
            "[debug] %s:%d:%s(): ", \
            __FILE__, __LINE__, __func__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n");

#else
  /** \brief    Debug message stub (NOP)
   */
# define base_debug(...)
#endif


/* Enable commandline parsing debugging when requested
 *
 * This requires --enable-debug for now.
 */
#ifdef HAVE_DEBUG_BASE_CMDLINE
  /** \brief    Cmdline: print debugging info
   */
# define base_debug_cmdline base_debug
#else
  /** \brief    Cmdline: print debugging info (NOP)
   */
# define base_debug_cmdline(...)
#endif


/* Enable unittest debugging when requested
 *
 * This requires --enable-debug for now.
 */
#ifdef HAVE_DEBUG_UNITTEST
  /** \brief    Unittest: print debugging info
   */
# define base_debug_unittest base_debug
#else
  /** \brief    Unittest: print debugging info (NOP)
   */
# define base_debug_unittest(...)
#endif

/* Enable debugging of base/cpu (--enable-debug-base-cpu)
 *
 * This requires --enable-debug for now.
 */
#ifdef HAVE_DEBUG_BASE_CPU
  /** \brief    base/cpu: print debugging info
   */
# define base_debug_base_cpu base_debug
#else
  /** \brief    base/cpu: print debugging info (NOP)
   */
# define base_debug_base_cpu(...)
#endif


#endif
