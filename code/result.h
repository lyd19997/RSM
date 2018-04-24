#pragma once
#include"const.h"
#include"graph.h"
#include"requestList.h"
using namespace std;
class Result {
public:
	int requestNum;
	int receiveNum;
	double income = 0;
	int cost = 0;
	Graph topo;
	RequestList requests;
	string algName;
	double runTime;//�㷨����ʱ�� ��ms�� (clock() - startTime)*1.0 / CLOCKS_PER_SEC
	vector<vector<double> > volPerTimeEdge;
	vector<int> peakPerEdge;
	vector<int> passPathIndex;

	void outResult();

	Result(Graph topo_, RequestList requests_);//��ʼ��vector
	void getRunTime();//��runTime��ֵ
private:
	double startTime;
};