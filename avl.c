#include "avl.h"

#include <math.h>
#include <stdio.h>

struct _avl {
	double data;
	struct _avl *left;
	struct _avl *right;
};

static void _rebalance(avl **a);
static void _rotate_left(struct _avl **a);
static void _rotate_right(struct _avl **a);
static void recurse_inorder(struct _avl *n, void (*func)(double *));
void tree_inorder(struct _avl *t, void (*func)(double *));

void tree_inorder(struct _avl *t, void (*func)(double *))
{
	recurse_inorder(t, func);
}

static void recurse_inorder(struct _avl *n, void (*func)(double *))
{
	if(!n) {
		return;
	}
	recurse_inorder(n->left, func);
	func(&n->data);
	recurse_inorder(n->right, func);
}


avl *avl_create(void)
{
	return NULL;
}

size_t avl_height(avl *a)
{
	if(!a) {
		return 0;
	}
	size_t left = avl_height(a->left);
	size_t right = avl_height(a->right);

	return 1 + (left > right ? left : right);
}


void avl_insert(avl **a, double value)
{
	if(!a) {
		return;
	}

	if(!*a) {
		*a = calloc(1, sizeof(**a));
		if(!*a) {
			return;
		}
		(*a)->data = value;
		return;
	}

	avl *t = *a;

	if(value <= t->data) {
		avl_insert(&t->left, value);
	} else {
		avl_insert(&t->right, value);
	}

	_rebalance(a);
}

static void _rotate_right(struct _avl **a)
{
	// TODO ABC
	struct _avl* child = (*a)->left;
	(*a)->left = child->right;
	child->right = *a;
	*a = child;
}

static void _rotate_left(struct _avl **a)
{
	// TODO ABC
	struct _avl* child = (*a)->right;
	(*a)->right = child->left;
	child->left = *a;
	*a = child;
}

double get_max(struct _avl *node)
{
	if(node->right) {
		return get_max(node->right);
	} else {
		return node->data;
	}
}

static void _rebalance(avl **a)
{
	struct _avl *t = *a;
	// If tree needs rebalancing, do so
	size_t left_height = avl_height(t->left);
	size_t right_height = avl_height(t->right);

	if(left_height > right_height && left_height - right_height > 1) {
		size_t left_left_height = avl_height(t->left->left);
		size_t left_right_height = avl_height(t->left->right);
		if(left_left_height < left_right_height) {
			// First rotate child to the left
			_rotate_left(&t->left);
		}
		// Rotate right
		_rotate_right(a);
	} else if(left_height < right_height && right_height - left_height > 1) {
		size_t right_right_height = avl_height(t->right->right);
		size_t right_left_height = avl_height(t->right->left);
		if(right_right_height < right_left_height) {
			// First rotate child to the right
			_rotate_right(&t->right);
		}
		// Rotate left
		_rotate_left(a);
	}
}

void avl_remove(avl **a, double value, double delta)
{
	if(!a) {
		return;
	}
	if(!*a) {
		return;
	}

	struct _avl *t = *a;
	if(fabs(t->data - value) < delta) {
		if(!t->left && !t->right) {
			free(t);
			*a = NULL;
			return;
		} else if(!t->left || !t->right) {
			if(t->left) {
				*a = t->left;
			} else {
				*a = t->right;
			}
			free(t);
		} else {
			double new_value = get_max(t->left);
			t->data = new_value;
			// TOTAL CHEATING
			// But Liam said it was OK
			avl_remove(a, new_value, delta);
		}

	} else if(t->data < value) {
		avl_remove(&t->right, value, delta);
	} else {
		avl_remove(&t->left, value, delta);
	}

	_rebalance(a);
}

void avl_print(const avl *a)
{
	if(!a) {
		return;
	}
	avl_print(a->left);
	printf("%lf ", a->data);
	avl_print(a->right);
}


void avl_disassemble(avl *a)
{
	if(!a) {
		return;
	}

	avl_disassemble(a->left);
	avl_disassemble(a->right);
	free(a);
}

