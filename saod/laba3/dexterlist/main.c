#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "graph.h"
#include <sys/time.h>
// #include "graph.c"
#include "heap.h"
// #include "heap.c"


#define N 20  // N = [2, max] N - это количество вершин для связного графа
#define Z 100  //граф-решетка Z x Z = M вершин
#define M Z*Z  //кол-во вершин для графа-решетки



void without_loops(graph *g, int num) //убираем все петли (назначаем в (1;1) = -1, (2;2) = -1, (3;3) = -1 и т.д. -1 буду позже заменены на нули)
{
    for(int i = 0; i < ((g->nvert) * (g->nvert)); i++) //бежим по матрице смежности
    {
        if(i % ((g->nvert + 1)) == 0)
            g->m[i] = num;
    }
}

void graph_output(graph *g) //вывод графа на экран
{
    printf("\n");
    for(int i = 0; i < ((g->nvert) * (g->nvert)); i++) //бежим по матрице смежности
    {
        printf("%d  ", g->m[i]);
        if ((i+1) % (g->nvert) == 0)
            printf("\n");
    }
}

//number - номер вершины, для которой заполняется строка в матрице смежности
void filling_in_the_graph(graph *g, int number)
{
        //сюда пришла вершина number после вызова функции
        int random, sum = 0;
        g->visited[number-1] = 1; //бесполезное действие, можно удалить
        for (int i = (number * (g->nvert) - g->nvert); i < (number * (g->nvert)); i++) //бежим по строке вершины number, считаем кол-во ячеек, содержащих 0
        {
            if (g->m[i] == 0)
                sum++; //считаем количество пустых ячеек //для примера строка была для 2 вершины (1 -1 0 0 0), тогда sum = 3 
        }
        int *mass = malloc(sizeof(int) * sum); //выделяем память под вершины, которые в ячейках строки для number вершины содержат 0 //в примере sizeof(int) * 3
        int count = 0;

        for (int i = N * (number-1); i < N * number; i++)  //бежим по строке добавляем все индексы ячеек имеющих 0: (1 -1 0 0 0) индексы (5 6 7 8 9)
        {                                                  //
            if (g->m[i] == 0)
            {
                mass[count] = i; //кладем индекс в массив
                //printf("mass[%d]=%d\n", count, i);
                count++;
            }    
        } 
        //printf("!!--%d\n", mass[0]);

        random = 1 + rand()%(sum - 1 + 1); //генерим число, показывающее сколько ребер будет идти из этой вершины помимо существующих 
                                        //или же random показывает сколько 0 мы изменим на числа (от 1 до n) в строке
        //printf("--%d--\n", random);
        int recurcy = 0, i = 0; //recurcy будет в конце функции номером вершины, через которую идет ветвь рекурсии.

        
        while(i<random) // цикл для заполнения ячеек строки рандомными числами (от 1 до n) 
                        //пока количество заполненных ячеек не станет >= random (число сколько мы планировали заполнить)
        {
            int edge = 0 + rand()%(sum-1 - 0 + 1); //индекс рандомного элемента mass, из которого извлечется элемент, и элемент этот - номер ячейки в матрице для строки number
            //printf("%d\n", edge);
            if( g->m[mass[edge]] == 0)  //если ребро можно построить
            {
                g->m[mass[edge]] =  1 + rand()%(5 - 1 + 1); //строим рандомное ребро
                recurcy = mass[edge];                       //это из (5 6 7 8 9) число, где edge индекс массива mass
                i++;
                
            }
        }

        for (int i = N * (number-1); i < N * number; i++) //кладем в незатронутые ребра -1
        {                                                 // к примеру для вершины 1 мы к ребру 2 не проводили путь, тогда в (1;2) нужно положить -1
            if (g->m[i] == 0)
                g->m[i] = -1;
        }
        
        int cell = 0;
        cell = number - 1;
        for (int i = N * (number-1); i < N * number; i++) // симметрия верхнего треугольника в нижнию, имеем (1;2) = 6, значит заполняем (2;1) = 6, так для каждой пары (a;b)
        {
            g->m[cell] = g->m[i];
            cell = cell + N;
        }

        //обработка рекурсивной вершины к примеру имеет 7 позиция mass (какая-то вершина) будет 
        recurcy = recurcy % N + 1; //номер вершины которая идет на рекурсию
        free(mass);
        //graph_output(g);

        //printf("recurcy = %d\n", recurcy);

         

        int ask = 0;  //Нужно ли идти в вершину? она может иметь заполненную строку, тогда не идем в нее
        for (int i = N * (recurcy-1); i < N * recurcy; i++)  //(-1 -1 0 0) идем - есть нули, (-1 4 5 6) не идем - нет нулей 
        {
            if (g->m[i] == 0)
                ask++;
        }
        //printf("ask = %d\n", ask);
        if (ask != 0)  //g->visited[recurcy] != 1 //если хотя бы одно ребро можно построить (хотя бы один нолик см 105 строку)
        {
            filling_in_the_graph(g, recurcy); 
        }        
}

