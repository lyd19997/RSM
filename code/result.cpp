#include"result.h"


Result::Result(Graph topo_, RequestList requests_) :topo(topo_), requests(requests_), startTime(clock()), volPerTimeEdge(PEROID, vector<double>(topo_.getEdgeNum(), 0)),
peakPerEdge(topo_.getEdgeNum(), 0), passPathIndex(requests.size(), -1), passMultiPathindex(requests_.size(), vector<int>(50, 0))//暂时就这样吧
{
}


void Result::getRunTime() {
	runTime = (clock() - startTime)*1.0 / CLOCKS_PER_SEC;
}

void Result::outResult() {
	ofstream out(AlgorithmPath + algName + OutPath + ".txt");
	out << algName << endl;
	out << runTime << endl;
	out << receiveNum << endl;
	out << requestNum << endl;
	
	out << income << endl;
	out << cost << endl;
	
	out << "--passPathIndex--" << endl;
	for (vector<int>::iterator ite = passPathIndex.begin(); ite != passPathIndex.end(); ++ite)
		out << *ite << " ";
	out << endl;

	out << "--peakPerEdge--" << endl;
	for (vector<int>::iterator ite = peakPerEdge.begin(); ite != peakPerEdge.end(); ++ite)
		out << *ite << " ";
	out << endl;

	out << "--volPerTimeEdge--" << endl;
	for (vector<vector<double>>::iterator it = volPerTimeEdge.begin(); it != volPerTimeEdge.end(); ++it)
	{
		for (vector<double>::iterator ite = it->begin(); ite != it->end(); ++ite)
			out << *ite << " ";
		out << endl;
	}
}


