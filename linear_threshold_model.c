struct LTModel{
	Graph *g;

	/**
		Represents threshold value for each 
		node above which it will accept 
		the idea(innovation).
	*/
	float *thresholdValue;

	/*
		The time step at which a node
		accepts the idea(innovation).
	*/
	int *timeOfAdoption;

	int numberOfActivatedNodes;

	void setGraph(Graph *g1,float *threshold){
		g = g1;
		thresholdValue = threshold;
		timeOfAdoption = (int *)malloc(g->nodes);
		numberOfActivatedNodes = 0;
	}

	/**
		This function runs the linear threshold
		model given the initial set of activated 
		nodes. k is the number of initial activated nodes.
	*/
	int runModel(int *initNodes,int k){
		
		int i;
		for(i = 0;i < g->nodes ; i++){
			timeOfAdoption[i] = -1;
		}

		for(i = 0;i < k ; i++){
			timeOfAdoption[initNodes[i]] = 0;
		}

		int currActivations = k;
		int currTime = 1;
		numberOfActivatedNodes = currActivations;

		while(currActivations!=0){
			currActivations = 0;

			for(i = 0;i < g->nodes;i++){
				if(timeOfAdoption[i]==-1){
					float weight = 0.0;
					for(int j = 0;j < g->nodes ; j++){
						if(timeOfAdoption[j]!=-1)
							weight+=g->adjMat[i][j];
					}
					if(weight>=threshold[i]){
						timeOfAdoption[i] = currTime;
						currActivations++;
					}
				}
			}

			numberOfActivatedNodes+=currActivations;
			currTime++;
		}

		return numberOfActivatedNodes;
	}
};