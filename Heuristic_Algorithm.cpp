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

TIntVIntV BkVCC::BkVCC_ENUM(PUNGraph &G, int k)
{
	TIntVIntV G_R, G_S, G_S_prime; 

	//find k-core
	G = TSnap::GetKCore(G, k);
	printf("G[%d-core]: \nnode_nums = %d, edge_nums = %d\n", k, G->GetNodes(), G->GetEdges());
	//Seeding
	G_S = Seeding(G, k);

	do {
		G_S_prime = G_S;
		//expanding G_S
		Expanding(k, G_S);
		//merging G_S
		Merging(k, G_S, G_R);
		
	} while (G_S != G_S_prime); // TODO: 如何判断是否发生变化，仅靠长度肯定是不行的 FINISH:用顶点数组表示一张图，对比数组即可

	// G_R = G_R Union G_S
	// 
	// error can not use this function
	// 无法比较两个图是否相同，所以sort相关的函数比如AddMergedV(), Merge()都不能使用，除非重载运算符
	G_R.AddVMerged(G_S);


	return G_R;
}

TIntV BkVCC::LkVCS(PUNGraph G, int k, int u, int alpha)
{
	// params: 
	//	  alpha: upper bound for selecting subset with size k


	PUNGraph P = TSnap::GetEgonetHop(G, u, 2); // P = N_2(u)

	//printf("P: \nnode_nums = %d, edge_nums = %d\n", P->GetNodes(), P->GetEdges());
	
	PUNGraph P_prime = TSnap::GetKCore(P, k);
	//for (TUNGraph::TNodeI NI1 = P_prime->BegNI(); NI1 < P_prime->EndNI(); NI1++) {
	//	cout << NI1.GetId() << " ";
	//}
	//cout << endl;
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
	nb_set.Add(u);

	TIntVIntV res = subsets(nb_set, k, alpha);

	for (int i = 0; i < res.Len() && i <= alpha; i++) {

		TIntV R = res[i];
		R.AddUnique(u); // R = R Union u
		R.Merge();
		
		/*for (TIntV::TIter TI = R.BegI(); TI < R.EndI(); TI++) {
			cout << *TI << " ";
		}
		cout << endl;*/
		bool direct = true;
		while (direct) {

			//R may be updated
			PUNGraph G_R = TSnap::GetSubGraph(G, R);
			//set flag1 = ...
			if (flag1(G_R)) {
				//cout << "flag1" << endl;
				return R;
			}
			else {
				// set flag2 = ...
				if (flag2(P_prime, G_R)) {
					//cout << "flag2" << endl;
					direct = false;
				}
				else {
					//cout << "flag3" << endl;
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
			/*cout << NI1.GetId() << " " << NI2.GetId() << endl;*/
			//printf("G: \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
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
			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId())) continue;
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
	int i = 0;
	int alpha = 1000;

	try
	{
		TFIn inFile("./seedgraph/" + dataset + ".seed");
		G_S.Load(inFile);
	}
	catch (TPt<TExcept>)
	{
		cout << endl << "***seedgraph does not exist, running seeding function...***" << endl;
	}
	
	if (G_S.Len() != 0) return G_S;


	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		CandMaintain.AddDat(NI.GetId(), 0);
		deg.AddDat(NI.GetId(), NI.GetDeg());
	}
	deg.SortByDat(); //non-decreasing order

	for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
		//cout << HI.GetKey() <<" " << HI.GetDat() << endl;

		int v = HI.GetKey();
		if (CandMaintain.GetDat(v) == 0) {
			G_C = LkVCS(G, k, v, alpha);
			//cout<< ++i <<endl;
			if (G_C.Empty()) continue;

			G_S.AddUnique(G_C); //怎么判断是否相等（Unique）？
			//G_S.Add(G_C);
			

			for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {
				CandMaintain.GetDat(*TI) = 1;
			}
		}
		if (CandMaintain.GetDat(v) == 1) {
			//cout << "sweeped" << endl;
		}
	}

	TFOut outFile("./seedgraph/" + dataset + ".seed");
	G_S.Save(outFile);
	return G_S;
}

//void printNodesNum(PUNGraph G) {
//	cout << "Nodes:"<< G->GetNodes() << endl;
//}

