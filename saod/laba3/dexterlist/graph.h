#pragma once

typedef struct {
	int nvert;
	int *m;
	int *visited;
} graph;

graph *graph_create(int nvert);

void graph_clear(graph *g);

void graph_free(graph *g);

void graph_set_edge(graph *g, int i, int j, int weight);

int graph_get_edge(graph *g, int i, int j);
