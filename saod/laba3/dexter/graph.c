#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <sys/time.h>

double wtime()
{
        struct timeval t;
        gettimeofday(&t, NULL);
        return (double)t.tv_sec + (double)t.tv_usec * 1E-6;

}




struct heap *heap_create(int maxsize)
{
    struct heap *h;
    h = malloc(sizeof(*h));
    if (h != NULL) {
        h->maxsize = maxsize;
        h->nnodes = 0;
        h->nodes = malloc(sizeof(*h->nodes) * (maxsize + 1)); /* Последний индекс - maxsize */
        if (h->nodes == NULL) {
        free(h);
        return NULL;
        }
    }
        return h;
}

void heap_swap(struct heapnode *a, struct heapnode *b)
{
    struct heapnode temp = *a;
    *a = *b;
    *b = temp;
}

void heap_free(struct heap *h)
{
    free(h->nodes);
    free(h);
}

int heap_insert(struct heap *h, int key, char *value)
{
    if (h->nnodes >= h->maxsize) /* Переполнение кучи */
        return -1;

    h->nnodes++;
    h->nodes[h->nnodes].key = key;
    h->nodes[h->nnodes].value = value;
    // printf("Element in heap: %d\n", h->nodes[h->nnodes].key);

    /* HeapifyDown */
    for (int z = h->nnodes; z > 1 && h->nodes[z].key < h->nodes[z / 2].key; z = z / 2){

        // printf("Swap: %d -> %d\t", h->nodes[z].key, h->nodes[z / 2].key);
        heap_swap(&h->nodes[z], &h->nodes[z / 2]);
        // printf("swap: %d -> %d\t", h->nodes[z].key, h->nodes[z / 2].key);
        // printf("Distance: %d\n", key);
    }
        
    return 0;
}

struct heapnode *heap_min(struct heap *h)
{
    if (h->nnodes == 0)
        return NULL;
    return &h->nodes[1];
}



struct graph *graph_create(int nvertices)
{
    struct graph *g;
    g = malloc(sizeof(*g));
    g->nvertices = nvertices;
    g->m = malloc(sizeof(int) * nvertices * nvertices);
    g->visited = malloc(sizeof(int) * nvertices);
    graph_clear(g); // Опционально, O(n^2)
    return g;
}

void graph_clear(struct graph *g)
{
    int i, j;
    for (i = 0; i < g->nvertices; i++) {
    g->visited[i] = 0;
        for (j = 0; j < g->nvertices; j++) {
        g->m[i * g->nvertices + j] = 0;
        }
    }
}

void graph_set_edge(struct graph *g, int i, int j, int w)
{
    g->m[(i - 1) * g->nvertices + (j - 1)] = w;
    g->m[(j - 1) * g->nvertices + (i)] = w;
}

void graph_free(struct graph *g)
{
    free(g->m);
    free(g->visited);
    free(g);
}

int graph_get_edge(struct graph *g, int i, int j)
{
    return g->m[(i - 1) * g->nvertices + j - 1];
}

void basic_graph(struct graph *g, const int z)
{
    short i, j, w;
    for (i = 1; i < z + 1; i++)
    {
        printf("\n");
        for (j = 1; j < z + 1; j++)
        {
            
            w = rand() % 16;
            if (i == j)
            {
                graph_set_edge(g, i, j, 0);
                printf("%d\t  ", graph_get_edge(g, i, j));
                continue; 
            }
                
            else 
            {
                graph_set_edge(g, i, j, w);
                // graph_set_edge(g, j, i, w); 
            }
                
            printf("%d\t  ", graph_get_edge(g, i, j));
        }
    }    
}
void grid_graph(struct graph *g, const int z) //заполнение графа-решетки
{
    int m = z * z;
    int t = z;
    int i, j = 0; //номер строки матрицы включая 0
    for (i = t; i < m; i++) //бежим по будущим единицам это столбцы // рисуем все вертикальные ребра
    {
        // printf(" %d x %d + %d = %d\n", j, m, i, j * m + i);
        g->m[j * m + i] = 1; //заполняем верхний треугольник верхняя строчка
        g->m[i * m + j] = g->m[j * m + i]; //заполняем нижний треугольник нижняя строчка
        j++;
    }
    t = 1;
    j = 0;
    for (i = t; i < m; i++) //бежим по будущим единицам  //рисуем все горизонтальные ребра
    {
        if ((j * m + i) % z != 0)
        {
            // printf(" %d x %d + %d = %d\n", j, m, i, j * m + i);
            g->m[j * m + i] = 1; //заполняем верхний треугольник от центра вверх
            g->m[i * m + j] = g->m[j * m + i]; //заполняем нижний треугольник от центра вниз
        }
        j++;
    } 
     
}

void Dijkstra(struct graph *g, int start, const int v)
{
    
    g->dist = heap_create(v);

    int distance[v], count, index, i, u, n=start+1;
    for (i = 0; i < v; i++)
    {
        distance[i]= __INT_MAX__ ;
        g->visited[i] = 0;
    }
    distance[start] = 0;
    for (count = 0; count < v; count++)
    {
        int min = __INT_MAX__;
        for (i = 0; i < v; i++)
            if (g->visited[i] == 0 && distance[i]<=min)
            {
                min=distance[i]; 
                index=i;
            }
        u=index;
        g->visited[u]= 1;
        for (i = 0; i < v; i++)
        {
            if (v == 100)
            {
                if (g->visited[i] == 0 && g->m[i * g->nvertices + u] && distance[u] != __INT_MAX__ && distance[u] + g->m[i * g->nvertices + u] < distance[i])
                    distance[i] = distance[u] + g->m[i * g->nvertices + u];
            }
            if (v <= 20)
            {
                if (g->visited[i] == 0 && g->m[i * g->nvertices + u+1] && distance[u] != __INT_MAX__ && distance[u] + g->m[i * g->nvertices + u+1] < distance[i])
                    distance[i] = distance[u] + g->m[i * g->nvertices + u+1];
            }

            
        }
         
    }
    for (i = 0; i < v; i++)
        if (distance[i] != 0)
            heap_insert(g->dist, distance[i], "Priority");

    printf("Стоимость пути из начальной вершины до остальных:\t\n");
    for (i=0; i < v; i++) 
        if (distance[i]!= __INT_MAX__)
            printf(" из %d в %d; dist = %d \n" , n, i+1, distance[i]);
        else
            printf(" из %d в %d маршрут недоступен\n", n, i+1);
    
    struct heapnode* a = heap_min(g->dist);
    printf("Минимальный элемент в куче (приоритет): %d\n", a->key);
}