/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   symtab.h
 * \brief   Symbol table implementation - header
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


symtab_node_t *symtab_node_add(symtab_node_t *node, const char *name);
void symtab_node_free(symtab_node_t *node);
void symtab_node_dump(symtab_node_t *node);

#endif
