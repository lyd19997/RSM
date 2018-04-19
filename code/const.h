#pragma once
#include<fstream>
#include<map>
#include<queue>
#include<functional>
#include<time.h>
#include<vector>
#include<iostream>

#define EPS 1e-5
//==============
#define PEROID 50
#define MaxTime 50
//=============
#define MIN_DURATION 1
#define LAMBDA 10
#define VERTEXSIZE 19
#define EDGESIZE 18
#define MEAN_TRANSFER_SIZE 5
#define MaxVertexNum 30
#define MaxBandwidth 3000
#define MaxBandwidthPrice 10

#define BANDWIDTH_CAPACITY 30
const std::string Path = ".\\data\\";
const std::string GraphPath = Path + "Graph\\";
const std::string RequestPath = Path + "Transfer\\";

//const std::string GraphPathIn = GraphPath + "Google_topo.txt";
const std::string GraphInPath = GraphPath + "Topo_node_" + std::to_string(VERTEXSIZE) + ".txt";
const std::string GraphOutPath = GraphInPath;//GraphPath + "Topo_node_" + std::to_string(NODE_NUM) + ".txt";

const std::string RequestPathIn = RequestPath + "Topo_node_" + std::to_string(VERTEXSIZE) + "_lambda_" + std::to_string(LAMBDA) +
"_Peroid_" + std::to_string(PEROID) + "_Duration_" + std::to_string(MIN_DURATION) + "_TransferIn.txt";
const std::string RequestPathOut = RequestPath;//TrafficPath + "Topo_node_" + std::to_string(NODE_NUM) + "_lambda_" + std::to_string(LAMBDA) + "_TransferIn.txt";
												 //-------------out----------------------
const std::string OutTrasferPath = Path /*+"_offline"*/ + "\\" + "Topo_node_" + std::to_string(VERTEXSIZE) + "_lambda_" + std::to_string(LAMBDA) +
"_Peroid_" + std::to_string(PEROID);
const std::string OutLinkPath = OutTrasferPath;//Path + Algorithm_Type + "\\";
