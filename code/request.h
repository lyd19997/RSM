<<<<<<< HEAD
#pragma once
#include<utility>
using namespace std;

class Request {
public:
	const int id, value;
	const int src, dst, start, end;
	const double rate;


	pair<int, int> getSrcDst();
	Request(int id_, int src_, int dst_, int start_, int end_, int value_, double rate_);
=======
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
>>>>>>> b88f161684f75c19f5021875ea0188aeff629035
};