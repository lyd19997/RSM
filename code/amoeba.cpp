#include"Amoeba.h"
#include<queue>
using namespace std;

Amoeba::Amoeba(Graph topo_, RequestList requests_) :topo(topo_), requests(requests_), passPathIndex(requests_.size(), -1), remainCapacityPerEdge(PEROID, vector<double>()), startTime(clock()), res(topo_,requests_) {
	for (int t = 0; t < PEROID; ++t)
		for (int i = 0; i < topo.getEdgeNum(); ++i)
			remainCapacityPerEdge[t].push_back(topo.linkCapacity(i));
}

vector<int> Amoeba::schedule() {
	//requests.sortRequestbyValue();
	for (int i = 0; i < requests.size(); ++i)
	{
		cout << "push " << i << endl;
		passPathIndex[i] = pushInPath(i, remainCapacityPerEdge);
		if (passPathIndex[i] == -1)
		{
			vector<int> rescheduleReq = topTenRelatedReq(i);
			reschedule(rescheduleReq);
		}
	}
	outRes();
	return passPathIndex;
}

int Amoeba::pushInPath(int indexReq, vector<vector<double>> &remainCapacityPerEdge) {
	for (int i = min(0, topo.pathSize(requests[indexReq].getSrcDst()) - 1); i < min(INF,topo.pathSize(requests[indexReq].getSrcDst())); ++i)
	{
		vector<int> edgeList = topo.getPath(requests[indexReq].getSrcDst(), i);
		auto it = edgeList.begin();
		for (int t = requests[indexReq].start; t <= requests[indexReq].end; ++t)
		{
			//cout << remainCapacityPerEdge[t][*it] << "    " << requests[indexReq].rate<<end;;//debug
			for (it = edgeList.begin(); it != edgeList.end() &&
										remainCapacityPerEdge[t][*it] >= requests[indexReq].rate; ++it)
				;
			if (it != edgeList.end()) break;
		}
		if (it == edgeList.end())
		{
			for (int t = requests[indexReq].start; t <= requests[indexReq].end; ++t)
				for (it = edgeList.begin(); it != edgeList.end(); ++it)
				{
					remainCapacityPerEdge[t][*it] -= requests[indexReq].rate;
					//if (remainCapacityPerEdge[t][*it] < 0) { cout << "error pushinpath" << endl; while (1); }//debug
				}
			return i;
		}
	}
	return -1;
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
				vector<int> edgeList = topo.getPath(requests[indexReq].getSrcDst(), p);
				for (vector<int>::iterator it = edgeList.begin(); it != edgeList.end(); ++it)
					B += (topo.linkCapacity(*it) - remainCapacityPerEdge[t][*it])*topo.linkInPath(*it, requests[i].getSrcDst(), p)*requests[i].rate;
			}
		}
		Q.push(pair<double, int>(A + B, i));
	}
	vector<int> res;
	int cnt = 9;
	while (!Q.empty() && cnt--)
	{
		res.push_back(Q.top().second);
		Q.pop();
	}
	res.push_back(indexReq);
	return res;
}

void Amoeba::reschedule(vector<int> topTen) {
	double maxRevenue = 0;
	vector<vector<double> > preAllocateRemain(remainCapacityPerEdge);
	for (vector<int>::iterator it = topTen.begin(); it != topTen.end(); ++it)
	{
		//cout << passPathIndex[*it] << endl;
		if (passPathIndex[*it] == -1) continue;
		vector<int> edgeList = topo.getPath(requests[*it].getSrcDst(), passPathIndex[*it]);
		for (int t = requests[*it].start; t <= requests[*it].end; ++t)
			for (vector<int>::iterator ite = edgeList.begin(); ite != edgeList.end(); ++ite)
				preAllocateRemain[t][*ite] += requests[*it].rate;// , preAllocateRemain[t][*ite] -= requests[*it].rate;
		maxRevenue += requests[*it].value;
	}
	vector<int> resPassPath(topTen.size(), -1);
	int flag = 0;
	for (int set = 1; set < (1 << topTen.size()) - 1; ++set)
	{
		vector<vector<double> > allocate(preAllocateRemain);
		double revenue = 0;
		vector<int> passPath(topTen.size(), -1);
		for (int i = 0; i < topTen.size(); ++i)
		{
			if (set&(1 << i))
			{
				passPath[i] = pushInPath(topTen[i], allocate);
				revenue += (passPath[i] == -1 ? 0 : requests[topTen[i]].value);
			}
		}
		if (revenue > maxRevenue)
		{
			flag = 1;
			maxRevenue = revenue;
			resPassPath = passPath;
			remainCapacityPerEdge = allocate;
		}
	}
	if (flag)
	{
		for (vector<int>::iterator it = topTen.begin(); it != topTen.end(); ++it)
			passPathIndex[*it] = resPassPath[it - topTen.begin()];
	}
}

void Amoeba::outRes() {
	res.algName = "Amoeba";
	res.cost = 0;
	res.income = 0;
	res.receiveNum = 0;
	for (int i = 0; i < requests.size(); ++i)
		if (passPathIndex[i] != -1)
			res.income += requests[i].value, res.receiveNum += 1;
	res.passPathIndex = passPathIndex;
	for (int e = 0; e < topo.getEdgeNum(); ++e)
		res.peakPerEdge[e] = topo.linkCapacity(e);
	res.requestNum = requests.size();
	res.runTime = (clock() - startTime)*1.0 / CLOCKS_PER_SEC;
	for (int t = 0; t < PEROID; ++t)
	{
		for (int e = 0; e < topo.getEdgeNum(); ++e)
			res.volPerTimeEdge[t][e] = topo.linkCapacity(e) - remainCapacityPerEdge[t][e];
	}
}