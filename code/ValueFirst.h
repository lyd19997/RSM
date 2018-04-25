////
//// Created by Zhixing Zhang on 2018/4/20.
////
//
//#include "graph.h"
//#include "requestList.h"
//#include "result.h"
//#include "const.h"
//
//using namespace std;
//
//class ValueFirst {
//public:
//    ValueFirst(Graph &topo, RequestList &requests);
//    ValueFirst(int vNum, int eNum, int time, RequestList requests_);
//    ValueFirst(const char *gFilename, int time, RequestList requests_);
//    clock_t startTime;
//    int VertexNum;
//    int EdgeNum;
//    int totalTime;
//    double bandwidthTime[MaxTime][VERTEXSIZE][VERTEXSIZE];//记录每个时间每条边上带宽的使用
//
//    Graph graph;
//    RequestList requests;
//    Result result;
//
//
//    vector<vector<int>> final_path;
//
//    double getEdgeBandwidthUsage(int src, int dst, int time); //获取t时刻边(src->dst)的带宽利用
//
//    void pathSelecting();
//    void printResult();
//    void result_input();
//
//};
//
//


#pragma once
#include"const.h"
#include"requestList.h"
#include"graph.h"
#include"result.h"

class ValueFirst {
public:
	ValueFirst(Graph topo, RequestList requests);
	vector<int> schedule();
	Result res;
private:
	clock_t startTime;

	Graph topo;
	RequestList requests;
	vector<int> passPathIndex;
	vector<vector<double>> remainCapacityPerEdge;



	int pushInPath(int indexReq, vector<vector<double>> &remainCapacityPerEdge);
	void outRes();
};