#include "Snap.h"
#include "KVCC_Algorithm.h"
int main() {	
	//int k = 3;
	//PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/DBLP.txt", 0, 1);
	//printf("G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
	///*for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) 
	//	printf("%d %d %d\n", NI.GetId(), NI.GetOutDeg(), NI.GetInDeg());*/

	//PUNGraph G2 = TSnap::GetKCore(G, k);
	//printf("G2: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G2), TSnap::CntUniqUndirEdges(G2));
	//TCnComV ConV;
	//TSnap::GetSccs(G2, ConV);
	////printf("CnCom_nums = %d", ConV.Len());
	////printf("length % d\n", ConV.Len()); // -- 3
	////printf("ConV[0] % d\n", ConV[0].Len()); // -- 10
	//TVec<PUNGraph> G_set;
	//for (TCnComV::TIter C = ConV.BegI(); C < ConV.EndI(); C++) {
	//	printf("%d\n", C->NIdV);
	//	G_set.Add(TSnap::GetSubGraph(G2, C->NIdV));
	//}
	////TVec<TUNGraph> G_set = 
	//printf("G_set len = %d", G_set.Len());
	int k = 3;
	//PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/DBLP.txt", 0, 1);
	
    PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/paper_fig.txt", 0, 1);
	PUNGraph G2 = TSnap::GetKCore(G, k);
	//PNGraph Graph = TSnap::GenRndGnm<PNGraph>(100, 1000);
	//PUNGraph G = TSnap::ConvertGraph<PUNGraph, PNGraph>(Graph);
	//printf("%d", )
	printf("G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
	printf("G2: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G2), TSnap::CntUniqUndirEdges(G2));
	PUNGraph SC = Compute_SC(G2, k);
	//printf("SC: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(SC), TSnap::CntUniqUndirEdges(SC));
	//TUNGraV G_set = KVCC_ENUM(G, k);	

	PNEANet SC_bar = Construct_DG(SC);
	//PNEANet SC_bar = Construct_DG(G);
	printf("SC_bar : \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(SC_bar), TSnap::CntUniqUndirEdges(SC_bar));
	//int offset = SC->GetMxNId();
	int offset = G->GetMxNId();
	//printf("%d\n", offset);
	int MaxFlow = 0;
	TIntV ResNet;
	PNEANet SC_bar2 = TNEANet::New();
	*SC_bar2 = *SC_bar;
	int n = TSnap::MyGetMaxFlowIntEK(SC_bar, 0 + offset, 17, ResNet);
	if (n < k) {
		for (int i = 0; i < ResNet.Len(); i++) {
			//printf("%d: %d\n", i, ResNet[i]);
			if (ResNet[i] == 1) {
				printf("%d: %d->%d\n", i, SC_bar->GetEI(i).GetSrcNId(), SC_bar->GetEI(i).GetDstNId());
				SC_bar->DelEdge(SC_bar->GetEI(i).GetSrcNId(), SC_bar->GetEI(i).GetDstNId());
			}
		}
	}
	PNGraph BFSTree1, BFSTree2;
	BFSTree1 = TSnap::MyGetBfsTree(SC_bar, 0+offset, TRUE, FALSE);
	BFSTree2 = TSnap::MyGetBfsTree(SC_bar, 17, FALSE, TRUE);
	//for (TNGraph::TNodeI TNI = BFSTree->BegNI(); TNI < BFSTree->EndNI(); TNI++) {
	//	printf("%d\n", TNI.GetId());

	//}
	printf("SC_bar : \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(SC_bar), TSnap::CntUniqUndirEdges(SC_bar));
	printf("SC_bar2 : \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(SC_bar2), TSnap::CntUniqUndirEdges(SC_bar2));
	for (TNEANet::TEdgeI EI = SC_bar2->BegEI(); EI < SC_bar2->EndEI(); EI++) {
		if (BFSTree1->IsNode(EI.GetSrcNId())&& BFSTree2->IsNode(EI.GetDstNId())) {
			printf("%d->%d\n",EI.GetSrcNId(), EI.GetDstNId());
		}
	}
	//for (TUNGraph::TNodeI NI1 = SC->BegNI(); NI1 < SC->EndNI(); NI1++) {
	//	for (TUNGraph::TNodeI NI2 = NI1; NI2 < SC->EndNI(); NI2++) {
	//		//if (NI2 == NI1) NI2++;
	//		//printf("Nid %d->Nid %d MaxFlow : %d\n", NI1.GetId(), NI2.GetId(), TSnap::GetMaxFlowIntEK(G_bar, NI1.GetId() + offset, NI2.GetId()));
	//		//if (!SC_bar->IsNode(NI2.GetId())) continue;
	//		//printf("%d  %d\n", NI1.GetId(), NI2.GetId());
	//		int n = TSnap::MyGetMaxFlowIntEK(SC_bar, NI1.GetId() + offset, NI2.GetId(), ResNet);
	//		if (n < 3) {
	//			for (int i = 0; i < ResNet.Len(); i++) {
	//				//printf("%d: %d\n", i, ResNet[i]);
	//				if (ResNet[i] > 0) {
	//					printf("%d->%d\n", SC_bar->GetEI(ResNet[i]).GetSrcNId(), SC_bar->GetEI(ResNet[i]).GetDstNId());
	//				}
	//			}
	//		}
	//		
	//		//if (n > MaxFlow) {
	//		//	MaxFlow = n;
	//		//	printf("%d\n", MaxFlow);
	//		//}
	//	}
	//}


	//for (int NId1 = offset; NId1 < 2*offset; NId1++) {
	//	for (int NId2 = 0; NId2 < offset-1; NId2++) {
	//		printf("Nid %d->Nid %d MaxFlow : %d\n", NId1 - offset, NId2, TSnap::GetMaxFlowIntEK(G_bar, NId1, NId2));
	//	} 
	//}

	//PNEANet SC_bar = Construct_DG(SC);
	//int offset = SC->GetMxNId();
	//printf("%d\n", offset);

	//for (int NId1 = offset; NId1 < 2 * offset; NId1++) {
	//	for (int NId2 = 0; NId2 < offset - 1; NId2++) {
	//		printf("Nid %d->Nid %d MaxFlow : %d\n", NId1 - offset, NId2, TSnap::GetMaxFlowIntEK(SC_bar, NId1, NId2));
	//	}
	//}


	//TSnap::DrawGViz<PNEANet>(G_bar, gvlDot, "paper_fig_bar.png", "", "");

		
	

}