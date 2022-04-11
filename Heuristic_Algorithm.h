#pragma once
#include<Snap.h>

typedef TVec<TInt> TIntV;
typedef TVec<PUNGraph> TUNGraV;
typedef TPt<TUNGraph> PUNGraph;

class BkVCC {
public:
	PUNGraph G;
	int k;
	BkVCC();
	BkVCC(PUNGraph G_, int k_);
	TUNGraV BkVCC_ENUM(PUNGraph G, int k);
protected:
	PUNGraph LkVCS(PUNGraph G, int k, int u);
	TUNGraV Seeding(PUNGraph G, int k);
	TUNGraV Expanding(PUNGraph G, int k, TUNGraV &G_S);
	void Update(PUNGraph G, PUNGraph& GI, int u);
	TUNGraV Merging(PUNGraph G, int k, TUNGraV& G_S);
};

