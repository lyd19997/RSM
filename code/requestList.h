#pragma once
#include"request.h"
#include<vector>
#include <string>

using namespace std;


class RequestList :public vector<Request> {
public:
	RequestList(int topoVertexSize);//根据图节点数 产生请求集合
	RequestList(string pathIn);// 根据路径 读取请求集合
	void sortRequestbyValue();
	double rateMax();
};