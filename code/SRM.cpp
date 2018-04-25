//
// Created by Zhixing Zhang on 2018/4/25.
//

#include "SRM.h"

SRM::SRM(Graph &topo, RequestList &requests):graph(topo),requests(requests), model(*env) , result(topo, requests), opt(topo, requests) {
    startTime = clock();
    VertexNum = graph.getVertexNum();
    totalTime = PEROID;
    requestsNum = requests.size();//***
    for (int i = 0; i < VertexNum; i++) {
        for (int j = 0; j < VertexNum; j++) {
            final_bandwidth[i][j] = 0;
        }
    }
    for (int t = 0; t < totalTime; t++) {
        for (int i = 0; i < VertexNum; i++) {
            for (int j = 0; j < VertexNum; j++) {
                bandwidthTime[t][i][j] = 0;
            }
        }
    }
    result.requestNum = requests.size();
    result.receiveNum = requests.size();
    result.algName = "SRM";
    opt.requestNum = requests.size();
    opt.receiveNum = requests.size();
    opt.algName = "SRM-OPT";
    schedule();
}

void SRM::schedule() {
    PrReqPath_init();
    bandwidthSrcToDst_init();
    iReqPathEdge_init();
    addConstraints();
    setObj();
    linearSolver();
}

void SRM::PrReqPath_init() {
    for (int i = 0; i < requestsNum; i++) {
        int start = requests[i].src, end = requests[i].dst;//int start = requests.requests[i][0], end = requests.requests[i][1];*******
        vector<vector<int>> paths = graph.Paths[start][end];
        vector<GRBVar> temp;
        for (int j = 0; j < paths.size(); j++) {
            GRBVar tempVar = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS);
            temp.push_back(tempVar);
        }
        GRBVar tempVar = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS);
        temp.push_back(tempVar);
        PrReqPath.push_back(temp);
    }
}

void SRM::bandwidthSrcToDst_init() {
    for (int s = 0; s < VertexNum; s++) {
        for (int t = 0; t < VertexNum; t++) {
            if (graph.G[s][t]) {
                bandwidthSrcToDst[s][t] = model.addVar(0, MaxBandwidth, 0.0, GRB_CONTINUOUS);
            }
        }
    }
}

void SRM::iReqPathEdge_init() {
    for (int i = 0; i < requestsNum; i++) {
        vector<vector<vector<int>>> temp_i;
        int start = requests[i].src, end = requests[i].dst;//int start = requests.requests[i][0], end = requests.requests[i][1];**************
        vector<vector<int>> paths = graph.Paths[start][end];
        for (int j = 0; j < paths.size(); j++) {
            vector<vector<int>> temp_j;
            for (int x = 0; x < VertexNum; x++) {
                vector<int> temp_s;
                for (int y = 0; y < VertexNum; y++) {
                    temp_s.push_back(0);
                }
                temp_j.push_back(temp_s);
            }
            vector<int> path = paths[j];
            for (int p = 0; p < path.size() - 1; p++) {
                //                for(int q = p + 1; q < path.size(); q++){
                temp_j[path[p]][path[p + 1]] = 1;
                //                }
            }
            temp_i.push_back(temp_j);
        }
        vector<vector<int>> temp_j;
        for (int x = 0; x < VertexNum; x++) {
            vector<int> temp_s;
            for (int y = 0; y < VertexNum; y++) {
                temp_s.push_back(0);
            }
            temp_j.push_back(temp_s);
        }
        temp_i.push_back(temp_j);
        iReqPathEdge.push_back(temp_i);
    }
}

void SRM::addConstraints() {
    for (int i = 0; i < requestsNum; i++) {
        GRBLinExpr lhs = 0;
        for (GRBVar x : PrReqPath[i]) {
            lhs += x;
        }
        model.addConstr(lhs, GRB_EQUAL, 1.0);
    }

    for (int t = 0; t < totalTime; t++) {
        for (int e_s = 0; e_s < VertexNum; e_s++) {
            for (int e_t = 0; e_t < VertexNum; e_t++) {
                if (graph.G[e_s][e_t]) {
                    GRBLinExpr lhs = 0;
                    for (int i = 0; i < requestsNum; i++) {
                        //                        GRBLinExpr temp = 0;
                        for (int j = 0; j < PrReqPath[i].size(); j++) {
                            lhs += PrReqPath[i][j] * iReqPathEdge[i][j][e_s][e_t] * ((requests[i].start >= t || requests[i].end <= t) ? 0 : requests[i].rate * 1.0);//* requests.r_i_t[i][t] * 1.0;****
                        }
                        //                        lhs += temp;
                    }
                    model.addConstr(lhs <= bandwidthSrcToDst[e_s][e_t]);
                }
            }
        }
    }
}

void SRM::setObj() {
    for (int i = 0; i < VertexNum; i++) {
        for (int j = 0; j < VertexNum; j++) {
            if (graph.G[i][j]) {
                B += bandwidthSrcToDst[i][j] * graph.BandwidthPrice[i][j];
            }
        }
    }
    for(int i = 0; i < requestsNum; i++){
        GRBLinExpr temp = 0;
        int start = requests[i].src, end = requests[i].dst, value = requests[i].value;//int start = requests.requests[i][0], end = requests.requests[i][1];*******
        vector<vector<int>> paths = graph.Paths[start][end];
        for(int j = 0; j < paths.size(); j++){
            temp += PrReqPath[i][j];
        }
        A += temp * value;
    }
    model.setObjective(A - B, GRB_MAXIMIZE);
}

bool SRM::linearSolver() {
    model.optimize();
    model.write("SRM.lp");
    for (int i = 0; i < VertexNum; i++) {
        for (int j = 0; j < VertexNum; j++) {
            if (graph.G[i][j]) {
                cout << i << " " << j << " " << bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X) << "\n";
                opt.peakPerEdge[graph.getEdgeIndex(pair<int, int>(i, j))] =  int(bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X) + 0.5);
                result.peakPerEdge[graph.getEdgeIndex(pair<int, int>(i, j))] =  int(bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X) + 0.5);
                graph.BandwidthLim[i][j] = int(bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X) + 0.5);
            }
        }
    }
    for (int i = 0; i < requestsNum; i++) {
        for (GRBVar x_j : PrReqPath[i]) {
            cout << "for request " << i << "path " << x_j.get(GRB_DoubleAttr_X) << "\n";
        }
    }
    cout << "A - B " << model.get(GRB_DoubleAttr_ObjVal) << "\n";
    opt.income = int(model.get(GRB_DoubleAttr_ObjVal) + 0.5);
    return true;
}

int SRM::getEdgeBandwidthUsage(int src, int dst, int time) {
    return 0;
}


void SRM::outOpt() {

}


