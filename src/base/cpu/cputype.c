/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   cputypes.c
 * \brief   CPU types
 */

#include <stdlib.h>

#include "cputype.h"


/** \brief  List of CPU type descriptions
 */
static const char *cpu_type_desc[] = {
    "65XX: 6502/6510 with undocumented opcodes",
    "6502: 6502 without undocumented opcodes",
    "65C02: WDC 65C02", /* not W6C02 */
    "R65C02: Rockwell R65C02",
    "W65C02, WDC W65C02",
    "65CE02: CSG 65CE02",
    "2A03: Ricoh 2A03/2A07 (NES)",
    "65816: 65816 (SuperCPU)"
};


/** \brief  Get description for CPU \a cpu_type
 *
 * \param[in]   cpu_type    CPU type ID
 *
 * \return  description for CPU or "unknown CPU" when \a cpu_type is out of range
 */
const char *cpu_type_get_desc(cpu_type_t cpu_type)
{
    if (cpu_type < 0 ||
            cpu_type >= (cpu_type_t)(sizeof cpu_type_desc / sizeof cpu_type_desc[0])) {
        return "unknown CPU";
    }
    return cpu_type_desc[cpu_type];
}


