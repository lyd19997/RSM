#include"const.h"
//#include"BL_RSM.h"
#include"RL_RSM.h"
#include "FlowBase.h"
#include "MinCost.h"

int main() {
	RequestList requests(VERTEXSIZE, "RequestList1.txt");
//	Graph topo(VERTEXSIZE, EDGESIZE);
//	Blrsm alg(topo, requests);
//	alg.TAA();
    printf("MAA\n");
	MAA maa(VERTEXSIZE, EDGESIZE, PEROID, requests);
	printf("flowbase\n");
    FlowBase fb(VERTEXSIZE, EDGESIZE, PEROID, requests);
    printf("min cost\n");
    MinCost mc(VERTEXSIZE, EDGESIZE, PEROID, requests);
	//...
	return 0;
}