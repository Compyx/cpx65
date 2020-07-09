/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   addrmode.h
 * \brief   Addressing modes - header
 */

#ifndef BASE_CPU_ADDRMODE_H
#define BASE_CPU_ADDRMODE_H

#include <stdbool.h>


/** \brief  Addressing modes
 */
typedef enum addrmode_id_e {
    AMD_ILL = -1,   /**< illegal */
    AMD_IMP,        /**< implied */
    AMD_ACC,        /**< accumulator (same as implied) */
    AMD_IMM,        /**< immediate */
    AMD_ZP,         /**< zero page */
    AMD_ZPX,        /**< zero page, indexed by X */
    AMD_ZPY,        /**< zero page, indexed by Y */
    AMD_IZX,        /**< indirect zero page, indexed by X */
    AMD_IZY,        /**< indirect zero page, indexed by Y */
    AMD_ABS,        /**< absolute */
    AMD_ABX,        /**< absolute, indexed by X */
    AMD_ABY,        /**< absolute, indexed by Y */
    AMD_IAB,        /**< indirect absolute */
    AMD_REL,        /**< relative */
    /* 65C02 additions */
    AMD_IZP,        /**< indirect zero page */
    AMD_IAX,        /**< indirect absolute, indexed by X */
    AMD_ZPR         /**< zero page + relative (65C02 BBR and BBS opcodes) */,
    /* HuC6280 additions */
    AMD_BLK,        /**< block transfer */
    AMD_IMA,        /**< immediate, absolute */
    AMD_IMAX,       /**< immediate, absolute,X */
    AMD_IMZ,        /**< immediate, zero page */
    AMD_IMZX        /**< immediate, zero page,X */
} addrmode_id_t;


/** \brief  Maximum valid addressing mode ID
 */
#define AMD_MAX    AMD_IMZX


/** \brief  Addressing mode object
 */
typedef struct addrmode_info_s {
    const char *desc;       /**< three/four letter form as used in #addrmode_id_t */
    int oprcount;           /**< operand count */
    const int oprsizes[4];  /**< operand sizes in bytes */
    const char *example;    /**< longer, example description */
} addrmode_info_t;


bool        addrmode_is_valid(addrmode_id_t amd);
const char *addrmode_get_desc(addrmode_id_t amd);
const char *addrmode_get_example(addrmode_id_t amd);
int         addrmode_get_opr_count(addrmode_id_t amd);
const int  *addrmode_get_opr_sizes(addrmode_id_t amd);



#endif
