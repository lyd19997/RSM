#pragma once
//
// Created by Zhixing Zhang on 2018/4/17.
//
#include <cmath>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include "const.h"
#define RANDNUM 6
using namespace std;



class Graph {
public:
	int VertexNum;
	int EdgeNum;
	const int Prices[RANDNUM] = { 50, 50, 50, 50, 50, 50 };//{ 4, 6, 14, 28, 68, 85};
	const int Capacity[RANDNUM] = { 50, 50, 50, 50, 50, 50 };
	bool G[VERTEXSIZE][VERTEXSIZE];
	double Bandwidth[VERTEXSIZE][VERTEXSIZE];
	double BandwidthLim[VERTEXSIZE][VERTEXSIZE];//每条边的最大容量限定
	int BandwidthPrice[VERTEXSIZE][VERTEXSIZE];

	int pair2EIndex[VERTEXSIZE][VERTEXSIZE];

	void pair2EIndex_init();

	int getEdgeIndex(pair<int, int> srcDst);//得到的是有向边的编号

	vector<vector<int>>Paths[VERTEXSIZE][VERTEXSIZE];

	Graph(const char * filename);//从文件中生成图
	Graph(int vertexNum, int edgeNum);//生成无向联通图

	void initBandwidthLim(const char * filename);//从文件中初始化最大带宽限定


	void findPaths();
	void findPath(int start, int terminate, bool visited[]);


	bool linkInPath(int e_src, int e_dst, int src, int dst, int pathIndex);// 确定边（e_src,e_dst）是否在(src->dst)的第pathIndex条路径上

	int getVertexNum();
	int getEdgeNum();

	void printGraph();
	void printPaths();

	int pathSize(pair<int, int>srcDst);//---
	double pathCapacity(int src, int dst, int pathIndex);//确定（src->dst）的第pathIndex条路径的容量

	double linkCapacity(int egeIndex);//--
	double pathCapacity(pair<int, int>srcDst, int pathIndex);//--


    pair<int, int>findSrcDst(int edgeIndex); //由边的编号映射到点
    //--------4.21---------
    bool linkInPath(int edgeIndex, pair<int, int>srcDst, int pathIndex);//---
    vector<int> getPath(pair<int, int>srcDst, int pathIndex);//--
    int getPrice(int edgeIndex);
    int pathCapacityEdgeIndex(pair<int, int>srcDst, int pathIndex);
    void sortByLength();
    void outputGraph();
    //output file
    //capacity
    /*1.根据路径长度升序排序
    2.  5个接口重新封装
    3.输出到文件中 。路径在const.h 里
    4.产生容量
    5.结果输出Res类里
    */
};



