
#ifndef AVL_H
 #define AVL_H

#include <stdlib.h>

typedef struct _avl avl;

avl *avl_create(void);

size_t avl_height(avl *a);

void avl_insert(avl **a, double value);

void avl_remove(avl **a, double value, double delta);

void avl_print(const avl *a);

void avl_disassemble(avl *a);

#endif
