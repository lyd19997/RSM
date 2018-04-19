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
};