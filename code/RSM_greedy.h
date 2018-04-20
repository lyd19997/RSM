#pragma once
#include"const.h"
#include"requestList.h"
#include"graph.h"

class RsmGreedy {
public:
	RsmGreedy(Graph topo, RequestList requests);
	vector<int> schedule();
private:
	Graph topo;
	RequestList requests;
	vector<vector<double> > volPerTimeEdge;
	vector<int> peakPerEdge;
	vector<int> passPathIndex;
	bool pushInPath(int indexReq);
};