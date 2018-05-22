//
// Created by Zhixing Zhang on 2018/5/8.
//
#pragma once
#include"gurobi_c++.h"
#include"const.h"
#include"graph.h"
#include"result.h"
#include "requestList.h"
#include "BL_RSM.h"

#include "cstdio"

class ILP {
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
    clock_t startTime;

    vector<int> passPathIndex;


    ILP(Graph &topo, RequestList &requests);
    void schedule();
    void PrReqPath_init();
    void bandwidthSrcToDst_init();
    void iReqPathEdge_init();
    void addConstraints();
    void setObj();
    bool linearSolver();


};