void BkVCC::Expanding(int k, TIntVIntV& G_S)
{
	int u;
	TIntV delta_S, delta_S_bar;
	for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
		//for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
		//	cout << *NI << " ";
		//}
		//cout << endl;
		while (flag3(*GI, u, delta_S, delta_S_bar)) { //exist u belong delta_S, intersection > k
			
			/*Update(G, *GI, u);*/
			GI->AddUnique(u);
			/*cout << "Nodes:" << GI->Len() << endl;*/  //Test if graph updates valid
		}
		GI->Merge();
	}
	G_S.Merge();
	return;
}

TIntV BkVCC::GetBoundary(TIntV G_S, TIntV &delta_S_bar)
{
	TIntV delta_S;
	delta_S.Clr();
	delta_S_bar.Clr();
	//PUNGraph G_S_Graph = TSnap::GetSubGraph(G, G_S);
	//cout << G_S_Graph->GetNodes() << endl;
	//for (TUNGraph::TNodeI NI = G_S_Graph->BegNI(); NI < G_S_Graph->EndNI(); NI++) {
	//	cout << NI.GetId() << " ";
	//}
	//cout << endl;
	//for (TUNGraph::TNodeI NI = G_S_Graph->BegNI(); NI < G_S_Graph->EndNI(); NI++) {
	//	cout << NI.GetId() << endl;
	//	for (int i = 0; i < NI.GetInDeg(); i++) {
	//		cout << "nbr:" << NI.GetNbrNId(i) << endl;
	//		if (!G_S_Graph->IsNode(NI.GetNbrNId(i))) {
	//			delta_S.Add(NI.GetId());
	//			delta_S_bar.Add(NI.GetNbrNId(i));
	//			//break;
	//		}
	//	}
	//}
	/*cout << G_S.Len() << endl;*/
	//for (TIntV::TIter NI = G_S.BegI(); NI < G_S.EndI(); NI++) {
	//	cout << *NI << " ";
	//}
	//cout << endl;
	for (TIntV::TIter NI = G_S.BegI(); NI < G_S.EndI(); NI++) {
		/*cout << *NI << endl;*/
		TUNGraph::TNodeI Node = G->GetNI(*NI);
		for (int i = 0; i < Node.GetInDeg(); i++) {
			/*cout << "nbr:" << Node.GetNbrNId(i) << endl;*/
			if (!G_S.IsIn(Node.GetNbrNId(i))) {
				delta_S.Add(Node.GetId());
				delta_S_bar.Add(Node.GetNbrNId(i));
				//break;
			}
		}
	}
	delta_S.Merge();
	delta_S_bar.Merge();
	//cout << delta_S.Len() << endl;
	//cout << delta_S_bar.Len() << endl;
	
	return delta_S;
}



bool BkVCC::flag3(TIntV G_S, int& u, TIntV& delta_S, TIntV& delta_S_bar)
{
	
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;
	//cout << "delta_S: ";
	//for (TIntV::TIter TI = delta_S.BegI(); TI < delta_S.EndI(); TI++) {
	//	cout << *TI <<" ";
	//}
	//cout << endl;
	//cout << "delta_S_bar: ";
	//for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
	//	cout << *TI << " ";
	//}
	//cout << endl;
	//cout << delta_S.Len() << endl;
	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = G->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (!nb_u.IsIn(CtrNode.GetInNId(i)))
				nb_u.Add(CtrNode.GetInNId(i));
		}
		/*cout << nb_u.Len() << endl;*/
		//cout << "nb_u: ";
		//for (TIntV::TIter TI = nb_u.BegI(); TI < nb_u.EndI(); TI++) {
		//	cout <<*TI<<" ";
		//}
		//cout << endl;
		//cout << "nb_nums: " << TSnap::GetCommon(nb_u, delta_S) << endl;
		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
			u = *TI;
			return true;
		}
	}
	return false;
}

//void BkVCC::Update(PUNGraph G, TIntV& GI, int u)
//{
//	TIntV Nodes;
//	GI->GetNIdV(Nodes);
//	Nodes.Add(u);
//	GI = TSnap::GetSubGraph(G, Nodes);
//	GI.Add(u);
//}



