#pragma once
#include<utility>
using namespace std;

class Request {
public:
	 int id, value;
	 int src, dst, start, end;
	 double rate;


	pair<int, int> getSrcDst();
	Request(int id_, int src_, int dst_, int start_, int end_, int value_, double rate_);

};