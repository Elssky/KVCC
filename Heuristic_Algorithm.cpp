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

TIntVIntV BkVCC::BkVCC_ENUM(PUNGraph G, int k)
{
	TIntVIntV G_R, G_S, G_S_prime; 

	//find k-core
	PUNGraph G2 = TSnap::GetKCore(G, k);
	printf("G2: \nnode_nums = %d, edge_nums = %d\n", G2->GetNodes(), G2->GetEdges());
	//Seeding
	G_S = Seeding(G2, k);

	while (G_S.Len() != G_S_prime.Len()) { // TODO: 如何判断是否发生变化，仅靠长度肯定是不行的
		G_S_prime = G_S;
		//expanding G_S
		//Expanding(G2, k, G_S);
		//merging G_S
		//Merging(G2, k, G_S, G_R);
	}

	// G_R = G_R Union G_S
	// 
	// error can not use this function
	// 无法比较两个图是否相同，所以sort相关的函数比如AddMergedV(), Merge()都不能使用，除非重载运算符
	G_R.AddV(G_S);


	return G_R;
}

TIntV BkVCC::LkVCS(PUNGraph G, int k, int u, int alpha)
{
	// params: 
	//	  alpha: upper bound for selecting subset with size k

	PUNGraph P = TSnap::GetEgonetHop(G, u, 2); // P = N_2(u)
	//printf("P : \nnode_nums = %d, edge_nums = %d\n", P->GetNodes(), P->GetEdges());
	PUNGraph P_prime = TSnap::GetKCore(P, k);
	 //printf("P_prime: \nnode_nums = %d, edge_nums = %d\n", P_prime->GetNodes(), P_prime->GetEdges());
	int e; //take place
	if (!P_prime->IsNode(u)) {
		return {};
	}

	
	TIntV nb_set;
	TUNGraph::TNodeI u_Node = P_prime->GetNI(u);
	for (int i = 0; i < u_Node.GetInDeg(); ++i) {
		nb_set.Add(u_Node.GetInNId(i));
	}

	//nb_set.DelIfIn(u); //remove u

	TIntVIntV res = subsets(nb_set, k, alpha);

	for (int i = 0; i < res.Len() && i <= alpha; i++) {

		TIntV R = res[i];

		R.Add(u); // R = R Union u
		R.Merge();
		bool direct = true;
		while (direct) {

			//R may be updated
			PUNGraph G_R = TSnap::GetSubGraph(G, R);
			//set flag1 = ...
			if (flag1(G_R)) {
				cout << "flag1" << endl;
				return R;
			}
			else {
				// set flag2 = ...
				if (flag2(P_prime, G_R)) {
					cout << "flag2" << endl;
					direct = false;
				}
				else {
					cout << "flag3" << endl;
					//find ... and radomly choose ... vertices from ... adding into R
					Adding2Subset(P_prime, G_R, R);
				}
			}

		}

	}

	return {};
}

bool BkVCC::flag1(PUNGraph G_R)
{
	
	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId())) continue;

			if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId()) < k  ) {
				return false;
			}
		}
	}
	return true;
}

bool BkVCC::flag2(PUNGraph P_prime, PUNGraph G_R)
{

	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId()))continue;

			if (TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId()) < k) {
				return true;
			}
		}
	}
	return false;
}

void BkVCC::Adding2Subset(PUNGraph P_prime, PUNGraph G_R, TIntV &R)
{
	
	TIntV NbrV1, NbrV2;
	int num = 0;
	int total;
	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
			if (NI2 == NI1)continue;
			if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId(), NbrV1) < k) {
				TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId(), NbrV2);
				total = k - NbrV1.Len();
				//This is not randomly choose, which is different with paper
				for (TIntV::TIter NI = NbrV2.BegI(); NI < NbrV2.EndI(); NI++) {
					if (!NbrV1.IsIn(*NI)) {
						R.Add(*NI);
						num++;
						if (num >= total) return;
					}
				}
			}
		}
	}
}

TIntVIntV BkVCC::Seeding(PUNGraph G, int k)
{
	TIntVIntV G_S;
	TIntV G_C;
	TIntH CandMaintain, deg;
	int alpha = INT_MAX;

	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		CandMaintain.AddDat(NI.GetId(), 0);
		deg.AddDat(NI.GetId(), NI.GetDeg());
	}
	deg.SortByDat(false); //non decreasing order

	for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
		//cout << HI.GetKey() <<" " << HI.GetDat() << endl;

		int v = HI.GetKey();
		if (CandMaintain.GetDat(v) == 0) {
			G_C = LkVCS(G, k, v, alpha);			
			if (G_C.Empty()) continue;

			//G_S.AddUnique(G_C); //怎么判断是否相等（Unique）？
			G_S.Add(G_C);
			

			for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {
				CandMaintain.GetDat(*TI) = 1;
			}
		}
	}
	
	return G_S;
}

