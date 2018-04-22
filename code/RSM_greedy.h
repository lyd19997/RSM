#pragma once
#include"const.h"
#include"requestList.h"
#include"result.h"
#include"graph.h"

class RsmGreedy {
public:
	RsmGreedy(Graph topo, RequestList requests);
	void schedule();
private:
	Graph topo;
	RequestList requests;
	Result res;
	vector<vector<double> > volPerTimeEdge;
	vector<int> peakPerEdge;
	vector<int> passPathIndex;
	bool pushInPath(int indexReq);
	void resOut();
};