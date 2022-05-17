/** \file   operators.h
 * \brief   Operator parsing/handling - header
 * \author  Bas Wassink <b.wassink@ziggo.nl>
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

#ifndef BASE_OPERATORS_H
#define BASE_OPERATORS_H

/** \brief  Operator ID enum
 */
typedef enum operator_id_e {
    OPR_ID_INVALID = -1,    /**< invalid/error */

    OPR_ID_LPAREN,          /**< left parenthesis '(' */
    OPR_ID_RPAREN,          /**< right parenthesis ')' */

    OPR_ID_SCOPE,           /**< scope resolution '.' */

    OPR_ID_LSB,             /**< least significant byte '<' */
    OPR_ID_MSB,             /**< most significant byte '>' */

    OPR_ID_INCR_POST,       /**< postfix increment '++' */
    OPR_ID_DECR_POST,       /**< postfix decrement '--' */
    OPR_ID_LBRACKET,        /**< left bracket '[' */
    OPR_ID_RBRACKET,        /**< right bracket ']' */
    OPR_ID_MEMBER,          /**< member selection '.' */

    OPR_ID_INCR_PRE,        /**< prefix increment '++' */
    OPR_ID_DECR_PRE,        /**< prefix decrement '--' */
    OPR_ID_PLUS,            /**< unary plus '+' */
    OPR_ID_MINUS,           /**< unary minus '-' */
    OPR_ID_NOT_LOG,         /**< logical not '!' */
    OPR_ID_NOT_BIT,         /**< bitwise not /one's complement '~' */

    OPR_ID_MUL,             /**< multiplication '*' */
    OPR_ID_DIV,             /**< division '/' */
    OPR_ID_MOD,             /**< modulo '%' (might cause issues with binary prefix) */

    OPR_ID_ADD,             /**< addition '+' */
    OPR_ID_SUB,             /**< subtraction '-' */

    OPR_ID_LSHIFT,          /**< left shift '<<' */
    OPR_ID_RSHIFT,          /**< right shift '>>' */

    OPR_ID_LT,              /**< less than '<' */
    OPR_ID_LTE,             /**< less than or equal '<=' */
    OPR_ID_GT,              /**< greater than '>' */
    OPR_ID_GTE,             /**< greater than or equal '>=' */

    OPR_ID_EQUAL,           /**< equal '==' */
    OPR_ID_NOTEQUAL,        /**< not equal '!=' */

    OPR_ID_AND_BIT,         /**< bitwise AND '&' */

    OPR_ID_XOR,             /**< bitwise XOR '^' */

    OPR_ID_OR_BIT,          /**< bitwise OR '|' */

    OPR_ID_AND_LOG,         /**< logical AND '&&' */

    OPR_ID_OR_LOG,          /**< logical OR '|| */
#if 0
    OPR_ID_TERNARY,         /**< ternary operator '?:' */
#endif
    OPR_ID_ASSIGN,          /**< assignment '=' */
    OPR_ID_ASSIGN_ADD,      /**< assignment by addition '+=' */
    OPR_ID_ASSIGN_SUB,      /**< assignment by subtraction '-=' */
    OPR_ID_ASSIGN_MUL,      /**< assignment by multiplicatin '*=' */
    OPR_ID_ASSIGN_DIV,      /**< assignment by division '/=' */
    OPR_ID_ASSIGN_MOD,      /**< assignment by modulo '%=' */
    OPR_ID_ASSIGN_LSHIFT,   /**< assignment by left shift '<<=' */
    OPR_ID_ASSIGN_RSHIFT,   /**< assignment by right shift '>>=' */
    OPR_ID_ASSIGN_AND,      /**< assignment by bitwise AND '&=' */
    OPR_ID_ASSIGN_XOR,      /**< assignment by bitwise XOR '^=' */
    OPR_ID_ASSIGN_OR,       /**< assignment by bitwise OR '+=' */

    OPR_ID_COMMA,           /**< comma ',' */

    OPERATOR_COUNT          /**< total number of operators */
} operator_id_t;

/** \brief  Operator associativity enum
 */
typedef enum operator_assoc_e {
    OPR_ASSOC_ERR = -1, /**< error */
    OPR_LTR,            /**< left-to-right associative */
    OPR_RTL,            /**< right-to-left associative */
} operator_assoc_t;

/** \brief  Operator 'arity' enum
 */
typedef enum operator_arity_e {
    OPR_ARITY_ERR = -1, /**< error */
    OPR_UNARY = 1,      /**< unary operator (one operand) */
    OPR_BINARY,         /**< binary operator (two operands) */
    OPR_TERNARY         /**< ternary operator (three operands: only '?:') */
} operator_arity_t;

/** \brief  Operator table element
 */
typedef struct operator_info_s {
    const char *        text;   /**< string literal */
    operator_id_t       id;     /**< ID */
    operator_assoc_t    assoc;  /**< associativity */
    operator_arity_t    arity;  /**< 'arity' */
    int                 prec;   /**< precedence */
} operator_info_t;


const char *            operator_text(operator_id_t id);
operator_assoc_t        operator_assoc(operator_id_t id);
operator_arity_t        operator_arity(operator_id_t id);
int                     operator_prec(operator_id_t id);
const operator_info_t * operator_info(operator_id_t id);
const operator_info_t * operator_get_full_table(void);

operator_id_t operator_parse(const char *s);
#endif
