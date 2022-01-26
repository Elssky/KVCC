#include"Sweep_Algorithm.h"
#include"set"
#include"vector"
#include <map>
#include"algorithm"
#include <iterator>


VCCE_S::VCCE_S(PUNGraph G_, int k_) {
	G = G_;
	k = k_;
}

TUNGraV VCCE_S::KVCC_ENUM(PUNGraph G, int k) {
	//func: Find k-VCCs in Graph G
	//params:
	//	G: Undirected Graph
	//	k: an integer
	//return:
	//	all k-vertex connected components (Type: TUNGraV )


	//printf("G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
	//for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
	//	printf("%d\n", NI.GetId());
	//}
	//step1: initialize set VCC as empty
	TUNGraV VCC;  // return as K-Vccs;

	//step2: remove vertices u and incident edges, that d(u) < k
	PUNGraph G2 = TSnap::GetKCore(G, k);
	G2->SSV = G->SSV;
	G2->IsSub = G->IsSub;
	if(!G2->IsSub) Detect_SSV(G2, k);
	//printf("SSV_len: %d\n", G->SSV.Len());
	//printf("G2: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G2), TSnap::CntUniqUndirEdges(G2));

	//step3: identify connected compotent set in G
	TCnComV ConV;
	TSnap::GetSccs(G2, ConV);
	//printf("1\n");

	TVec<PUNGraph> G_set; // Connected Component in G2;
	for (TCnComV::TIter I = ConV.BegI(); I < ConV.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		PUNGraph GI = TSnap::GetSubGraph(G2, I->NIdV);
		GI->IsSub = G2->IsSub;
		
		for (TIntV::TIter NI = G2->SSV.BegI(); NI < G2->SSV.EndI(); NI++) {
			if (GI->IsNode(*NI)) GI->SSV.Add(*NI);
		}
		G_set.Add(GI);
	}
	//printf(" G_set.Len(): %d\n", G_set.Len());
	//printf("2\n");
	//step4: find vertex cut in G
	TIntV S; //Vertex_Cut
	//printf("G_set_len = %d\n", G_set.Len());
	for (TUNGraV::TIter GI = G_set.BegI(); GI < G_set.EndI(); GI++) {
		//clock_t t1 = clock();
		S = Global_Cut(*GI, k);
		//m++;
		//_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
		//printf("SSV_len: %d\n", (*GI)->SSV.Len());
		//printf("%d\n", S.Empty());
		if (S.Empty()) {

			VCC.Add(*GI);
			//printf("GI: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
		}
		else {
			TUNGraV G_i = Overlap_Partition(*GI, S);
			//printf("%d\n", G_i.Len());
			for (TUNGraV::TIter G_ij = G_i.BegI(); G_ij < G_i.EndI(); G_ij++) {
				//printf("SSV_len: %d\n", (*G_ij)->SSV.Len());
				TUNGraV VCC_i = KVCC_ENUM(*G_ij, k);
				//printf("%d\n", VCC_i.Len());
				for (TUNGraV::TIter GI_j = VCC_i.BegI(); GI_j < VCC_i.EndI(); GI_j++) {

					VCC.Add(*GI_j);
					//for (TUNGraV::TIter GI = VCC.BegI(); GI < VCC.EndI(); GI++) {
					//	printf("GI: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
					//}
				}

			}
		}

	}
	//printf("3\n");
	return VCC;
}

