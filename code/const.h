#pragma once
#include<fstream>
#include<map>
#include<queue>
#include<functional>
#include<time.h>
#include<vector>
#include<iostream>
#include<time.h>
#include<string>
#include "algorithm"

//------const--------
#define EPS 1e-5
#define INF 0xfffffff
//-------ISP--------
#define PEROID 10
#define MaxTime 10
//-------request---------
#define MIN_DURATION 2
#define LAMBDA 50
#define MEAN_TRANSFER_SIZE 20
#define MEAN_VALUE 1
//--------topo---------
#define VERTEXSIZE 4
#define EDGESIZE 5
#define MaxVertexNum 30
#define MaxBandwidth 3000
#define MaxBandwidthPrice 10
#define BANDWIDTH_CAPACITY 30
//-------path--------------
const std::string Path = "C:\\data\\";

const std::string GraphPath = Path + "Graph\\";
const std::string RequestPath = Path + "Request\\";
const std::string AlgorithmPath = Path + "Algorithm\\";

const std::string GraphPathIn = GraphPath + "Topo_node_" + std::to_string(VERTEXSIZE) + ".txt";
const std::string GraphPathOut = GraphPathIn;

const std::string RequestPathIn = RequestPath
+ "Topo_node_" + std::to_string(VERTEXSIZE)
+ "_Peroid_" + std::to_string(PEROID)
+ "_lambda_" + std::to_string(LAMBDA) + "_Duration_" + std::to_string(MIN_DURATION) +"_Meansize"+ std::to_string(MEAN_TRANSFER_SIZE) + "_RequestIn.txt";
const std::string RequestPathOut = RequestPathIn;

const std::string OutPath = "_Topo_node_" + std::to_string(VERTEXSIZE)
+ "_Peroid_" + std::to_string(PEROID)
+ "_lambda_" + std::to_string(LAMBDA) + "_Duration_" + std::to_string(MIN_DURATION) + "_RequestSize_" + std::to_string(MEAN_TRANSFER_SIZE) + "_";
