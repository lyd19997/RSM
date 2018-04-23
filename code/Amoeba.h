#pragma once
#include"const.h"
#include"requestList.h"
#include"graph.h"
#include"result.h"

class Amoeba {
public:
	Amoeba(Graph topo, RequestList requests);
	vector<int> schedule();
	Result res;
private:
	clock_t startTime;

	Graph topo;
	RequestList requests;
	vector<int> passPathIndex;
	vector<vector<double>> remainCapacityPerEdge;

	

	int pushInPath(int indexReq, vector<vector<double>> &remainCapacityPerEdge);
	vector<int> topTenRelatedReq(int indexReq);
	void reschedule(vector<int> topTen);
	void outRes();
};