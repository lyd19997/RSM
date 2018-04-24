#include"const.h"
#include"BL_RSM.h"
#include"RL_RSM.h"
#include "FlowBase.h"
#include "MinCost.h"
#include "ValueFirst.h"
#include"Amoeba.h"
#include"RSM_greedy.h"
#include <time.h>


int main() {

	srand((unsigned)time(0));
	//RequestList requests(VERTEXSIZE);
	//Graph topo(VERTEXSIZE, EDGESIZE);

	RequestList requests(RequestPathOut);
	Graph topo(GraphPathIn.c_str());
	//-------RL-SRM---------
	//opt
	//MAA alg(topo, requests);
	//FlowBase alg(topo, requests);
	//MinCost alg(topo, requests);


	//-------BL-RSM---------
	//opt
	Blrsm alg(topo, requests);  
	//Amoeba alg(topo, requests);   
	//ValueFirst alg(topo, requests);


	//----------------------
	//opt
	//alg
	//RsmGreedy alg(topo, requests);  
	alg.schedule();
	alg.res.outResult();
	//alg.result.outResult();
	
	return 0;
}