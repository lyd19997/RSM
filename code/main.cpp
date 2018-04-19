#include"const.h"
#include"BL_RSM.h"
#include"RL_RSM.h"
#include "FlowBase.h"
#include"graph.h"
#include"requestList.h"


int main() {
	RequestList requests(VERTEXSIZE);
	//RequestList requests(RequestPathIn);
	Graph topo(VERTEXSIZE, EDGESIZE);
	Graph topo(GraphPathIn);
	Blrsm alg(topo, requests);
	alg.TAA();
	MAA maa(VERTEXSIZE, EDGESIZE, PEROID, requests);
    FlowBase fb(VERTEXSIZE, EDGESIZE, PEROID, requests);
	//...
	return 0;
}