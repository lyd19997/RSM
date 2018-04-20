//
// Created by Zhixing Zhang on 2018/4/20.
//

#include "graph.h"
#include "requestList.h"
#include "const.h"

using namespace std;

class ValueFirst {
public:
    ValueFirst(Graph &topo, RequestList &requests);
    ValueFirst(int vNum, int eNum, int time, RequestList requests_);
    ValueFirst(const char *gFilename, int time, RequestList requests_);

    int VertexNum;
    int EdgeNum;
    int totalTime;
    double bandwidthTime[MaxTime][MaxVertexNum][MaxVertexNum];//记录每个时间每条边上带宽的使用

    Graph graph;
    RequestList requests;

    vector<vector<int>> final_path;

    double getEdgeBandwidthUsage(int src, int dst, int time); //获取t时刻边(src->dst)的带宽利用

    void sortRequestbyValue();
    void pathSelecting();
    void printResult();

};


