#include<Snap.h>
#include"KVCC_Algorithm.h"

class VCCE_S : public VCCE {
public:
	int Nodes; //Nodes Num
	//int flag = 0;
	//TIntV SSV = {}; //strong side vertex
	VCCE_S(PUNGraph G_, int k_, int Compute_SSV_times_ = 0);
	TUNGraV KVCC_ENUM(PUNGraph G, int k, int flag = 0);
	int Compute_SSV_times;
	int pru_node = 0;
	int non_pru_node = 0;
	int c = 0;
protected:
	void Sweep(PUNGraph G, int v, TIntH& pru, TIntH& deposit, MyTCnComV& CS);
	PUNGraph Compute_SC(PUNGraph G, int k, MyTCnComV& ConV);
	void Detect_SSV(PUNGraph& G);  //Strong Side Vertex
	TIntV Global_Cut(PUNGraph &G, int k, int flag);
	//bool IsSSV(PUNGraph G, int NId, int** Com_Neighbor);
	bool IsSSV(PUNGraph G, int NId);
	void Check_SSV(PUNGraph& G, PUNGraph& subG, TIntV S);
	TUNGraV Overlap_Partition(PUNGraph G, TIntV Vertex_Cut);
	void Get_SSV(PUNGraph &G);
};