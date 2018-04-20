#pragma once
#include"const.h"
using namespace std;
class Result {
public:
	int requestNum;
	int receiveNum;
	string algName;
	vector<vector<double> > volPerTimeEdge;
	vector<int> peakPerEdge;
	vector<int> passPathIndex;

	void outResult();
};