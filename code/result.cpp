#include"result.h"


Result::Result(Graph topo_, RequestList requests_) :topo(topo_), requests(requests_), startTime(clock()), volPerTimeEdge(PEROID, vector<double>(topo_.getEdgeNum(), 0)),
peakPerEdge(topo_.getEdgeNum(), 0), passPathIndex(requests.size(), -1)
{
}


void Result::getRunTime() {
	runTime = (clock() - startTime)*1.0 / CLOCKS_PER_SEC;
}

void Result::outResult() {
	ofstream out(OutPath + algName + ".txt");
}

//int requestNum;
//int receiveNum;
//int income;
//int cost;
//Graph topo;
//RequestList requests;
//string algName;
//double runTime;//算法运行时间 （ms） (clock() - startTime)*1.0 / CLOCKS_PER_SEC
//vector<vector<double> > volPerTimeEdge;
//vector<int> peakPerEdge;
//vector<int> passPathIndex;
