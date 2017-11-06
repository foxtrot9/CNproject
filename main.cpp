#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

// #define MYDEBUG 1
#undef MYDEBUG
// #define MYTEST 1
#undef MYTEST


using namespace std;

typedef struct {
	int nodes;    // Number of nodes
	float **adjMat;    // Adjacency Matrix
} Graph;

Graph *makeGraph(int nodes) {
	Graph *g1 = (Graph *) calloc(1, sizeof(Graph));
	g1->nodes = nodes;
	/*	allocate the matrix	*/
	g1->adjMat = (float **) calloc(nodes, sizeof(float *));
	int i;
	for (i = 0; i < g1->nodes; ++i) {
		g1->adjMat[i] = (float *) calloc(nodes, sizeof(float));
	}
	return g1;
}

void addEdge(Graph *g, int source, int target, float edgeValue) {
	g->adjMat[target][source] = edgeValue;
	g->adjMat[source][target] = edgeValue;
}

void normalizeGraph(Graph *g) {
	int i, j;
	float sum = 0;
	for (i = 0; i < g->nodes; ++i) {
		sum = 0;
		for (j = 0; j < g->nodes; ++j) {
			sum += g->adjMat[i][j];
		}
		if (sum > 0.001f) {
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
	ICmodel *ic = (ICmodel *) calloc(1, sizeof(ICmodel));
	ic->g = g;
	/*	allocate the array	*/
	ic->timeOfAdoption = (int *) calloc(g->nodes, sizeof(int));
}

void resetICmodel(ICmodel *ic) {
	int i;
	int n = ic->g->nodes;
	int *arr = ic->timeOfAdoption;
	for (i = 0; i < n; ++i) {
		arr[i] = -1;
	}
	ic->numberOfActivation = 0;
}

void runICModel(ICmodel *ic, int k, int *a0) {
	resetICmodel(ic);
	int i;
	for (i = 0; i < k; ++i) {
		ic->timeOfAdoption[a0[i]] = 0;
	}
	queue<int> *ran = new queue<int>; // ran -> recentlyActivatedNodes
	for (i = 0; i < k; ++i) {
		ran->push(a0[i]);
	}
	srand(time(NULL));
	double r = 0.0;
	int current = 0, j;
	int time = 1;
	while (!(ran->empty())) {
		// One round
		queue<int> *ran2 = new queue<int>;
		while (!(ran->empty())) {
			current = ran->front();
			ran->pop();
			for (j = 0; j < ic->g->nodes; j++) {
				if (ic->timeOfAdoption[j] == -1) {
					r = ((float) rand() / (RAND_MAX));
					if (r < ic->g->adjMat[j][current]) {
						#ifdef MYDEBUG
						printf("S:%d T:%d P:%f Th:%f Time:%d\n",current, j, r, ic->g->adjMat[j][current], time);
						#endif
						ic->timeOfAdoption[j] = time;
						ran2->push(j);
					}
				}
			}
		}
		time++;
		delete ran;
		ran = ran2;
	}
	/*	Calculate number of activations	*/
	for (i = 0; i < ic->g->nodes; ++i) {
		if (ic->timeOfAdoption[i] >= 0) {
			ic->numberOfActivation++;
		}
	}
}

void runHillClimb(ICmodel *tic) {
	int runs = 10;
	int cap = 20;
	int k = 0;
	int currentScore = 0;
	int currentScoreNext = 0;
	int maxMarginalScore = 0;
	int maxMarginalCurrent = 0;
	int node;
	int setArray[cap];
	for (k = 1; k <= cap; ++k) {
		for (node = 0; node < tic->g->nodes; ++node) {
			if (k != 1) {
				for (int temp = 0; temp < k; ++temp) {
					if (setArray[temp] == node)
						continue;
				}
			}
			setArray[k-1] = node;
			for (int x = 0; x < runs; ++x) {
				runICModel(tic, k, setArray);
				currentScoreNext += tic->numberOfActivation;
			}
			currentScoreNext = currentScoreNext/runs;
			// printf("c:%d cn:%d m:%d mn:%d\n", currentScore, currentScoreNext, maxMarginalScore, maxMarginalCurrent);
			if ((currentScoreNext - currentScore) > maxMarginalScore) {
				maxMarginalScore = currentScoreNext - currentScore;
				maxMarginalCurrent = node;
				printf("DEBUG: c:%d cn:%d m:%d mn:%d\n", currentScore, currentScoreNext, maxMarginalScore, maxMarginalCurrent);
			}
		}
		//#ifdef MYDEBUG
		//printf("Confirmed: c:%d cn:%d m:%d mn:%d\n", currentScore, currentScoreNext, maxMarginalScore,
		//       maxMarginalCurrent);
		printf("Add: %d to optimal set k = %d\n", maxMarginalCurrent, k);
		//#endif
		setArray[k - 1] = maxMarginalCurrent;
		currentScore = currentScore + maxMarginalScore;
		maxMarginalScore = -1;
		maxMarginalCurrent = -1;
		//printf("Confirmed: c:%d cn:%d m:%d mn:%d\n\n", currentScore, currentScoreNext, maxMarginalScore,
		//      maxMarginalCurrent);
	}
}

void printGraph(Graph *tg) {
	int i, j;
	for (i = 0; i < tg->nodes; ++i) {
		for (j = 0; j < tg->nodes; ++j) {
			printf("%f ", tg->adjMat[i][j]);
		}
		printf("\n");
	}
}


void printArray(int *arr, int sz) {
	int i;
	for (i = 0; i < sz; ++i) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void printArray(float *arr, int sz) {
	int i;
	for (i = 0; i < sz; ++i) {
		printf("%f ", arr[i]);
	}
	printf("\n");
}

#ifdef MYTEST
void testICmodelHelper(Graph *tg) {
	addEdge(tg, 8, 9, 2.0f);
	//printGraph(tg);
	//printf("--\n");
	addEdge(tg, 9, 8, 2.0f);
	//printGraph(tg);
	addEdge(tg, 6, 7, 3.0f);
	addEdge(tg, 0, 1, 6.25f);
	addEdge(tg, 1, 2, 0.25f);
	addEdge(tg, 2, 1, 0.25f);
	addEdge(tg, 3, 2, 1.0f);
	addEdge(tg, 5, 4, 1.5f);
	addEdge(tg, 4, 5, 1.5f);
	addEdge(tg, 2, 4, 0.5f);
	addEdge(tg, 4, 2, 0.5f);
	addEdge(tg, 2, 3, 1.0f);
	addEdge(tg, 7, 6, 3.0f);
	addEdge(tg, 1, 0, 6.25f);
	//printGraph(tg);
	//printf("--\n");
	normalizeGraph(tg);
	//printGraph(tg);
}
#endif

#ifdef MYTEST
void testICmodel() {
	Graph *tg = makeGraph(10);
	testICmodelHelper(tg);
	ICmodel *tic = makeICmodel(tg);
	int k = 1;
	int arr[k] = {2};
	runICModel(tic, k, arr);
	printf("%d\n", tic->numberOfActivation);
	printArray(tic->timeOfAdoption, 10);
}
#endif

Graph *takeInput() {
	FILE *fin = fopen("/home/mit/SEM7/CN/CNproject/data/netscience_parsed.txt", "r");
	int tn = 0;
	fscanf(fin, "%d", &tn);
	Graph *tg = makeGraph(tn);
	fscanf(fin, "%d", &tn);
	int i = 0, s, t;
	float v;
	for (i = 0; i < tn; ++i) {
		fscanf(fin, "%d %d %f", &s, &t, &v);
		addEdge(tg, s, t, v);
	}
	normalizeGraph(tg);
	return tg;
}

int main() {
	#ifdef MYTEST
	testICmodel();
	#endif
	Graph *tg = takeInput();
	ICmodel *tic = makeICmodel(tg);
	/*
	#ifdef MYDEBUG
	int k = 20;
	int arr[k] = {2, 5, 1048, 12, 201, 516, 517, 690, 626, 1444, 774, 1130, 1557, 1454, 1356, 1255, 828, 834, 880, 888};
	runICModel(tic, k, arr);
	#endif
	 */
	runHillClimb(tic);
	#ifdef MYDEBUG
	printf("%d\n", tic->numberOfActivation);
	printArray(tic->timeOfAdoption, tg->nodes);
	#endif
	return 0;
}