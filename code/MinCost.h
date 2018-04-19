//
// Created by Zhixing Zhang on 2018/4/19.
//

#include "graph.h"
#include "requestList.h"
#include "algorithm"
#include <climits>
#include "const.h"

using namespace std;





class MinCost {
public:
    MinCost(Graph &topo, RequestList &requests);
    MinCost(int vNum, int eNum, int time, RequestList requests_);

    MinCost(const char *gFilename, int time, RequestList requests_);

    int VertexNum;
    int EdgeNum;
    int totalTime;
    int bandwidthTime[MaxTime][MaxVertexNum][MaxVertexNum];//记录每个时间每条边上带宽的使用
//    long int requestsNum;

    vector<vector<int>> VaReqPath;//x_i,j

//    int bandwidthSrcToDst[MaxVertexNum][MaxVertexNum];//c_e


    vector<vector<vector<vector<int>>>> iReqPathEdge; //I_i,j,e

    Graph graph;

    vector<vector<int>> final_path;
    int final_bandwidth[MaxVertexNum][MaxVertexNum];//记录最终rounding之后的每条边的带宽
    int peekBandwidth[MaxVertexNum][MaxVertexNum];//记录峰值带宽

    void peekBandwidth_init();

    RequestList requests;


    void VaReqPath_init();

//    void bandwidthSrcToDst_init();

//    void bandwidthTime_update();

    void iReqPathEdge_init();

    void pathSelecting();

    void bandwidthCal();

    void printResult();

    int getEdgeBandwidthUsage(int src, int dst, int time); //获取t时刻边(src->dst)的带宽利用
};


