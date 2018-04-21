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

using namespace std;



class Graph {
public:
	int VertexNum;
	int EdgeNum;

	bool G[MaxVertexNum][MaxVertexNum];
	double Bandwidth[MaxVertexNum][MaxVertexNum];
	int BandwidthLim[MaxVertexNum][MaxVertexNum];//ÿ���ߵ���������޶�
	int BandwidthPrice[MaxVertexNum][MaxVertexNum];

	int pair2EIndex[VERTEXSIZE][VERTEXSIZE];

	void pair2EIndex_init();

	int getEdgeIndex(pair<int, int> srcDst);//�õ���������ߵı��

	vector<vector<int>>Paths[MaxVertexNum][MaxVertexNum];

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

	int linkCapacity(int egeIndex);//--
	int pathCapacity(pair<int, int>srcDst, int pathIndex);//--


    pair<int, int>findSrcDst(int edgeIndex); //�ɱߵı��ӳ�䵽��
    //--------4.21---------
	bool linkInPath(int edgeIndex, pair<int, int>srcDst, int pathIndex);
    vector<int> getPath(pair<int, int>srcDst, int pathIndex);//--
    int getPrice(int edgeIndex);
    int pathCapacityEdgeIndex(pair<int, int>srcDst, int pathIndex);
    void sortByLength();
    //output file
    //capacity
    /*1.����·��������������
    2.  5���ӿ����·�װ
    3.������ļ��� ��·����const.h ��
    4.��������
    5.������Res����
    */
};



