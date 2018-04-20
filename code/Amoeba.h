#pragma once
#include"const.h"
#include"requestList.h"
#include"graph.h"

class Amoeba {
public:
	Amoeba(Graph &topo, RequestList &requests);
	vector<int> schedule();
private:
	Graph topo;
	RequestList requests;
	vector<int> passPathIndex;
	bool pushInPath();
	vector<int> topTenRelatedReq(int indexReq);
	void reschedule();
};