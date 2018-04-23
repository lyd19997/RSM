//
// Created by Zhixing Zhang on 2018/4/17.
//

#include "graph.h"



Graph::Graph(const char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	fscanf(fp, "%d %d", &VertexNum, &EdgeNum);
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			G[i][j] = false;
			Bandwidth[i][j] = 0;
			BandwidthLim[i][j] = 0;
			BandwidthPrice[i][j] = 0;
		}
	}
	for (int i = 0; i < EdgeNum; i++) {
		int start, terminate, price, lim;
		fscanf(fp, "%d %d %d %d", &start, &terminate, &price, &lim);
		G[start][terminate] = true;
		BandwidthPrice[start][terminate] = price;
		BandwidthLim[start][terminate] = lim;
        G[terminate][start] = true;
        BandwidthPrice[terminate][start] = price;
        BandwidthLim[terminate][start] = lim;
	}
	pair2EIndex_init();
	printGraph();
	findPaths();
	sortByLength();
	printPaths();
}

Graph::Graph(int vertexNum, int edgeNum) {//使用点数和边数生成无向联通图
	VertexNum = vertexNum;
	EdgeNum = edgeNum;
	if (EdgeNum < VertexNum - 1) EdgeNum = VertexNum - 1;
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			G[i][j] = false;
			Bandwidth[i][j] = 0;
            BandwidthLim[i][j] = 0;
            BandwidthPrice[i][j] = 0;
		}
	}
	vector<int> connected, unconnected;
	for (int i = 1; i < VertexNum; i++) unconnected.push_back(i);
	int leftNum = VertexNum - 1;
	connected.push_back(0);
	while (leftNum > 0) {
		auto start = static_cast<int>(rand() % connected.size());
		auto end = static_cast<int>(rand() % unconnected.size());
		G[connected[start]][unconnected[end]] = true;
		G[unconnected[end]][connected[start]] = true;
		int capacity = Capacity[rand() % RANDNUM], prices = Prices[rand() % RANDNUM];
        BandwidthLim[connected[start]][unconnected[end]] = capacity;
        BandwidthLim[unconnected[end]][connected[start]] = capacity;
        BandwidthPrice[connected[start]][unconnected[end]] = prices;
        BandwidthPrice[unconnected[end]][connected[start]] = prices;
//		vector<int> temp;
//		temp.push_back(start);
//		temp.push_back(end);
//		Paths[start][end].push_back(temp);
//		temp.clear();
//		temp.push_back(end);
//		temp.push_back(start);
//		Paths[end][start].push_back(temp);
		connected.push_back(unconnected[end]);
		unconnected.erase(unconnected.begin() + end);
		leftNum--;
	}
	leftNum = EdgeNum - VertexNum + 1;
	while (leftNum > 0) {
		auto start = static_cast<int>(rand() % connected.size());
		auto end = static_cast<int>(rand() % connected.size());
		if (start != end && !G[connected[start]][connected[end]]) {
			G[connected[start]][connected[end]] = true;
			G[connected[end]][connected[start]] = true;
            int capacity = Capacity[rand() % RANDNUM], prices = Prices[rand() % RANDNUM];
            BandwidthLim[connected[start]][unconnected[end]] = capacity;
            BandwidthLim[unconnected[end]][connected[start]] = capacity;
            BandwidthPrice[connected[start]][unconnected[end]] = prices;
            BandwidthPrice[unconnected[end]][connected[start]] = prices;
//			vector<int> temp;
//			temp.push_back(start);
//			temp.push_back(end);
//			Paths[start][end].push_back(temp);
//			temp.clear();
//			temp.push_back(end);
//			temp.push_back(start);
//			Paths[end][start].push_back(temp);
			leftNum--;
		}
	}
    pair2EIndex_init();
	//printGraph();
	findPaths();
	sortByLength();
	//printPaths();
    outputGraph();
}

void Graph::findPaths() {
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			if (i != j) {
				//bool visited[VertexNum];
				bool *visited = new bool[VertexNum];
				for (int k = 0; k < VertexNum; k++)
					visited[k] = false;
				findPath(i, j, visited);
			}
		}
	}

}

