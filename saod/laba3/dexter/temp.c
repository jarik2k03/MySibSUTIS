#include <stdio.h>
#include <stdlib.h>
#define V 6

void Dijkstra(int GR[V][V], int start)
{
    int distance[V], count, index, i, u, m=start+1;
    short visited[V];
    for (i=0; i<V; i++)
    {
        distance[i]= __INT_MAX__ ;
        visited[i]=0;
    }
    distance[start]=0;
    for (count=0; count<V-1; count++)
    {
        int min = __INT_MAX__;
        for (i=0; i<V; i++)
            if (!visited[i] && distance[i]<=min)
            {
                min=distance[i]; 
                index=i;
            }
        u=index;
        visited[u]= 1;
        for (i=0; i<V; i++)
            if (!visited[i] && GR[u][i] && distance[u]!=__INT_MAX__ && distance[u]+GR[u][i]<distance[i])
                distance[i]=distance[u]+GR[u][i];
    }
    printf("Стоимость пути из начальной вершины до остальных:\t\n");
    for (i=0; i<V; i++) 
        if (distance[i]!= __INT_MAX__)
            printf(" из %d в %d; dist = %d \n" , m, i+1, distance[i]);
    else printf(" из %d в %d маршрут недоступен\n", m, i+1);
}

int main()
{
    short i, j, w;
    int start;
    int tmp; 
    int GR[V][V] = {
    {0, 6, 9, 3, 1, 15},
    {10, 0, 9, 13, 10, 11},
    {2, 11, 0, 6, 12, 2},
    {4, 8, 11, 0, 7, 13},
    {6, 10, 14, 3, 0, 15},
    {9, 10, 6, 2, 13, 0}  };
    printf("\nНачальная вершина: "); 
    scanf("%d", &start);
    Dijkstra(GR, start-1);    
}    
