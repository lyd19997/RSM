#pragma once
//
// Created by Zhixing Zhang on 2018/4/17.
//
#include <cmath>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#define MaxVertexNum 30
#define MaxBandwidthPrice 10
#define MaxBandwidth 30

class Graph {
public:
	int VertexNum;
	int EdgeNum;

	bool G[MaxVertexNum][MaxVertexNum];
	double Bandwidth[MaxVertexNum][MaxVertexNum];
	int BandwidthLim[MaxVertexNum][MaxVertexNum];//没条边的最大容量限定
	int BandwidthPrice[MaxVertexNum][MaxVertexNum];

	vector<vector<int>> Paths[MaxVertexNum][MaxVertexNum];

	Graph(const char * filename);//从文件中生成图
	Graph(int vertexNum, int edgeNum);//生成无向联通图

	void initBandwidthLim(const char * filename);//从文件中初始化最大带宽限定


	void findPaths();
	void findPath(int start, int terminate, bool visited[]);

	bool linkInPath(int edgeIndex, pair<int, int>srcDst, int pathIndex);//---
	bool linkInPath(int e_src, int e_dst, int src, int dst, int pathIndex);// 确定边（e_src,e_dst）是否在(src->dst)的第pathIndex条路径上

	int getVertexNum();
	int getEdgeNum();

	void printGraph();
	void printPaths();

	int pathSize(pair<int, int>srcDst);//---
	int pathCapacity(int src, int dst, int pathIndex);//确定（src->dst）的第pathIndex条路径的容量

	int linkCapacity(int degeIndex);//--
	int pathCapacity(pair<int, int>srcDst, int pathIndex);//--
};



