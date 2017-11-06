#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int nodes;	// Number of nodes
	int rows;	// Number of rows
	int columns;	// Number of columns
	float **adjMat;	// Adjancy Matrix
} Graph;

Graph makeGraph(int rows, int columns) {
	Graph g1;
	g1.nodes = 100;
	g1.rows = rows;
	g1.columns = columns;
	g1.adjMat = (float **) malloc(rows*columns);
	return g1;
}

int main() {
	printf("Hello World!\n");
	return 0;
}