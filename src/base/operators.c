/** \file   operators.c
 * \brief   Operator parsing/handling
 * \author  Bas Wassink <b.wassink@ziggo.nl>
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


#include <stddef.h>
#include <stdbool.h>

#include "mem.h"
#include "error.h"

#include "operators.h"


/** \brief  Maximum length of an operator string literal
 */
#define OPERATOR_MAX_LEN    3


/** \brief  Table of operators
 *
 * \note    Some of these share a string literal and thus require context to
 *          properly identify.
 */
static const operator_info_t operator_table[OPERATOR_COUNT] = {
    { "(",      OPR_ID_LPAREN,          OPR_LTR,    OPR_BINARY, 18 },
    { ")",      OPR_ID_RPAREN,          OPR_LTR,    OPR_BINARY, 18 },

    { ".",      OPR_ID_SCOPE,           OPR_LTR,    OPR_BINARY, 17 },

    { "<",      OPR_ID_LSB,             OPR_RTL,    OPR_UNARY,  16 },
    { ">",      OPR_ID_MSB,             OPR_RTL,    OPR_UNARY,  16 },

    { "++",     OPR_ID_INCR_POST,       OPR_LTR,    OPR_UNARY,  15 },
    { "--",     OPR_ID_DECR_POST,       OPR_LTR,    OPR_UNARY,  15 },
    { "[",      OPR_ID_LBRACKET,        OPR_LTR,    OPR_UNARY,  15 },
    { "]",      OPR_ID_RBRACKET,        OPR_LTR,    OPR_UNARY,  15 },
    { ".",      OPR_ID_MEMBER,          OPR_LTR,    OPR_BINARY, 15 },

    { "++",     OPR_ID_INCR_PRE,        OPR_RTL,    OPR_UNARY,  14 },
    { "--",     OPR_ID_DECR_PRE,        OPR_RTL,    OPR_UNARY,  14 },
    { "+",      OPR_ID_PLUS,            OPR_RTL,    OPR_UNARY,  14 },
    { "-",      OPR_ID_MINUS,           OPR_RTL,    OPR_UNARY,  14 },
    { "!",      OPR_ID_NOT_LOG,         OPR_RTL,    OPR_UNARY,  14 },
    { "~",      OPR_ID_NOT_BIT,         OPR_RTL,    OPR_UNARY,  14 },

    { "*",      OPR_ID_MUL,             OPR_LTR,    OPR_BINARY, 13 },
    { "/",      OPR_ID_DIV,             OPR_LTR,    OPR_BINARY, 13 },
    { "%",      OPR_ID_MOD,             OPR_LTR,    OPR_BINARY, 13 },

    { "+",      OPR_ID_ADD,             OPR_LTR,    OPR_BINARY, 12 },
    { "-",      OPR_ID_SUB,             OPR_LTR,    OPR_BINARY, 12 },

    { "<<",     OPR_ID_LSHIFT,          OPR_LTR,    OPR_BINARY, 11 },
    { ">>",     OPR_ID_RSHIFT,          OPR_LTR,    OPR_BINARY, 11 },

    { "<",      OPR_ID_LT,              OPR_LTR,    OPR_BINARY, 10 },
    { "<=",     OPR_ID_LTE,             OPR_LTR,    OPR_BINARY, 10 },
    { ">",      OPR_ID_GT,              OPR_LTR,    OPR_BINARY, 10 },
    { ">=",     OPR_ID_GTE,             OPR_LTR,    OPR_BINARY, 10 },

    { "==",     OPR_ID_EQUAL,           OPR_LTR,    OPR_BINARY,  9 },
    { "!=",     OPR_ID_NOTEQUAL,        OPR_LTR,    OPR_BINARY,  9 },

    { "&",      OPR_ID_AND_BIT,         OPR_LTR,    OPR_BINARY,  8 },

    { "^",      OPR_ID_XOR,             OPR_LTR,    OPR_BINARY,  7 },

    { "|",      OPR_ID_OR_BIT,          OPR_LTR,    OPR_BINARY,  6 },

    { "&&",     OPR_ID_AND_LOG,         OPR_LTR,    OPR_BINARY,  5 },

    { "||",     OPR_ID_OR_LOG,          OPR_LTR,    OPR_BINARY,  4 },
#if 0
    { "?",      OPR_ID_TERNARY,         OPR_RTL,    OPR_TERNARY, 4 },
#endif
    { "=",      OPR_ID_ASSIGN,          OPR_RTL,    OPR_BINARY,  3 },
    { "+=",     OPR_ID_ASSIGN_ADD,      OPR_RTL,    OPR_BINARY,  3 },
    { "-=",     OPR_ID_ASSIGN_SUB,      OPR_RTL,    OPR_BINARY,  3 },
    { "*=",     OPR_ID_ASSIGN_MUL,      OPR_RTL,    OPR_BINARY,  3 },
    { "/=",     OPR_ID_ASSIGN_DIV,      OPR_RTL,    OPR_BINARY,  3 },
    { "%=",     OPR_ID_ASSIGN_MOD,      OPR_RTL,    OPR_BINARY,  3 },
    { "<<=",    OPR_ID_ASSIGN_LSHIFT,   OPR_RTL,    OPR_BINARY,  3 },
    { ">>=",    OPR_ID_ASSIGN_RSHIFT,   OPR_RTL,    OPR_BINARY,  3 },
    { "&=",     OPR_ID_ASSIGN_AND,      OPR_RTL,    OPR_BINARY,  3 },
    { "^=",     OPR_ID_ASSIGN_XOR,      OPR_RTL,    OPR_BINARY,  3 },
    { "|=",     OPR_ID_ASSIGN_OR,       OPR_RTL,    OPR_BINARY,  3 },

    { ",",      OPR_ID_COMMA,           OPR_LTR,    OPR_BINARY,  2 }
};


