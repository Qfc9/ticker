#include "tree.h"

#include <math.h>
#include <stdio.h>

struct company 
{
	char symbol[6];
	size_t cents;
	char *name;
};

struct _tree 
{
	struct company *data;
	struct _tree *left;
	struct _tree *right;
};

/*
static void _rebalance(tree **a);
static void _rotate_left(struct _tree **a);
static void _rotate_right(struct _tree **a);
*/
static void recurse_inorder(struct _tree *n, void (*func)(char *, char *, size_t *));

void tree_inorder(tree *t, void (*func)(char *, char *, size_t *))
{
	recurse_inorder(t, func);
}

static void recurse_inorder(struct _tree *n, void (*func)(char *, char *, size_t *))
{
	if(!n) {
		return;
	}
	recurse_inorder(n->left, func);
	func(n->data->symbol, n->data->name, &n->data->cents);
	recurse_inorder(n->right, func);
}


tree *tree_create(void)
{
	return NULL;
}

size_t tree_height(tree *a)
{
	if(!a) {
		return 0;
	}
	size_t left = tree_height(a->left);
	size_t right = tree_height(a->right);

	return 1 + (left > right ? left : right);
}

// REDO
// void tree_insert(tree **a, double value)
// {
// 	if(!a) {
// 		return;
// 	}

// 	if(!*a) {
// 		*a = calloc(1, sizeof(**a));
// 		if(!*a) {
// 			return;
// 		}
// 		(*a)->data = value;
// 		return;
// 	}

// 	tree *t = *a;

// 	if(value <= t->data) {
// 		tree_insert(&t->left, value);
// 	} else {
// 		tree_insert(&t->right, value);
// 	}

// 	_rebalance(a);
// }

/*
static void _rotate_right(struct _tree **a)
{
	// TODO ABC
	struct _tree* child = (*a)->left;
	(*a)->left = child->right;
	child->right = *a;
	*a = child;
}

static void _rotate_left(struct _tree **a)
{
	// TODO ABC
	struct _tree* child = (*a)->right;
	(*a)->right = child->left;
	child->left = *a;
	*a = child;
}

// REDO
struct company *get_max(struct _tree *node)
{
	if(node->right) {
		return get_max(node->right);
	} else {
		return node->data;
	}
}

static void _rebalance(tree **a)
{
	struct _tree *t = *a;
	// If tree needs rebalancing, do so
	size_t left_height = tree_height(t->left);
	size_t right_height = tree_height(t->right);

	if(left_height > right_height && left_height - right_height > 1) {
		size_t left_left_height = tree_height(t->left->left);
		size_t left_right_height = tree_height(t->left->right);
		if(left_left_height < left_right_height) {
			// First rotate child to the left
			_rotate_left(&t->left);
		}
		// Rotate right
		_rotate_right(a);
	} else if(left_height < right_height && right_height - left_height > 1) {
		size_t right_right_height = tree_height(t->right->right);
		size_t right_left_height = tree_height(t->right->left);
		if(right_right_height < right_left_height) {
			// First rotate child to the right
			_rotate_right(&t->right);
		}
		// Rotate left
		_rotate_left(a);
	}
}
*/

// REDO
// void tree_remove(tree **a, struct company value)
// {
// 	if(!a) {
// 		return;
// 	}
// 	if(!*a) {
// 		return;
// 	}

// 	struct _tree *t = *a;
// 	if(fabs(t->data->cents - value->cents)) {
// 		if(!t->left && !t->right) {
// 			free(t);
// 			*a = NULL;
// 			return;
// 		} else if(!t->left || !t->right) {
// 			if(t->left) {
// 				*a = t->left;
// 			} else {
// 				*a = t->right;
// 			}
// 			free(t);
// 		} else {
// 			struct company new_value = get_max(t->left);
// 			t->data = new_value;
// 			// TOTAL CHEATING
// 			// But Liam said it was OK
// 			tree_remove(a, new_value);
// 		}

// 	} else if(t->data < value) {
// 		tree_remove(&t->right, value);
// 	} else {
// 		tree_remove(&t->left, value);
// 	}

// 	_rebalance(a);
// }

// REDO
void tree_print(const tree *a)
{
	if(!a) {
		return;
	}
	tree_print(a->left);
	printf("%lu ", a->data->cents);
	tree_print(a->right);
}


void tree_disassemble(tree *a)
{
	if(!a) {
		return;
	}

	tree_disassemble(a->left);
	tree_disassemble(a->right);
	free(a);
}

