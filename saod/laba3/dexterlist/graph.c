#include "graph.h"
#include <stdlib.h>

graph *graph_create(int nvert)
{
	graph *g = malloc(sizeof(graph));
	if (g != NULL) {
		g->nvert = nvert;
		g->visited = malloc(sizeof(int) * nvert);
		g->m = malloc(sizeof(int) * nvert * nvert);
		if (g->m) {
			graph_clear(g);
			return g;
		}
	}
	return NULL;
}

void graph_clear(graph *g)
{
	for (int i = 0; i < g->nvert; i++) {
		g->visited[i] = 0;
		for (int j = 0; j < g->nvert; j++) {
			g->m[i * g->nvert + j] = 0;
		}
	}
}

void graph_free(graph *g)
{
	free(g->m);
	free(g);
}

void graph_set_edge(graph *g, int i, int j, int weight)
{
	g->m[(i - 1) * g->nvert + (j - 1)] = weight;
	g->m[(j - 1) * g->nvert + (i - 1)] = weight;
}

int graph_get_edge(graph *g, int i, int j)
{
	return g->m[(i - 1) * g->nvert + (j - 1)];
}

void graph_dfs(graph *g, int v)
{
	g->visited[v - 1] = 1;
	for (int i = 0; i < g->nvert; i++) {
		if ((g->m[(v - 1) * g->nvert + i] > 0) && (g->visited[i] == 0)) {
			graph_dfs(g, i + 1);
		}
	}
}