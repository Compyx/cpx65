/** \file   symtab.c
 * \brief   Symbol table implementation
 *
 * Simple recursive binary tree implementation.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "debug.h"

#include "mem.h"

#include "symtab.h"



/** \brief  Allocate and initialize a symbol table node
 *
 * Allocates a symtab_node_t instance on the heap initializes all fields to
 * `NULL`, copies \a name.
 *
 * \param[in]   name    symbol name
 *
 * \return  symbol table node
 */
static symtab_node_t *symtab_node_alloc(const char *name)
{
    symtab_node_t *node;

    assert(name != NULL && *name != '\0');

    node = base_malloc(sizeof *node);
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->object = NULL;
    node->name = base_strdup(name);
    node->type = SYM_ILL;
    return node;
}


/** \brief  Free node and all of its children, including child symbol tables
 *
 * \param[in]   node    symbol table node
 */
void symtab_node_free(symtab_node_t *node)
{
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        symtab_node_free(node->left);
    }
    if (node->right != NULL) {
        symtab_node_free(node->right);
    }
    if (node->type == SYM_SYMTAB) {
        symtab_node_free(node->object);
    }
    base_free(node->name);
    base_free(node);
}


/** \brief  Add node with \a name to tree \a node
 *
 * \param[in,out]   node    node to add add node with \a name to
 * \param[in]       name    node name
 *
 * \return  pointer to new node or `NULL` on error
 */
symtab_node_t *symtab_node_add(symtab_node_t *node, const char *name)
{
    int d;

    if (node == NULL) {
        node = symtab_node_alloc(name);
    } else {
        d = strcmp(name, node->name);
        if (d == 0) {
            abort();
        }
        if (d < 0) {
            /* base_debug("new('%s') < root('%s')\n", name, node->name); */
            node->left = symtab_node_add(node->left, name);
            node->parent = node->left;
        } else {
            /* base_debug("new('%s') > root('%s')\n", name, node->name); */
            node->right = symtab_node_add(node->right, name);
            node->parent = node->right;
        }
    }
    return node;
}


/** \brief  Dump node names in order
 *
 * \param[in]   node    starting node
 */
void symtab_node_dump(symtab_node_t *node)
{
    if (node != NULL) {
        symtab_node_dump(node->left);
        printf("%s\n", node->name);
        symtab_node_dump(node->right);
    }
}




void symtab_init(symtab_t *table)
{
    table->root = NULL;
}


void symtab_free(symtab_t *table)
{
    if (table->root != NULL) {
        symtab_node_free(table->root);
    }
    table->root = NULL;
}


symtab_node_t *symtab_add(symtab_t *table, const char *name)
{
    symtab_node_t *node;


    if (table->root == NULL) {
        node = symtab_node_alloc(name);
    } else {
        node = symtab_node_add(table->root, name);
    }
    if (node != NULL) {
        table->root = node;
    }
    return node;
}


void symtab_dump(symtab_t *table)
{
    symtab_node_dump(table->root);
}
