#include"BL_RSM.h"
#include"const.h"
#include<math.h>
#define DELTA0(x) (x-(1+x)*log(1+x))
#define SCALING(x) ((x)*exp(-1 * (x))) 

Blrsm::Blrsm(Graph topo_, RequestList requests_) :topo(topo_), requests(requests_), res(topo_, requests_), passPathIndex(requests_.size(), -1) {// scaling,delta 1-k,
	delta.push_back(0);
	//cout << requests.rateMax() << "  " << exp(-1) << endl;//debug
	for (int i = 0; i < requests.size(); ++i)
	{
		if (maxValue < requests[i].value) maxValue = requests[i].value;
		if (maxRate < requests[i].rate) maxRate = requests[i].rate;
	}
	gReq2One(-1);
	for (int i = 0; i < topo.getEdgeNum(); ++i)
	{
		int cnt = 100;
		double left = 0, right = 1;
		double mid = (left + right) / 2;
		//cout << powl(PEROID*(topo.getEdgeNum() + 1), -1 / (double)topo.linkCapacity(i)) <<"   "<< topo.linkCapacity(i) << endl;//debug
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


void Blrsm::gReq2One(int x) {
	for (int i = 0; i < requests.size(); ++i)
	{
		requests[i].value *= powl(maxValue, x);
		requests[i].rate *= powl(maxRate, x);
	}
	for (int i = 0; i < topo.getEdgeNum(); ++i)
		topo.BandwidthLim[topo.findSrcDst(i).first][topo.findSrcDst(i).second] *= powl(maxRate, x);
}

void Blrsm::schedule() {
	passPathIndex = TAA();
	gReq2One(1);
	outRes();
}

void Blrsm::optimal() {
	vector<vector<double> > passMultiPathIndex = relaxation_LP();
	gReq2One(1);
	outResOpt(passMultiPathIndex);
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
	double bound = -1 * log(topo.getEdgeNum() + 1) / Fs;//精度
	cout << DELTA0(1) << endl;//debug
	while ((bound - DELTA0(mid) > EPS || bound < DELTA0(mid)) && !(cnt--<0 && bound>DELTA0(mid))) {
		if (bound < DELTA0(mid))
			left = mid;
		else
			right = mid;
		mid = (right + left) / 2;
		//cout << cnt << endl;//debug
		if (cnt < -100)
			cout << "calc error in  delta0" << endl;
	}
	delta[0] = mid;

	for (int i = 0; i < requests.size(); ++i)
	{
		double minPr = 1;
		for (int j = topo.pathSize(requests[i].getSrcDst()) - 1; j >= -1; --j)
		{
			if (j >= 0 && !x_ij[i][j]) continue;
			//if (j >= 0 && x_ij[i][j] == 1)
			//{
			//	resX[i] = j;
			//	break;
			//}
			double tmpPr = PrUpperBound(i, resX, j);
			if (tmpPr < minPr)
			{
				minPr = tmpPr;
				resX[i] = j;
			}
		}
		cout << i << " : " << resX[i] << "   " << minPr << endl;
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
	//cout <</* branch<<"  "<<*/res_ << endl;
	//---------------------------------------------------------
	double debug = 0;
	for (int k = 0; k < PEROID*topo.getEdgeNum(); ++k)//[T][N]
	{
		double sumCapacity = 0;
		for (int i = 0; i < deep; ++i)          // < deep
			sumCapacity += requests[i].rate*((k / topo.getEdgeNum()<requests[i].start || k / topo.getEdgeNum()>requests[i].end) ? 0 : 1)
				*(resX[i] < 0 ? 0 : 1)
				*topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), resX[i]);
		
		sumCapacity += requests[deep].rate*((k / topo.getEdgeNum()<requests[deep].start || k / topo.getEdgeNum()>requests[deep].end) ? 0 : 1)  //   ==  deep
			*(branch < 0 ? 0 : 1)
			*topo.linkInPath(k%topo.getEdgeNum(), requests[deep].getSrcDst(), branch);


		prod = 1;
		for (int i = deep + 1; i < requests.size(); ++i)  //          >  deep
		{
			double sumDelta = 1;
			for (int j = 0; j < topo.pathSize(requests[i].getSrcDst()); ++j)
				if ((k / topo.getEdgeNum() >= requests[i].start&&k / topo.getEdgeNum() <= requests[i].end) && topo.linkInPath(k%topo.getEdgeNum(), requests[i].getSrcDst(), j))
				sumDelta += Pr_ij[i][j] * powl(1 + delta[k%topo.getEdgeNum()+1], requests[i].rate) - Pr_ij[i][j];
			prod *= sumDelta;
		}
		res_ += powl(1 + delta[k%topo.getEdgeNum()+1], sumCapacity - topo.linkCapacity(k%topo.getEdgeNum()))*prod;
		debug += powl(1 + delta[k%topo.getEdgeNum() + 1], sumCapacity - topo.linkCapacity(k%topo.getEdgeNum()))*prod;
		//cout << res_<<" "<< prod << " " << powl(1 + delta[k%topo.getEdgeNum()+1], sumCapacity - topo.linkCapacity(k%topo.getEdgeNum())) 
		//	<<" "<< sumCapacity <<"  "<< topo.linkCapacity(k%topo.getEdgeNum()) << endl;//debug
		/*if (res_ > 1)
			cout << k%topo.getEdgeNum() <<" ?? "<< topo.linkCapacity(k%topo.getEdgeNum()) << endl;*/
	}
	//cout << debug << "--"<< endl;
	//if (res_ > 1)
		cout <</* branch<<"  "<<*/res_ << endl;//debug
	return res_;
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
	res.getRunTime();
	res.cost = 0;
	res.income = 0;
	res.receiveNum = 0;
	for (int i = 0; i < requests.size(); ++i)
		if (passPathIndex[i] != -1)
			res.income += requests[i].value, res.receiveNum += 1;
	res.passPathIndex = passPathIndex;
	for (int e = 0; e < topo.getEdgeNum(); ++e)
		res.peakPerEdge[e] = topo.linkCapacity(e);
	res.requestNum = requests.size();
	
	for (int i = 0; i < requests.size(); ++i)
	{
		if (passPathIndex[i] == -1) continue;
		vector<int> edgeList = topo.getPath(requests[i].getSrcDst(), passPathIndex[i]);
		for (vector<int>::iterator ite = edgeList.begin(); ite != edgeList.end(); ++ite)
		{
			for (int t = requests[i].start; t <= requests[i].end; ++t)
				res.volPerTimeEdge[t][*ite] += requests[i].rate;
		}
	}
}

void Blrsm::outResOpt(vector<vector<double> > passMultiPathIndex) {
	res.algName = "BL-SRM-Optimal";
	res.cost = 0;
	res.income = 0;
	res.receiveNum = 0;
	for (int i = 0; i < requests.size(); ++i)
		for (int j = 0; j < topo.pathSize(requests[i].getSrcDst()); ++j)
			res.income += passMultiPathIndex[i][j] * requests[i].value, res.receiveNum += passMultiPathIndex[i][j];

	for (int e = 0; e < topo.getEdgeNum(); ++e)
		res.peakPerEdge[e] = topo.linkCapacity(e);
	res.requestNum = requests.size();
	res.getRunTime();

	res.passMultiPathindex = passMultiPathIndex;
	for (int i = 0; i < requests.size(); ++i)
	{
		for (int j = 0; j < passMultiPathIndex[i].size(); ++j)
		{
			if (passMultiPathIndex[i][j] == 0) continue;
			vector<int> edgeList = topo.getPath(requests[i].getSrcDst(), j);
			for (vector<int>::iterator ite = edgeList.begin(); ite != edgeList.end(); ++ite)
			{
				for (int t = requests[i].start; t <= requests[i].end; ++t)
					res.volPerTimeEdge[t][*ite] += requests[i].rate*passMultiPathIndex[i][j];
			}
		}
	}
}