#include"Sweep_Algorithm.h"
#include"set"
#include"vector"
#include"algorithm"
#include <iterator>

VCCE_S::VCCE_S(PUNGraph G_, int k_) {
	G = G_;
	k = k_;
}

TIntV VCCE_S::Global_Cut(PUNGraph G, int k)
{
	TIntV S, SSV;
	MyTCnComV CS;
	TIntH deposit, dist, pru;
	int u, v;
	

	//1. compute sparse certification SC
	PUNGraph SC = Compute_SC(G, k, CS);

	//2. direct graph
	PNEANet SC_bar = Construct_DG(SC);
	//3. detect strong side vertices
	SSV = Detect_SSV(G, k);

	if (SSV.Empty()) {
		u = GetMnDegNId(SC);
	}
	else {
		u = SSV.GetRndVal();
	}
	//4.Init
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		I->g_deposit = 0;
	}
	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		deposit[NI.GetId()] = 0;
		pru[NI.GetId()] = FALSE;
	}
	//5.Sweep
	Sweep(G, u, u, pru, deposit, CS);
	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		TSnap::GetShortPath(G, u, dist, FALSE);
	}
	dist.SortByDat(FALSE);
	for (TIntH::TIter HI = dist.BegI(); HI < dist.EndI(); HI++) {
		v = HI.GetKey();
		if (pru[v] == TRUE) continue;
		S = Loc_Cut(u, v, SC_bar, SC, k);
		if (!S.Empty()) return S;
		Sweep(G, u, v, pru, deposit, CS);

	}
	//6.
	if (!SSV.IsIn(u)) {
		int e;
		PUNGraph Neigh = TSnap::GetEgonet(SC, u, e);
		for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {
			for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {
				bool flag = FALSE;
				for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
					if (I->IsNIdIn(NI1.GetId()) && I->IsNIdIn(NI2.GetId())) {
						flag = TRUE;
						break;
					}

				}
				if (flag == TRUE) continue;
				S = Loc_Cut(NI1.GetId(), NI2.GetId(), SC_bar, SC, k);
				if (S.Empty() == FALSE) return S;
			}
		}
	}

	//IsSubGraph = TRUE; 


	return S;
}


PUNGraph VCCE_S::Compute_SC(PUNGraph G, int k, MyTCnComV &ConV)
{

	PUNGraph SC = TUNGraph::New();
	PUNGraph G_ = TUNGraph::New();
	*G_ = *G;
	PNGraph BFSTree;
	int NodeNums = TSnap::CntNonZNodes(G_);
	std::vector<int> visited; //mark visited node
	std::vector<int> del;  //fix wrong BFS Tree
	for (int i = 0; i < k; i++) {
		visited.clear();
		//printf("F%d:\n", i);
		int j = 0;
		if (i == k - 1) {
			TSnap::GetSccs(G_, ConV); //add new function at 'cncom.h' Line 406
		}
		while (visited.size() < NodeNums) {
			for (TUNGraph::TNodeI NI = G_->BegNI(); NI < G_->EndNI(); NI++) {

				if (std::find(visited.begin(), visited.end(), NI.GetId()) == visited.end()) {
					//printf("T%d:\n", j++);
					BFSTree = TSnap::MyGetBfsTree(G_, NI.GetId(), TRUE, FALSE);  //My BFS Function modified in bfsdfs.h
					del.clear();
					//printf("%d\n", TSnap::CntUniqDirEdges(BFSTree));

					for (TNGraph::TNodeI TNI = BFSTree->BegNI(); TNI < BFSTree->EndNI(); TNI++) {
						visited.push_back(TNI.GetId());
						if (i == 0) {
							if (!SC->IsNode(TNI.GetId()))
								SC->AddNode(TNI.GetId());
						}
					}

					for (TNGraph::TEdgeI TEI = BFSTree->BegEI(); TEI < BFSTree->EndEI(); TEI++) {
						// if not use MyGetBfsTree() before, use the follow code
						//if (std::find(del.begin(), del.end(), TEI.GetDstNId()) == del.end()) {
						//	//printf("%d -> %d\n", TEI.GetSrcNId(), TEI.GetDstNId());
						//	SC->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						//	del.push_back(TEI.GetDstNId());
						//	G->DelEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						//}
						//printf("%d -> %d\n", TEI.GetSrcNId(), TEI.GetDstNId());
						SC->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						G_->DelEdge(TEI.GetSrcNId(), TEI.GetDstNId());
					}
				}
			}
		}
	}
	return SC;

}

TIntV VCCE_S::Detect_SSV(PUNGraph G, int k)
{	
	//判断是否为子图，若是，可利用P57 Lemma11、12推导出的加快检测SSV的方法；
	//Todo: P57 Lemma11、12推导出的加快检测SSV的方法
	TIntV SSV;
	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		if (IsSSV(G, NI.GetId())) SSV.Add(NI.GetId());
	}

	return SSV;
}



void VCCE_S::Sweep(PUNGraph G, int u, int v, TIntH& pru, TIntH& deposit, MyTCnComV& CS) {
	pru[v] = TRUE;
	int e;
	PUNGraph Neigh = TSnap::GetEgonet(G, u, e);
	for (TUNGraph::TNodeI NI = Neigh->BegNI(); NI < Neigh->EndNI(); NI++) {
		if (pru[NI.GetId()] == FALSE) {
			deposit[NI.GetId()]++;
		}
		if (IsSSV(G, v) || deposit[NI.GetId()] >= k)
			Sweep(G, u, NI.GetId(), pru, deposit, CS);
	}
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		if (I->IsNIdIn(v) && I->processed == FALSE) {
			I->g_deposit++;
		}
		if (IsSSV(G, v) || I->g_deposit >= k) {
			I->processed = TRUE;
			PUNGraph CC = TSnap::GetSubGraph(G, I->NIdV);
			for (TUNGraph::TNodeI NI = CC->BegNI(); NI < CC->EndNI(); NI++) {
				if(pru[NI.GetId()] == FALSE)
					Sweep(G, u, NI.GetId(), pru, deposit, CS);
			}
		}
	}
}

bool VCCE_S::IsSSV(PUNGraph G, int NId) {
	
	int e;
	std::set<int> set1, set2, set3;
	PUNGraph Neigh = TSnap::GetEgonet(G, NId, e); //N(u)
	for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {

		set1.clear();
		PUNGraph Neigh1 = TSnap::GetEgonet(G, NI1.GetId(), e);
		for (TUNGraph::TNodeI NI = Neigh1->BegNI(); NI < Neigh1->EndNI(); NI++) {
			set1.insert(NI.GetId());
		}

		for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {

			set2.clear();
			set3.clear();

			if (NI1 == NI2) continue;
			if (!G->IsEdge(NI1.GetId(), NI2.GetId())) return FALSE;

			PUNGraph Neigh2 = TSnap::GetEgonet(G, NI2.GetId(), e);
			for (TUNGraph::TNodeI NI = Neigh2->BegNI(); NI < Neigh2->EndNI(); NI++) {
				set2.insert(NI.GetId());
			}
			std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::insert_iterator<std::set<int>>(set3, set3.begin()));
			if (set3.size() < k) return FALSE;

		}
	}

	return TRUE;
}
