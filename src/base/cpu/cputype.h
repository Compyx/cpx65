/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   cputypes.h
 * \brief   CPU types - header
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
    CPU_W65C02,     /**< WDC W65C02 */
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

