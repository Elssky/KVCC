#include<Snap.h>

class MyTUNGraph;

typedef TVec<TInt> TIntV;
typedef TVec<PUNGraph> TUNGraV;
typedef TPt<MyTUNGraph> MyPUNGraph;

class MyTUNGraph : TUNGraph {
	TIntV SSV;
};

class VCCE {
public:
	int* vertex_map_;
	double _time = 0;
	double _time2 = 0;
	double _time3 = 0;
	double _time4 = 0;
	int m = 0;
	int m2 = 0;
	int m3 = 0;
	int m4 = 0;
	PUNGraph G;
	int k;
	VCCE();
	VCCE(PUNGraph G_, int k_);
	TUNGraV KVCC_ENUM(PUNGraph G, int k);
protected:
	TIntV Global_Cut(PUNGraph G, int k);
	TUNGraV Overlap_Partition(PUNGraph G, TIntV Vertex_Cut);
	PNEANet Construct_DG(PUNGraph G);
	PUNGraph Compute_SC(PUNGraph G, int k);
	TIntV Loc_Cut(int source, int sink, PNEANet DG, PUNGraph G, int k);
};

template <class PGraph>
int GetMnDegNId(const PGraph& Graph);

/// Returns the maximum integer valued flow in the network \c Net from source \c SrcNId to sink \c SnkNId according to Ford-Fulkerson algorithm. 
//int GetMaxFlowIntFF(PNEANet& Net, const int& SrcNId, const int& SnkNId, PNEANet& ResNet);