void restoration_of_the_graph(graph *g) //замена всех -1 на 0
{
    for (int i = 0; i < ((g->nvert) * (g->nvert)); i++)
    {
        if (g->m[i] == -1)
            g->m[i] = 0;      
    }
    for (int i = 0; i < g->nvert; i++) //очищаем visited
    {
        g->visited[i] = 0;
    }
}
  

void grid_graph(graph *h, int z) //заполнение графа-решетки
{
    int t = z;
    int inc = 0; //номер строки матрицы включая 0
    for (int i = t; i < z * z; i++) //бежим по будущим единицам это столбцы // рисуем все вертикальные ребра
    {
        //printf(" %d x %d + %d = %d\n", inc, M, i, inc * M + i);
        h->m[inc * M + i] = 1 + rand()%(1 - 1 + 1); //заполняем верхний треугольник верхняя строчка
        h->m[i * M + inc] = h->m[inc * M + i]; //заполняем нижний треугольник нижняя строчка
        inc++;
    }
    t = 1;
    inc = 0;
    for (int i = t; i < z * z; i++) //бежим по будущим единицам  //рисуем все горизонтальные ребра
    {
        if ((inc * M + i) % Z != 0)
        {
            //printf(" %d x %d + %d = %d\n", inc, M, i, inc * M + i);
            h->m[inc * M + i] = 1 + rand()%(1 - 1 + 1); //заполняем верхний треугольник от центра вверх
            h->m[i * M + inc] = h->m[inc * M + i]; //заполняем нижний треугольник от центра вниз
        }
        inc++;
    }  
}


