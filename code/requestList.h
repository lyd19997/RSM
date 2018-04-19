<<<<<<< HEAD
#pragma once
#include"request.h"
#include<vector>
using namespace std;


class RequestList :public vector<Request> {
public:
	RequestList(int topoVertexSize);//根据图节点数 产生请求集合
	RequestList(string pathIn);// 根据路径 读取请求集合
=======
#pragma once
#include"request.h"
#include<vector>
#include "string"

using namespace std;


class RequestList {
public:

	RequestList(int topoVertexSize, string RequestPathOut);

    RequestList(string PathIn);

    long int size() const;


	Request & operator[](int i);
	vector<Request>::iterator begin();
	vector<Request>::iterator end();


private:
	vector<Request> requests;
>>>>>>> b88f161684f75c19f5021875ea0188aeff629035
};