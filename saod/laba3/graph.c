#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#define Vmax 100
#define Emax 4950
// #include "stdafx.h"

void GenRandomGraphs(int NOEdge, int NOVertex)
{
    int i, j, edge[NOEdge][2], count;
    i = 0;

    while(i < NOEdge)
    {
        edge[i][0] = rand()%NOVertex+1;
        edge[i][1] = rand()%NOVertex+1;

        if(edge[i][0] == edge[i][1])
        {
            continue;
        }
        else
        {
        for(j = 0; j < i; j++)
        {
            if((edge[i][0] == edge[j][0] && edge[i][1] == edge[j][1]) || (edge[i][0] == edge[j][1] && edge[i][1] == edge[j][0]))  i--;
        }
        }
        i++;
    }
    printf("\nThe generated random graph is: ");
    for(i = 0; i < NOVertex; i++)
    {
        count = 0;
        printf("\n\t %d -> { ", i+1);
        for(j = 0; j < NOEdge; j++)
        {
            if(edge[j][0] == i+1)
            {
                printf("%d ", edge[j][1]);
                count++;
            } 
            else if(edge[j][1] == i+1)
            {
                printf("%d ", edge[j][0]);
                count++;

            } 
            else if(j== NOEdge-1 && count == 0)
            {
                printf("Isolated Vertex!");
            }      
        }

        printf(" }\n");
   }
}



// const int Vmax=100;
int terminal[Emax], weight[Emax], point[Emax];
int head[Vmax], last[Vmax];
int n, m, v, u, w, k=0, i;
char r;

void Add(int v, int u, int w) //добавление ребра
{
    k=k+1;
    terminal[k]=u; weight[k]=w;
    //если вершина v новая, то
    //первая смежная ей вершина имеет номер k
    if (head[v]==0) head[v]=k;
    //если вершина v уже просматривалась, то
    //следующая смежная с ней вершина имеет номер k
    if (last[v]!=0) point[last[v]]=k;
    last[v]=k;
}
//главная функция
int main()
{

    printf("Кол-во вершин \n");
    scanf("%d", &n);
    printf("Кол-во ребер \n");
    scanf("%d", &m);

    printf("Вводите ребра и их вес (v, u, w):\n");
    
    for (i=0; i<m; i++)
    {
        scanf("%d", &v);
        scanf("%d", &u);
        scanf("%d", &w);
        printf("Ребро ориент.? (y/n)\n");
        scanf("%c%*c", &r);
        if (r=='y') 
        {
            Add(v, u, w);
        }
        if (r=='n') 
        {
            Add(v, u, w);
            Add(u, v, w);
        }
        printf("...\n");
    }
    m=m*2;
//вывод списка ребер
    printf("Список ребер графа:\n");
    for (i=0; i<m; i++)
    {
        k=head[i];
        while (k>0)
        {
            printf("( %d -> %d ) $= %d\n", i, terminal[k], weight[k]);
            k=point[k];
        }
    }
}



