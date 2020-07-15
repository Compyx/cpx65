/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   symtab.c
 * \brief   Symbol table implementation
 *
 * Simple recursive binary tree implementation.
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
        /* new node */
        node = symtab_node_alloc(name);
    } else if ((d = strcmp(name, node->name)) == 0) {
        base_debug("Name conflict!\n");
        abort();
    } else if (d < 0) {
        base_debug("new('%s') < root('%s')\n", name, node->name);
        node->left = symtab_node_add(node->left, name);
        node->parent = node->left;
    } else {
        base_debug("new('%s') > root('%s')\n", name, node->name);
        node->right = symtab_node_add(node->right, name);
        node->parent = node->right;
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