TIntV VCCE_S::Global_Cut(PUNGraph G, int k)
{
	
	TIntV S;
	MyTCnComV CS;	
	int u, v;
	
	//1. compute sparse certification SC

	PUNGraph SC = Compute_SC(G, k, CS);
	int N = SC->GetNodes();
	TIntH deposit, dist, pru;
	//printf("THash_len:%d\n", deposit.Len());
	SC->SSV = G->SSV;
	SC->IsSub = G->IsSub;
	//2. direct graph
	PNEANet SC_bar = Construct_DG(SC);
	
	//3. detect strong side vertices

	
	Detect_SSV(SC, k);

	
	//printf("SSV_len: %d\n", SC->SSV.Len());
	//printf("G->SSV: %d\n", G->SSV.Len());
	
	//printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
	
	
	if (SC->SSV.Empty()) {
		u = GetMnDegNId(SC);
	}
	else {
		u = SC->SSV.GetRndVal();
	}
	//4.Init
	clock_t t1 = clock();
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		I->g_deposit = 0;
	}
	
	for (TUNGraph::TNodeI NI = SC->BegNI(); NI < SC->EndNI(); NI++) {
		deposit.AddDat(NI.GetId(), 0);
		pru.AddDat(NI.GetId(), 0);

	}

	printf("THash_len:%d\n", deposit.Len());
	 
	
	
	//
	/*int M = SC->GetMxNId();
	for (int i = 0; i <= M; i++) {
		deposit.AddKey(i);
		pru.AddKey(i);
		deposit[i] = 0;
		pru[i] = 0;
	}*/
	m++;
	_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;


	
	TSnap::GetShortPath(G, u, dist, FALSE);
	dist.SortByDat(FALSE);

	printf("THash_len:%d\n", dist.Len());
	//for (TIntH::TIter HI = pru.BegI(); HI < pru.EndI(); HI++) {
	//	v = HI.GetKey();
	//	pru[v] = 1;
	//	//deposit[v] = 0;
	//}

	//5.Phase1 & Sweep
	Sweep(SC, u, u, pru, deposit, CS);
	
	for (TIntH::TIter HI = dist.BegI(); HI < dist.EndI(); HI++) {
		
		v = HI.GetKey();
		if (pru[v] == TRUE) {
			continue;
		}
		S = Loc_Cut(u, v, SC_bar, SC, k);
		
		if (!S.Empty()) {
			
			return S;
		}
		Sweep(SC, u, v, pru, deposit, CS);

	}
	
	//6.Phase2
	if (!SC->SSV.IsIn(u)) {
		int e;
		PUNGraph Neigh = TSnap::GetEgonet(SC, u, e);
		for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {
			for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {
				if (NI1 == NI2) continue;
				bool flag = FALSE;
				for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
					if (I->IsNIdIn(NI1.GetId()) && I->IsNIdIn(NI2.GetId())) {
						flag = TRUE;
						break;
					}

				}
				if (flag == TRUE) continue;
				S = Loc_Cut(NI1.GetId(), NI2.GetId(), SC_bar, SC, k);
				if (!S.Empty()) {

					return S;
				}
			}
		}
	}
	
	return {};
}

TUNGraV VCCE_S::Overlap_Partition(PUNGraph G, TIntV Vertex_Cut) {
	//params:
	//	G: Undirected Graph
	//	S: Vertex_Cut
	//return:
	//	Overlap_Partition Graphs (Type: TUNGraV )
	PUNGraph G_ = TUNGraph::New();
	*G_ = *G;
	TUNGraV G_set;
	//printf("Vertex_Cut: %d\n", Vertex_Cut.Len());
	//printf("G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
	TSnap::DelNodes(G_, Vertex_Cut); // 相应的边会自动删除吗 To test
	//printf("G_: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G_), TSnap::CntUniqUndirEdges(G_));
	TCnComV ConV;
	TSnap::GetSccs(G_, ConV);
	//printf("ConV: %d\n", ConV.Len());
	for (TCnComV::TIter I = ConV.BegI(); I < ConV.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		for (TIntV::TIter NI = Vertex_Cut.BegI(); NI < Vertex_Cut.EndI(); NI++) {
			I->Add(NI->Val);
			//printf("%d\n", NI->Val);
		}
		//继承strong side vertex
		PUNGraph GI = TSnap::GetSubGraph(G, I->NIdV);
		Check_SSV(G, GI, Vertex_Cut);
		//printf("%d\n", G->SSV.Len());
		/*for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {
			if (GI->IsNode(*NI)) GI->SSV.Add(*NI);
		}*/
		G_set.Add(GI);
	}



	return G_set;
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
	SC->SSV = G->SSV;
	SC->IsSub = G->IsSub;
	return SC;

}

