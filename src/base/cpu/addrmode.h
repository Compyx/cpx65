/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   addrmode.h
 * \brief   Addressing modes - header
 */

#ifndef BASE_CPU_ADDRMODE_H
#define BASE_CPU_ADDRMODE_H


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
    AMD_BLT,        /**< block transfer */
    AMD_IMA,        /**< immediate, absolute */
    AMD_IMZ         /**< immediate, zero page */
} addrmode_id_t;


/** \brief  Maximum valid addressing mode ID
 */
#define AMD_MAX    AMD_IMZ


/** \brief  Addressing mode description object
 */
typedef struct addrmode_desc_s {
    const char *desc_enum;     /**< three letter form as used in #addrmode_id_t */
    const char *desc_example;  /**< longer, example description */
} addrmode_desc_t;


const char *addrmode_get_desc_enum(addrmode_id_t amd);
const char *addrmode_get_desc_example(addrmode_id_t amd);


#endif
