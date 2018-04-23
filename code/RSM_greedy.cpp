#include"const.h"
#include"RSM_greedy.h"

RsmGreedy::RsmGreedy(Graph topo_, RequestList requests_) :peakPerEdge(topo_.getEdgeNum(), 0), topo(topo_), requests(requests_), passPathIndex(requests_.size(), -1), volPerTimeEdge(PEROID, vector<double>(topo_.getEdgeNum(), 0)),res(topo_,requests_) {

}

void RsmGreedy::schedule() {
	int flag = 1;
	while (flag)
	{
		flag = 0;
		for (int i = 0; i < requests.size(); ++i)
		{
			if (passPathIndex[i] == -1)
				 flag |= pushInPath(i);
		}
	}
	resOut();
}


bool RsmGreedy::pushInPath(int indexReq) {
	double income = requests[indexReq].value;
	double minCost = INF;
	vector<vector<double> > minCostAllocate;
	vector<int> minPeak;
	for (int i = 0; i < topo.pathSize(requests[indexReq].getSrcDst()); ++i)
	{
		vector<vector<double> > preAllocateVol(volPerTimeEdge);
		for (int t = requests[indexReq].start; t <= requests[indexReq].end; ++t)
		{
			vector<int> edgeList = topo.getPath(requests[indexReq].getSrcDst(), i);
			for (vector<int>::iterator it = edgeList.begin(); it != edgeList.end(); ++it)
				preAllocateVol[t][*it] += requests[indexReq].rate;
		}
		vector<int> peak(peakPerEdge);
		double cost = 0;
		for (int i = 0; i < topo.getEdgeNum(); ++i)
		{
			for (int t = requests[indexReq].start; t <= requests[indexReq].end; ++t)
				if (preAllocateVol[t][i] > peak[i]) peak[i] = ceil(preAllocateVol[t][i]);
			cost += (peak[i] - peakPerEdge[i])*topo.getPrice(i);
		}
		if (cost < income&&cost < minCost)
		{
			minPeak = peak;
			minCost = cost;
			minCostAllocate = preAllocateVol;
			passPathIndex[indexReq] = i;
		}
	}
	if (minCost != INF)
	{
		peakPerEdge = minPeak;
		volPerTimeEdge = minCostAllocate;
		return 1;
	}
	return 0;
}

void RsmGreedy::resOut() {
	res.algName = "RsmGreedy";
	res.cost = 0;
	res.income = 0;
	res.receiveNum = 0;
	for (int i = 0; i < requests.size(); ++i)
		res.income += (passPathIndex[i] == -1 ? 0 : requests[i].value), res.receiveNum += 1;
	res.passPathIndex = passPathIndex;
	res.peakPerEdge = peakPerEdge;
	for (int i = 0; i < peakPerEdge.size(); ++i)
		res.cost += topo.getPrice(i)*peakPerEdge[i];
	res.requestNum = requests.size();
	res.getRunTime();
	res.volPerTimeEdge = volPerTimeEdge;
}