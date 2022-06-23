#include <stdio.h>
#include "bstree.h"

struct bstree *bstree_create(int key, char* value)
{
    struct bstree *node;
    node = malloc(sizeof(*node));
    if (node != NULL) {
        node->key = key;
        node->value = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void bstree_add(struct bstree *tree, int key, char* value)
{
    if (tree == NULL)
    {
        return;
    }
    struct bstree *node;
    struct bstree *parent;
    while (tree != NULL) {
    parent = tree;
    if (key < tree->key)    tree = tree->left;
    else if (key > tree->key)   tree = tree->right;
    else    return;
    }
    node = bstree_create(key, value);
    if (key < parent->key)  parent->left = node;
    else    parent->right = node;
    
}

struct bstree *bstree_lookup(struct bstree *tree, int key)
{
    while (tree != NULL) {
        if (key == tree->key)   return tree;
        else if (key < tree->key)   tree = tree->left;
        else    tree = tree->right;
    }
    return tree;
}

