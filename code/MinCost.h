//
// Created by Zhixing Zhang on 2018/4/19.
//

#include "graph.h"
#include "requestList.h"
#include "algorithm"
#include <climits>
#include "result.h"
#include "const.h"

using namespace std;





class MinCost {
public:
    MinCost(Graph &topo, RequestList &requests);
    MinCost(int vNum, int eNum, int time, RequestList requests_);

    MinCost(const char *gFilename, int time, RequestList requests_);

    int VertexNum;
    int totalTime;

    clock_t startTime ;
    double bandwidthTime[MaxTime][VERTEXSIZE][VERTEXSIZE];//记录每个时间每条边上带宽的使用

    RequestList requests;
    Graph graph;
    Result result;

    vector<vector<int>> final_path;
    int final_bandwidth[VERTEXSIZE][VERTEXSIZE];//记录最终rounding之后的每条边的带宽
    double peekBandwidth[VERTEXSIZE][VERTEXSIZE];//记录峰值带宽

    void peekBandwidth_init();



    void pathSelecting();

    void bandwidthCal();

    void printResult();

    double getEdgeBandwidthUsage(int src, int dst, int time); //获取t时刻边(src->dst)的带宽利用
    void result_input();
};


