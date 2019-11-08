/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   cputypes.h
 * \brief   CPU types - header
 */

#ifndef BASE_CPU_CPUTYPES_H
#define BASE_CPU_CPUTYPES_H


/** \brief  CPU type enumerator
 */
typedef enum cpu_type_e {
    CPU_65XX,       /**< standard 6502 with undocumented opcodes (default) */
    CPU_6502,       /**< standard 6502 without undocumented opcodes */
    CPU_65C02,      /**< WDC 65C02 */
    CPU_R65C02,     /**< Rockwell R65C02 */
    CPU_W65C02,     /**< WDC W65C02 */
    CPU_65CE02,     /**< CSG 65CE02 */
    CPU_2A03,       /**< Ricoh 2A03/2A07 (NES)*/
    CPU_65816,      /**< 65816 (SuperCPU) */
} cpu_type_t;



const char *cpu_type_get_desc(cpu_type_t cpu_type);

#endif

