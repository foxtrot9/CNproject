#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int nodes;	// Number of nodes
	float **adjMat;	// Adjacency Matrix
} Graph;

Graph *makeGraph(int nodes) {
	Graph *g1 = (Graph *) malloc(sizeof(Graph));
	g1->nodes = nodes;
	/* allocate the matrix */
	g1->adjMat = (float **) malloc(nodes * sizeof(float *));
	int i;
	for (i = 0; i < g1->nodes; ++i)
	{
		g1->adjMat[i] = (float *) malloc(nodes * sizeof(float));
	}
	return g1;
}

void addEdge(Graph *g, int source, int target, float edgeValue) {
	g->adjMat[target][source] = edgeValue;
}

void normalizeGraph(Graph *g) {
	int i, j;
	float sum = 0;
	for (i = 0; i < g->nodes; ++i) {
		sum = 0;
		for (j = 0; j < g->nodes; ++j) {
			sum += g->adjMat[i][j];
		}
		if(sum > 0.001f) {
			for (j = 0; j < g->nodes; ++j) {
				g->adjMat[i][j] = g->adjMat[i][j] / sum;
			}
		}
	}
}

int main() {
	printf("Hello World!\n");
	return 0;
}