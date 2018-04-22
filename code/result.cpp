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
	cout << requestNum << endl;
	cout << receiveNum << endl;
	cout << income << endl;
	cout << cost << endl;
	cout << algName << endl;
	cout << runTime << endl;
	cout << "--volPerTimeEdge--" << endl;
	for (vector<vector<double>>::iterator it = volPerTimeEdge.begin(); it != volPerTimeEdge.end(); ++it)
	{
		for (vector<double>::iterator ite = it->begin(); ite != it->end(); ++ite)
			cout << *ite << " ";
		cout << endl;
	}
	cout << "--peakPerEdge--" << endl;
	for (vector<int>::iterator ite = peakPerEdge.begin(); ite != peakPerEdge.end(); ++ite)
		cout << *ite << " ";
	cout << endl;
	cout << "--passPathIndex--" << endl;
	for (vector<int>::iterator ite = passPathIndex.begin(); ite != passPathIndex.end(); ++ite)
		cout << *ite << " ";
	cout << endl;
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
