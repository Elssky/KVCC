#pragma once
#include <Snap.h>
#include <cliques.h>
#include <fstream>
#include <iostream>
#include <string>
#include "Utility.h"

using namespace std;

typedef TVec<TInt> TIntV;
typedef TVec<PUNGraph> TUNGraV;
typedef TPt<TUNGraph> PUNGraph;
typedef TVec<TIntV> TIntVIntV;
typedef TVec <TPair<TIntV, TIntV>> TPrVIntV;



class BkVCC {
public:
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
	BkVCC();
	BkVCC(PUNGraph G_, int k_);
	TIntVIntV BkVCC_ENUM(PUNGraph& G, int k, int alpha = 1000);
	TStr dataset;
protected:
	TIntV LkVCS(PUNGraph G, int k, int u, int alpha);
	bool flag1(PUNGraph G_R);
	bool flag2(PUNGraph P_prime, PUNGraph G_R);
	void Adding2Subset(PUNGraph P_prime, PUNGraph G_R, TIntV& R);
	TIntVIntV Seeding(PUNGraph G, int k, int alpha);
	void Expanding(int k, TIntVIntV& G_S);
	TIntV GetBoundary(TIntV G_S, TIntV &delta_S_bar);	
	bool flag3(TIntV G_S, int& u, TIntV& delta_S, TIntV& delta_S_bar);
	/*void Update(PUNGraph G, PUNGraph& GI, int u);*/
	void Merging(int k, TIntVIntV& G_S, TIntVIntV& G_R);
	TPrVIntV flag4(TIntVIntV G_S);
	bool IskVCC(TIntV G_S, int k);
	bool IsMergeValid(TIntV G_S, TIntV G_S_prime);
	bool IsMergeValid_maxflow(TIntV G_S, TIntV G_S_prime);
	PNEANet Construct_DG(PUNGraph G);
	/*PUNGraph Merge2Graph(PUNGraph G, PUNGraph GI1, PUNGraph GI2);*/

	TIntV flag3(TIntV G_S, TIntV& delta_S, TIntV& delta_S_bar);
};

