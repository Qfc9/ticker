
#ifndef tree_H
#define tree_H

#include <stdlib.h>

typedef struct _tree tree;
tree *createTree (void);
void processCompany(tree **t, char *line);
size_t tree_height (tree * a);
void treeUpdate (tree ** t, char *ticker, char *name, double value);
void tree_insert (tree ** a, char *symbol, char *name, size_t price);
void tree_print (const tree * a);
void tree_disassemble (tree * a);
void tree_inorder (tree * t, void (*func) (char *, char *, size_t *));

#endif