//void printNodesNum(PUNGraph G) {
//	cout << "Nodes:"<< G->GetNodes() << endl;
//}
//
//TUNGraV BkVCC::Expanding(PUNGraph G, int k, TUNGraV& G_S)
//{
//	int u;
//	TIntV delta_S, delta_S_bar;
//	for (TUNGraV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
//		while (flag3(G, *GI, u, delta_S, delta_S_bar)) { //exist u belong delta_S, intersection > k
//			printNodesNum(*GI);
//			Update(G, *GI, u);
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
//
//
//bool BkVCC::flag3(PUNGraph G, PUNGraph G_S, int& u, TIntV& delta_S, TIntV& delta_S_bar)
//{
//	TIntV nb_u;
//	delta_S = GetBoundary(G, G_S, delta_S_bar);
//
//	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
//		//nb_u = getneighbor(G, *TI);
//		const TUNGraph::TNodeI& CtrNode = G->GetNI(*TI);
//		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
//			if (!nb_u.IsIn(CtrNode.GetInNId(i)))
//				nb_u.Add(CtrNode.GetInNId(i));
//			
//		}
//		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
//			u = *TI;
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
//	PUNGraph GI1, GI2;
//	for (TUNGraV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
//		if (IskVCC(G, *GI, k)) {
//			G_R.Add(*GI);
//			G_S.DelIfIn(*GI);
//		}
//	}
//	while (flag4(G, G_S, GI1, GI2)) { //exist GI1, GI2 satisfied IsMergeValid
//		PUNGraph G_Union = Merge2Graph(G, GI1, GI2);
//		if (IskVCC(G, G_Union, k)) {
//			G_R.Add(G_Union);
//			G_S.DelIfIn(GI1);
//			G_S.DelIfIn(GI2);
//		}
//		else {
//			G_S.Add(G_Union);
//			G_S.DelIfIn(GI1);
//			G_S.DelIfIn(GI2);
//		}
//		
//	}
//	return TUNGraV();
//}
//
//bool BkVCC::flag4(PUNGraph G, TUNGraV G_S, PUNGraph& G1, PUNGraph& G2)
//{
//	for (TUNGraV::TIter GI1 = G_S.BegI(); GI1 < G_S.EndI(); GI1++) {
//		for (TUNGraV::TIter GI2 = GI1; GI2 < G_S.EndI(); GI2++) {
//			//if (*GI2 == *GI1)continue;
//			if (IsMergeValid(G, *GI1, *GI2)) {
//				G1 = *GI1;
//				G2 = *GI2;
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//bool BkVCC::IskVCC(PUNGraph G, PUNGraph G_S, int k)
//{
//	//Corollary 1
//	int u;
//	TIntV delta_S, delta_S_bar;
//	if (!flag3(G, G_S, u, delta_S, delta_S_bar) && min(delta_S.Len(), delta_S_bar.Len()) < k) {
//		return true;
//	}
//	return false;
//}
//
//bool BkVCC::IsMergeValid(PUNGraph G, PUNGraph G_S, PUNGraph G_S_prime)
//{
//	//Theorem 7
//	TIntV S_Nodes, S_prime_Nodes, S_ints_S_prime, S_sub_S_prime, S_prime_sub_S;
//	PUNGraph S_exc_S_prime, S_prime_exc_S;
//	int nb_1, nb_2;
//	G_S->GetNIdV(S_Nodes);
//	G_S_prime->GetNIdV(S_prime_Nodes);
//	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
//		if (S_prime_Nodes.IsIn(*TI)) {
//			S_ints_S_prime.Add(*TI);
//		}
//	}
//
//	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
//		if (!S_ints_S_prime.IsIn(*TI)) {
//			S_sub_S_prime.Add(*TI);
//		}
//	}
//	for (TIntV::TIter TI = S_prime_Nodes.BegI(); TI < S_prime_Nodes.EndI(); TI++) {
//		if (!S_ints_S_prime.IsIn(*TI)) {
//			S_prime_sub_S.Add(*TI);
//		}
//	}
//	//S_exc_S_prime = TSnap::GetSubGraph(G, S_sub_S_prime);
//	//S_prime_exc_S = TSnap::GetSubGraph(G, S_prime_sub_S);
//
//	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
//		TUNGraph::TNodeI NI = G->GetNI(*TI);
//		for (int i = 0; i < NI.GetInDeg(); i++) {
//			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
//				nb_1++;
//			}
//		}
//	}
//
//	for (TIntV::TIter TI = S_prime_sub_S.BegI(); TI < S_prime_sub_S.EndI(); TI++) {
//		TUNGraph::TNodeI NI = G->GetNI(*TI);
//		for (int i = 0; i < NI.GetInDeg(); i++) {
//			if (S_sub_S_prime.IsIn(NI.GetNbrNId(i))) {
//				nb_2++;
//			}
//		}
//	}
//
//	if (S_ints_S_prime.Len() + min(nb_1, nb_2) >= k) {
//		return true;
//	}
//	
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
//	TIntV S_sub_S_prime, S_prime_sub_S;
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
