#include"BL_RSM.h"
#include"const.h"
#include<math.h>
#define DELTA0(x) (x-(1+x)*log(1+x))
#define SCALING(x) ((x)*exp(-1 * (x))) 

Blrsm::Blrsm(Graph &topo, RequestList &requests) :topo(topo), requests(requests), res(topo, requests) {// scaling,delta 1-k,
	delta.push_back(0);
	//cout << requests.rateMax() << "  " << exp(-1) << endl;//debug
	for (int i = 0; i < topo.getEdgeNum(); ++i)
	{
		int cnt = 100;
		double left = 0, right = 1;
		double mid = (left + right) / 2;
		//cout << powl(PEROID*(topo.getEdgeNum() + 1), -1 / (double)topo.linkCapacity(i)) <<"   "<< pow(PEROID*(topo.getEdgeNum() + 1), -1 / (topo.linkCapacity(i)*1.0)) << endl;//debug
		double bound = exp(-1)*powl(PEROID*(topo.getEdgeNum() + 1), -1 / (double)topo.linkCapacity(i));//精度
		while ((bound - SCALING(mid) > EPS || bound < SCALING(mid)) && !(cnt--<0 && bound>SCALING(mid))) {
			if (bound < SCALING(mid))
				right = mid;
			else
				left = mid;
			mid = (right + left) / 2;
			//cout << bound <<"   "<<SCALING(mid) << endl;//debug
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
			//cout << j << "  " << topo.pathSize(requests[i].getSrcDst()) << endl;//debug
			//cout << topo.pathCapacityEdgeIndex(requests[i].getSrcDst(), j) << endl; //返回-1？
			Pr_ij.back()[j] = x_ij[i][j] * scaling[topo.pathCapacityEdgeIndex(requests[i].getSrcDst(), j)];//...pathCapacityEdgeIndex
			Fs += Pr_ij.back()[j] * requests[i].value;
		}
	}
	 
	//-- init delta 0 
	int cnt = 100;
	double left = 0, right = 1;
	double mid = (left + right) / 2;
	double bound = -1 * log(topo.getEdgeNum() + 1) / Fs;//精度
	cout << DELTA0(1) << endl;//debug
	while ((bound - DELTA0(mid) > EPS || bound < DELTA0(mid)) && !(cnt--<0 && bound>DELTA0(mid))) {
		if (bound < DELTA0(mid))
			left = mid;
		else
			right = mid;
		mid = (right + left) / 2;
		//cout << cnt << endl;//debug
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
			if (PrMin > PrUpperBound(i, resX, j))
			{
				PrMin = PrUpperBound(i, resX, j);
				pathMinPr = j;
			}
		}
		if (pathMinPr == -2) { cout << i << " Wa pathMinPr == -2" << endl; while (1); }//debug
		resX[i] = pathMinPr;
		cout << i << " : " << resX[i] << "  -  " << PrMin << endl;
	}
	return resX;
}

double Blrsm::PrUpperBound(int deep, const vector<int> &resX, int branch) {
	double res_ = 0;
	double Fbound = Fs*(1 - delta[0]);
	double gotValue = 0;
	for (int i = 0; i < deep; ++i)           //        < deep 
		gotValue += requests[i].value*(resX[i] < 0 ? 0 : 1);
	if (branch >= 0)             // == deep 
		gotValue += requests[deep].value;

	double prod = 1;
	for (int i = deep+1; i < requests.size(); ++i) //       >deep
	{
		double sumPr = 0;
		for (int j = 0; j < topo.pathSize(requests[i].getSrcDst()); ++j)
			sumPr += Pr_ij[i][j];
		prod *= sumPr*powl(1 + delta[0], -1 * requests[i].value) + 1 - sumPr;
	}
	res_ += powl(1 + delta[0], Fbound - gotValue)*prod;
	cout << res_ << endl;//debug
	//---------------------------------------------------------
	for (int k = 0; k < PEROID*topo.getEdgeNum(); ++k)//[T][N]
	{
		double sumCapacity = 0;
		for (int i = 0; i < deep; ++i)
			sumCapacity += requests[i].rate*((k / topo.getEdgeNum()<requests[i].start || k / topo.getEdgeNum()>requests[i].end) ? 0 : 1)
				*(resX[i] < 0 ? 0 : 1)
				*topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), resX[i]);
		
		prod = 1;
		for (int i = deep; i < requests.size(); ++i)
		{
			double sumDelta = 1;
			for (int j = 0; j < topo.pathSize(requests[i].getSrcDst()); ++j)
				if ((k / topo.getEdgeNum() >= requests[i].start&&k / topo.getEdgeNum() <= requests[i].end) && topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), j))
				sumDelta += Pr_ij[i][j] * powl(1 + delta[k%topo.getEdgeNum()], requests[i].rate) - Pr_ij[i][j];
			prod *= sumDelta;
		}
		res_ += powl(1 + delta[k%topo.getEdgeNum()], sumCapacity - topo.linkCapacity(k%topo.getEdgeNum()))*prod;
		cout << res_<<" "<< prod << " " << powl(1 + delta[k%topo.getEdgeNum()], sumCapacity - topo.linkCapacity(k%topo.getEdgeNum())) << endl;//debug
		if (res_ > 1)
			cout << k <<" ?? "<< k%topo.getEdgeNum() << endl;
	}

	return res_;
}

vector<vector<double> > Blrsm::relaxation_LP() {
	vector<vector<double> > res; res.clear();
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
		for (int t = 0; t < PEROID; ++t)
		{
			for (int e = 0; e < topo.getEdgeNum(); ++e)
			{
				//cout << e << "  " << topo.findSrcDst(e).first << "--" << topo.findSrcDst(e).second << endl;
				GRBLinExpr lhsExpr = 0;
				for (int i = 0; i < requests.size(); ++i)
				{
					for (int j = addr[i]; j < addr[i + 1]; ++j)
					{
						//if (topo.findSrcDst(e) == pair<int, int>(0, 6) || topo.findSrcDst(e) == pair<int, int>(6, 0))
						//	cout << e << "  " << requests[i].src << "  " << requests[i].dst << "  " << j - addr[i] << endl;
						if (!(t < requests[i].start || t > requests[i].end) && topo.linkInPath(e, requests[i].getSrcDst(), j - addr[i]))
						{
							//cout << "in" << endl;
							lhsExpr += requests[i].rate*xReqPath[j];
						}
					}
				}
				//cout << topo.linkCapacity(k%topo.getEdgeNum()) << std::endl;//debug
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
				res.push_back(vector<double>());
				for (int j = addr[i]; j < addr[i + 1]; ++j)
					res.back().push_back(xReqPath[j].get(GRB_DoubleAttr_X));
			}
		}
	}
	catch (...) {
		
	}
	if (res.empty() && requests.size()) {
		cout << "LP error" << endl;
		while (1);
	}
	return res;
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