#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int nodes;	// Number of nodes
	float **adjMat;	// Adjacency Matrix
} Graph;

Graph makeGraph(int nodes) {
	Graph g1;
	g1.nodes = nodes;
	g1.adjMat = (float **) malloc(nodes*nodes);
	return g1;
}

void addEdge(Graph g, int source, int target, float edgeValue) {
	g.adjMat[target][source] = edgeValue;
}

void normalizeGraph(Graph g) {
	int i, j;
	float sum = 0;
	for (i = 0; i < g.nodes; ++i) {
		sum = 0;
		for (j = 0; j < g.nodes; ++j) {
			sum += g.adjMat[i][j];
		}
		if(sum > 0.001f) {
			for (j = 0; j < g.nodes; ++j) {
				g.adjMat[i][j] = g.adjMat[i][j] / sum;
			}
		}
	}
}

int main() {
	printf("Hello World!\n");
	return 0;
}