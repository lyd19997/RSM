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
	//Blrsm opt(topo, requests); opt.optimal(); opt.res.outResult();
	//Blrsm bl(topo, requests);  bl.schedule(); bl.res.outResult();
	Amoeba am(topo, requests);   am.schedule(); am.res.outResult();
	ValueFirst vf(topo, requests); vf.result.outResult();


	//----------------------
	
	return 0;
}