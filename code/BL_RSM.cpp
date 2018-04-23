#include"BL_RSM.h"
#include"const.h"
#include<math.h>
#define DELTA0(x) (x-(1+x)*log(1+x))
#define SCALING(x) ((x)*exp(-1 * (x))) 

Blrsm::Blrsm(Graph &topo, RequestList &requests) :topo(topo), requests(requests), res(topo, requests) {// scaling,delta 1-k,
	delta.push_back(0);
	for (int i = 0; i < topo.getEdgeNum(); ++i)
	{
		int cnt = 100;
		double left = 0, right = 1;
		double mid = (left + right) / 2;
		double bound = exp(-1)*pow(PEROID*(topo.getEdgeNum() + 1), -1 / topo.linkCapacity(i));//����
		while ((bound - SCALING(mid) > EPS || bound < SCALING(mid)) && !(cnt--<0 && bound>SCALING(mid))) {
			if (bound < SCALING(mid))
				right = mid;
			else
				left = mid;
			mid = (right + left) / 2;
		}
		scaling.push_back(mid);
		delta.push_back((1 - mid) / mid);
	}
}

void Blrsm::schedule() {
	passPathIndex = TAA();
	outRes();
}

vector<int> Blrsm::TAA() {
	vector<int> resX(requests.size(),-1);
	vector<vector<double> > x_ij = relaxation_LP();

	for (int i = 0; i < x_ij.size(); ++i)                      //init Pr 
	{
		Pr_ij.push_back(vector<double>(x_ij[i].size(), 0));
		for (int j = 0; j < x_ij[i].size(); ++j) 
		{
			Pr_ij.back()[j] = x_ij[i][j] * scaling[topo.pathCapacityEdgeIndex(requests[i].getSrcDst(), j)];//...pathCapacityEdgeIndex
			Fs += Pr_ij.back()[j] * requests[i].value;
		}
	}
	 
	//-- init delta 0 
	int cnt = 100;
	double left = 0, right = 1;
	double mid = (left + right) / 2;
	double bound = -1 * log(topo.getEdgeNum() + 1) / Fs;//����
	while ((bound - DELTA0(mid) > EPS || bound < DELTA0(mid)) && !(cnt--<0 && bound>DELTA0(mid))) {
		if (bound < DELTA0(mid))
			left = mid;
		else
			right = mid;
		mid = (right + left) / 2;
	}
	delta[0] = mid;
	//--
	for (int i = 0; i < requests.size(); ++i)
	{
		double PrMin = 1; // PrMin > 1
		int pathMinPr = -2;
		for (int j = -1; j < topo.pathSize(requests[i].getSrcDst()); ++j) // j == -1 
		{
			if (j >= 0 && x_ij[i][j] == 0) continue;
			double upperBound = PrUpperBound(i, resX, j);
			if (PrMin > upperBound&&upperBound > 0)
			{
				PrMin = upperBound;
				pathMinPr = j;
			}
		}
		if (pathMinPr == -2) { cout << i << "  Wa pathMinPr == -2" << endl; while (1); }//debug
		resX[i] = pathMinPr;
	}
	return resX;
}

double Blrsm::PrUpperBound(int deep, const vector<int> &resX, int branch) {
	double res = 0;
	double Fbound = Fs*(1 - delta[0]);
	double gotValue = 0;
	for (int i = 0; i < deep; ++i)
		gotValue += requests[i].value*(resX[i] < 0 ? 0 : 1);
	if (branch >= 0)
		gotValue += requests[deep].value;

	double prod = 1;
	for (int i = deep; i < requests.size(); ++i) 
	{
		double sumPr = 0;
		for (int j = 0; j < topo.pathSize(requests[i].getSrcDst()); ++j)
			sumPr += Pr_ij[i][j];
		prod *= sumPr*powl(1 + delta[0], -1 * requests[i].value) + 1 - sumPr;
	}
	res += powl(1 + delta[0], Fbound - gotValue)*prod;
	//---------------------------------------------------------
	for (int k = 0; k < PEROID*topo.getEdgeNum(); ++k)//[T][N]
	{
		double sumCapacity = 0;
		for (int i = 0; i < deep; ++i)
			if ((k / topo.getEdgeNum() <= requests[i].end&&k / topo.getEdgeNum() >= requests[i].start) && resX[i] >= 0 && topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), resX[i]))
				sumCapacity += requests[i].rate;

		prod = 1;
		for (int i = deep; i < requests.size(); ++i)
		{
			double sumDelta = 1;
			for (int j = 0; j < topo.pathSize(requests[i].getSrcDst()); ++j)
				if (k / topo.getEdgeNum() >= requests[i].start&&k / topo.getEdgeNum() <= requests[i].end&&topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), j))
					sumDelta += Pr_ij[i][j] * powl(1 + delta[k%topo.getEdgeNum()], requests[i].rate) - Pr_ij[i][j];
			prod *= sumDelta;
		}
		res += powl(1 + delta[k%topo.getEdgeNum()], sumCapacity - topo.linkCapacity(k%topo.getEdgeNum()))*prod;
	}

	return res;
}

