#pragma once
#include<utility>
using namespace std;

class Request {
public:
	 int id;
	 int src, dst, start, end;
	 double rate, value;


	pair<int, int> getSrcDst();
	Request(int id_, int src_, int dst_, int start_, int end_, double value_, double rate_);

};