void Graph::findPath(int start, int terminate, bool *visited) {
	visited[start] = true;
	for (int i = 0; i < VertexNum; i++) {
		if (!visited[i] && G[start][i]) {
			if (i == terminate) {
				vector<int> tempPath;
				tempPath.push_back(start);
				tempPath.push_back(terminate);
				Paths[start][terminate].push_back(tempPath);
			}
			else {
				findPath(i, terminate, visited);
				if (!Paths[i][terminate].empty()) {
					for (int j = 0; j < Paths[i][terminate].size(); j++) {
						vector<int> tempPath(Paths[i][terminate][j]);
						auto it = find(tempPath.begin(), tempPath.end(), start);
						if (it == tempPath.end()) {
							tempPath.insert(tempPath.begin(), start);
							Paths[start][terminate].push_back(tempPath);
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < Paths[start][terminate].size(); i++) {
		for (int j = i + 1; j < Paths[start][terminate].size(); j++) {
			if (Paths[start][terminate][i] == Paths[start][terminate][j]) {
				Paths[start][terminate].erase(Paths[start][terminate].begin() + j);
				j--;
			}
		}
	}
	visited[start] = false;
}

bool cmp(vector<int> path1, vector<int> path2){
	return path1.size() < path2.size();
}

void Graph::sortByLength() {
	for(int i = 0; i < VertexNum; i++){
		for(int j = 0; j < VertexNum; j++){
			sort(Paths[i][j].begin(), Paths[i][j].end(), cmp);
		}
	}
}

int Graph::getVertexNum() {
	return VertexNum;
}

int Graph::getEdgeNum() {
	return EdgeNum * 2;
}

void Graph::printGraph() {
	int count = 0;
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			printf("%d ", G[i][j]);
			if (G[i][j]) {
				count++;
			}
		}
		printf("\n");
	}
	printf("EdgeNum: %d\n", EdgeNum);
	printf("Count: %d\n", count);
}

void Graph::printPaths() {
	for (int i = 0; i < VertexNum; i++) {
		for (int j = 0; j < VertexNum; j++) {
			if (i == j) continue;
			printf("Paths from %d to %d\n", i, j);
			for (auto &x : Paths[i][j]) {
				for (int y : x) {
					printf("%d -> ", y);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
}

int Graph::pathCapacity(pair<int, int>srcDst, int pathIndex) {
	return pathCapacity(srcDst.first, srcDst.second, pathIndex);
}

int Graph::pathCapacity(int src, int dst, int pathIndex) {
	int min = MaxBandwidth;
	if (pathIndex >= Paths[src][dst].size()) return 0;
	vector<int> path = Paths[src][dst][pathIndex];

	for (int e_s = 0; e_s < path.size() - 1; e_s++) {
		int e_d = e_s + 1;
		if (BandwidthLim[path[e_s]][path[e_d]] < min)
			min = BandwidthLim[path[e_s]][path[e_d]];
	}
	return min;
}

bool Graph::linkInPath(int e_src, int e_dst, int src, int dst, int pathIndex) {
	if (pathIndex >= Paths[src][dst].size()) return false;
	vector<int> path = Paths[src][dst][pathIndex];
	for (int e_s = 0; e_s < path.size() - 1; e_s++) {
		int e_d = e_s + 1;
		if (path[e_s] == e_src && path[e_d] == e_dst) return true;
	}
	return false;
}

void Graph::initBandwidthLim(const char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	for (int i = 0; i < EdgeNum; i++) {
		int src, dst, bandwidth;
		fscanf(fp, "%d %d %d", &src, &dst, &bandwidth);
		BandwidthLim[src][dst] = bandwidth;
	}
}


int Graph::pathSize(pair<int, int>srcDst) {
	return Paths[srcDst.first][srcDst.second].size();
}

int Graph::linkCapacity(int edgeIndex) {
    pair<int, int> srcDst = findSrcDst(edgeIndex);
	return BandwidthLim[srcDst.first][srcDst.second];
}

bool Graph::linkInPath(int edgeIndex, pair<int, int>srcDst, int pathIndex) {
    pair<int, int> linksrcDst = findSrcDst(edgeIndex);
	return linkInPath(linksrcDst.first, linksrcDst.second, srcDst.first, srcDst.second, pathIndex);
}


void Graph::pair2EIndex_init() {
    int count = 0;
    for(int i = 0; i < VertexNum; i++){
        for(int j = i; j < VertexNum; j++){
            if(G[i][j]){
                pair2EIndex[i][j] = count;
                count ++;
                pair2EIndex[j][i] = count;
                count ++;
            }else{
				pair2EIndex[i][j] = -1;
				pair2EIndex[j][i] = -1;
            }
        }
    }
}

int Graph::getEdgeIndex(pair<int, int> srcDst) {
	return pair2EIndex[srcDst.first][srcDst.second];
}


pair<int, int> Graph::findSrcDst(int edgeIndex) {
    for(int i = 0; i < VertexNum; i++){
        for(int j = 0; j < VertexNum; j++){
            if(pair2EIndex[i][j] == edgeIndex) return pair<int, int>(i, j);
        }
    }
    return pair<int, int>(0, 0);
}

vector<int> Graph::getPath(pair<int, int> srcDst, int pathIndex) {
    return Paths[srcDst.first][srcDst.second][pathIndex];
}

int Graph::getPrice(int edgeIndex) {
    pair<int, int> srcDst = findSrcDst(edgeIndex);
    return BandwidthPrice[srcDst.first][srcDst.second];
}

int Graph::pathCapacityEdgeIndex(pair<int, int> srcDst, int pathIndex) {
    int min = INT_MAX, src = 0, dst = 0;
    int path_size = Paths[srcDst.first][srcDst.second].size();
    if(pathIndex >= path_size) return -1;
    vector<int> path(Paths[srcDst.first][srcDst.second][pathIndex]);
    for(int i = 0; i < path.size() - 1; i++){
        if(Bandwidth[path[i]][path[i + 1]] < min){
            min = BandwidthLim[path[i]][path[i + 1]];
			src = path[i];
			dst = path[i + 1];
        }
    }
    int index = getEdgeIndex(pair<int, int>(src, dst));
    return index;
}

void Graph::outputGraph() {
    ofstream out(GraphPathOut);
    out << VertexNum << " " << EdgeNum << endl;
    for(int i = 0; i < VertexNum; i++ ){
        for(int j = i + 1; j < VertexNum; j++){
            out << i << " " << j << " " << BandwidthPrice[i][j] << " "<< BandwidthLim[i][j] << endl;
        }
    }
    out.close();
}
