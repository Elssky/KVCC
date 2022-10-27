#ifndef SWEEP_ALGORITHM_H
#define SWEEP_ALGORITHM_H


#include<Snap.h>
#include"KVCC_Algorithm.h"
#include "Utility.h"



class VCCE_S : public VCCE {
public:
	int Nodes; //Nodes Num
	//int flag = 0;
	//TIntV SSV = {}; //strong side vertex
	VCCE_S(PUNGraph G_, int k_, int Compute_SSV_times_ = 0);
	TIntVIntV KVCC_ENUM(PUNGraph &sub_G, int k, int flag = 0);
	int Compute_SSV_times;
	int pru_node = 0;
	int non_pru_node = 0;
	int c = 0;
	TStr dataset;
protected:
	void Sweep(PUNGraph G, int v, TIntH& pru, TIntH& deposit, MyTCnComV& CS);
	PUNGraph Compute_SC(TIntV subG, int k, MyTCnComV& CS, PUNGraph All_G);
	void Detect_SSV(PUNGraph& G);  //Strong Side Vertex
	TIntV Global_Cut(TIntV &subG, int k, int flag, PUNGraph All_G);
	//bool IsSSV(PUNGraph G, int NId, int** Com_Neighbor);
	bool IsSSV(PUNGraph G, int NId);
	void Check_SSV(PUNGraph& G, PUNGraph& subG, TIntV S);
	TIntVIntV Overlap_Partition(TIntV subG, TIntV Vertex_Cut, PUNGraph All_G);
	//All_G表示当前轮次的大图，用于讲TIntV转换成PUNGraph时取子图用
	//void Get_SSV(PUNGraph &G);
};

#endif