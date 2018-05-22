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
		//cout << SCALING(0.5) << endl;
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
	linkInPathInit();
}



void Blrsm::linkInPathInit() {
    int edgeNum = topo.getEdgeNum();
    for(int i = 0; i < VERTEXSIZE; i++){
        for(int j = 0; j < VERTEXSIZE; j++){
            if (j == i) continue;
            vector<vector<bool>> temp_;
            int pathSize = topo.pathSize(pair<int, int>(i, j));
            for(int n = 0; n < pathSize; n++){
                vector<bool> temp;
                for(int e = 0; e < edgeNum; e++){
                    temp.push_back(topo.linkInPath(e, pair<int, int>(i, j), n));
                }
                temp_.push_back(temp);
            }
            linkInPath[i][j] = temp_;
        }
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
	//int cnt = 0;
	//for (int i = 0; i < passMultiPathIndex.size(); ++i)
	//	for (int j = 0; j < passMultiPathIndex[i].size(); ++j)
	//		if (passMultiPathIndex[i][j] == 1)
	//			++cnt;
	//cout << cnt << endl;
	//while (1);
}

vector<int> Blrsm::TAA() {
	vector<int> resX(requests.size(),-1);
	x_ij = relaxation_LP();

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
		{
			mid = 1.7182818284;
//			exit(1);
			break;
		}
//			cout << bound <<" error delta0 "<<DELTA0(1)<< endl;
//			goto number2;
	}
	delta[0] = mid;
	//------------bound-----------
	double E = 0, opt = 0;
	for (int i = 0; i < x_ij.size(); ++i)                      //init Pr
	{
		for (int j = 0; j < x_ij[i].size(); ++j)
		{
			E += Pr_ij[i][j] * requests[i].value *maxValue;
			opt += x_ij[i][j] * requests[i].value *maxValue;;
		}
	}
	//cout << E << " " << 1 - delta[0] << " -> " << E*(1 - delta[0]) << endl;
	//cout << opt << " " << E << " " << (E / opt) << " p: " << (E / opt)*(1 - delta[0]) << endl;
	//system("pause");
	//-------------
	preCalcPow();
	for (int i = 0; i < requests.size(); ++i)
	{
		double minPr = INF;//1;
		for (int j = topo.pathSize(requests[i].getSrcDst()) - 1; j >= -1; --j)
		{
			if (j >= 0 && !x_ij[i][j]) continue;

			double tmpPr = PrUpperBound(i, resX, j);
			if (tmpPr < minPr)
			{
				minPr = tmpPr;
				resX[i] = j;
			}
		}
		cout << i << " : " << resX[i] << "   " << minPr << endl;
	}
	//--------------------------
			number2:
	double incomeBL = 0, incomeLP = 0;
	vector<int> resTmp(requests.size(), -1);
	for (int i = 0; i < requests.size(); ++i)
	{
		incomeBL += (resX[i] == -1 ? 0 : requests[i].value);
		double sumPr=0;
		for (int j = 0; j < x_ij[i].size(); ++j) {//topo.pathSize(requests[i].getSrcDst())
			sumPr+=x_ij[i][j];
			if (x_ij[i][j] == 1) {
				incomeLP += requests[i].value;
				resTmp[i] = j;
			}
		}
//		if (abs(sumPr-1)<EPS) {
//			incomeLP += requests[i].value;
//			resTmp[i] = 0;//----
//		}
	}
	if (incomeBL < incomeLP)
		return resTmp;
	return resX;
}

void Blrsm::preCalcPow() {
	valuePow.clear();
	for (int i = 0; i < requests.size(); ++i)
		valuePow.push_back(powl(1 + delta[0], -1 * requests[i].value));

	ratePow.clear();
	for (int e = 0; e < topo.getEdgeNum(); ++e)
	{
		ratePow.push_back(vector<double>());
		for (int i = 0; i < requests.size(); ++i)
			ratePow.back().push_back(powl(1 + delta[e + 1], requests[i].rate) - 1);
	}
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
		prod *= sumPr*valuePow[i] + 1 - sumPr;
	}
	res_ += powl(1 + delta[0], Fbound - gotValue)*prod;
	//cout <</* branch<<"  "<<*/res_ << endl;
	//---------------------------------------------------------
	//double debug = 0;
	for (int t = 0; t < PEROID; ++t)
	{
		for (int e = 0; e < topo.getEdgeNum(); ++e)
			//for (int k = 0; k < PEROID*topo.getEdgeNum(); ++k)//[T][N]
		{
			if (!topo.linkCapacity(e))
				continue;
			double sumCapacity = 0;
			for (int i = 0; i < deep; ++i)          // < deep
				sumCapacity += requests[i].rate*((t < requests[i].start || t > requests[i].end) ? 0 : 1)
				*(resX[i] < 0 ? 0 : 1)
				*topo.linkInPath(e, requests[i].getSrcDst(), resX[i]);

			sumCapacity += requests[deep].rate*((t < requests[deep].start || t > requests[deep].end) ? 0 : 1)  //   ==  deep
				*(branch < 0 ? 0 : 1)
				*topo.linkInPath(e, requests[deep].getSrcDst(), branch);


			prod = 1;
			for (int i = deep + 1; i < requests.size(); ++i)  //          >  deep
			{
				double sumDelta = 1;
				int pathSize_ = topo.pathSize(requests[i].getSrcDst());
                int start = requests[i].start, end = requests[i].end;
                int src = requests[i].getSrcDst().first, dst = requests[i].getSrcDst().second;
				for (int j = 0; j < pathSize_; ++j)
					if (t >= start&&t <= end && linkInPath[src][dst][j][e])
						sumDelta += Pr_ij[i][j] * ratePow[e][i];// -Pr_ij[i][j];
				prod *= sumDelta;
			}
			res_ += powl(1 + delta[e + 1], sumCapacity - topo.linkCapacity(e))*prod;
			//debug += powl(1 + delta[k%topo.getEdgeNum() + 1], sumCapacity - topo.linkCapacity(k%topo.getEdgeNum()))*prod;
		}
	}
		//cout <</* branch<<"  "<<*/res_ << endl;//debug
	return res_;
}

