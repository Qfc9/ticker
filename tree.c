#define _XOPEN_SOURCE 600

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "tree.h"

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


static void _rebalance(struct _tree **a);
static void _rotate_left(struct _tree **a);
static void _rotate_right(struct _tree **a);
static void recurse_inorder(struct _tree *n, void (*func)(char *, char *, size_t *));
static void treeRemove(tree **a, char *ticker, size_t value);
static struct _tree * treeGetMax(struct _tree *t);
static struct _tree *treeSearchForName(struct _tree *t, char *name);
static struct company *treeCreateStock(char *symbol, char *name, size_t price);

tree *createTree(void)
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

void tree_insert(tree **a, char *symbol, char *name, size_t price)
{
	if(!a) {
		return;
	}

	if(!*a) {
		*a = calloc(1, sizeof(**a));
		if(!*a) {
			return;
		}
		struct company *newStock = treeCreateStock(symbol, name, price);
		if(!newStock)
		{
			printf("FREE STOCK\n");
			free(*a);
			return;
		}
		(*a)->data = newStock;
		return;
	}

	tree *t = *a;

	if(price <= t->data->cents) {
		tree_insert(&t->left, symbol, name, price);
	} else {
		tree_insert(&t->right, symbol, name, price);
	}

	_rebalance(a);
}

void tree_print(const tree *a)
{
	if(!a) {
		return;
	}
	tree_print(a->left);
	printf("%-5s", a->data->symbol);
	printf("%- 11.2lf ", centsToDollars(a->data->cents));
	printf("%s\n", a->data->name);
	tree_print(a->right);
}


void tree_disassemble(tree *a)
{
	if(!a) {
		return;
	}

	tree_disassemble(a->left);
	tree_disassemble(a->right);
	free(a->data->name);
	free(a->data);
	free(a);
}

void tree_inorder(tree *t, void (*func)(char *, char *, size_t *))
{
	if(!t) {
		return;
	}

	recurse_inorder(t, func);
}

void treeUpdate(tree **t, char *ticker, char *name, double value)
{
	tree *result = treeSearchForName(*t, ticker);
	
	size_t covertedVal = dollarsToCents(value);
	size_t newValue = 0;

	if(!result)
	{
		tree_insert(t, ticker, name, covertedVal);
		return;
	}

	if(value < 0 && covertedVal <= result->data->cents)
	{
		newValue = result->data->cents - covertedVal;
	}
	else if((covertedVal + result->data->cents) <= 100000000)
	{
		newValue = result->data->cents + covertedVal;
	}
	else
	{
		return;
	}

	char newTick[6];
	char newName[64];
	strncpy(newTick, result->data->symbol, 5);
	strncpy(newName, result->data->name, 63);
	newTick[5] = '\0';
	newName[63] = '\0';

	treeRemove(t, ticker, result->data->cents);
	tree_insert(t, newTick, newName, newValue);
}

// return old cents
static void treeRemove(tree **a, char *ticker, size_t value)
{
	if(!a) {
		return;
	}
	if(!*a) {
		return;
	}

	struct _tree *t = *a;

	if((t->data->cents == value) && (strcmp(t->data->symbol, ticker) == 0)) {
		if(!t->left && !t->right) {
			free(t->data->name);
			free(t->data);
			free(t);

			*a = NULL;
			return;
		} 
		else if(!t->left || !t->right) {
			if(t->left) {
				*a = t->left;
			} 
			else {
				*a = t->right;
			}
			free(t->data->name);
			free(t->data);
			free(t);
		} 
		else {
			tree *newValue = treeGetMax(t->left);
			free(t->data->name);
			free(t->data);

			struct company *newStock = treeCreateStock(newValue->data->symbol, newValue->data->name, newValue->data->cents);
			t->data = newStock;

			treeRemove(&t->left, newValue->data->symbol, newValue->data->cents);
			_rebalance(a);
		}

	} 
	else if(t->data->cents < value) {
		treeRemove(&t->right, ticker, value);
	} 
	else {
		treeRemove(&t->left, ticker, value);
	}
}

static struct _tree * treeGetMax(struct _tree *t)
{
	if(t->right) {
		return treeGetMax(t->right);
	} else {
		return t;
	}
}

static struct _tree *treeSearchForName(struct _tree *t, char *name)
{
	if(!t) {
		return NULL;
	}

	struct _tree *result = NULL;

	if(strcmp(name, t->data->symbol) == 0)
	{
		result = t;
	}
	else
	{
		result = treeSearchForName(t->left, name);
		if(!result)
		{
			result = treeSearchForName(t->right, name);
		}
	}

	return result;
}

// Change Underscore names
static struct company *treeCreateStock(char *symbol, char *name, size_t price)
{
	struct company *new_stock = malloc(sizeof(*new_stock));
	if(!new_stock)
	{
		return NULL;
	}

	new_stock->name = strdup(name);
	if(!new_stock->name)
	{
		free(new_stock);
		return NULL;
	}

	strncpy(new_stock->symbol, symbol, sizeof(new_stock->symbol) - 1);
	new_stock->symbol[sizeof(new_stock->symbol)-1] = '\0';

	new_stock->cents = price;

	return new_stock;
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

static void _rebalance(struct _tree **a)
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