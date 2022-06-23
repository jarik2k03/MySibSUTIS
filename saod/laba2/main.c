#include <stdio.h>
#include "node.h"
#include "tree.h"
#include <sys/time.h>
#include <stdlib.h>

double wtime()
{
        struct timeval t;
        gettimeofday(&t, NULL);
        return (double)t.tv_sec + (double)t.tv_usec * 1E-6;

}

int main()
{
    // struct bstree *tree, *node;
    // tree = bstree_create(12, "Koala");
    // bstree_add(tree, 4, "Flamingo");
    // bstree_add(tree, 14, "Fox");
    // node = bstree_lookup(tree, 4);
    // printf("Found value for key %d: %s\n", node->key, node->value);
    // node = bstree_min(tree);
    // printf("Minimal key: %d, value: %s\n", node->key, node->value);
    // return 0;
    bstree* tree;
    char words[1000];
    double t;
tree = bstree_create(words[0], 0); // Создаём корень дерева
for (i = 2; i < 200000; i++)
{
   tree = bstree_add(words[i - 1], i - 1);
    if (i % 10000 = 0) 
    {
        w = word[getRand(0, i - 1)]; // Выбор случайного слова
        t = wtime();
        node = bstree_lookup(tree, w);
        t = wtime() - t;
        printf("n = %d; time = %.6lf", i - 1, t); 
    }

}



}