/** \brief  List of valid characters for operators
 */
static const char valid_operator_chars[] = "!%&()*+,-./<=>?~";


/** \brief  Test if a character is a valid operator character
 *
 * \param[in]   ch  character to test
 *
 * \return  bool
 */
inline static bool valid_char(int ch)
{
    const char *v;

    for (v = valid_operator_chars; *v != '\0' && ch >= *v; v++) {
        if (*v == ch) {
            return true;
        }
    }
    return false;
}


/** \brief  Check if an operator ID is valid
 *
 * Check if \a id is a valid enum value and thus valid index into the
 * #operators_table array.
 * Sets #BASE_ERR_ENUM in case \a id is invalid.
 *
 * \param[in]   id  operator ID
 *
 * \return  bool
 * \throw   BASE_ERR_ENUM   \a id is invalid
 */
static bool valid_id(operator_id_t id)
{
    if (id < 0 || id >= OPERATOR_COUNT) {
        base_errno = BASE_ERR_ENUM;
        return false;
    }
    return true;
}


/** \brief  Get operator as string
 *
 * \param[in]   id  operator ID
 *
 * \return  string or `NULL` on error
 * \throw   BASE_ERR_ENUM   \a id is invalid
 */
const char *operator_text(operator_id_t id)
{
    if (!valid_id(id)) {
        return NULL;
    }
    return operator_table[id].text;
}


/** \brief  Get operator associativity
 *
 * \param[in]   id  operator ID
 *
 * \return  associativity or -1 on error
 * \throw   BASE_ERR_ENUM   \a id is invalid
 */
operator_assoc_t operator_assoc(operator_id_t id)
{
    if (!valid_id(id)) {
        return OPR_ASSOC_ERR;
    }
    return operator_table[id].assoc;
}


/** \brief  Get operator arity
 *
 * \param[in]   id  operator ID
 *
 * \return  arity or -1 on error
 * \throw   BASE_ERR_ENUM   \a id is invalid
 */
operator_arity_t operator_arity(operator_id_t id)
{
    if (!valid_id(id)) {
        return OPR_ARITY_ERR;
    }
    return operator_table[id].arity;
}


/** \brief  Get operator precedence
 *
 * Get operator precedence number, a higher number means higher precedence
 *
 * \param[in]   id  operator ID
 *
 * \return  precedence or -1 on error
 * \throw   BASE_ERR_ENUM   \a id is invalid
 */
int operator_prec(operator_id_t id)
{
    if (!valid_id(id)) {
        return -1;
    }
    return operator_table[id].prec;
}


/** \brief  Get operator info
 *
 * Get operator info as a row from the #operators_table.
 *
 * \param[in]   id  operator ID
 *
 * \return  pointer to table element or `NULL` on error
 * \throw   BASE_ERR_ENUM   \a id is invalid
 */
const operator_info_t *operator_info(operator_id_t id)
{
    if (!valid_id(id)) {
        return NULL;
    }
    return &operator_table[id];
}


/** \brief  Get full operator table
 *
 * Get full operator table for unit tests.
 *
 * \return  operator table
 */
const operator_info_t *operator_get_full_table(void)
{
    return operator_table;
}


/** \brief  Parse string for an operator string literal
 *
 * Parse \a s doing greedy matching of operator strings.
 *
 * \param[in]   s   string to parse
 *
 * \return  operator ID or #OPR_ID_INVALID if not found
 */
operator_id_t operator_parse(const char *s)
{
    const char *p = s;
    size_t len;

    if (s == NULL || *s == '\0') {
        return OPR_ID_INVALID;
    }

    /* greedy matching: find largest string with valid chars */
    while (*p != '\0' && (p - s) < OPERATOR_MAX_LEN && valid_char(*s)) {
        p++;
    }
    len = (size_t)(p - s);
    /* try matching a string of reducing length */
    while (len-- > 0) {
        for (size_t i = 0; i < base_array_len(operator_table); i++) {
            /* skip strncmp() for len 1 */
            if (len == 1 && operator_table[i].text[0] == *s) {
                return (operator_id_t)i;
            } else if (strncmp(operator_table[i].text, s, len) == 0) {
                return (operator_id_t)i;
            }
        }
    }
    return OPR_ID_INVALID;
}
