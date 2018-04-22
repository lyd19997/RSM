//
// Created by Zhixing Zhang on 2018/4/17.
//

#include "FlowBase.h"

FlowBase::FlowBase(Graph &topo, RequestList &requests) : graph(topo), requests(requests), result(){
    startTime = clock();
    VertexNum = graph.getVertexNum();
    EdgeNum = graph.getEdgeNum();
    totalTime = PEROID;
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            final_bandwidth[i][j] = 0;
        }
    }
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[t][i][j] = 0;
            }
        }
    }
    result.requestNum = requests.size();
    result.receiveNum = requests.size();
    result.algName = "FlowBase";
    VaReqPath_init();
    iReqPathEdge_init();
    pathSelecting();
    bandwidthTime_init();
    bandwidthCal();
    printResult();
    result_input();
}

FlowBase::FlowBase(int vNum, int eNum, int time, RequestList requests_) :graph(vNum, eNum), requests(requests_), result(){
    startTime = clock();
    VertexNum = vNum;
    EdgeNum = eNum;
    totalTime = time;
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            final_bandwidth[i][j] = 0;
        }
    }
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[t][i][j] = 0;
            }
        }
    }
    result.requestNum = requests.size();
    result.receiveNum = requests.size();
    result.algName = "FlowBase";
    VaReqPath_init();
    iReqPathEdge_init();
    pathSelecting();
    bandwidthTime_init();
    bandwidthCal();
    printResult();
    result_input();
}

FlowBase::FlowBase(const char* gFilename, int time, RequestList requests_) :graph(gFilename), requests(requests_), result(){
    startTime = clock();
    VertexNum = graph.getVertexNum();
    EdgeNum = graph.getEdgeNum();
    totalTime = time;
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            final_bandwidth[i][j] = 0;
        }
    }
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[t][i][j] = 0;
            }
        }
    }
    result.requestNum = requests.size();
    result.receiveNum = requests.size();
    result.algName = "FlowBase";
    VaReqPath_init();
    iReqPathEdge_init();
    pathSelecting();
    bandwidthTime_init();
    bandwidthCal();
    printResult();
    result_input();
}

void FlowBase::VaReqPath_init() {
    for(int i = 0; i < requests.size(); i++){
        int start = requests[i].src, end = requests[i].dst;
        vector<vector<int>> paths = graph.Paths[start][end];
        vector<int> temp;
        for(int j = 0; j < paths.size(); j++){
            int value = 0;
            for(int p = 0; p < paths[j].size() - 1; p++){
                value += graph.BandwidthPrice[paths[j][p]][paths[j][p + 1]];
            }
            temp.push_back(value);
        }
        VaReqPath.push_back(temp);
    }
}

 

void FlowBase::bandwidthTime_init() {
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < requests.size(); i++){
            if(requests[i].start > t || t > requests[i].end) continue;
            for(int p = 0; p < final_path[i].size() - 1; p++){
                bandwidthTime[t][final_path[i][p]][final_path[i][p+1]] += requests[i].rate;
            }
        }
    }
}

void FlowBase::iReqPathEdge_init() {
    for(int i = 0; i < requests.size(); i++){
        vector<vector<vector<int>>> temp_i;
        int start = requests[i].src, end = requests[i].dst;
        vector<vector<int>> paths = graph.Paths[start][end];
        for(int j = 0; j < paths.size(); j++){
            vector<vector<int>> temp_j;
            for(int x = 0; x < VertexNum; x++){
                vector<int> temp_s;
                for(int y = 0; y < VertexNum; y++){
                    temp_s.push_back(0);
                }
                temp_j.push_back(temp_s);
            }
            vector<int> path = paths[j];
            for(int p = 0; p < path.size() - 1; p++){
                temp_j[path[p]][path[p+1]] = 1;
            }
            temp_i.push_back(temp_j);
        }
        iReqPathEdge.push_back(temp_i);
    }
}

void FlowBase::pathSelecting() {
    for(int i = 0; i < requests.size(); i++){
        int start = requests[i].src, end = requests[i].dst;
        vector<vector<int>> paths = graph.Paths[start][end];
        vector<int> tempVal(VaReqPath[i]);
        int min = MaxBandwidthPrice, index = 0;
        for(int j = 0; j < tempVal.size(); j++){
            if(tempVal[j] < min){
                min = tempVal[j];
                index = j;
            }
        }
        vector<int> path(paths[index]);
        final_path.push_back(path);
        result.passPathIndex[i]=index;
        printf("path for request#%d: ", i);
        for(int j = 0; j < path.size(); j++){
            printf("%d->", path[j]);
        }
        printf("\n");
    }
}

void FlowBase::bandwidthCal() {
    for(int s = 0; s < VertexNum; s++){
        for(int d = 0; d < VertexNum; d++){
            double maxBand = 0;
            for(int t = 0; t < totalTime; t++){
                if (bandwidthTime[t][s][d] > maxBand){
                    maxBand = bandwidthTime[t][s][d];
                }
            }
            final_bandwidth[s][d] = int (maxBand + 0.5);
            result.peakPerEdge[graph.getEdgeIndex(pair<int, int>(s, d))] = int (maxBand + 0.5);
        }
    }
}

void FlowBase::printResult() {
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
    result.cost = cost;
    printf("%d\n", cost);
}

double FlowBase::getEdgeBandwidthUsage(int src, int dst, int time) {
    return bandwidthTime[time][src][dst];
}

void FlowBase::result_input() {
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                result.volPerTimeEdge[t][graph.getEdgeIndex(pair<int, int>(i, j))] = getEdgeBandwidthUsage(i, j, t);
            }
        }
    }
    result.runTime = (clock() - startTime) * 1.0 / CLOCKS_PER_SEC;
}

