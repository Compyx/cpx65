/** \file   debug.h
 * \brief   Debug messages
 * \ingroup base
 */

/*
cpx65 - Assembler for MOS 6502 and variants.
Copyright (C) 2019-2022  Bas Wassink.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
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
