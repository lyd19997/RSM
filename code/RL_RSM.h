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
//#include "stdlib.h"


using namespace std;



class MAA {
public:
	MAA(const char *gFilename, int time, RequestList requests_);

	MAA(int vNum, int eNum, int time, RequestList requests_);

	int VertexNum;
	int EdgeNum;
	int totalTime;
	int bandwidthTime[MaxTime][MaxVertexNum][MaxVertexNum];//记录每个时间每条边上带宽的使用
	long int requestsNum;
	GRBEnv* env = new GRBEnv();
	GRBModel model;

	vector<vector<GRBVar>> PrReqPath;//x_i,j

	GRBVar bandwidthSrcToDst[MaxVertexNum][MaxVertexNum];//c_e

	GRBQuadExpr B = 0;

	vector<vector<vector<vector<int>>>> iReqPathEdge; //I_i,j,e

	Graph graph;

	vector<vector<int>> final_path;
	int final_bandwidth[MaxVertexNum][MaxVertexNum];//记录最终rounding之后的每条边的带宽

	RequestList requests;

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

	int getEdgeBandwidthUsage(int src, int dst, int time); //获取t时刻边(src->dst)的带宽利用
};

