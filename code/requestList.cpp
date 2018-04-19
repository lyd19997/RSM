#include"requestList.h"
#include "cmath"
#include"const.h"
#include <iostream>

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

RequestList::RequestList(int topoVertexSize, string RequestPathOut) {
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
			requests.push_back(Request(src, (dst < src ? dst : dst + 1), t, t + rand() % (2 * MIN_DURATION) + MIN_DURATION, 1/*value*/, randomExponential(1.0)*MEAN_TRANSFER_SIZE));
		}
	}
	ofstream out(RequestPathOut);
	out << size() << endl;
	out << id << endl;
	for (vector<Request>::iterator it = begin(); it != end(); ++it) {
		out  << it->src << " " << it->dst << " " << it->start << " " << it->end << " " << it->value << " " << it->rate << endl;
	}
	out.close();
}

RequestList::RequestList(string PathIn) {
	ifstream in(PathIn);
	if (!in.is_open()) {
		cout << "requestIn error" << endl;
		while (1);
	}
	int num, id, src, dst, arriveTime, deadline, value;
	double rate;
	in >> num;
	for (int i = 0; i < num; ++i) {
		in >> id >> src >> dst >> arriveTime >> deadline >> value >> rate;
		requests.push_back(Request(src, dst, arriveTime, deadline, value, rate));
	}
}

int RequestList::size() const {
    return requests.size();
}

vector<Request>::iterator RequestList::end() {
    return requests.end();
}

vector<Request>::iterator RequestList::begin() {
    return requests.begin();
}

Request &RequestList::operator[](int i) {
    return requests[i];
}


