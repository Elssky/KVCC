#include"Sweep_Algorithm.h"
#include"set"
#include"vector"
#include <map>
#include"algorithm"
#include <iterator>


VCCE_S::VCCE_S(PUNGraph G_, int k_) {
	G = G_;
	k = k_;
	Nodes = G->GetNodes();
	//Nodes = G->GetMxNId() + 1;
	int Mxnode = G->GetMxNId();
	vertex_map_ = new int[Mxnode];
	memset(vertex_map_, 0, sizeof(int) * Mxnode);
	int i = 0;
	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		vertex_map_[NI.GetId()] = i++;
	}
}

TUNGraV VCCE_S::KVCC_ENUM(PUNGraph G, int k, int flag) {
	//func: Find k-VCCs in Graph G
	//params:
	//	G: Undirected Graph
	//	k: an integer
	//return:
	//	all k-vertex connected components (Type: TUNGraV )

	//step1: initialize set VCC as empty
	TUNGraV VCC;  // return as K-Vccs;

	//step2: remove vertices u and incident edges, that d(u) < k
	PUNGraph G2 = TSnap::GetKCore(G, k);


	for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {	//G2->SSV = G->SSV(node in G2)
		if (G2->IsNode(*NI)) G2->SSV.Add(*NI);
	}
	//if (flag == 0) {//G2->SSV.Len() == 0
	//	//printf("G2_");
	//	//Detect_SSV(G2);
	//	Get_SSV(G2);
	//	flag = 1;
	//}
	printf("flag:%d\n", flag);
	//step3: identify connected compotent set in G
	TCnComV ConV;
	TSnap::GetSccs(G2, ConV);
	TVec<PUNGraph> G_set; // Connected Component in G2;
	for (TCnComV::TIter I = ConV.BegI(); I < ConV.EndI(); I++) {
		PUNGraph GI = TSnap::GetSubGraph(G2, I->NIdV);
		//GI->SSV = G2->SSV;
		for (TIntV::TIter NI = G2->SSV.BegI(); NI < G2->SSV.EndI(); NI++) { //or GI->SSV = G2->SSV;
			if (GI->IsNode(*NI)) GI->SSV.Add(*NI);
		}
		G_set.Add(GI);
	}
	printf("G_set_len: %d\n", G_set.Len());
	
	//step4: find vertex cut in G
	TIntV S; //Vertex_Cut

	for (TUNGraV::TIter GI = G_set.BegI(); GI < G_set.EndI(); GI++) {
		//clock_t t1 = clock();
		S = Global_Cut(*GI, k, flag);
		//m++;
		//_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
		//printf("SSV_len: %d\n", (*GI)->SSV.Len());
		//printf("%d\n", S.Empty());
		if (S.Empty()) {

			VCC.Add(*GI);
		}
		else {
			TUNGraV G_i = Overlap_Partition(*GI, S);
			//printf("%d\n", G_i.Len());
			for (TUNGraV::TIter G_ij = G_i.BegI(); G_ij < G_i.EndI(); G_ij++) {
				//printf("SSV_len: %d\n", (*G_ij)->SSV.Len());
				TUNGraV VCC_i = KVCC_ENUM(*G_ij, k, 1);
				VCC.AddV(VCC_i);
				//printf("%d\n", VCC_i.Len());

			}
		}

	}

	return VCC;
}

