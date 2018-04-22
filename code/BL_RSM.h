#pragma once
#include"gurobi_c++.h"
#include"const.h"
#include"graph.h"
#include"result.h"
#include "requestList.h"

using namespace std;

class Blrsm {
public:
	void schedule();
	Blrsm(Graph &topo, RequestList &requests);

private:
	Graph &topo;
	RequestList &requests;
	Result res;
	vector<int> TAA();
	vector<vector<double> > Pr_ij;
	vector<double> delta;
	vector<double> scaling;
	double Fs;
	
	vector<int> passPathIndex;

	vector<vector<double> > relaxation_LP();
	double PrUpperBound(int deep, const vector<int> &resX,int branch);
	void outRes();
};