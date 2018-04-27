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
    result.algName = "SRM";
    opt.requestNum = requests.size();
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
    TAASolver();
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
        vector<vector<vector<int>>> temp_i;//i,j,e == 1| 0
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
                            lhs += PrReqPath[i][j] * iReqPathEdge[i][j][e_s][e_t] * ((requests[i].start > t || requests[i].end < t) ? 0 : requests[i].rate * 1.0);//* requests.r_i_t[i][t] * 1.0;****
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
                opt.peakPerEdge[graph.getEdgeIndex(pair<int, int>(i, j))] =  int (ceil(bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X)));
                graph.BandwidthLim[i][j] = ceil(bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X));//????????????????
				//cout << bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X) << endl;
            }
        }
    }
    for (int i = 0; i < requestsNum; i++) {
        for (GRBVar x_j : PrReqPath[i]) {
            cout << "for request " << i << "path " << x_j.get(GRB_DoubleAttr_X) << "\n";
        }
    }
    cout << "A - B " << model.get(GRB_DoubleAttr_ObjVal) << "\n";
	//-----------------opt result------------------
	opt.income = (model.get(GRB_DoubleAttr_ObjVal));
	for (int i = 0; i < requests.size(); ++i)
		for (int j = 0; j < PrReqPath[i].size() - 1; ++j)   // size - 1
			opt.receiveNum += PrReqPath[i][j].get(GRB_DoubleAttr_X);

	opt.cost = 0;
	for (int e = 0; e < graph.getEdgeNum(); ++e)
		opt.cost += opt.peakPerEdge[e] * graph.BandwidthPrice[graph.findSrcDst(e).first][graph.findSrcDst(e).second];
	opt.income += opt.cost;
	opt.getRunTime();
	
	for (int i = 0; i < requests.size(); ++i)
	{
		opt.passMultiPathindex.push_back(vector<double>());
		for (int j = 0; j < PrReqPath[i].size() - 1; ++j)
			opt.passMultiPathindex[i].push_back(PrReqPath[i][j].get(GRB_DoubleAttr_X));
	}
	
	for (int i = 0; i < requests.size(); ++i)
	{
		for (int j = 0; j < opt.passMultiPathindex[i].size(); ++j)
		{
			if (opt.passMultiPathindex[i][j] == 0) continue;
			vector<int> edgeList = graph.getPath(requests[i].getSrcDst(), j);
			for (vector<int>::iterator ite = edgeList.begin(); ite != edgeList.end(); ++ite)
			{
				for (int t = requests[i].start; t <= requests[i].end; ++t)
					opt.volPerTimeEdge[t][*ite] += requests[i].rate*opt.passMultiPathindex[i][j];
			}
		}
	}
    return true;
}

void SRM::TAASolver() {
    Blrsm alg(graph, requests);
    alg.schedule();
    alg.res.outResult();
    passPathIndex = alg.res.passPathIndex;
    result.passPathIndex = alg.res.passPathIndex;
    result.peakPerEdge = alg.res.peakPerEdge;
    result.volPerTimeEdge = alg.res.volPerTimeEdge;
    result.receiveNum = alg.res.receiveNum;
    result.requestNum = alg.res.requestNum;
    result.income = alg.res.income;
	//----------rsm cost----------
	result.cost = 0;
	for (int e = 0; e < graph.getEdgeNum(); ++e)
		result.cost += result.peakPerEdge[e] * graph.BandwidthPrice[graph.findSrcDst(e).first][graph.findSrcDst(e).second];
	//-------------------
    for(int i = 0; i < requestsNum; i++){
        if(passPathIndex[i] == -1){
            cout <<"add "<< ADD(i) << endl;
        }else{
            if(!CHA(i)){
                cout <<"del "<< DEL(i) << endl;
            }
        }
    }

}


void SRM::outOpt() {

}

bool SRM::ADD(int reqIndex) {
    int src = requests[reqIndex].src, dst = requests[reqIndex].dst;
    int start = requests[reqIndex].start, end = requests[reqIndex].end;
    vector<vector<int>> paths(graph.Paths[src][dst]);
    for(int j = 0; j < paths.size(); j++){
        vector<vector<double> > volPerTimeEdge(result.volPerTimeEdge);
        vector<int> peakPerEdge(result.peakPerEdge);
        
		for(int p = 0; p < paths[j].size() - 1; p++){
            int e_s = paths[j][p], e_d = paths[j][p + 1];
            int e_index = graph.getEdgeIndex(pair<int, int>(e_s, e_d));
            for(int t = start; t <= end; t++){
                volPerTimeEdge[t][e_index] += requests[reqIndex].rate;
            }
        }
        
		double cost = calCost(peakPerEdge, volPerTimeEdge);
        if(requests[reqIndex].value - cost > 0){
            result.volPerTimeEdge = volPerTimeEdge;
            result.peakPerEdge = peakPerEdge;
            result.passPathIndex[reqIndex] = j;
            result.receiveNum += 1;
            result.income += requests[reqIndex].value;
            result.cost += cost;
            return true;
        }else{
            volPerTimeEdge.clear();
            peakPerEdge.clear();
        }
    }
    return false;
}

