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
	void optimal();
	Blrsm(Graph topo_, RequestList requests_);
	Result res;
private:
	Graph topo;
	RequestList requests;
	
	vector<int> TAA();
	vector<vector<double> > Pr_ij;
	vector<double> delta;
	vector<double> scaling;
	double Fs;
	
	vector<int> passPathIndex;

	vector<vector<double> > relaxation_LP();
	double PrUpperBound(int deep, const vector<int> &resX,int branch);
	void gReq2One(int x);
	void outRes();
	void outResOpt(vector<vector<double> > passMultiPathIndex);


	double maxValue = 0, maxRate = 0;//request
	vector<double> valuePow;
	vector<vector<double> > ratePow;
	void preCalcPow();
};