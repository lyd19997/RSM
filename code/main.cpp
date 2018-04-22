#include"const.h"
#include"BL_RSM.h"
#include"RL_RSM.h"
#include "FlowBase.h"
#include "MinCost.h"
#include "ValueFirst.h"
#include <time.h>


int main() {

	srand((unsigned)time(0));
	RequestList requests(VERTEXSIZE);
	Graph topo(VERTEXSIZE, EDGESIZE);
//	Blrsm alg(topo, requests);
//	alg.TAA();
    printf("MAA\n");
	// MAA maa(VERTEXSIZE, EDGESIZE, PEROID, requests);
	MAA maa(topo, requests);
	printf("flowbase\n");
//	FlowBase fb(topo, requests);
//   // FlowBase fb(VERTEXSIZE, EDGESIZE, PEROID, requests);
//    printf("min cost\n");
//    MinCost mc(topo, requests);
////    MinCost mc(VERTEXSIZE, EDGESIZE, PEROID, requests);
//	//...
//	printf("value first\n");
//	ValueFirst vf(topo, requests);
	return 0;
}