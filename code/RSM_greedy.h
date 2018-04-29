#pragma once
#include"const.h"
#include"requestList.h"
#include"result.h"
#include"graph.h"

class RsmGreedy {
public:
	RsmGreedy(Graph topo, RequestList requests);
	void schedule();
	Result res;
private:
	Graph topo;
	RequestList requests;
	
	vector<vector<double> > volPerTimeEdge;
	vector<int> peakPerEdge;
	vector<int> passPathIndex;
	double pushInPath(int indexReq, bool in);
	void resOut();
};