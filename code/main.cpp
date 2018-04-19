#include"const.h"
#include"BL_RSM.h"
#include"RL_RSM.h"
#include "FlowBase.h"
#include "MinCost.h"


#define DEBUG

int main() {
#ifdef DEBUG
    freopen("result.txt", "w", stdout);
#endif
	RequestList requests(VERTEXSIZE);
	Graph topo(VERTEXSIZE, EDGESIZE);
	Blrsm alg(topo, requests);
	alg.TAA();
    printf("MAA\n");
//	MAA maa(VERTEXSIZE, EDGESIZE, PEROID, requests);
	MAA maa(topo, requests);
	printf("flowbase\n");
	FlowBase fb(topo, requests);
//    FlowBase fb(VERTEXSIZE, EDGESIZE, PEROID, requests);
    printf("min cost\n");
    MinCost mc(topo, requests);
//    MinCost mc(VERTEXSIZE, EDGESIZE, PEROID, requests);
	//...
	return 0;
}