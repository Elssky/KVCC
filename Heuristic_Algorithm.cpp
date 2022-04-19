//#include "Heuristic_Algorithm.h"
//#include "Utility.h"
//
//BkVCC::BkVCC()
//{
//}
//
//BkVCC::BkVCC(PUNGraph G_, int k_)
//{
//	G = G_;
//	k = k_;
//}
//
//TUNGraV BkVCC::BkVCC_ENUM(PUNGraph G, int k)
//{
//	TUNGraV G_R, G_S, G_S_prime;
//
//	//find k-core
//	PUNGraph G2 = TSnap::GetKCore(G, k);
//
//	//Seeding
//	G_S = Seeding(G2, k);
//
//	while (G_S != G_S_prime) {
//		G_S_prime = G_S;
//		//expanding G_S
//
//		//merging G_S
//
//	}
//
//	// G_R = G_R Union G_S
//	G_R.AddVMerged(G_S);
//
//	return G_R;
//}
//
//PUNGraph BkVCC::LkVCS(PUNGraph G, int k, int u, int alpha)
//{
//	// params: 
//	//	  alpha: upper bound for selecting subset with size k
//	PUNGraph P = TSnap::GetEgonetHop(G, u, 2); // P = N_2(u)
//	PUNGraph P_prime = TSnap::GetKCore(P, k);
//	int e; //take place
//	if (!P_prime->IsNode(u)) {
//		return {};
//	}
//
//	PUNGraph nb_P_prime = TSnap::GetEgonet(P_prime, u, e);
//	TIntV nb_set;
//	for (TUNGraph::TNodeI NI = nb_P_prime->BegNI(); NI < nb_P_prime->EndNI(); NI++) {
//		nb_set.Add(NI.GetId());
//	}
//	nb_set.DelIfIn(u); //remove u
//	nb_set.Merge(); // Sorts the vector and only keeps a single element of each value.
//	TVec<TIntV> res = subsets(nb_set, k);
//	for (int i = 0; i < res.Len() && i <= alpha; i++) {
//
//		TIntV R = res[i];
//		res[i].Add(u); // R = R Union u
//		PUNGraph G_R = TSnap::GetSubGraph(G, R);
//		bool direct = true;
//		while (direct == true) {
//			//set flag1 = ...
//			if (flag1(G, G_R)) {
//				return TSnap::GetSubGraph(G, R);
//			}
//			else {
//				// set flag2 = ...
//				if (flag2(P_prime, G_R)) {
//					direct = false;
//				}
//				else {
//					//find ... and radomly choose ... vertices from ... adding into R
//				}
//			}
//
//		}
//
//	}
//
//	return PUNGraph();
//}
//
//bool BkVCC::flag1(PUNGraph G, PUNGraph G_R)
//{
//	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
//		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
//			if (NI2 == NI1)continue;
//			if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId()) < k) {
//				return false;
//			}
//		}
//	}
//	return true;
//}
//
//bool BkVCC::flag2(PUNGraph P_prime, PUNGraph G_R)
//{
//
//	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
//		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
//			if (NI2 == NI1)continue;
//			if (TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId()) < k) {
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//TUNGraV BkVCC::Seeding(PUNGraph G, int k)
//{
//	TUNGraV G_S;
//	PUNGraph G_C;
//	TIntH CandMaintain, deg;
//	int alpha = INT_MAX;
//
//	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
//		CandMaintain.AddDat(NI.GetId(), 0);
//		deg.AddDat(NI.GetId(), NI.GetDeg());
//	}
//	deg.SortByDat();
//	for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
//		int v = HI.GetKey();
//		if (CandMaintain.GetDat(v) == 0) {
//			G_C = LkVCS(G, k, v, alpha);
//			if (G_C->GetNodes() != 0) {
//				G_S.AddUnique(G_C);
//				for (TUNGraph::TNodeI NI = G_C->BegNI(); NI < G_C->EndNI(); NI++) {
//					CandMaintain.GetDat(v) = 1;
//				}
//			}
//		}
//	}
//
//	return G_S;
//}
//
//void printNodesNum(PUNGraph G) {
//	cout << "Nodes:"<< G->GetNodes() << endl;
//}
//
//TUNGraV BkVCC::Expanding(PUNGraph G, int k, TUNGraV& G_S)
//{
//	for (TUNGraV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
//		while (flag3) { //exist u belong delta_S, intersection > k
//			printNodesNum(*GI);
//			//Update(G, *GI, u);
//			printNodesNum(*GI);  //Test if graph updates valid
//		}
//	}
//	return TUNGraV();
//}
//
//TIntV BkVCC::GetBoundary(PUNGraph G, PUNGraph G_S, TIntV &delta_S_bar)
//{
//	TIntV delta_S;
//	for (TUNGraph::TNodeI NI = G_S->BegNI(); NI < G_S->EndNI(); NI++) {
//		for (int i = 0; i < NI.GetInDeg(); i++) {
//			if (!G_S->IsNode(NI.GetNbrNId(i))) {
//				delta_S.Add(NI.GetId());
//				delta_S_bar.Add(NI.GetNbrNId(i));
//				//break;
//			}
//		}
//	}
//	delta_S.Merge();
//	delta_S_bar.Merge();
//	return delta_S;
//}
//
//bool BkVCC::flag3(PUNGraph G, PUNGraph G_S, int u)
//{
//	TIntV delta_S_bar, nb_u;
//	TIntV delta_S = GetBoundary(G, G_S, delta_S_bar);
//
//	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
//		//nb_u = getneighbor(G, *TI);
//		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
//			return true;
//		}
//	}
//	return false;
//}
//
//void BkVCC::Update(PUNGraph G, PUNGraph& GI, int u)
//{
//	TIntV Nodes;
//	GI->GetNIdV(Nodes);
//	Nodes.Add(u);
//	GI = TSnap::GetSubGraph(G, Nodes);
//}
//
//
//
//TUNGraV BkVCC::Merging(PUNGraph G, int k, TUNGraV& G_S, TUNGraV& G_R)
//{
//	for (TUNGraV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
//		if (IskVCC(G, *GI, k)) {
//			G_R.Add(*GI);
//			G_S.DelIfIn(*GI);
//		}
//	}
//	while (true) { //exist GI1, GI2 satisfied IsMergeValid
//		//PUNGraph G_Union = Merge2Graph(G, GI1, GI2);
//		/*if (IskVCC(G, G_Union, k)) {
//			G_R.Add(G_Union);
//			G_S.DelIfIn(GI1);
//			G_S.DelIfIn(GI2);
//		}
//		else {
//			G_S.Add(G_Union);
//			G_S.DelIfIn(GI1);
//			G_S.DelIfIn(GI2);
//		}*/
//		
//	}
//	return TUNGraV();
//}
//
//bool BkVCC::IskVCC(PUNGraph G, PUNGraph G_S, int k)
//{
//	//Corollary 1
//	return false;
//}
//
//bool BkVCC::IsMergeValid(PUNGraph G, PUNGraph G_S, PUNGraph G_S_prime, int k)
//{
//	//Theorem 7
//	return false;
//}
//
//PUNGraph BkVCC::Merge2Graph(PUNGraph G, PUNGraph GI1, PUNGraph GI2)
//{
//	//input: G[S], G[S']
//	//return G[S Union S']
//	TIntV Nodes1, Nodes2;
//	GI1->GetNIdV(Nodes1);
//	GI2->GetNIdV(Nodes2);
//	Nodes1.AddVMerged(Nodes2);
//	return TSnap::GetSubGraph(G, Nodes1);
//}
//
//
//
//
//
//
//
//
