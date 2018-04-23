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
#define PEROID 50
#define MaxTime 50
//-------request---------
#define MIN_DURATION 10
#define LAMBDA 5
#define MEAN_TRANSFER_SIZE 5
//--------topo---------
#define VERTEXSIZE 19
#define EDGESIZE 18
#define MaxVertexNum 30
#define MaxBandwidth 3000
#define MaxBandwidthPrice 10
#define BANDWIDTH_CAPACITY 30
//-------path--------------
const std::string Path = ".\\data\\";

const std::string GraphPath = Path + "Graph\\";
const std::string RequestPath = Path + "Request\\";
const std::string AlgorithmPath = Path + "Algorithm\\";

const std::string GraphInPath = GraphPath + "Topo_node_" + std::to_string(VERTEXSIZE) + ".txt";
const std::string GraphOutPath = GraphInPath;

const std::string RequestPathIn = RequestPath
+ "Topo_node_" + std::to_string(VERTEXSIZE)
+ "_Peroid_" + std::to_string(PEROID)
+ "_lambda_" + std::to_string(LAMBDA) + "_Duration_" + std::to_string(MIN_DURATION) + std::to_string(MEAN_TRANSFER_SIZE) + "_TransferIn.txt";
const std::string RequestPathOut = RequestPath;
												

const std::string OutPath = AlgorithmPath
+ "\\Topo_node_" + std::to_string(VERTEXSIZE)
+ "_Peroid_" + std::to_string(PEROID)
+ "_lambda_" + std::to_string(LAMBDA) + "_Duration_" + std::to_string(MIN_DURATION) + "_RequestSize_" + std::to_string(MEAN_TRANSFER_SIZE) + "_";
