struct LTHillClimbing{
	Graph *g;
	LTModel *ltModel;

	int *selected;
	int *initNodes;

	/**
		Model should be set up with graph
		and threshold values.
	*/
	void setModel(LTModel *model){
		ltModel = model;
		g = ltModel->g;

		selected = (int *)malloc(g->nodes*sizeof(int));
		int i;
		for(i = 0;i < g->nodes ; i++){
			selected[i] = 0;
		}

		initNodes = (int *)malloc(g->nodes*sizeof(int));
	}

	void selectBestInitialNodes(int k){

		int currProfit = 0;

		int i;
		for(i = 0;i < k;i++){

			int maxGain = -1;
			int maxGainNode = -1;

			int j;
			for(j=0;j<g->nodes;j++){
				if(selected[j]==0){
					initNodes[i] = j;

					int profit = ltModel->runModel(initNodes,i+1);
					int gain = profit-currProfit;
					if(gain>maxGain){
						maxGain = gain;
						maxGainNode = j;
					}
				}
			}

			currProfit+=maxGain;
			selected[maxGainNode] = 1;
			initNodes[i] = maxGainNode;
		}
	}
};