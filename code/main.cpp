#include"const.h"
#include"BL_RSM.h"
#include"RL_RSM.h"
#include "FlowBase.h"
#include "MinCost.h"
#include "ValueFirst.h"
#include"Amoeba.h"
#include"RSM_greedy.h"
#include "SRM.h"
#include <time.h>


int main() {

	srand((unsigned)time(0));
	RequestList requests(VERTEXSIZE);
	Graph topo(VERTEXSIZE, EDGESIZE);
<<<<<<< HEAD

	//RequestList requests(RequestPathOut);
	//Graph topo(GraphPathIn.c_str());
=======
//	Graph topo("data/Graph/Topo_node_10.txt");
//	RequestList requests(RequestPathOut);
	//Graph topo(GraphPathIn.c_str());

	SRM srm(topo, requests);
	srm.opt.outResult();
>>>>>>> c6e76643d6eef0992a547659daeb1b1fa1baef58
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
<<<<<<< HEAD
	Blrsm bl(topo, requests);  bl.schedule(); bl.res.outResult();
	Amoeba am(topo, requests);   am.schedule(); am.res.outResult();
	Blrsm opt(topo, requests); opt.optimal(); opt.res.outResult();
	ValueFirst vf(topo, requests); vf.schedule(); vf.res.outResult();

=======
//	//opt
//	Blrsm alg(topo, requests);
//	//Amoeba alg(topo, requests);
//	//ValueFirst alg(topo, requests);
//
//
//	//----------------------
//	//opt
//	//alg
//	//RsmGreedy alg(topo, requests);
//	alg.schedule();
//	alg.res.outResult();
>>>>>>> c6e76643d6eef0992a547659daeb1b1fa1baef58

	
	return 0;
}