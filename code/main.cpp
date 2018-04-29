#include"const.h"
#include"BL_RSM.h"
#include"RL_RSM.h"
#include "FlowBase.h"
#include "MinCost.h"
#include "ValueFirst.h"
#include"Amoeba.h"
#include"RSM_greedy.h"
#include "SRM.h"
#include "SRMNEW.h"
#include <time.h>


int main() {

	srand((unsigned)time(0));
	//RequestList requests(VERTEXSIZE);
	//Graph topo(VERTEXSIZE, EDGESIZE);
	RequestList requests(RequestPathIn);
	Graph topo(GraphPathIn.c_str());
	//SRMNEW srmnew(topo, requests);
	//srmnew.result.outResult();
//	Graph topo("data/Graph/Topo_node_4.txt");
//	ofstream out("data/TopoPrices.txt");
//	for(int i = 0; i < topo.getEdgeNum(); i++){
//	    out << i << " " << topo.getPrice(i) << endl;
//	}
//	RequestList requests(RequestPathOut);
	//Graph topo(GraphPathIn.c_str());

//	SRM srm(topo, requests);
//	srm.opt.outResult();
//	srm.result.outResult();
//	cout << srm.result.income - srm.result.cost << endl;
//	cout << srm.opt.income - srm.opt.cost << endl;
	//-------RL-SRM---------
	//opt
//	MAA alg(topo, requests);
//	alg.result.outResult();
//	alg.opt.outResult();
//	FlowBase fb(topo, requests);
//	fb.result.outResult();
//	MinCost mc(topo, requests);
//	mc.result.outResult();
//	mc.printResult();
//    topo.printGraph();
//    topo.printPaths();
//
//    ValueFirst vf(topo, requests);
//    vf.result.outResult();


	//-------BL-RSM---------
//	//opt
//	Blrsm alg(topo, requests);
//	//Amoeba alg(topo, requests);
//	//ValueFirst alg(topo, requests);
//
//
//	//----------------------
//	//opt
//	//alg
	Blrsm alg(topo, requests);
	alg.schedule();
	alg.res.outResult();
	cout << alg.res.income - alg.res.cost << endl;

	
	return 0;
}