void VCCE_S::Detect_SSV(PUNGraph& G, int k)
{	

	//判断是否为子图，若是，可利用P57 Lemma11、12推导出的加快检测SSV的方法；
	//Todo: P57 Lemma11、12推导出的加快检测SSV的方法
	//printf("%d\n", G->IsSub);
	if (G->IsSub) {
		for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {
			if (!IsSSV(G, *NI)) {
				G->SSV.Del(*NI);
				//printf("%d\n", NI.GetId());
			}
		}
	}
	else {
		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			if (IsSSV(G, NI.GetId())) {
				G->SSV.Add(NI.GetId());
				//printf("%d\n", NI.GetId());
			}
		}
	}
	
}



void VCCE_S::Sweep(PUNGraph G, int u, int v, TIntH& pru, TIntH& deposit, MyTCnComV& CS) {
	//printf("%d\n", pru.IsKey(v));
	
	pru[v] = TRUE;
	//pru.get
	int e;
	PUNGraph Neigh = TSnap::GetEgonet(G, v, e);
	for (TUNGraph::TNodeI NI = Neigh->BegNI(); NI < Neigh->EndNI(); NI++) {
		int w = NI.GetId();
		//printf("%d: %d\n", w, pru.IsKey(w));
		if (pru[w] == FALSE) {
			deposit[w]++;
			//printf("deposit[w]: %d\n", deposit[w]);
			if (G->SSV.IsIn(v) || deposit[w] >= k)
				Sweep(G, u, w, pru, deposit, CS);
		}	
	}
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		if (I->IsNIdIn(v) && I->processed == FALSE) {
			I->g_deposit++;
			if (G->SSV.IsIn(v) || I->g_deposit >= k) {
				I->processed = TRUE;
				PUNGraph CC = TSnap::GetSubGraph(G, I->NIdV);
				for (TUNGraph::TNodeI NI = CC->BegNI(); NI < CC->EndNI(); NI++) {
					int w = NI.GetId();
					if (pru[w] == FALSE)
						Sweep(G, u, w, pru, deposit, CS);
				}
			}
			break;
		}
	}

}

bool VCCE_S::IsSSV(PUNGraph G, int NId) {

	int e;
	//std::set<int> set1, set2, set3;
	PUNGraph Neigh = TSnap::GetEgonet(G, NId, e); //N(u)
	for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {

		//set1.clear();
		//PUNGraph Neigh1 = TSnap::GetEgonet(G, NI1.GetId(), e);
		//for (TUNGraph::TNodeI NI = Neigh1->BegNI(); NI < Neigh1->EndNI(); NI++) {
		//	set1.insert(NI.GetId());
		//}

		for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {

			//set2.clear();
			//set3.clear();

			if (NI1 == NI2) continue;
			if (!G->IsEdge(NI1.GetId(), NI2.GetId())) return FALSE;

			//PUNGraph Neigh2 = TSnap::GetEgonet(G, NI2.GetId(), e);
			//for (TUNGraph::TNodeI NI = Neigh2->BegNI(); NI < Neigh2->EndNI(); NI++) {
			//	set2.insert(NI.GetId());
			//}
			//std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::insert_iterator<std::set<int>>(set3, set3.begin()));
			//if (set3.size() < k) return FALSE;

		}
	}

	return TRUE;
}

void VCCE_S::Check_SSV(PUNGraph& G, PUNGraph& subG, TIntV S) {

	//判断是否为子图，若是，可利用P57 Lemma11、12推导出的加快检测SSV的方法；
	//Todo: P57 Lemma11、12推导出的加快检测SSV的方法
	//printf("%d\n", G->IsSub);
	subG->IsSub = TRUE;
	subG->SSV = G->SSV;

	//std::set<int> set1, set2, set3;
	//for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {
	//	set1.clear();
	//	set2.clear();
	//	set3.clear();
	//	int e;
	//	//printf("%d\n", *NI);
	//	PUNGraph Neigh = TSnap::GetEgonet(G, *NI, e);
	//	for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {
	//		set1.insert(NI1.GetId());
	//	}
	//	for (TIntV::TIter NI2 = S.BegI(); NI < S.EndI(); NI++) {
	//		set2.insert(*NI2);
	//	}

	//	std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::insert_iterator<std::set<int>>(set3, set3.begin()));
	//	if (set3.empty())
	//		subG->SSV.Add(*NI);
	//}

}