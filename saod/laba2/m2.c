#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include "node.h"
#include "tree.h"
#define DC_SIZE 12000000
#define TEST 200000

void getkeys(char (*dc)[20]);
void set_tree_test(bstree **tree, char (*dc)[20], int size);
void set_hash_test(Node **hash, char (*dictionary)[20], int size);

double wtime()
{
struct timeval t;
gettimeofday(&t, NULL);
return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main() {
    double start_tree, finish_tree;
    double start_hash, finish_hash;
    bstree *tree = NULL;
    bstree *tree_test = NULL;
    Node *hash[HASH_SIZE];
    Node *hash_test = NULL;
    char (*dictionary)[20] = malloc(DC_SIZE * sizeof(char));
    FILE *ex1_tree = fopen("graphics/ex2_tree.txt", "a+");
    FILE *ex1_hash = fopen("graphics/ex2_hash.txt", "a+");

    getkeys(dictionary);

    fprintf(ex1_tree, "%d\t", TEST);
    fprintf(ex1_hash, "%d\t", TEST);
    set_tree_test(&tree, dictionary, TEST);
    set_hash_test(hash, dictionary, TEST);
    start_tree = wtime();
    bstree_add(tree, &dictionary[TEST / 2][0], rand() % 100000);
    finish_tree = wtime();
    fprintf(ex1_tree, "%.6lf\n", finish_tree - start_tree);
    start_hash = wtime();
    hashtab_add(hash, &dictionary[TEST / 2][0], rand() % 100000);
    finish_hash = wtime();
    fprintf(ex1_hash, "%.6lf\n", finish_hash - start_hash);

    // hashtab_free(hash, TEST);
    // bstree_free(tree);
}

void getkeys(char (*dc)[20]) {
    FILE *dictionary = fopen("dictionary.txt", "r");

    for (int i = 0; i < 200000; i++) {
        fscanf(dictionary, "%s", dc[i]);
    }

    fclose(dictionary);
}

void set_tree_test(bstree **tree, char (*dictionary)[20], int size) {
    *tree = bstree_create(&dictionary[0][0], rand()%100000);

    for (int i = 1; i < size; i++) {
        bstree_add(*tree, &dictionary[i][0], rand()%100000);
    }
}

void set_hash_test(Node **hash, char (*dictionary)[20], int size) {
    hashtab_init(hash);

    for (int i = 1; i < size; i++) {
        hashtab_add(hash, &dictionary[i][0], rand()%100000);
    }
}