vector<vector<double> > Blrsm::relaxation_LP() {
	vector<vector<double> > res_; res_.clear();
	vector<int> addr(1, 0);
	cout << requests.size()<<"++++++++++++++++++++++++++++++++++"<<endl;
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
			model.addConstr(lhsExpr, GRB_LESS_EQUAL, 1); // constrain 1------------GRB_EQUAL    GRB_LESS_EQUAL
		}
		for (int t = 0; t < PEROID; ++t)
		{
			for (int e = 0; e < topo.getEdgeNum(); ++e)
			{
				GRBLinExpr lhsExpr = 0;
				for (int i = 0; i < requests.size(); ++i)
				{
				    int start = requests[i].start, end = requests[i].end;
					for (int j = addr[i]; j < addr[i + 1]; ++j)
					{
						if (!(t < start || t > end) && topo.linkInPath(e, requests[i].getSrcDst(), j - addr[i]))
						{
							//cout << "in" << endl;
							lhsExpr += requests[i].rate*xReqPath[j];
						}
					}
				}
				model.addConstr(lhsExpr, GRB_LESS_EQUAL, topo.linkCapacity(e)); // constrain 2
				//cout << topo.linkCapacity(e) << endl;
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
		else{
		    cout <<"???BL"<<endl;
		    while(1);
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
	{
//		res.peakPerEdge[e] = int(ceil(topo.linkCapacity(e)));
		//res.cost += topo.BandwidthPrice[topo.findSrcDst(e).first][topo.findSrcDst(e).second] * topo.BandwidthLim[topo.findSrcDst(e).first][topo.findSrcDst(e).second];

	}

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
	for (int e = 0; e < topo.getEdgeNum(); ++e)
	{
		int max_ =0;
		for (int t = 0; t < PEROID; ++t)
		{
			if (max_< ceil(res.volPerTimeEdge[t][e]))
				max_= int(ceil(res.volPerTimeEdge[t][e]));
		}
		res.peakPerEdge[e] = max_;
		if (max_>res.peakPerEdge[e]){
			//cout <<e<<" error peak"<<endl,errorHandling();
//			while(1);
			break;
		}

	}
	//errorHandling();
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
		res.peakPerEdge[e] = int(ceil(topo.linkCapacity(e)));
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

typedef struct X{
	int i;
	int j;
	double pr;
}X;

bool operator<(const X &x, const X &y)
{
	return x.pr > y.pr;
}

void updateX(vector<X> &x, int reqIndex ) {
	for(int i = 0; i < x.size(); i++){
		if(x[i].i == reqIndex){
			x.erase(x.begin() + i);
			i--;
		}
	}
	sort(x.begin(), x.end());
}


void Blrsm::errorHandling() {
	for(int t = 0; t < PEROID; t++){
		for(int i = 0; i < topo.getEdgeNum(); i++) res.volPerTimeEdge[t][i] = 0;
	}
	vector<X> x;
	for(int i = 0; i < requests.size(); i++){
		double tempPr = 1;
		for(int j = 0; j < x_ij[i].size(); j++){
			X temp;
			temp.i = i;
			temp.j = j;
			temp.pr = x_ij[i][j];
			x.push_back(temp);
			tempPr -= x_ij[i][j];
		}
		X temp;
		temp.i = i;
		temp.j = -1;
		temp.pr = tempPr;
		x.push_back(temp);
	}
	res.receiveNum = 0;
	res.income = 0;
	sort(x.begin(), x.end());
	for(int k = 0; k < x.size(); k++) {
		int reqIndex = x[k].i, pathIndex = x[k].j;
		if (pathIndex == -1) {
			res.passPathIndex[reqIndex] = -1;
			updateX(x, reqIndex);
		} else {
			pair<int, int> srcDst = requests[reqIndex].getSrcDst();
			int start = requests[reqIndex].start, end = requests[reqIndex].end;
			vector<int> path = topo.getPath(srcDst, pathIndex);
			bool pathAva = true;
			for (int t = start; t <= end; t++) {
				for (int e : path) {
					if (res.volPerTimeEdge[t][e] + requests[reqIndex].rate > topo.linkCapacity(e)) {
						pathAva = false;
						break;
					}
				}
				if (!pathAva) break;
			}
			if (!pathAva) {
				x.erase(x.begin() + k);
				k--;
			} else {
				for (int t = start; t <= end; t++) {
					for (int e : path) {
						res.volPerTimeEdge[t][e] += requests[reqIndex].rate;
					}

				}
				res.passPathIndex[reqIndex] = pathIndex;
				updateX(x, reqIndex);
				res.receiveNum += 1;
				res.income += requests[reqIndex].value;
			}
		}
	}


}



