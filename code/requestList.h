#pragma once
#include"request.h"
#include<vector>
using namespace std;


class RequestList :public vector<Request> {
public:
	RequestList(int topoVertexSize);//����ͼ�ڵ��� �������󼯺�
	RequestList(string pathIn);// ����·�� ��ȡ���󼯺�
};