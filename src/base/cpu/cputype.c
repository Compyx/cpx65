/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   cputypes.c
 * \brief   CPU types
 */

#include <stdlib.h>

#include "../helpers.h"
#include "../mem.h"

#include "cputype.h"


/** \brief  List of CPU types
 *
 * Contains a list of { name, define, description } for each support CPU.
 */
static const cpu_type_info_t cpu_type_list[] = {
    { "6502",       CPU_6502,       "MOS 6502 without undocumented opcodes" },
    { "6510",       CPU_65XX,       "MOS 6502/6510 with undocumented opcodes" },
    { "65xx",       CPU_65XX,       "MOS 6502/6510 with undocumented opcodes" },
    { "65c02",      CPU_65C02,      "WDC 65C02" },
    { "r65c02",     CPU_R65C02,     "Rockwell R65C02" },
    { "w65c02",     CPU_W65C02,     "WDC W65C02" },
    { "65ce02",     CPU_65CE02,     "CSG 65CE02" },
    { "2a03",       CPU_2A03,       "Ricoh 2A03/2A07" },
    { "huc6280",    CPU_HUC6280,    "Hudson Soft 6280" },
    { "65816",      CPU_65816,      "WDC 65816" }
};


/** \brief  Get name for CPU \a cpu_type
 *
 * \param[in]   type    CPU type ID
 *
 * \return  name for CPU or `NULL` when \a cpu_type is out of range
 */
const char *cpu_type_get_name(int type)
{
    if (type >= 0) {
        for (size_t i = 0; i < BASE_ARRAY_SIZE(cpu_type_list); i++) {
            if (cpu_type_list[i].type == type) {
                return cpu_type_list[i].name;
            }
        }
    }
    return NULL;
}


/** \brief  Get description for CPU \a cpu_type
 *
 * \param[in]   type    CPU type ID
 *
 * \return  description for CPU or `NULL` when \a cpu_type is out of range
 */
const char *cpu_type_get_desc(int type)
{
    if (type >= 0) {
        for (size_t i = 0; i < BASE_ARRAY_SIZE(cpu_type_list); i++) {
            if (cpu_type_list[i].type == type) {
                return cpu_type_list[i].desc;
            }
        }
    }
    return NULL;
}


/** \brief  Get CPU type ID by name
 *
 * \param[in]   name    cpu name
 *
 * \return  cpu ID or -1 on error
 */
int cpu_type_get_id_by_name(const char *name)
{
    for (size_t i = 0; i < BASE_ARRAY_SIZE(cpu_type_list); i++) {
        if (base_strcasecmp(cpu_type_list[i].name, name) == 0) {
            return cpu_type_list[i].type;
        }
    }
    return -1;
}

