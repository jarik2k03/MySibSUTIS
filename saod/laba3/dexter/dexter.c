#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <sys/time.h>
#define V 100
#define M V * V
    

int main()
{
    struct graph *g;
    
    double beginD, endD;
    int start;

    
    if (V <= 20) {
        g = graph_create(V);
        basic_graph(g, V);
    }
           
    if (V == 100) {
        g = graph_create(M);
        grid_graph(g, V);
    }
        
    
    printf("\nНачальная вершина: "); 
    scanf("%d", &start);
    beginD = wtime();
    Dijkstra(g, start-1, V);
    endD = wtime();

    printf("Время алгоритма Дейкстры: %f \n", endD - beginD);
    graph_free(g);
}