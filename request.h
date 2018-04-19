#pragma once
#include<utility>
using namespace std;

class Request {
public:
	const int value;
	const int src, dst, start, end;
	const double rate;


	pair<int, int> getSrcDst();
	Request(int src_, int dst_, int start_, int end_, int value_, double rate_);
};