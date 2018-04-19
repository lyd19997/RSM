#pragma once
#include"gurobi_c++.h"
#include"const.h"
#include"graph.h"
#include "requestList.h"

using namespace std;

class Blrsm {
public:
	vector<int> TAA();
	Blrsm(const Graph &topo,RequestList requests);

private:
	const Graph &topo;
	const RequestList &requests;
	vector<vector<double> > Pr_ij;
	vector<double> delta;
	vector<double> scaling;
	double Fs;
	
	
	vector<vector<double> > relaxation_LP();
	double PrUpperBound(int deep, const vector<int> &resX,int branch);
};