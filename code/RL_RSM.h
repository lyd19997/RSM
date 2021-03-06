#pragma once
//
// Created by Zhixing Zhang on 2018/4/17.
//
#include "graph.h"
#include "requestList.h"
#include "requestList.h"
#include "gurobi_c++.h"
#include "cstdio"
#include "const.h"
#include "result.h"
//#include "stdlib.h"


using namespace std;



class MAA {
public:
    MAA(Graph &topo, RequestList &requests);
	MAA(const char *gFilename, int time, RequestList requests_);

	MAA(int vNum, int eNum, int time, RequestList requests_);

	int VertexNum;
	int EdgeNum;
	int totalTime;
	double bandwidthTime[MaxTime][VERTEXSIZE][VERTEXSIZE];//记录每个时间每条边上带宽的使用
	long int requestsNum;
	GRBEnv* env = new GRBEnv();
	GRBModel model;

	vector<vector<GRBVar>> PrReqPath;//x_i,j

	GRBVar bandwidthSrcToDst[VERTEXSIZE][VERTEXSIZE];//c_e

	GRBLinExpr B = 0;

	vector<vector<vector<vector<int>>>> iReqPathEdge; //I_i,j,e

	Graph graph;

	vector<vector<int>> final_path;
	int final_bandwidth[VERTEXSIZE][VERTEXSIZE];//记录最终rounding之后的每条边的带宽

	RequestList requests;
	Result result;
	Result opt;
	clock_t startTime;

	void PrReqPath_init();
	void bandwidthSrcToDst_init();
	void iReqPathEdge_init();
	void addConstraints();
	void setObj();
	bool linearSolver();
	//    int randomizedEvent(vector<>)
	void pathSelecting();
	void bandwidthRounding();
	void printResult();

	void bandwidthTime_init();
	void input_result();
	void opt_bandwidthTime();

	int getEdgeBandwidthUsage(int src, int dst, int time); //获取t时刻边(src->dst)的带宽利用
};