void my(graph *h, int vertex, int *d, int *prev)
{
    int sum = 0;
    int out = 1;

    heap *lot; //куча
	heapnode node;  //узел кучи key/value
 
    lot = heap_create(h->nvert); //создаем кучу с размером количества вершин

	for (int i = 1; i <= h->nvert; i++) { 
        prev[i] = -1;          //заполняем массив с путем -1 
        if (i != vertex)       //если не 1 вершина, кладем бесконечность
            d[i] = INT_MAX; //d[i] = INT_MAX
        else d[i] = 0;

		heap_insert(lot, d[i], i); //добавляем в кучу вершины с приоритетом
	}

    for (int i = 1; i <= h->nvert; i++) //массив по вершинам
    {
        if(out == 0)
        {
            printf("\n куча до:  \n");
            for(int k = 1; k <= lot->nnodes; k++)
            {
                printf("(%d)%d ", lot->nodes[k].value, lot->nodes[k].key);
            }  
        }

        node = heap_extract_min(lot); //извлекаем вершину с минимальным приоритетом, получаем узел с key и value; в куче заменили извлеченный элемент на последний;
                                      //nnodes-1
        //printf("\nизвлекли вершину %d", node.value);
        if(out == 0)
        {
            printf("\n куча после: \n");
            for(int k = 1; k <= lot->nnodes; k++)
            {
                printf("(%d)%d ", lot->nodes[k].value, lot->nodes[k].key);
            }
            printf("\n");
        }
        h->visited[node.value - 1] = 1; //отмечаем вершину как посещеную
        for (int j = 1; j <= h->nvert; j++) //цикл по смежным
        {
            if ((h->m[(node.value - 1) * (h->nvert) + (j - 1)] != 0) && (h->visited[j - 1] == 0)) //если ребро есть и вершина не посещена
            {
                    if(out == 0) printf("\n!!!---для %d смежная %d ребро %d\n", node.value, j, h->m[(node.value - 1) * (h->nvert) + (j - 1)]);
                    sum = d[node.value] + graph_get_edge(h, node.value, j); //длина пути до вершины + вес ребра между ними меньше длины уже существующей?

                    if(out == 0) printf("(%d + %d = %d) < %d\n", d[node.value], graph_get_edge(h, node.value, j), sum, d[j]);
                    if (sum < d[j] && sum >= 0)  //d[j] посчитанный раннее путь //мы нашли более кратчайший путь?
                    {                                                                  
                        d[j] = sum;              //записываем как минимальный
                        if(out == 0) printf("кратчайший путь в %d записали %d\n", j, d[j]);
                        if(out == 0) printf("\n");
                        
                        for (int x = 1; x <= lot->nnodes; x++) //бежим по длине кучи (массива кучи)
                        {
                            if(lot->nodes[x].value == j)  //ищем вершину со значением (value) j
                            {
                                heap_decrease_key(lot, x, d[j]); //уменьшаем приоритет
                                prev[j] = node.value; //добавляем в прев-массив как часть пути
                            }
                            //printf("то что БЫ было: %d, что теперь: %d\n", j, x);
                        }
                        // heap_decrease_key(lot, j, d[j]);
                        // prev[j] = node.value;
                    }
            }    
        }
    }
}

void the_shortest_path(int *prev, int *d, int ver)
{
    printf("\n");
    for (int i = 2; i <= ver; i++)
    {
        printf("путь из 1 во %d вершину = %d\n", i, d[i]);
        int sum = i;
        int b = 0;
        while(sum != -1)
        {
            if (b == 1)
            {
                printf(" -> ");
            }
            printf("%d", sum);
            sum = prev[sum];
            b = 1;
        }
        printf("\n");
    }
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);

    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main(void) 
{
    // srand(time(NULL));

    double a = 0, b = 0;

    //работа со связным графом
    graph *g = graph_create(N); //создание графа
    
    without_loops(g, -1); //убираем все петли (назначаем в (1;1) = -1, (2;2) = -1, (3;3) = -1 и т.д. -1 буду позже заменены на нули)

    filling_in_the_graph(g, 1); //создание матрицы рандомного связного графа !!!!!ВНИМАНИЕ!!!!! НУЖНО ЗАМЕНИТЬ -1 НА 0 В МАТРИЦЕ СМЕЖНОСТИ

    restoration_of_the_graph(g); //все -1 заменяются на привычные 0

    graph_output(g); //вывод графа



    //работа с графом-решеткой
    graph *h = graph_create(M); //создание графа-решетки

    grid_graph(h, Z); //заполнение графа-решетки
	
    //graph_output(h);




    //алгоритм дейкстры для связного графа
    int vertex = 1;

    int *d = malloc((g->nvert + 1) * sizeof(int));
	int *prev = malloc((g->nvert + 1) * sizeof(int));

    a = wtime();
    my(g, vertex, d, prev);
    b = wtime();
    
    the_shortest_path(prev, d, g->nvert);

    free(d);
    free(prev);

    printf("время работы для свзного графа = %f\n", b - a);
    



    //алгоритм дейкстры для графа-решетки
    vertex = 1;

    d = malloc((h->nvert + 1) * sizeof(int));
	prev = malloc((h->nvert + 1) * sizeof(int));

    a = wtime();
    my(h, vertex, d, prev); 
    b = wtime();
    the_shortest_path(prev, d, h->nvert);
    printf("1 в 10000 = %d\n", d[M]); //формула (vertex - 1) x 2;  vertex - это кол-во вершин, у меня Z.

    free(d);
    free(prev);
    
    printf("время работы для графа-решетки = %f\n", b - a);




}