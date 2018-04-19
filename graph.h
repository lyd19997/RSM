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
	int BandwidthLim[MaxVertexNum][MaxVertexNum];//û���ߵ���������޶�
	int BandwidthPrice[MaxVertexNum][MaxVertexNum];

	vector<vector<int>> Paths[MaxVertexNum][MaxVertexNum];

	Graph(const char * filename);//���ļ�������ͼ
	Graph(int vertexNum, int edgeNum);//����������ͨͼ

	void initBandwidthLim(const char * filename);//���ļ��г�ʼ���������޶�


	void findPaths();
	void findPath(int start, int terminate, bool visited[]);

	bool linkInPath(int edgeIndex, pair<int, int>srcDst, int pathIndex);//---
	bool linkInPath(int e_src, int e_dst, int src, int dst, int pathIndex);// ȷ���ߣ�e_src,e_dst���Ƿ���(src->dst)�ĵ�pathIndex��·����

	int getVertexNum();
	int getEdgeNum();

	void printGraph();
	void printPaths();

	int pathSize(pair<int, int>srcDst);//---
	int pathCapacity(int src, int dst, int pathIndex);//ȷ����src->dst���ĵ�pathIndex��·��������

	int linkCapacity(int degeIndex);//--
	int pathCapacity(pair<int, int>srcDst, int pathIndex);//--
};



