#include"Amoeba.h"
#include<queue>
using namespace std;

Amoeba::Amoeba(Graph topo, RequestList requests_) :topo(topo), requests(requests_), passPathIndex(requests_.size(), -1) {
	for (int i = 0; i < topo.getEdgeNum(); ++i)
		remainCapacityPerEdge.push_back(topo.linkCapacity(i));
}


bool Amoeba::pushInPath(int indexReq) {
	for (int i = 0; i < topo.pathSize(requests[indexReq].getSrcDst()); ++i)
	{
		vector<int> edgeList = topo.getPath(requests[indexReq].getSrcDst(), i);
		vector<int>::iterator it;
		for (it = edgeList.begin(); it != edgeList.end() && !(remainCapacityPerEdge[*it] < requests[indexReq].rate); ++it)
			;
		if (it == edgeList.end())
		{
			for (it = edgeList.begin(); it != edgeList.end(); ++it)
				remainCapacityPerEdge[*it] -= requests[indexReq].rate;
			return 1;
		}
	}
	return 0;
}

vector<int> Amoeba::topTenRelatedReq(int indexReq) {
	priority_queue < pair<double, int>, vector<pair<double, int> >, std::less<pair<double, int>>> Q;
	for (int i = 0; i < indexReq; ++i)
	{
		if (passPathIndex[i] == -1) continue;

		double A = requests[i].end - requests[i].start + 1 + requests[indexReq].end + requests[indexReq].start + 1 -
			(max(max(requests[i].start, requests[i].end), max(requests[indexReq].start, requests[indexReq].end)) - min(min(requests[i].start, requests[i].end), min(requests[indexReq].start, requests[indexReq].end)) + 1);
		if (A < 0) continue;
		int st = max(requests[i].start, requests[indexReq].start);
		int td = min(requests[i].end, requests[indexReq].end);
		double B = 0;
		for (int t = st; t <= td; ++t)
		{
			for (int p = 0; p < topo.pathSize(requests[indexReq].getSrcDst()); ++p)
			{
				vector<int> edgeList = topo.getPath(requests[indexReq].getSrcDst(), i);
				for (vector<int>::iterator it = edgeList.begin(); it != edgeList.end(); ++it)
					B += (topo.linkCapacity(*it) - remainCapacityPerEdge[*it])*topo.linkInPath(*it, requests[i].getSrcDst(), p)*requests[i].rate;
			}
		}
		Q.push(pair<double, int>(A + B, i));
	}
	vector<int> res;
	int cnt = 10;
	while (!Q.empty() && cnt--)
	{
		res.push_back(Q.top().second);
		Q.pop();
	}
	return res;
}

void Amoeba::reschedule(vector<int> topTen, int indexReq) {
	double revenue = 0;
	map<int, int> passPath;
	vector<double> preAllocateRemain(remainCapacityPerEdge);
	for (vector<int>::iterator it = topTen.begin(); it != topTen.end(); ++it)
	{
		for (vector<int>::iterator ite = topo.getPath(requests[*it], passPathIndex[*it]).begin(); ite != topo.getPath(requests[*it], passPathIndex[*it]).end(); ++ite)
			preAllocateRemain[*ite] += requests[*it].rate;
		revenue += requests[*it].value;
	}
	for (int set = 1; set < (1 << topTen.size()) - 1; ++set)
	{
		vector<double> allocate(remainCapacityPerEdge.size(), 0);
		for (int i = 0; i < topTen.size(); ++i)
		{
			if (set&(1 << i))
			{
				for (vector<int>::iterator ite = topo.getPath(requests[i], passPathIndex[i]).begin(); ite != topo.getPath(requests[i], passPathIndex[i]).end(); ++ite)//------
					preAllocateRemain[*ite] += requests[i].rate;
			}
		}
	}
}