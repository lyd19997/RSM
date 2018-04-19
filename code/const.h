#pragma once
#include<fstream>
#include<map>
#include<queue>
#include<functional>
#include<time.h>
#include<vector>

#define EPS 1e-5
#define PEROID 50
#define MIN_DURATION 1
#define LAMBDA 10
#define VERTEXSIZE 19
#define EDGESIZE 14
#define MEAN_TRANSFER_SIZE 5
#define DELTA0(x) (x-(1+x)*log(1+x))
#define SCALING(x) ((x)*exp(-1 * (x))) 
const std::string  RequestPathOut = "";
