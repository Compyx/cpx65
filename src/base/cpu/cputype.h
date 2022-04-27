/** \file   cputypes.h
 * \brief   CPU types - header
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

#ifndef BASE_CPU_CPUTYPES_H
#define BASE_CPU_CPUTYPES_H


/** \brief  CPU type enumerator
 */
typedef enum cpu_type_e {
    CPU_UNKNOWN = -1,   /**< illegal/unknown CPU type */
    CPU_65XX,       /**< standard 6502/6510 with undocumented opcodes (default) */
    CPU_6502,       /**< standard 6502 without undocumented opcodes */
    CPU_65C02,      /**< WDC 65C02 */
    CPU_R65C02,     /**< Rockwell R65C02 */
    CPU_W65C02S,    /**< WDC W65C02S */
    CPU_65CE02,     /**< CSG 65CE02 */
    CPU_2A03,       /**< Ricoh 2A03/2A07 (NES)*/
    CPU_HUC6280,    /**< Hudson Soft 6280 (TurboGrafx-16) */
    CPU_65816,      /**< 65816 (SuperCPU) */
} cpu_type_t;


typedef struct cpu_type_info_s {
    const char *name;   /**< string identifier for -m command line option */
    cpu_type_t  type;   /**< type enum */
    const char *desc;   /**< description */
} cpu_type_info_t;



const char *cpu_type_get_name(int type);
const char *cpu_type_get_desc(int type);
int         cpu_type_get_id_by_name(const char *name);

#endif
