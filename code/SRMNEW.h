//
// Created by Zhixing Zhang on 2018/4/28.
//
#pragma once
#include"gurobi_c++.h"
#include"const.h"
#include"graph.h"
#include"result.h"
#include "requestList.h"
#include "BL_RSM.h"
#include "RL_RSM.h"

#include "cstdio"

class SRMNEW {
public:
    Graph graph;
    RequestList requestsList;
    Result result;
    vector<bool> removed;
    vector<bool> tempermoved;

    SRMNEW(Graph &topo, RequestList &requests);
    Result scheduleMAA(RequestList &rl);
    Result scheduleBL(RequestList &rl);

    void resultEqual(Result &res1, Result &res2);
};

