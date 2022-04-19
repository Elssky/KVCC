//#pragma once
//#include<Snap.h>
//
//typedef TVec<TInt> TIntV;
//typedef TVec<PUNGraph> TUNGraV;
//typedef TPt<TUNGraph> PUNGraph;
//
//class BkVCC {
//public:
//	PUNGraph G;
//	int k;
//	BkVCC();
//	BkVCC(PUNGraph G_, int k_);
//	TUNGraV BkVCC_ENUM(PUNGraph G, int k);
//protected:
//	PUNGraph LkVCS(PUNGraph G, int k, int u, int alpha);
//	bool flag1(PUNGraph G, PUNGraph G_R);
//	bool flag2(PUNGraph P_prime, PUNGraph G_R);
//	TUNGraV Seeding(PUNGraph G, int k);
//	TUNGraV Expanding(PUNGraph G, int k, TUNGraV &G_S);
//	TIntV GetBoundary(PUNGraph G, PUNGraph G_S, TIntV &delta_S_bar);
//	bool flag3(PUNGraph G, PUNGraph G_S, int u);
//	void Update(PUNGraph G, PUNGraph& GI, int u);
//	TUNGraV Merging(PUNGraph G, int k, TUNGraV& G_S, TUNGraV& G_R);
//	bool IskVCC(PUNGraph G, PUNGraph G_S, int k);
//	bool IsMergeValid(PUNGraph G, PUNGraph G_S, PUNGraph G_S_prime, int k);
//	PUNGraph Merge2Graph(PUNGraph G, PUNGraph GI1, PUNGraph GI2);
//};
//
