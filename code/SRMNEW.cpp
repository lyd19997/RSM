//
// Created by Zhixing Zhang on 2018/4/28.
//

#include "SRMNEW.h"

SRMNEW::SRMNEW(Graph &topo, RequestList &requests): graph(topo),requestsList(requests), result(topo, requests){
    for(int i = 0; i < requests.size(); i++){
        removed.push_back(false);
        tempermoved.push_back(false);
    }
    result.algName = "SRMNEW";
    result.requestNum = requestsList.size();
    RequestList tempRl(requestsList);
    result.income = 0;
    result.cost = INF;
//    vector<double> revenue;
    ofstream out( "SRMNEW_log_LAMBDA_" + to_string(LAMBDA) + ".txt");
    for(int i = 0; i < EPOCHNUM; i++){
        Result tempRes1 = scheduleMAA(tempRl);
//        out << "MAA "<<" revenue"<< tempRes1.income - tempRes1.cost << " income" <<tempRes1.income<< " "<<"cost"<<tempRes1.cost << " receiveNum"<<tempRes1.receiveNum<<" requestnum"<< tempRl.size()<<endl;
//        revenue.push_back(tempRes1.income - tempRes1.cost);
        if(tempRes1.income - tempRes1.cost > result.income - result.cost) resultEqual(result, tempRes1);
        else if (i!=0){
            for(int i = 0; i < result.peakPerEdge.size(); i++) {
                double flow = 0;
                for (int t = 0; t < result.volPerTimeEdge.size(); t++) {
                    flow += result.volPerTimeEdge[t][i];
                }
                double usage = flow / PEROID / result.peakPerEdge[i];
                if (usage * 100 > THRESHOLDVALUE) {
                    graph.BandwidthLim[graph.findSrcDst(i).first][graph.findSrcDst(i).second] = result.peakPerEdge[i];
                } else {
                    graph.BandwidthLim[graph.findSrcDst(i).first][graph.findSrcDst(i).second] =
                            result.peakPerEdge[i] * THRESHOLDVALUE * 1.0 / 100;
                }

            }
        }

        out << "MAA "<<" revenue"<< tempRes1.income - tempRes1.cost << " income" <<tempRes1.income<< " "<<"cost"<<result.cost << " receiveNum"<<tempRes1.receiveNum<<" requestnum"<< tempRl.size()<<endl;
//        if(find(revenue.begin(), revenue.end(), tempRes1.income - tempRes1.cost) != revenue.end()) break;
//        else break;
        Result tempRes2 = scheduleBL(tempRl);
        out << "BLSRM " <<" revenue"<< tempRes2.income - tempRes2.cost << " income" <<tempRes2.income<< " "<<"cost"<<tempRes2.cost << " receiveNum "<<tempRes2.receiveNum<< " requestnum"<< tempRl.size()<<endl;
//        revenue.push_back(tempRes2.income - tempRes2.cost);
        if(tempRes2.income - tempRes2.cost > result.income - result.cost) {
            resultEqual(result, tempRes2);
            int count = 0;
            for(int i = 0; i < tempRes2.passPathIndex.size(); i++){
                if(tempRes2.passPathIndex[i] == -1){
                    tempRl.erase(tempRl.begin() + (i - count));
                    count ++;
                }
            }
            for(int i = 0; i < requests.size(); i++){
                removed[i] = tempermoved[i];
            }
        }
//        if(find(revenue.begin(), revenue.end(), tempRes2.income - tempRes2.cost) != revenue.end()) break;
//        else break;
    }
//    result.requestNum = requestsList.size();
    vector<int> path(result.passPathIndex);
    result.receiveNum = 0;
//    result.income = 0;
//    result.cost = 0;
	result.passPathIndex.clear();
    for(int i = 0, count = 0; i < requests.size(); i++){
        if(removed[i]){
            result.passPathIndex.push_back(-1);
			++count;
        }else{
            result.passPathIndex.push_back(path[i - count]);
            result.receiveNum += 1;
//            result.income += requests[i].value;
        }
    }
    for(int i = 0; i < graph.getEdgeNum(); i++){
//        result.cost += graph.BandwidthPrice[graph.findSrcDst(i).first][graph.findSrcDst(i).second] * 1.0 * result.peakPerEdge[i];
    }
    result.getRunTime();
}

void SRMNEW::resultEqual(Result &res1, Result &res2){
//    res1.receiveNum = res2.receiveNum;
    res1.income = res2.income;
    res1.cost = res2.cost;
    res1.peakPerEdge.clear();
    for(int peak : res2.peakPerEdge) res1.peakPerEdge.push_back(peak);
    res1.passPathIndex.clear();
    for(int index : res2.passPathIndex) res1.passPathIndex.push_back(index);
    res1.volPerTimeEdge.clear();
    for(vector<double> vol : res2.volPerTimeEdge) {
        vector<double> temp(vol);
        res1.volPerTimeEdge.push_back(temp);
    }
}

Result SRMNEW::scheduleMAA(RequestList &rl) {
    MAA maa(graph, rl);
    double income = 0;
    for(Request req: rl){
        income += req.value;
    }
    Result res(maa.result);
    res.income = income;
    for(int i = 0; i < res.peakPerEdge.size(); i++) {
        double flow = 0;
        for (int t = 0; t < res.volPerTimeEdge.size(); t++) {
            flow += res.volPerTimeEdge[t][i];
        }
        double usage = flow / PEROID / res.peakPerEdge[i];
        if (usage * 100 > THRESHOLDVALUE) {
            graph.BandwidthLim[graph.findSrcDst(i).first][graph.findSrcDst(i).second] = res.peakPerEdge[i];
        } else {
            graph.BandwidthLim[graph.findSrcDst(i).first][graph.findSrcDst(i).second] =
                    res.peakPerEdge[i] * THRESHOLDVALUE * 1.0 / 100;
        }
    }
    return res;
}

Result SRMNEW::scheduleBL(RequestList &rl) {
    Blrsm blrsm(graph, rl);
    blrsm.schedule();
    double cost = 0;
    for(int i = 0; i < graph.getEdgeNum(); i++){
        cost += /*graph.BandwidthLim[graph.findSrcDst(i).first][graph.findSrcDst(i).second]*/blrsm.res.peakPerEdge[i] * graph.BandwidthPrice[graph.findSrcDst(i).first][graph.findSrcDst(i).second];
    }
    Result res(blrsm.res);
    res.cost = cost;
    res.receiveNum = 0;
    res.income = 0;
    int count = 0;
    for(int i = 0; i < res.passPathIndex.size(); i++){
        if(res.passPathIndex[i] == -1){
            tempermoved[rl[i - count].id] = true;
            count ++;
        }else {
            res.receiveNum += 1;
            res.income += rl[i].value;
        }
    }
    return res;
}
