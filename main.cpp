#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

using namespace std;

typedef struct {
	int nodes;	// Number of nodes
	float **adjMat;	// Adjacency Matrix
} Graph;

Graph *makeGraph(int nodes) {
	Graph *g1 = (Graph *) malloc(sizeof(Graph));
	g1->nodes = nodes;
	/*	allocate the matrix	*/
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

typedef struct {
	Graph *g;
	int *timeOfAdoption;
	int numberOfActivation;
} ICmodel;

ICmodel *makeICmodel(Graph *g) {
	ICmodel *ic = (ICmodel *) malloc(sizeof(ICmodel));
	ic->g = g;
	/*	allocate the array	*/
	int arr[g->nodes];
	ic->timeOfAdoption = arr;
}

void resetICmodel(ICmodel *ic) {
	int i;
	for (i = 0; i < ic->g->nodes; ++i) {
		ic->timeOfAdoption[i] = -1;
	}
	ic->numberOfActivation = 0;
}

void runICModel(ICmodel *ic, int k, int *a0) {
	resetICmodel(ic);
	int i;
	for (i = 0; i < k; ++i) {
		ic->timeOfAdoption[a0[i]] = 0;
	}
	queue<int> *ran; // ran -> recentlyActivatedNodes
	for (i = 0; i < k; ++i) {
		ran->push(a0[i]);
	}
	srand(time(NULL));
	double r = 0.0;
	int current = 0, j;
	int time = 1;
	while (!(ran->empty())) {
		// One round
		queue<int> *ran2;
		while (!(ran->empty())) {
			current = ran->front();
			for (j = 0; j < ic->g->nodes; j++) {
				if(ic->timeOfAdoption[j] == -1) {
					r = ((double) rand() / (RAND_MAX));
					if (r < ic->g->adjMat[j][current]) {
						ic->timeOfAdoption[j] = time;
						ran2->push(j);
					}
				}
			}
		}
		time++;
		ran = ran2;
	}
	/*	Calculate number of activations	*/
	for (i = 0; i < ic->g->nodes; ++i) {
		if (ic->timeOfAdoption[i] >= 0) {
			ic->numberOfActivation++;
		}
	}
}

int main() {
	printf("Hello World CPP!\n");
	return 0;
}