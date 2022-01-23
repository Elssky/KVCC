#include<Snap.h>
#include"KVCC_Algorithm.h"

class VCCE_S : public VCCE {
public:
	//TIntV SSV = {}; //strong side vertex
	VCCE_S(PUNGraph G_, int k_);
protected:
	void Sweep(PUNGraph G, int u, int v, TIntH& pru, TIntH& deposit, MyTCnComV& CS);
	PUNGraph Compute_SC(PUNGraph G, int k, MyTCnComV& ConV);
	TIntV Detect_SSV(PUNGraph G, int k);  //Strong Side Vertex
	TIntV Global_Cut(PUNGraph G, int k);
	bool IsSSV(PUNGraph G, int NId);

};