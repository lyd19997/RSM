#include"requestList.h"
#include"const.h"
#include "cmath"
#include<algorithm>
using namespace std;

int possion(int lambda)  /* 产生一个泊松分布的随机数，Lamda为总体平均数*/
{
	const int precision = 1000;
	int k = 0;
	long double p = 1.0;
	long double l = exp(-lambda);
	while (p >= l)
	{
		double u = (rand() % precision);
		u /= precision;
		p *= u;
		k++;
	}
	return k - 1;
}

double randomExponential(double lambda)
{
	double pV = 0.0;
	while (true)
	{
		pV = (double)rand() / (double)RAND_MAX;
		if (pV != 1)
		{
			break;
		}
	}
	pV = (-1.0 / lambda)*log(1 - pV);
	return pV;
}

RequestList::RequestList(int topoVertexSize) {
	int id = 0;
	//double threshold = 0, sumDemand = 0;
	for (int t = 0; t < (PEROID - 3 * MIN_DURATION); ++t)
	{
		int arrivalRate = possion(LAMBDA);
		for (int j = 1; j <= arrivalRate; j++)
		{
			int pos = rand() % (topoVertexSize*(topoVertexSize - 1));
			int src = pos / (topoVertexSize - 1);
			int dst = pos % ((topoVertexSize - 1));
			push_back(Request(id++, src, (dst < src ? dst : dst + 1), t, t + rand() % (2 * MIN_DURATION) + MIN_DURATION, randomExponential(1.0)*MEAN_VALUE, randomExponential(1.0)*MEAN_TRANSFER_SIZE));
		}
	}
	ofstream out(RequestPathOut);
	out << size() << endl;
	for (vector<Request>::iterator it = begin(); it != end(); ++it) {
		out << it->id << " " << it->src << " " << it->dst << " " << it->start << " " << it->end << " " << it->value << " " << it->rate << endl;
	}
	out.close();
}

RequestList::RequestList(string PathIn) {
	ifstream in(PathIn);
	if (!in.is_open()) {
		std::cout << "requestIn error" << endl;
		exit(1);
	}
	int num, id, src, dst, arriveTime, deadline, value;
	double rate;
	in >> num;
	for (int i = 0; i < num; ++i) {
		in >> id >> src >> dst >> arriveTime >> deadline >> value >> rate;
		push_back(Request(id, src, dst, arriveTime, deadline, value, rate));
	}
}

bool cmp(Request a, Request b) {
	return a.value / (a.end - a.start + 1) / a.rate > b.value / (b.end - b.start + 1) / b.rate;
}

void RequestList::sortRequestbyValue() {
	sort(begin(), end(), cmp);
}

double RequestList::rateMax() {
	int maxRate = 0;
	for (vector<Request>::iterator it = begin(); it != end(); ++it)
		if (it->rate > maxRate)
			maxRate = it->rate;
	return maxRate;
}