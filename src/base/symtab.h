/** \file   symtab.h
 * \brief   Symbol table implementation - header
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

#ifndef BASE_SYMTAB_H
#define BASE_SYMTAB_H

enum {
    SYM_ILL = -1,
    SYM_SYMTAB,
    SYM_SYMBOL,
    SYM_MACRO,
    SYM_FUNC
};


typedef struct symtab_node_s {
    struct symtab_node_s *parent;
    struct symtab_node_s *left;
    struct symtab_node_s *right;
    void *object;
    char *name;
    int type;
} symtab_node_t;

typedef struct symtab_s {
    symtab_node_t *root;
} symtab_t;



symtab_node_t *symtab_node_add(symtab_node_t *node, const char *name);
void symtab_node_free(symtab_node_t *node);
void symtab_node_dump(symtab_node_t *node);


void symtab_init(symtab_t *table);
void symtab_free(symtab_t *table);

symtab_node_t *symtab_add(symtab_t *symtab, const char *name);
void symtab_dump(symtab_t *table);

#endif