void BkVCC::Merging(int k, TIntVIntV& G_S, TIntVIntV& G_R)
{
	TIntV GI1, GI2;
	//cout << G_S.Len() << endl;
	TIntVIntV G_S_temp = G_S;
	for (TIntVIntV::TIter GI = G_S_temp.BegI(); GI < G_S_temp.EndI(); GI++) {
		/*cout << "GI: ";
		for (TIntV::TIter TI = GI->BegI(); TI < GI->EndI(); TI++) {
			cout << *TI << " ";
		}
		cout << endl;*/
		if (IskVCC(*GI, k)) {
			G_R.Add(*GI);
			G_S.DelIfIn(*GI);
		}
	}
	TPrVIntV G_Merge_cand = flag4(G_S);
	//cout << "Len: " << G_Merge_cand.Len() << endl;

	for (TPrVIntV::TIter PI = G_Merge_cand.BegI(); PI < G_Merge_cand.EndI(); PI++) {
		GI1 = PI->GetVal1();
		GI2 = PI->GetVal2();
		//cout << "GI1: ";
		//for (TIntV::TIter TI = GI1.BegI(); TI < GI1.EndI(); TI++) {
		//	cout << *TI <<" ";
		//}
		//cout << endl;
		
		//cout << "GI2: ";
		//for (TIntV::TIter TI = GI2.BegI(); TI < GI2.EndI(); TI++) {
		//	cout << *TI << " ";
		//}
		//cout << endl;
		//exist GI1, GI2 satisfied IsMergeValid
		//PUNGraph G_Union = Merge2Graph(G, GI1, GI2);
		TIntV G_Union = {};
		G_Union.AddVMerged(GI1);
		G_Union.AddVMerged(GI2);
		//cout << "G_Union: ";
		//for (TIntV::TIter TI = G_Union.BegI(); TI < G_Union.EndI(); TI++) {
		//	cout << *TI << " ";
		//}
		//cout << endl;
		if (IskVCC(G_Union, k)) {
			G_R.Add(G_Union);
			G_S.DelIfIn(GI1);
			G_S.DelIfIn(GI2);
		}
		else {
			G_S.Add(G_Union);
			G_S.DelIfIn(GI1);
			G_S.DelIfIn(GI2);
		}

	}
	
	return;
}

//为什么这里同样的代码能出现不一样的结果？laugh_cry..
TPrVIntV BkVCC::flag4(TIntVIntV G_S)
{
	TPrVIntV G_Merge_cand = {};
	for (TIntVIntV::TIter GI1 = G_S.BegI(); GI1 < G_S.EndI(); GI1++) {
		for (TIntVIntV::TIter GI2 = GI1; GI2 < G_S.EndI(); GI2++) {
			if (*GI2 == *GI1) continue;
			if (IsMergeValid(*GI1, *GI2)) {
				//G1 = *GI1;
				//G2 = *GI2;
				G_Merge_cand.AddMerged(TPair<TIntV, TIntV>(*GI1, *GI2));
				//return true;
			}
		}
	}
	//return false;
	return G_Merge_cand;
}

bool BkVCC::IskVCC(TIntV G_S, int k)
{
	//Corollary 1
	int u;
	TIntV delta_S, delta_S_bar;
	if (!flag3(G_S, u, delta_S, delta_S_bar) && min(delta_S.Len(), delta_S_bar.Len()) < k) {
		return true;
	}
	return false;
}

bool BkVCC::IsMergeValid(TIntV G_S, TIntV G_S_prime)
{
	//Theorem 7
	TIntV S_Nodes, S_prime_Nodes, S_ints_S_prime, S_sub_S_prime, S_prime_sub_S;
	int nb_1 = 0, nb_2 = 0;
	S_Nodes = G_S;
	S_prime_Nodes = G_S_prime;
	//S_ints_S_prime.Clr();
	//S_sub_S_prime.Clr();
	//S_prime_sub_S.Clr();
	//G_S->GetNIdV(S_Nodes);
	//G_S_prime->GetNIdV(S_prime_Nodes);
	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
		if (S_prime_Nodes.IsIn(*TI)) {
			S_ints_S_prime.Add(*TI);
		}
	}

	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
		if (!S_ints_S_prime.IsIn(*TI)) {
			S_sub_S_prime.Add(*TI);
		}
	}
	for (TIntV::TIter TI = S_prime_Nodes.BegI(); TI < S_prime_Nodes.EndI(); TI++) {
		if (!S_ints_S_prime.IsIn(*TI)) {
			S_prime_sub_S.Add(*TI);
		}
	}

	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		TUNGraph::TNodeI NI = G->GetNI(*TI);
		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
				nb_1++;
			}
		}
	}

	for (TIntV::TIter TI = S_prime_sub_S.BegI(); TI < S_prime_sub_S.EndI(); TI++) {
		TUNGraph::TNodeI NI = G->GetNI(*TI);
		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_sub_S_prime.IsIn(NI.GetNbrNId(i))) {
				nb_2++;
			}
		}
	}

	if (S_ints_S_prime.Len() + min(nb_1, nb_2) >= k) {
		return true;
	}
	
	return false;
}

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