vector<vector<double> > Blrsm::relaxation_LP() {
	vector<vector<double> > res_; res_.clear();
	vector<int> addr(1, 0);
	for (vector<Request>::iterator it = requests.begin(); it != requests.end(); ++it)
		addr.push_back(addr.back() + topo.pathSize(it->getSrcDst()));
	try {
		GRBEnv env = GRBEnv();
		GRBModel model = GRBModel(env);
		GRBVar *xReqPath = model.addVars(addr.back(), GRB_CONTINUOUS);
		for (int i = 0; i < requests.size(); ++i)
		{
			GRBLinExpr lhsExpr = 0;
			for (int j = addr[i]; j < addr[i + 1]; ++j)
				lhsExpr += xReqPath[j];
			model.addConstr(lhsExpr, GRB_LESS_EQUAL, 1); // constrain 1
		}
		//for (int k = 0; k < PEROID*topo.getEdgeNum(); ++k)//[T][E]
		//{
		//	GRBLinExpr lhsExpr = 0;
		//	for (int i = 0; i < requests.size(); ++i)
		//	{
		//		for (int j = addr[i]; j < addr[i + 1]; ++j)
		//			if ((k / topo.getEdgeNum() >= requests[i].start && k / topo.getEdgeNum() <= requests[i].end) && topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), j))
		//				//cout <<"linkInpath  "<< topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), j) << endl;//debug
		//				lhsExpr += requests[i].rate*xReqPath[j];
		//	}
		//	cout << topo.linkCapacity(k%topo.getEdgeNum()) << std::endl;//debug
		//	model.addConstr(lhsExpr, GRB_LESS_EQUAL, topo.linkCapacity(k%topo.getEdgeNum())); // constrain 2
		//}
		for (int t = 0; t < PEROID; ++t)
		{
			for (int e = 0; e < topo.getEdgeNum(); ++e)
			{
				GRBLinExpr lhsExpr = 0;
				for (int i = 0; i < requests.size(); ++i)
				{
					for (int j = addr[i]; j < addr[i + 1]; ++j)
					{
						//debug
						//cout <<i<<"   "<< j-addr[i] << "   " << topo.pathSize(requests[i].getSrcDst()) << endl;
						//vector<int> edgeList = topo.getPath(requests[i].getSrcDst(), j - addr[i]);
						//cout << edgeList.size() << "  " << topo.findSrcDst(e).first << "-" << topo.findSrcDst(e).second << endl;
						//--
						if (t <= requests[i].end&&t >= requests[i].start&&topo.linkInPath(e, requests[i].getSrcDst(), j - addr[i]))
							lhsExpr += xReqPath[j] * requests[i].rate;
					}
				}
				model.addConstr(lhsExpr, GRB_LESS_EQUAL, topo.linkCapacity(e)); // constrain 2
			}
		}
		GRBLinExpr obj = 0;
		for (int i = 0; i < requests.size(); ++i)
		{
			for (int j = addr[i]; j < addr[i + 1]; ++j)
				obj += requests[i].value*xReqPath[j];
		}
		model.setObjective(obj, GRB_MAXIMIZE);
		model.optimize();
		if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL)
		{
			for (int i = 0; i < requests.size(); ++i)
			{
				res_.push_back(vector<double>());
				for (int j = addr[i]; j < addr[i + 1]; ++j)
					res_.back().push_back(xReqPath[j].get(GRB_DoubleAttr_X));
			}
		}
	}
	catch (...) {
		
	}
	if (res_.empty() && requests.size()) {
		cout << "LP error" << endl;
		while (1);
	}
	return res_;
}

void Blrsm::outRes() {
	res.algName = "BL-SRM";
	res.cost = 0;
	res.income = 0;
	res.receiveNum = 0;
	for (int i = 0; i < requests.size(); ++i)
		res.income += (passPathIndex[i] == -1 ? 0 : requests[i].value), res.receiveNum += 1;
	res.passPathIndex = passPathIndex;
	for (int e = 0; e < topo.getEdgeNum(); ++e)
		res.peakPerEdge[e] = topo.linkCapacity(e);
	res.requestNum = requests.size();
	res.getRunTime();
	for (vector<Request>::iterator it = requests.begin(); it != requests.end(); ++it)
	{
		for (vector<int>::iterator ite = topo.getPath(it->getSrcDst(), passPathIndex[it - requests.begin()]).begin(); ite != topo.getPath(it->getSrcDst(), passPathIndex[it - requests.begin()]).end(); ++ite)
		{
			for (int t = it->start; t <= it->end; ++t)
				res.volPerTimeEdge[t][*ite] += it->rate;
		}
	}
}