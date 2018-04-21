#pragma once
#include"const.h"
#include"graph.h"
#include"requestList.h"
using namespace std;
class Result {
public:
	int requestNum;
	int receiveNum;
	int income;
	int cost;
	Graph topo;
	RequestList requests;
	string algName;
	double runTime;//�㷨����ʱ�� ��ms�� (clock() - startTime)*1.0 / CLOCKS_PER_SEC
	vector<vector<double> > volPerTimeEdge;
	vector<int> peakPerEdge;
	vector<int> passPathIndex;

	void outResult();
};