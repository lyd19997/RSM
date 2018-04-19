//
// Created by Zhixing Zhang on 2018/4/17.
//

#include "RL_RSM.h"


MAA::MAA(const char* gFilename, int time, RequestList requests_) :graph(gFilename), requests(requests_), model(*env) {
	VertexNum = graph.getVertexNum();
	EdgeNum = graph.getEdgeNum();
	totalTime = time;
	requestsNum = requests.size();//***
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			final_bandwidth[i][j] = 0;
		}
	}
	for (int t = 0; t < totalTime; t++) {
		for (int i = 0; i < VertexNum; i++) {
			for (int j = 0; j < VertexNum; j++) {
				bandwidthTime[time][VertexNum][VertexNum] = 0;
			}
		}
	}
	PrReqPath_init();
	bandwidthSrcToDst_init();
	iReqPathEdge_init();
	addConstraints();
	setObj();
	linearSolver();
	pathSelecting();
	bandwidthTime_init();
	bandwidthRounding();
	printResult();
}

MAA::MAA(int vNum, int eNum, int time, RequestList requests_) :graph(vNum, eNum), requests(requests_), model(*env) {
	VertexNum = vNum;
	EdgeNum = eNum;
	totalTime = time;
	requestsNum = requests.size();//***
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			final_bandwidth[i][j] = 0;
		}
	}
	for (int t = 0; t < totalTime; t++) {
		for (int i = 0; i < VertexNum; i++) {
			for (int j = 0; j < VertexNum; j++) {
				bandwidthTime[time][VertexNum][VertexNum] = 0;
			}
		}
	}
	PrReqPath_init();
	bandwidthSrcToDst_init();
	iReqPathEdge_init();
	addConstraints();
	setObj();
	linearSolver();
	pathSelecting();
	bandwidthTime_init();
	bandwidthRounding();
	printResult();
}

bool MAA::linearSolver() {
	model.optimize();
	model.write("MAA.lp");
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			if (graph.G[i][j]) {
				cout << i << " " << j << " " << bandwidthSrcToDst[i][j].get(GRB_DoubleAttr_X) << "\n";
			}
		}
	}
	for (int i = 0; i < requestsNum; i++) {
		for (GRBVar x_j : PrReqPath[i]) {
			cout << "for request " << i << "path " << x_j.get(GRB_DoubleAttr_X) << "\n";
		}
	}
	cout << "B " << model.get(GRB_DoubleAttr_ObjVal) << "\n";

	return true;
}

void MAA::PrReqPath_init() {
	for (int i = 0; i < requestsNum; i++) {
		int start = requests[i].src, end = requests[i].dst;//int start = requests.requests[i][0], end = requests.requests[i][1];*******
		vector<vector<int>> paths = graph.Paths[start][end];
		vector<GRBVar> temp;
		for (int j = 0; j < paths.size(); j++) {
			GRBVar tempVar = model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS);
			temp.push_back(tempVar);
		}
		PrReqPath.push_back(temp);
	}
}

void MAA::bandwidthSrcToDst_init() {
	for (int s = 0; s < VertexNum; s++) {
		for (int t = 0; t < VertexNum; t++) {
			if (graph.G[s][t]) {
				bandwidthSrcToDst[s][t] = model.addVar(0, MaxBandwidth, 0.0, GRB_CONTINUOUS);
			}
		}
	}
}

void MAA::iReqPathEdge_init() {
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
		iReqPathEdge.push_back(temp_i);
	}

}

void MAA::addConstraints() {
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

void MAA::setObj() {
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			if (graph.G[i][j]) {
				B += bandwidthSrcToDst[i][j] * graph.BandwidthPrice[i][j];
			}
		}
	}
	model.setObjective(B, GRB_MINIMIZE);

}

void MAA::pathSelecting() {
	for (int i = 0; i < requestsNum; i++) {
		int start = requests[i].src, end = requests[i].dst;//int start = requests.requests[i][0], end = requests.requests[i][1];********
		vector<vector<int>> paths = graph.Paths[start][end];
		double temp = 0;
		for (int j = 0; j < PrReqPath[i].size(); j++) {
			temp += PrReqPath[i][j].get(GRB_DoubleAttr_X) * j;
		}
		int index = int(round(temp));
		printf("chose %d\n", index);
		vector<int> path(paths[index]);
		final_path.push_back(path);
	}

}

void MAA::bandwidthRounding() {

	for (int s = 0; s < VertexNum; s++) {
		for (int d = 0; d < VertexNum; d++) {
			int maxBand = 0;
			for (int t = 0; t < totalTime; t++) {
				if (bandwidthTime[t][s][d] > maxBand) {
					maxBand = bandwidthTime[t][s][d];
				}
			}
			final_bandwidth[s][d] = maxBand;
		}
	}
}

void MAA::printResult() {
	for (int i = 0; i < requestsNum; i++) {
		printf("route for request #%d\n", i);
		for (int j : final_path[i]) {
			printf("%d->", j);
		}
		printf("\n\n");
	}
	printf("Bandwidth\n");
	for (int s = 0; s < VertexNum; s++) {
		for (int d = 0; d < VertexNum; d++) {
			printf("%d ", final_bandwidth[s][d]);
		}
		printf("\n");
	}

}

int MAA::getEdgeBandwidthUsage(int src, int dst, int time) {
	return bandwidthTime[time][src][dst];
}

void MAA::bandwidthTime_init() {
	for (int t = 0; t < totalTime; t++) {
		for (int i = 0; i < requestsNum; i++) {
			if (requests[i].start > t || t > requests[i].end) continue;//if (requests.requests[i][2] > t || t > requests.requests[i][3]) continue;*****
			for (int p = 0; p < final_path[i].size() - 1; p++) {
				bandwidthTime[t][final_path[i][p]][final_path[i][p + 1]] += requests[i].rate;//requests.requests[i][4];****
			}
		}
	}
}


