#include "Heuristic_Algorithm.h"
#include "Utility.h"

BkVCC::BkVCC()
{
}

BkVCC::BkVCC(PUNGraph G_, int k_)
{
	G = G_;
	k = k_;
}

TUNGraV BkVCC::BkVCC_ENUM(PUNGraph G, int k)
{
	TUNGraV G_R, G_S, G_S_prime;

	//find k-core
	PUNGraph G2 = TSnap::GetKCore(G, k);

	//Seeding
	G_S = Seeding(G2, k);

	while (G_S != G_S_prime) {
		G_S_prime = G_S;
		//expanding G_S

		//merging G_S

	}

	// G_R = G_R Union G_S
	G_R.AddVMerged(G_S);

	return G_R;
}

PUNGraph BkVCC::LkVCS(PUNGraph G, int k, int u)
{
	PUNGraph P = TSnap::GetEgonetHop(G, u, 2); // P = N_2(u)
	PUNGraph P_prime = TSnap::GetKCore(P, k);
	int e; //take place
	if (!P_prime->IsNode(u)) {
		return {};
	}

	PUNGraph nb_P_prime = TSnap::GetEgonet(P_prime, u, e);
	TIntV nb_set;
	for (TUNGraph::TNodeI NI = nb_P_prime->BegNI(); NI < nb_P_prime->EndNI(); NI++) {
		nb_set.Add(NI.GetId());
	}
	nb_set.DelIfIn(u); //remove u
	nb_set.Merge(); // Sorts the vector and only keeps a single element of each value.
	TVec<TIntV> res = subsets(nb_set, k);
	for (int i = 0; i < res.Len(); i++) {

		TIntV R = res[i];
		res[i].Add(u); // R = R Union u
		bool direct = true;
		while (direct == true) {
			bool flag1 = false, flag2 = false;
			//set flag1 = ...
			if (flag1) {
				return TSnap::GetSubGraph(G, R);
			}
			else {
				// set flag2 = ...
				if (flag2) {
					direct = false;
				}
				else {
					//find ... and radomly choose ... vertices from ... adding into R
				}
			}

		}

	}

	return PUNGraph();
}

TUNGraV BkVCC::Seeding(PUNGraph G, int k)
{
	TUNGraV G_S;
	PUNGraph G_C;
	TIntH CandMaintain, deg;

	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		CandMaintain.AddDat(NI.GetId(), 0);
		deg.AddDat(NI.GetId(), NI.GetDeg());
	}
	deg.SortByDat();
	for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
		int v = HI.GetKey();
		if (CandMaintain.GetDat(v) == 0) {
			G_C = LkVCS(G, k, v);
			if (G_C->GetNodes() != 0) {
				G_S.AddUnique(G_C);
				for (TUNGraph::TNodeI NI = G_C->BegNI(); NI < G_C->EndNI(); NI++) {
					CandMaintain.GetDat(v) = 1;
				}
			}
		}
	}

	return G_S;
}

void printNodesNum(PUNGraph G) {
	cout << "Nodes:"<< G->GetNodes() << endl;
}

TUNGraV BkVCC::Expanding(PUNGraph G, int k, TUNGraV& G_S)
{
	for (TUNGraV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
		while (true) { //exist u belong delta_S, intersection > k
			printNodesNum(*GI);
			//Update(G, *GI, u);
			printNodesNum(*GI);  //Test if graph updates valid
		}
	}
	return TUNGraV();
}

void BkVCC::Update(PUNGraph G, PUNGraph& GI, int u)
{
	TIntV Nodes;
	GI->GetNIdV(Nodes);
	Nodes.Add(u);
	GI = TSnap::GetSubGraph(G, Nodes);
}

TUNGraV BkVCC::Merging(PUNGraph G, int k, TUNGraV& G_S)
{
	return TUNGraV();
}






