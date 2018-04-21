#pragma once
#include"const.h"
#include"requestList.h"
#include"graph.h"

class Amoeba {
public:
	Amoeba(Graph topo, RequestList requests);
	vector<int> schedule();
private:
	Graph topo;
	RequestList requests;
	vector<int> passPathIndex;
	vector<vector<double>> remainCapacityPerEdge;
	bool pushInPath(int indexReq, vector<vector<double>> &remainCapacityPerEdge);
	vector<int> topTenRelatedReq(int indexReq);
	void reschedule(vector<int> topTen);

};