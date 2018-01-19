
#ifndef tree_H
 #define tree_H

#include <stdlib.h>

typedef struct _tree tree;

tree *tree_create(void);

size_t tree_height(tree *a);

void tree_insert(tree **a, double value);

//void tree_remove(tree **a, struct company value);

void tree_print(const tree *a);

void tree_disassemble(tree *a);

void tree_inorder(tree *t, void (*func)(char *, char *, size_t *));

#endif