bool SRM::DEL(int reqIndex) {
    int src = requests[reqIndex].src, dst = requests[reqIndex].dst;
    int start = requests[reqIndex].start, end = requests[reqIndex].end;
    int pathChoose = result.passPathIndex[reqIndex];
    vector<vector<int>> paths(graph.Paths[src][dst]);
    vector<vector<double> > volPerTimeEdge(result.volPerTimeEdge);
    vector<int> peakPerEdge(result.peakPerEdge);
    for(int p = 0; p < paths[pathChoose].size() - 1; p++){
        int e_s = paths[pathChoose][p], e_d = paths[pathChoose][p + 1];
        int e_index = graph.getEdgeIndex(pair<int, int>(e_s, e_d));
        for(int t = start; t <= end; t++){
            volPerTimeEdge[t][e_index] -= requests[reqIndex].rate;
        }
    }
    double cost = calCost(peakPerEdge, volPerTimeEdge);
    if(-requests[reqIndex].value - cost > 0){
        result.volPerTimeEdge = volPerTimeEdge;
        result.peakPerEdge = peakPerEdge;
        result.passPathIndex[reqIndex] = -1;
        result.receiveNum -= 1;
        result.income -= requests[reqIndex].value;
        result.cost += cost;
        return true;
    }else{
        volPerTimeEdge.clear();
        peakPerEdge.clear();
        return false;
    }
}

bool SRM::CHA(int reqIndex) {
    int src = requests[reqIndex].src, dst = requests[reqIndex].dst;
    int start = requests[reqIndex].start, end = requests[reqIndex].end;
    vector<vector<int>> paths(graph.Paths[src][dst]);
    int pathChoose = result.passPathIndex[reqIndex];
    vector<vector<double> > volPerTimeEdge(result.volPerTimeEdge);
    vector<int> peakPerEdge(result.peakPerEdge);
    for(int p = 0; p < paths[pathChoose].size() - 1; p++){
        int e_s = paths[pathChoose][p], e_d = paths[pathChoose][p + 1];
        int e_index = graph.getEdgeIndex(pair<int, int>(e_s, e_d));
        for(int t = start; t <= end; t++){
            volPerTimeEdge[t][e_index] -= requests[reqIndex].rate;
        }
    }
    for(int j = 0; j < paths.size(); j++){
        if(j == pathChoose) continue;
        vector<vector<double> > volPerTimeEdge_(volPerTimeEdge);
        vector<int> peakPerEdge_(peakPerEdge);
        for(int p = 0; p < paths[j].size() - 1; p++){
            int e_s = paths[j][p], e_d = paths[j][p + 1];
            int e_index = graph.getEdgeIndex(pair<int, int>(e_s, e_d));
            for(int t = start; t <= end; t++){
                volPerTimeEdge_[t][e_index] += requests[reqIndex].rate;
            }
        }
        double cost = calCost(peakPerEdge_, volPerTimeEdge_);
        if(cost < 0){
            result.volPerTimeEdge = volPerTimeEdge_;
            result.peakPerEdge = peakPerEdge_;
            result.passPathIndex[reqIndex] = j;
            result.cost += cost;
            return true;
        }else{
            volPerTimeEdge_.clear();
            peakPerEdge_.clear();
        }
    }
    return false;
}

int SRM::calCost(vector<int> &peakPerEdge, vector<vector<double> > &volPerTimeEdge) {
    int postCost = 0, fuCost = 0;
    for(int e = 0; e < peakPerEdge.size(); e++){
        pair<int, int> srcDst = graph.findSrcDst(e);
        postCost += peakPerEdge[e] * graph.BandwidthPrice[srcDst.first][srcDst.second];
        double max = 0;
        for(int t = 0; t < volPerTimeEdge.size(); t++){
            if (volPerTimeEdge[t][e] > max){
                max = volPerTimeEdge[t][e];
            }
        }
        peakPerEdge[e] = int (ceil(max));
        fuCost += peakPerEdge[e] * graph.BandwidthPrice[srcDst.first][srcDst.second];
    }

    return fuCost - postCost;
}



// opt result
// --rsm cost