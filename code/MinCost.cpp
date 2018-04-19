//
// Created by Zhixing Zhang on 2018/4/19.
//

#include "MinCost.h"

using namespace std;

MinCost::MinCost(int vNum, int eNum, int time, RequestList requests_) :graph(vNum, eNum), requests(requests_){
    VertexNum = vNum;
    EdgeNum = eNum;
    totalTime = time;
//    requestsNum = requests.size();
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            final_bandwidth[i][j] = 0;
        }
    }
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[time][i][j] = 0;
            }
        }
    }
    peekBandwidth_init();
    pathSelecting();
    bandwidthCal();
    printResult();
}

MinCost::MinCost(const char *gFilename, int time, RequestList requests_) :graph(gFilename), requests(requests_){
    VertexNum = graph.getVertexNum();
    EdgeNum = graph.getEdgeNum();
    totalTime = time;
//    requestsNum = requests.size();
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            final_bandwidth[i][j] = 0;
        }
    }
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[time][i][j] = 0;
            }
        }
    }
    peekBandwidth_init();
    pathSelecting();
    bandwidthCal();
    printResult();


}

void MinCost::pathSelecting() {
    for(int i = 0; i < requests.size(); i++){
        int start = requests[i].src, end = requests[i].dst;
        vector<vector<int>> paths = graph.Paths[start][end];
        int min = INT_MAX, index = 0;
        int cost[paths.size()];
        for(int w = 0; w < paths.size(); w++){
            cost[w] = 0;
        }
        for(int t = requests[i].start; t <= requests[i].end; t++) {
            for (int p = 0; p < paths.size(); p++) {
                for (int j = 0; j < paths[p].size() - 1; j++) {
                    int restBandwidth = peekBandwidth[paths[p][j]][paths[p][j + 1]] - bandwidthTime[t][paths[p][j]][paths[p][j + 1]];
                    cost[p] += (requests[i].rate - restBandwidth) * graph.BandwidthPrice[paths[p][j]][paths[p][j + 1]];
                }
            }
        }
        for(int j  = 0; j < paths.size(); j++){
            if(min > cost[j]){
                min = cost[j];
                index = j;
            }
        }
        vector<int> path(paths[index]);
        final_path.push_back(path);
        for(int t = requests[i].start; t <= requests[i].end; t++) {
            for (int p = 0; p < path.size() - 1; p++) {
                bandwidthTime[t][path[p]][path[p + 1]] += requests[i].rate;
                if(bandwidthTime[t][path[p]][path[p + 1]] > peekBandwidth[path[p]][path[p + 1]]){
                    peekBandwidth[path[p]][path[p + 1]] = bandwidthTime[t][path[p]][path[p + 1]];
                }
            }
        }
    }
}

void MinCost::bandwidthCal() {
    for(int s = 0; s < VertexNum; s++){
        for(int d = 0; d < VertexNum; d++){

            final_bandwidth[s][d] = peekBandwidth[s][d];
        }
    }
}

void MinCost::printResult() {
    for(int i = 0; i < requests.size(); i++){
        printf("route for request #%d\n", i);
        for(int j : final_path[i]){
            printf("%d->", j);
        }
        printf("\n\n");
    }
    printf("Bandwidth\n");
    for(int s = 0; s < VertexNum; s++){
        for(int d = 0; d < VertexNum; d++){
            printf("%d ", final_bandwidth[s][d]);
        }
        printf("\n");
    }
    printf("\n\n");
    printf("final cost : ");
    int cost = 0;
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            cost += final_bandwidth[i][j] * graph.BandwidthPrice[i][j];
        }
    }
    printf("%d\n", cost);
}

int MinCost::getEdgeBandwidthUsage(int src, int dst, int time) {
    return bandwidthTime[time][src][dst];
}

void MinCost::peekBandwidth_init() {
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            peekBandwidth[i][j] = 0;
        }
    }
}


