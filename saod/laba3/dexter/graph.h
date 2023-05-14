
struct heapnode {
    int key; /* Приоритет (ключ) */
    char *value; /* Значение */
};

struct heap {
    int maxsize; /* Максимальный размер кучи */
    int nnodes; /* Число элементов */
    struct heapnode *nodes; /* Массив элементов. Для удобства реализации элементы нумеруются с 1 */
};
struct graph {
    struct heap* dist;
    int nvertices; // Число вершин
    int *m; // Матрица n x n
    int *visited;
};

void heap_swap(struct heapnode *a, struct heapnode *b);
void heap_free(struct heap *h);
int heap_insert(struct heap *h, int key, char *value);
struct heapnode *heap_min(struct heap *h);
struct heap *heap_create(int maxsize);


void Dijkstra(struct graph *g, int start, const int v);
struct graph *graph_create(int nvertices);
void graph_clear(struct graph *g);
void graph_set_edge(struct graph *g, int i, int j, int w);
void graph_free(struct graph *g);
int graph_get_edge(struct graph *g, int i, int j);
void grid_graph(struct graph *g, int z);
void basic_graph(struct graph *g, int z);
double wtime();