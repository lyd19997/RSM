<<<<<<< HEAD
#pragma once
#include"request.h"
#include<vector>
using namespace std;


class RequestList :public vector<Request> {
public:
	RequestList(int topoVertexSize);//����ͼ�ڵ��� �������󼯺�
	RequestList(string pathIn);// ����·�� ��ȡ���󼯺�
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