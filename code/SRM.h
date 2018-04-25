//
// Created by Zhixing Zhang on 2018/4/25.
//
#pragma once
#include"gurobi_c++.h"
#include"const.h"
#include"graph.h"
#include"result.h"
#include "requestList.h"
#include "BL_RSM.h"

#include "cstdio"

using namespace std;

class SRM {
public:
    int VertexNum;
    int totalTime;
    double bandwidthTime[MaxTime][VERTEXSIZE][VERTEXSIZE];
    long int requestsNum;
    GRBEnv* env = new GRBEnv();
    GRBModel model;
    vector<vector<GRBVar>> PrReqPath;//x_i,j
    GRBVar bandwidthSrcToDst[VERTEXSIZE][VERTEXSIZE];//c_e
    GRBLinExpr A = 0;
    GRBLinExpr B = 0;
    vector<vector<vector<vector<int>>>> iReqPathEdge; //I_i,j,e

    Graph graph;

    vector<vector<int>> final_path;
    int final_bandwidth[VERTEXSIZE][VERTEXSIZE];
    RequestList requests;
    Result result;
    Result opt;
    clock_t startTime;


    SRM(Graph &topo, RequestList &requests);
    void schedule();
    void PrReqPath_init();
    void bandwidthSrcToDst_init();
    void iReqPathEdge_init();
    void addConstraints();
    void setObj();
    bool linearSolver();

    int getEdgeBandwidthUsage(int src, int dst, int time); //获取t时刻边(src->dst)的带宽利用
    void outOpt();



};