TIntV VCCE_S::Global_Cut(PUNGraph &G, int k, int flag)
{
	
	TIntV S;
	MyTCnComV CS;	
	int u, v;
	
	//1. compute sparse certification SC

	PUNGraph SC = Compute_SC(G, k, CS);
	TIntH deposit, dist, pru;
	//SC->SSV = G->SSV;
	printf("SSV_len: %d\n", SC->SSV.Len());
	//2. direct graph
	PNEANet SC_bar = Construct_DG(SC);
	
	//3. detect strong side vertices
	//if(flag == 0) 
		Get_SSV(SC);
	//printf("SC_");
	//Detect_SSV(SC);
	G->SSV = SC->SSV;

	
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
	
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		I->g_deposit = 0;
	}
	
	for (TUNGraph::TNodeI NI = SC->BegNI(); NI < SC->EndNI(); NI++) {
		deposit.AddDat(NI.GetId(), 0);
		pru.AddDat(NI.GetId(), 0);
	}

	
	TSnap::GetShortPath(G, u, dist, FALSE);
	dist.SortByDat(FALSE);


	//5.Phase1 & Sweep
	Sweep(SC, u, pru, deposit, CS);
	
	for (TIntH::TIter HI = dist.BegI(); HI < dist.EndI(); HI++) {
		
		v = HI.GetKey();
		if (pru.GetDat(v) == TRUE) {
			pru_node++;
			continue;
		}
		S = Loc_Cut(u, v, SC_bar, SC, k);
		non_pru_node++;
		if (!S.Empty()) {
			
			return S;
		}
		Sweep(SC, v, pru, deposit, CS);
		
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
					if (I->IsNIdIn(NI1.GetId()) || I->IsNIdIn(NI2.GetId())) {
						if (I->IsNIdIn(NI1.GetId()) && I->IsNIdIn(NI2.GetId())) {
							flag = TRUE;
						}
						break;
					}
				}
				if (flag == TRUE) {					
					continue;
				}
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
		for (TIntV::TIter NI = Vertex_Cut.BegI(); NI < Vertex_Cut.EndI(); NI++) {
			I->Add(NI->Val);		
		}
		//继承strong side vertex
		PUNGraph GI = TSnap::GetSubGraph(G, I->NIdV);
		Check_SSV(G, GI, Vertex_Cut);
		G_set.Add(GI);
	}



	return G_set;
}

PUNGraph VCCE_S::Compute_SC(PUNGraph G, int k, MyTCnComV &CS)
{
	
	PUNGraph SC = TUNGraph::New();
	PUNGraph G_ = TUNGraph::New();
	*G_ = *G;
	PNGraph BFSTree;
	PUNGraph F_k = TUNGraph::New();//Forest k
	int NodeNums = TSnap::CntNonZNodes(G_);
	std::vector<int> visited; //mark visited node
	std::vector<int> del;  //fix wrong BFS Tree
	for (int i = 0; i < k; i++) {
		visited.clear();
		//printf("F%d:\n", i);
		int j = 0;
		
		while (visited.size() < NodeNums) {
			for (TUNGraph::TNodeI NI = G_->BegNI(); NI < G_->EndNI(); NI++) {

				if (std::find(visited.begin(), visited.end(), NI.GetId()) == visited.end()) {
					
					BFSTree = TSnap::MyGetBfsTree(G_, NI.GetId(), TRUE, FALSE);  //My BFS Function modified in bfsdfs.h
					del.clear();
					

					for (TNGraph::TNodeI TNI = BFSTree->BegNI(); TNI < BFSTree->EndNI(); TNI++) {
						visited.push_back(TNI.GetId());
						if (i == 0) {
							if (!SC->IsNode(TNI.GetId()))
								SC->AddNode(TNI.GetId());
						}
						if (i == k - 1) {
							F_k->AddNode(TNI.GetId());
						}
					}

					for (TNGraph::TEdgeI TEI = BFSTree->BegEI(); TEI < BFSTree->EndEI(); TEI++) {
						SC->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						G_->DelEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						if (i == k - 1) {
							F_k->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						}
					}
				}
			}
		}
	}
	TSnap::GetSccs(F_k, CS);
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {	
		if (I->Len() < 2) {
			CS.DelAll(*I);
		}
	}

	return SC;

}


//void VCCE_S::Detect_SSV(PUNGraph& G)
//{	
//
//	//判断是否为子图，若是，可利用P57 Lemma11、12推导出的加快检测SSV的方法；
//	//Todo: P57 Lemma11、12推导出的加快检测SSV的方法
//	//printf("Len:%d\n", G->SSV.Len());
//	if (G->SSV.Len() > 0) {
//		for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {
//			if (!IsSSV(G, *NI)) {
//				G->SSV.Del(*NI);
//			}
//		}
//	}
//	else {
//		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
//			if (IsSSV(G, NI.GetId())) {
//				G->SSV.Add(NI.GetId());
//			}
//		}
//	}
//	
//}



