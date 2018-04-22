//
// Created by Zhixing Zhang on 2018/4/20.
//

#include "ValueFirst.h"
using namespace std;

ValueFirst::ValueFirst(Graph &topo, RequestList &requests) : graph(topo), requests(requests) , result(){
    startTime = clock();
    VertexNum = graph.getVertexNum();
    EdgeNum = graph.getEdgeNum();
    totalTime = PEROID;
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[t][i][j] = 0;
            }
        }
    }
    result.requestNum = requests.size();
    result.algName = "ValueFirst";
    requests.sortRequestbyValue();
    pathSelecting();
    printResult();
    result_input();
}

ValueFirst::ValueFirst(int vNum, int eNum, int time, RequestList requests_) :graph(vNum, eNum), requests(requests_), result(){
    startTime = clock();
    VertexNum = vNum;
    EdgeNum = eNum;
    totalTime = time;
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[t][i][j] = 0;
            }
        }
    }
    requests.sortRequestbyValue();
    result.algName = "ValueFirst";
    result.requestNum = requests.size();
    pathSelecting();
    printResult();
    result_input();
}

ValueFirst::ValueFirst(const char *gFilename, int time, RequestList requests_) :graph(gFilename), requests(requests_), result(){
    startTime = clock();
    VertexNum = graph.getVertexNum();
    EdgeNum = graph.getEdgeNum();
    totalTime = time;
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                bandwidthTime[t][i][j] = 0;
            }
        }
    }
    requests.sortRequestbyValue();
    result.algName = "ValueFirst";
    result.requestNum = requests.size();
    pathSelecting();
    printResult();
    result_input();
}



void ValueFirst::pathSelecting() {
    for(Request request : requests){
        int src = request.src, dst = request.dst;
        int start = request.start, end = request.end;
        int value = request.value, id = request.id;
        double rate = request.rate;
        vector<vector<int>> paths (graph.Paths[src][dst]);
        vector<int> val;
        for(vector<int> path : paths){ val.push_back(0);}
        val.push_back(0);
        for(int t = start; t <= end; t++){
            for(int i = 0; i < paths.size(); i++){
                if(val[i] == -1) continue;
                for(int p = 0; p < paths[i].size() - 1; p++){
                    if(bandwidthTime[t][paths[i][p]][paths[i][p + 1]] + rate > graph.BandwidthLim[paths[i][p]][paths[i][p + 1]]){
                        val[i] = -1;
                        break;
                    }else{
                        val[i] = value;
                    }
                }
            }
        }
        int max = 0, index = val.size() - 1;
        for(int i = 0; i < val.size(); i++){
            if(val[i] > max){
                max = val[i];
                index = i;
            }
        }
        vector<int> fPath;
        if(index == val.size() - 1){
            fPath.push_back(-1);
            final_path.push_back(fPath);
            result.passPathIndex[id] = -1;
        }else{
            vector<int> f_Path(paths[index]);
            final_path.push_back(f_Path);
            for(int t = start; t <= end; t++){
                for(int p = 0; p < f_Path.size() - 1; p++){
                    bandwidthTime[t][f_Path[p]][f_Path[p + 1]] += rate;
                }
            }
            result.passPathIndex[id] = index;
            result.receiveNum += 1;
        }
    }
}

void ValueFirst::printResult() {
    int income = 0;
    for(int i = 0; i < requests.size(); i++){
        if(final_path[i].size() > 1){
            printf("route for request #%d\n", i);
            for(int j : final_path[i]){
                printf("%d->", j);
            }
            income += requests[i].value;
        }else{
            printf("no route for request #%d\n", i);
        }
        printf("\n\n");
    }
    printf("\n\n");
    printf("final income : ");
    printf("%d\n", income);
    result.income = income;
}

double ValueFirst::getEdgeBandwidthUsage(int src, int dst, int time) {
    return bandwidthTime[time][src][dst];
}

void ValueFirst::result_input() {
    for(int t = 0; t < totalTime; t++){
        for(int i = 0; i < VertexNum; i++){
            for(int j = 0; j < VertexNum; j++){
                result.volPerTimeEdge[t][graph.getEdgeIndex(pair<int, int>(i, j))] = getEdgeBandwidthUsage(i, j, t);
            }
        }
    }
    result.runTime = (clock() - startTime) * 1.0 / CLOCKS_PER_SEC;
}