void VCCE_S::Sweep(PUNGraph G, int v, TIntH& pru, TIntH& deposit, MyTCnComV& CS) {
	//printf("%d\n", pru.IsKey(v));
	
	pru.GetDat(v) = TRUE;
	int e;
	PUNGraph Neigh = TSnap::GetEgonet(G, v, e);
	for (TUNGraph::TNodeI NI = Neigh->BegNI(); NI < Neigh->EndNI(); NI++) {
		int w = NI.GetId();
		if (pru.GetDat(w) == FALSE) {
			deposit.GetDat(w)++;
			if (G->SSV.IsIn(v) || deposit.GetDat(w) >= k)
				Sweep(G, w, pru, deposit, CS);
		}	
	}
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		if (I->IsNIdIn(v)) {
			if (I->processed == FALSE) {
				I->g_deposit++;
				if (G->SSV.IsIn(v) || I->g_deposit >= k) {
					I->processed = TRUE;
					PUNGraph CC = TSnap::GetSubGraph(G, I->NIdV);
					for (TUNGraph::TNodeI NI = CC->BegNI(); NI < CC->EndNI(); NI++) {
						int w = NI.GetId();
						if (pru.GetDat(w) == FALSE)
							Sweep(G, w, pru, deposit, CS);
					}
				}
			}
			break;	
		}
	}

}
void VCCE_S::Get_SSV(PUNGraph &G) {
	printf("call here\n");
	clock_t t1 = clock();
	//int Nodes = G->GetNodes(); //Nodes 
	int** Com_Neighbor = new int* [Nodes]; //Com_Neighbor[Nodes][Nodes]
	for (int i = 0; i < Nodes; i++) {
		Com_Neighbor[i] = new int[Nodes];
		memset(Com_Neighbor[i], 0, sizeof(int)* Nodes);
	}

	
	for(TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		for (int i = 0; i < NI.GetInDeg(); i++) {
			int node1 = vertex_map_[NI.GetInNId(i)];			
			for (int j = i; j < NI.GetInDeg(); j++) {
				if (i == j) continue;
				int node2 = vertex_map_[NI.GetInNId(j)];
				Com_Neighbor[node1][node2]++;
			}
		}

	}
	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		if (IsSSV(G, NI.GetId(), Com_Neighbor))
			G->SSV.Add(NI.GetId());
	}
	printf("%d\n", G->SSV.Len());
	for (int i = 0; i < Nodes; i++)
		delete[] Com_Neighbor[i];
	delete[] Com_Neighbor;

	m++;
	_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
}

bool VCCE_S::IsSSV(PUNGraph G, int NId, int** Com_Neighbor) {
	TUNGraph::TNodeI NI = G->GetNI(NId);
	for (int i = 0; i < NI.GetInDeg(); i++) {
		int node1 = vertex_map_[NI.GetInNId(i)];
		for (int j = i; j < NI.GetInDeg(); j++) {
			if (i == j) continue;
			int node2 = vertex_map_[NI.GetInNId(j)];
			if (!G->IsEdge(NI.GetInNId(i), NI.GetInNId(j))) return FALSE;
			if (Com_Neighbor[node1][node2] < k) {
				return FALSE;
			}

		}
	}
	return TRUE;
}
//bool VCCE_S::IsSSV(PUNGraph G, int NId) {
//
//	int e;
//	std::set<int> set1, set2, set3;
//	PUNGraph Neigh = TSnap::GetEgonet(G, NId, e); //N(u)
//	for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {
//
//		set1.clear();
//		
//		for (int i = 0; i < NI1.GetInDeg(); i++) {
//			set1.insert(NI1.GetInNId(i));
//		}
//
//		for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {
//
//			set2.clear();
//			set3.clear();
//
//			if (NI1 == NI2) continue;
//			if (!G->IsEdge(NI1.GetId(), NI2.GetId())) return FALSE;
//			for (int i = 0; i < NI2.GetInDeg(); i++) {
//				set2.insert(NI2.GetInNId(i));
//			}
//
//			std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::insert_iterator<std::set<int>>(set3, set3.begin()));
//
//			if (set3.size() < k) return FALSE;
//
//		}
//	}
//
//	return TRUE;
//}

void VCCE_S::Check_SSV(PUNGraph& G, PUNGraph& subG, TIntV S) {

	//判断是否为子图，若是，可利用P57 Lemma11、12推导出的加快检测SSV的方法；
	//Todo: P57 Lemma11、12推导出的加快检测SSV的方法
	//printf("%d\n", G->IsSub);

	std::set<int> set1, set2, set3;

	for (TIntV::TIter NI2 = S.BegI(); NI2 < S.EndI(); NI2++) {
		set2.insert(*NI2);
	}
	
	for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {
		set1.clear();
		set3.clear();

		TUNGraph::TNodeI NI1 = G->GetNI(*NI);
		for (int i = 0; i < NI1.GetInDeg(); i++) {
			set1.insert(NI1.GetInNId(i));
		}
		std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::insert_iterator<std::set<int>>(set3, set3.begin()));
		if (set3.empty())
			subG->SSV.Add(*NI);
	}

}