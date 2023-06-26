#include "Heuristic_Algorithm.h"
#include"Sweep_Algorithm.h"
#include <queue>


char* k_str = new char[5];
char* alpha_str = new char[10];
//char* alg_str = new char[20];
//string alg[6] = { "VCCE", "BkVCC", "BkVCC(clique)", "BkVCC(k+1 clique)","BkVCC_flow", "BkVCC(clique)_flow" };


BkVCC::BkVCC()
{


}

BkVCC::BkVCC(PUNGraph G_, int k_)
{
	G = G_;
	k = k_;
}

TIntVIntV BkVCC::BkVCC_ENUM(PUNGraph& G, int k, int alpha)
{
	omp_set_num_threads(threads);
	TIntVIntV G_R, G_S, G_S_prime;

	//strcpy(alg_str, alg.c_str());
	itoa(k, k_str, 10);
	itoa(alpha, alpha_str, 10);
	char* k_str = new char[5];
	itoa(k, k_str, 10);


	//find k-core
	G = TSnap::GetKCore(G, k);
	TSnap::SaveEdgeList<PUNGraph>(G, "D:/Git Project/KVCC-ENUM/dataset/" + dataset + "_" + TStr(k_str) + "-core.txt", "");
	printf("G[%d-core]: \nnode_nums = %d, edge_nums = %d\n", k, G->GetNodes(), G->GetEdges());
	//Seeding
	clock_t t1 = clock();
	int j = 0;




	//�����Ѿ����ɵĽ��
	/*try
	{
		TFIn inFile("D:/Git Project/KVCC-ENUM//community/" + dataset + "_k=" + TStr(k_str) + ".kvcc");
		G_R.Load(inFile);
	}
	catch (TPt<TExcept>)
	{
		cout << endl << "***kvcc result does not exist, running BkVCC function...***" << endl;
	}*/

	
	G_S = Seeding(G, k, alpha);
	

	cout << "Seeding subgraph num:" << G_S.Len() << endl;
	_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;

	do {

		G_S_prime = G_S;
		

		//expanding G_S
		if (strcmp(expandMethod.c_str(), "") != 0)
			Expanding(k, G_S);

		if (strcmp(mergeMethod.c_str(), "") != 0) {
			//����˳��
			//G_S.Reverse();
			Merging(k, G_S, G_R);
			/*cout << "after Merging subgraph num:" << G_S.Len() << endl;*/
		}

	
	} while (G_S != G_S_prime); // TODO: ����ж��Ƿ����仯���������ȿ϶��ǲ��е� FINISH:�ö��������ʾһ��ͼ���Ա����鼴��
	//} while (G_S != G_S_prime);

	// G_R = G_R Union G_S
	// 
	// error can not use this function
	// �޷��Ƚ�����ͼ�Ƿ���ͬ������sort��صĺ�������AddMergedV(), Merge()������ʹ�ã��������������

	//09/04 ��ͼ���ڲ�Ҳ����������� ��������������Ԫ��[3,5,7,9,22]��[3,9,5,22,7]��
	//�������ǲ���ϲ��ģ���Ϊmerge�ж���Ҫ��ȫ��ͬ��������Ҫ��ÿ��Ԫ���ڲ��ڽ���һ��merging����
	G_R.AddVMerged(G_S);
	for (TIntVIntV::TIter TI = G_R.BegI(); TI < G_R.EndI(); TI++) {
		TI->Sort();
	}
	G_R.Merge();
	//������
	TFOut outFile("D:/Git Project/KVCC-ENUM/community/" + dataset + "_k=" + TStr(k_str) +
		"_alpha=" + TStr(alpha_str) + "_algorithm=" + TStr(alg.c_str()) + "_expand=" + expandMethod.c_str()
		+ "_merge=" + mergeMethod.c_str() + "_t=" + std::to_string(threads).c_str() + ".kvcc");
	G_R.Save(outFile);

	return G_R;
}


TIntV BkVCC::LkVCS(PUNGraph G, int k, int u, int alpha)
{
	// params: 
	//	  alpha: upper bound for selecting subset with size k


	PUNGraph P = TSnap::GetEgonetHop(G, u, 2); // P = N_2(u)

	//printf("P: \nnode_nums = %d, edge_nums = %d\n", P->GetNodes(), P->GetEdges());

	PUNGraph P_prime = TSnap::GetKCore(P, k);

	/*for (TUNGraph::TNodeI NI1 = P_prime->BegNI(); NI1 < P_prime->EndNI(); NI1++) {
		cout << NI1.GetId() << " ";
	}
	cout << endl;*/

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

	//nb_set.Add(u);

	TIntVIntV res = randSample(nb_set, k + 1, alpha);


	int wr_n1 = 0;
	//int wr_n2 = 0;
	TIntV wr_n2 = {};
	for (int i = 0; i < res.Len() ; i++) {
		TIntV R = res[i];
		R.AddUnique(u); // R = R Union u
		R.Merge();
		int a = 0, b = 0;
		int flag = 0;
		//if(R.IsIn(wr_n1) && R.IsIn(wr_n2)) continue;
		/*if (R.IsIn(wr_n1)) {
			for (int j = 0; j < wr_n2.Len(); j++) {
				if (R.IsIn(wr_n2[j])) {
					flag = 1;
					break;
					
				}
			}	
			if (flag == 1) continue;
		}*/
		
		bool direct = true;
		while (direct) {

			//R may be updated
			PUNGraph G_R = TSnap::GetSubGraph(G, R);
			//set flag1 = ...
			if (flag1(G_R, a, b)) {
				
				//cout << "flag1" << endl;
				/*for (int i = 0; i < R.Len(); i++) {
					cout << i << " ";
				}
				
				cout << endl;*/
				return R;
			}
			else {
				//cout << a << " " << b << endl;
				// set flag2 = ...
				//if (R.Len() > 2*k + 2) {
				//	direct = false;
				//	//cout << "here" << endl;
				//}
				if(flag2(P_prime, G_R, wr_n1, wr_n2)) {
					//cout << "flag2" << endl;
					
					direct = false;
				}
				else {
					//cout << "flag3" << endl;
					//find ... and radomly choose ... vertices from ... adding into R
					Adding2Subset2(P_prime, G_R, R, a, b);
				}
			}

		}

	}
	
	return {};
}

bool BkVCC::flag1(PUNGraph G_R, int &u, int &v)
{
	//if (G_R->GetNodes() < k + 2) return false;
	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
			/*cout << NI1.GetId() << " " << NI2.GetId() << endl;*/
			//printf("G: \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId())) continue;

			if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId()) < k) {
				u = NI1.GetId(), v = NI2.GetId();
				
				return false;
			}
		}
	}
	return true;
}

//bool BkVCC::flag2(PUNGraph P_prime, PUNGraph G_R, int& u, int& v)
//{
//
//	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
//		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
//			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId()))continue;
//
//			if (TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId()) < k) {
//				u = NI1.GetId(), v = NI2.GetId();
//				return true;
//			}
//		}
//	}
//	return false;
//}

bool BkVCC::flag2(PUNGraph P_prime, PUNGraph G_R, int &u, TIntV& vs)
{

	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId()))continue;

			if (TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId()) < k) {
				//u = NI1.GetId(), vs.AddUnique(NI2.GetId());
				return true;
			}
		}
	}
	return false;
}

void BkVCC::Adding2Subset(PUNGraph P_prime, PUNGraph G_R, TIntV& R)
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

void BkVCC::Adding2Subset2(PUNGraph P_prime, PUNGraph G_R, TIntV& R, int u, int v)
{

	TIntV NbrV1, NbrV2;
	TIntH deg;
	int num = 0, node;
	int total;
	TSnap::GetCmnNbrs(G_R, u, v, NbrV1);
	TSnap::GetCmnNbrs(P_prime, u, v, NbrV2);
	total = k - NbrV1.Len();
	for (TIntV::TIter NI = NbrV2.BegI(); NI < NbrV2.EndI(); NI++) {
		deg.AddDat(*NI, P_prime->GetNI(*NI).GetDeg());
	}
	deg.SortByDat(FALSE);
	/*NbrV2.Sort*/
	for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
		node = HI.GetKey();
		if (!NbrV1.IsIn(node)) {
			R.Add(node);
			num++;
			if (num >= total) return;
		}
	}
	/*for (TIntV::TIter NI = NbrV2.BegI(); NI < NbrV2.EndI(); NI++) {
		if (!NbrV1.IsIn(*NI)) {
			R.Add(*NI);
			num++;
			if (num >= total) return;
		}
	}*/
	//for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
	//	for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
	//		if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId())) continue;
	//		if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId(), NbrV1) < k) {
	//			TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId(), NbrV2);
	//			total = k - NbrV1.Len();
	//			//This is not randomly choose, which is different with paper
	//			for (TIntV::TIter NI = NbrV2.BegI(); NI < NbrV2.EndI(); NI++) {
	//				if (!NbrV1.IsIn(*NI)) {
	//					R.Add(*NI);
	//					num++;
	//					if (num >= total) return;
	//				}
	//			}
	//		}
	//	}
	//}
}

TIntVIntV BkVCC::Seeding(PUNGraph G, int k, int alpha)
{

	TIntVIntV G_S;
	TIntV G_C;
	TIntH CandMaintain, deg;
	int i = 0;


	//try
	//{
	//	TFIn inFile("./seedgraph/" + dataset + "_k=" + TStr(k_str) + "_alpha=" + TStr(alpha_str) + "_seed=" + TStr(seed.c_str()) + ".seed");
	//	//TFIn inFile("D:/Git Project/KVCC-ENUM/seedgraph/DBLP_k=5_alpha=1000_algorithm=BkVCC.seed");
	//	G_S.Load(inFile);
	//}
	//catch (TPt<TExcept>)
	//{
	//	cout << endl << "***seedgraph does not exist, running seeding function...***" << endl;
	//}

	if (G_S.Len() != 0) return G_S;


	if (strcmp(seed.c_str(), "rand") == 0) {
		if (threads <= 1) {
			for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
				CandMaintain.AddDat(NI.GetId(), 0);
				deg.AddDat(NI.GetId(), NI.GetDeg());
			}
			deg.SortByDat(); //non-decreasing order


			for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
				/*cout << HI.GetKey() << " " << HI.GetDat() << endl;*/
				int v = HI.GetKey();
			
				
				if (CandMaintain.GetDat(v) == 0) {
					//cout << HI.GetKey() << " " << HI.GetDat() << endl;
					G_C = LkVCS(G, k, v, alpha);
					//cout<< ++i <<endl;
					if (G_C.Empty()) continue;

					G_S.Add(G_C); //��ô�ж��Ƿ���ȣ�Unique����


					for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {
						CandMaintain.GetDat(*TI) = 1;
					}
				}
				if (CandMaintain.GetDat(v) == 1) {
					//cout << "sweeped" << endl;
				}
			}
			G_S.Merge();
		}
		else if (threads >= 2) {
			for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
				CandMaintain.AddDat(NI.GetId(), 0);
				deg.AddDat(NI.GetId(), NI.GetDeg());
			}
			deg.SortByDat(); //non-decreasing order
			/*cout << deg.Len() << endl;
			for (int i = 0; i < deg.Len(); i++) {
				
				cout << i <<" " << deg.GetKey(i) << " " << deg[i] << endl;
			}*/

		#pragma omp parallel
			{
				//int threadId = omp_get_thread_num();
				/*cout << omp_get_num_threads() << endl;*/
		#pragma omp for  private(G_C)
				for (int i = 0; i < deg.Len(); i++) {
					/*cout << i << ": " << deg[i] << endl;*/
					int v = CandMaintain.GetKey(i);
					if (CandMaintain.GetDat(v) == 0) {
					/*if (true) {*/
						/*cout << v << " " << deg.GetDat(v) << endl;*/
						G_C = LkVCS(G, k, v, alpha);
						/*cout << "G_C: " << G_C.Len() << endl;*/
						/*cout<< ++i <<endl;*/
						if (G_C.Empty()) continue;

			#pragma omp critical
						{
						G_S.Add(G_C); //��ô�ж��Ƿ���ȣ�Unique����

						for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {
			
							CandMaintain.GetDat(*TI) = 1;
						}
						}

					}


				}
				//for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
				//	cout << HI.GetKey() << " " << HI.GetDat() << endl;

				//	int v = HI.GetKey();
				//	if (CandMaintain.GetDat(v) == 0) {
				//		//cout << HI.GetKey() << " " << HI.GetDat() << endl;
				//		G_C = LkVCS(G, k, v, alpha);
				//		//cout<< ++i <<endl;
				//		if (G_C.Empty()) continue;

				//		G_S.Add(G_C); //��ô�ж��Ƿ���ȣ�Unique����


				//		for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {
				//			CandMaintain.GetDat(*TI) = 1;
				//		}
				//	}
				//	if (CandMaintain.GetDat(v) == 1) {
				//		//cout << "sweeped" << endl;
				//	}
				//}
				
			}
			G_S.Merge();
		}
		

	}
	else if (strcmp(seed.c_str(), "clique") == 0) {
		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);
	}
	else if (strcmp(seed.c_str(), "k-2-clique") == 0) {
		TCliqueOverlap::GetMaxCliques(G, k -2 , G_S);
	}
	else if (strcmp(seed.c_str(), "3-clique") == 0) {
		TCliqueOverlap::GetMaxCliques(G, 3, G_S);
	}
	else if (strcmp(seed.c_str(), "mix_Com") == 0) {
		//����clique����seed����ʹ��rand�ķ���ѡ�в������κ�seed�ĵ���Ϊ�����������µ�seed

		//step1. ��ȡk-core��ÿ����ͨ����
		TCnComV CnComV;
		TSnap::GetSccs(G, CnComV);
		cout << "CnCon(Len): " << CnComV.Len() << endl;
	#pragma omp parallel 
		{
	#pragma omp for private(G_C, CandMaintain, deg)
			for (int i = 0; i < CnComV.Len(); i++) {
				TCnCom C = CnComV.GetVal(i);
				cout << i << endl;
				/*cout << "C: " << C.Len() << endl;
			/*}
			for (TCnComV::TIter CI = CnComV.BegI(); CI < CnComV.EndI(); CI++) {
				TCnCom C = *CI;*/
				TIntVIntV G_Subcliques;
				PUNGraph G_CnCom = TSnap::GetSubGraph(G, C());
				TCliqueOverlap::GetMaxCliques(G_CnCom, k + 1, G_Subcliques);
				if (G_Subcliques.Empty()) {
					for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
						CandMaintain.AddDat(NI.GetId(), 0);
						deg.AddDat(NI.GetId(), NI.GetDeg());
					}
					deg.SortByDat(); //non-decreasing order

					/*#pragma omp parallel
					{*/
					//int threadId = omp_get_thread_num();
					/*cout << omp_get_num_threads() << endl;*/
	#					/*pragma omp for private(G_C)*/
					for (int j = 0; j < deg.Len(); j++) {
						/*cout << i << ": " << deg[i] << endl;*/
						int v = CandMaintain.GetKey(j);
						if (CandMaintain.GetDat(v) == 0) {
							/*if (true) {*/
								/*cout << v << " " << deg.GetDat(v) << endl;*/
							G_C = LkVCS(G, k, v, alpha);
							/*cout << "G_C: " << G_C.Len() << endl;*/
							/*cout<< ++i <<endl;*/
							if (G_C.Empty()) continue;

	#pragma omp critical
							G_S.Add(G_C); //��ô�ж��Ƿ���ȣ�Unique����
							/*#pragma omp critical*/
							for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

								CandMaintain.GetDat(*TI) = 1;
							}

						}

					}
				}
				else {
	#pragma omp critical
					G_S.AddVMerged(G_Subcliques);
				}
			}
		}		
		
		G_S.Merge();
	}
	else if (strcmp(seed.c_str(), "mix") == 0) {

		TUNGraph G_temp = *G;
		//step1.������max_cliques

		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);

		cout << G_S.Len()<< endl;

		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		//step2.������max_cliques�ĵ��CandMaintain��Ϊ1����ʾ�ѱ�ѡ����ͼ
		//TODO��ֱ�ӽ�max_cliques�еĵ�ɾ��
		//TUNGraph G_temp = *G;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				if(G_temp.IsNode(*NI)) G_temp.DelNode(*NI);

				//cout << i++ << ": " << CandMaintain.GetDat(*NI) << endl;

			}
		}
		cout <<"G_temp: " << G_temp.GetNodes() << endl;
		//cout << G_S.Len() << endl;

		//int i = 0;

		deg.SortByDat(); //non-decreasing order

	#pragma omp parallel
		{
			//int threadId = omp_get_thread_num();
			/*cout << omp_get_num_threads() << endl;*/
	#pragma omp for private(G_C)
			for (int i = 0; i < deg.Len(); i++) {
				/*cout << i << ": " << deg[i] << endl;*/
				int v = CandMaintain.GetKey(i);
				if (CandMaintain.GetDat(v) == 0) {
					/*if (true) {*/
						/*cout << v << " " << deg.GetDat(v) << endl;*/
					G_C = LkVCS(G, k, v, alpha);
					/*cout << "G_C: " << G_C.Len() << endl;*/
					/*cout<< ++i <<endl;*/
					if (G_C.Empty()) continue;

	#pragma omp critical
					G_S.Add(G_C); //��ô�ж��Ƿ���ȣ�Unique����
	#pragma omp critical
					for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

						CandMaintain.GetDat(*TI) = 1;
					}

				}


			}
		
		}
		G_S.Merge();
	}
	else if (strcmp(seed.c_str(), "k'-vcc") == 0) {
		TUNGraph G_temp = *G;
		int k_prime = k + 3;
		VCCE_S VCCE_S(G, k_prime, 1);
		PUNGraph tmp = TSnap::GetKCore(G, k_prime);
		printf("G(%d-core): \nnode_nums = %d, edge_nums = %d\n", k_prime, tmp->GetNodes(), tmp->GetEdges());
		VCCE_S.dataset = dataset;
		G_S = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);

		cout << G_S.Len() << endl;

		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		//step2.������max_cliques�ĵ��CandMaintain��Ϊ1����ʾ�ѱ�ѡ����ͼ
		//TODO��ֱ�ӽ�max_cliques�еĵ�ɾ��
		//TUNGraph G_temp = *G;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				if (G_temp.IsNode(*NI)) G_temp.DelNode(*NI);

				//cout << i++ << ": " << CandMaintain.GetDat(*NI) << endl;

			}
		}
		cout << "G_temp: " << G_temp.GetNodes() << endl;
		//cout << G_S.Len() << endl;

		//int i = 0;

		deg.SortByDat(); //non-decreasing order

#pragma omp parallel
		{
			//int threadId = omp_get_thread_num();
			/*cout << omp_get_num_threads() << endl;*/
#pragma omp for private(G_C)
			for (int i = 0; i < deg.Len(); i++) {
				/*cout << i << ": " << deg[i] << endl;*/
				int v = CandMaintain.GetKey(i);
				if (CandMaintain.GetDat(v) == 0) {
					/*if (true) {*/
						/*cout << v << " " << deg.GetDat(v) << endl;*/
					G_C = LkVCS(G, k, v, alpha);
					/*cout << "G_C: " << G_C.Len() << endl;*/
					/*cout<< ++i <<endl;*/
					if (G_C.Empty()) continue;

#pragma omp critical
					G_S.Add(G_C); //��ô�ж��Ƿ���ȣ�Unique����
#pragma omp critical
					for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

						CandMaintain.GetDat(*TI) = 1;
					}

				}


			}

		}
		G_S.Merge();
	}	
	else if (strcmp(seed.c_str(), "k'-vcc+clique") == 0) {
		TUNGraph G_temp = *G;

		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);

		//for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
		//	for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
		//		CandMaintain.GetDat(*NI) = 1;
		//		if (G_temp.IsNode(*NI)) G_temp.DelNode(*NI);

		//		//cout << i++ << ": " << CandMaintain.GetDat(*NI) << endl;

		//	}
		//}
		//cout << "G_temp(after-kvcc): " << G_temp.GetNodes() << endl;
		//if (G_temp.GetNodes() != 0) {
		int k_prime = k + 3;
		VCCE_S VCCE_S(G, k_prime, 1);
		PUNGraph tmp = TSnap::GetKCore(G, k_prime);
		printf("G(%d-core): \nnode_nums = %d, edge_nums = %d\n", k_prime, tmp->GetNodes(), tmp->GetEdges());
		VCCE_S.dataset = dataset;
		TIntVIntV k_prime_vcc = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
		G_S.AddV(k_prime_vcc);
		/*}*/
		
		G_S.Merge();

		//step2.������max_cliques�ĵ��CandMaintain��Ϊ1����ʾ�ѱ�ѡ����ͼ
		//TODO��ֱ�ӽ�max_cliques�еĵ�ɾ��
		//TUNGraph G_temp = *G;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				if (G_temp.IsNode(*NI)) G_temp.DelNode(*NI);

				//cout << i++ << ": " << CandMaintain.GetDat(*NI) << endl;

			}
		}
		cout << "G_temp: " << G_temp.GetNodes() << endl;
		//cout << G_S.Len() << endl;

		//int i = 0;

		deg.SortByDat(); //non-decreasing order

#pragma omp parallel
		{
			//int threadId = omp_get_thread_num();
			/*cout << omp_get_num_threads() << endl;*/
#pragma omp for private(G_C)
			for (int i = 0; i < deg.Len(); i++) {
				/*cout << i << ": " << deg[i] << endl;*/
				int v = CandMaintain.GetKey(i);
				if (CandMaintain.GetDat(v) == 0) {
					/*if (true) {*/
						/*cout << v << " " << deg.GetDat(v) << endl;*/
					G_C = LkVCS(G, k, v, alpha);
					/*cout << "G_C: " << G_C.Len() << endl;*/
					/*cout<< ++i <<endl;*/
					if (G_C.Empty()) continue;

#pragma omp critical
					G_S.Add(G_C); //��ô�ж��Ƿ���ȣ�Unique����
#pragma omp critical
					for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

						CandMaintain.GetDat(*TI) = 1;
					}

				}


			}

		}
		G_S.Merge();

	}

	//cout << G_S.Len();
	TFOut outFile("./seedgraph/" + dataset + "_k=" + TStr(k_str) + "_alpha=" + TStr(alpha_str) + "_seed=" + TStr(seed.c_str()) + ".seed");
	G_S.Save(outFile);

	/*for (TIntVIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {
		cout << TI->Len() << " ";
	}
	cout << endl;*/
	return G_S;
}

//void printNodesNum(PUNGraph G) {
//	cout << "Nodes:"<< G->GetNodes() << endl;
//}

//void BkVCC::Expanding(int k, TIntVIntV& G_S)
//{
//	clock_t t2 = clock();
//	int u;
//	TIntV delta_S, delta_S_bar;
//
//	double i = 0, per = 0, per_ = 0;
//
//	for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
//
//		/*for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
//			cout << *NI << " ";
//		}
//		cout << endl;*/
//
//		per = ceil(i / G_S.Len() * 100);
//
//		if (per - per_ >= 1) {
//			cout << per << "%" << endl;
//			per_ = per;
//		}
//
//
//		TIntV cand = flag3(*GI, delta_S, delta_S_bar);
//		if (cand.Empty()) { i++; continue; }
//		while (!cand.Empty()) { //exist u belong delta_S_bar, intersection > k
//
//			/*Update(G, *GI, u);*/
//			GI->AddVMerged(cand);
//			/*for (TIntV::TIter NI = cand.BegI(); NI < cand.EndI(); NI++) {
//				cout << *NI << " ";
//			}
//			cout << endl;*/
//			//cout << "Nodes:" <<cand.Len() << endl;  //Test if graph updates valid
//			cand = flag3(*GI, delta_S, delta_S_bar);
//		}
//
//
//		//while (flag3(*GI, u, delta_S, delta_S_bar)) { //exist u belong delta_S_bar, intersection > k
//		//	
//		//	/*Update(G, *GI, u);*/
//		//	GI->AddUnique(u);
//		//	cout << "Nodes:" << u << endl;  //Test if graph updates valid
//		//}
//
//		//GI->Merge();
//		//cout << "GI.Len:" << GI->Len() << endl;
//		i++;
//	}
//	G_S.Merge();
//
//	_time2 += (double)(clock() - t2) * 1.0 / (double)CLOCKS_PER_SEC;
//
//	return;
//}


void BkVCC::Expanding(int k, TIntVIntV& G_S) {
	clock_t t2 = clock();
	//int u;
	if (threads <= 1) {
		TIntV delta_S, delta_S_bar;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {

			//TODO:�޸Ļ�ȡcand�ķ�ʽ
			// ͨ��IMF�㷨���������ͬʱexpand,����Щ�㵥��expandʱ�ǲ�����������
			int cand;

			do {
				if (strcmp(expandMethod.c_str(), "flow") == 0) {
					cand = GetCand(*GI, delta_S, delta_S_bar);
				}
				else if (strcmp(expandMethod.c_str(), "nbr") == 0) {
					cand = flag3(*GI, delta_S, delta_S_bar);
				}
				else if (strcmp(expandMethod.c_str(), "ring") == 0) {
					cand = ring_expanding(*GI, delta_S, delta_S_bar);
					/*for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
						cout << *NI << " ";
					}
					cout << endl;*/
				}

				//cout << "cand:" << cand << endl;

			} while (cand != 0);

			//while (!cand.Empty()) { //exist u belong delta_S_bar, intersection > k

			//	/*Update(G, *GI, u);*/
			//	GI->AddVMerged(cand);
			//	/*for (TIntV::TIter NI = cand.BegI(); NI < cand.EndI(); NI++) {
			//		cout << *NI << " ";
			//	}
			//	cout << endl;*/
			//	//cout << "Nodes:" <<cand.Len() << endl;  //Test if graph updates valid
			//	cand = flag3(*GI, delta_S, delta_S_bar);
			//}
		}
	}
	else if (threads >= 2) {
#pragma omp parallel 
		{
			//int threadId = omp_get_thread_num();
#pragma omp for schedule(dynamic)
			for (int i = 0; i < G_S.Len(); i++) {
				int cand;
				TIntV delta_S, delta_S_bar;
				TIntV GI = G_S.GetVal(i);
				do {
					if (strcmp(expandMethod.c_str(), "flow") == 0) {
						cand = GetCand(GI, delta_S, delta_S_bar);
					}
					else if (strcmp(expandMethod.c_str(), "nbr") == 0) {
						cand = flag3(GI, delta_S, delta_S_bar);
					}
					else if (strcmp(expandMethod.c_str(), "ring") == 0) {
						cand = ring_expanding(GI, delta_S, delta_S_bar);
						
					}
					//cout << "cand:" << cand << endl;

				} while (cand != 0);
				G_S.GetVal(i) = GI;
				//std::cout << "thread: " << threadId << endl;
			}
		}
	}
	G_S.Merge();
	_time2 += (double)(clock() - t2) * 1.0 / (double)CLOCKS_PER_SEC;

	return;


}

//����ring_expanding�нڵ���뵽local-kvcc�У� ���ھӸ��µ�local-kvcc����

void update_neighbour(TIntVIntV& S, TIntIntVH& in_neighs, TIntIntVH& out_neighs, int v, TIntV& res, int level = 0) {
	TIntV neigh = out_neighs.GetDat(v);
	//cout << neigh.Len() << endl;

	//cout << level << endl;
	for (TIntV::TIter NI = neigh.BegI(); NI < neigh.EndI(); NI++) {
		/*cout << v <<" " << *NI << endl;*/
		if (in_neighs.GetDat(*NI).Len() != 0) {
			out_neighs.GetDat(*NI).DelIfIn(v);
			in_neighs.GetDat(*NI).Add(v);
		}
			
		/*if (*NI == 50825)
			cout << " 50825_in_nei:" << in_neighs.GetDat(*NI).Len() << endl;*/
		/*if (*NI == 7) {
			cout << in_neighs.GetDat(*NI).Len();
			for (TIntV::TIter nb = in_neighs.GetDat(*NI).BegI(); nb < in_neighs.GetDat(*NI).EndI(); nb++) {
				cout << *nb << " ";
			}
			cout << endl;
		}*/

	}
	for (TIntV::TIter NI = neigh.BegI(); NI < neigh.EndI(); NI++) {
		int idx = in_neighs.GetDat(*NI).Len() - 1; //�����ѭ���Ѿ����¹�����������Ϊ���idx��ʼֵӦ��-1
		/*cout << idx << endl;*/
		if (idx < 1) continue;
		if (idx == k - 1) {
			S[idx].DelIfIn(*NI);
			//S[idx + 1].AddMerged(*NI);
			res.Add(*NI);
			/*if (*NI == 50825)
				cout << " 50825_in_nei:" << in_neighs.GetDat(*NI).Len() << endl;*/
			in_neighs.GetDat(*NI) = {};
			/*if (res[20] == 50825) {
				if (res[29] == 50825) {
					cout << endl;
				}
				for (TIntV::TIter NI = res.BegI(); NI < res.EndI(); NI++) {
					cout << *NI << " ";
				}
				cout << endl;
			}*/
			update_neighbour(S, in_neighs, out_neighs, *NI, res, level++);
		}
		else if (idx < k - 1) {
			S[idx].DelIfIn(*NI);
			S[idx + 1].AddMerged(*NI);
		}

	}
}

int BkVCC::ring_expanding(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	//step1. ������ÿ��local-kvcc�ı߽�ڵ��һ���ھӽڵ�֮��ı�(�յ���ͼ)
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV G_sub = delta_S;
	G_sub.AddVMerged(delta_S_bar);
	PUNGraph Gra = TSnap::GetSubGraph(G, G_sub);

	//step2. ÿ���ڵ�����Լ���local-kvcc�ı߸������ֳɼ���S_(k+), S_(k-1), S_(k-2), ...
	// ʹ�ö�ά����洢����
	TIntVIntV S;
	for (int i = 0; i <= k; i++) {
		S.Add({});
	}
	TIntV res = {};
	TIntV nb_u1 = {}, nb_u2 = {};
	//in_neighs�洢��ÿ����ѡ�ڵ������local-kvcc���ھӣ���ʡ����ʱ��
	//out_neighs�洢��ÿ����ѡ�ڵ������delta_S_bar���ھ�
	TIntIntVH in_neighs, out_neighs;

	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u1.Clr();
		nb_u2.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = Gra->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (delta_S.IsIn(CtrNode.GetInNId(i))) //ֻ��local-kvcc�е��ھӼӽ�ȥ
				nb_u1.AddMerged(CtrNode.GetInNId(i));
			else
				nb_u2.AddMerged(CtrNode.GetInNId(i));


		}
		in_neighs.AddDat(*TI, nb_u1);
		out_neighs.AddDat(*TI, nb_u2);
		int index = nb_u1.Len();

		//indexΪS�����±�����
		if (index >= k) index = k; //����kͳһ�洢k+�Ľڵ�
		S[index].AddMerged(*TI);

	}

	//step3. S_(k+)�ĵ����ȼ��뵽kvcc�У����ھӸ��µ�kvcc�ı���������S
	int S_Len;

	for (TIntV::TIter TI = S[k].BegI(); TI < S[k].EndI(); TI++) {
		res.Add(*TI);
		in_neighs.GetDat(*TI) = {};
		/*if (res[20] == 50825) {
			for (TIntV::TIter NI = res.BegI(); NI < res.EndI(); NI++) {
				cout << *NI << " ";
			}
			cout << endl;
		}*/
		update_neighbour(S, in_neighs, out_neighs, *TI, res);

		
	}


	//step4. ��S_(k-1)��ʼ����clique����������S_(k-1)��2-clique��S_(k-2)��3-clique��S_(k-3)��4-clique��...,S_(k-i)��size>= (i+1)��maximal clique
	//Ȼ���ж���Щclique��local-kvcc�в������ھӵĸ����Ƿ�$\geq k$
	//���ǣ���ͬʱ���뵽local-kvcc�У�����������Χ�ھӡ�
	int num = 0;
	TIntV S_total;
	//int stop_num =  k - G_S.Len()
	for (int i = k - 1; i > 0; i--) {
		TIntVIntV cliques;
		TIntV neigh_Union;
		TIntV S_i_temp;
		do{
			S_i_temp = S[i];
			S_total.Clr();

			S_total.AddVMerged(S[i]);

			//S_remain.Clr();
			if (S_total.Len() < k - i + 1) continue;
			PUNGraph sub_G = TSnap::GetSubGraph(Gra, S_total);
			PUNGraph sub_core = TSnap::GetKCore(sub_G, k - i);
			//printf("sub_G: \nnode_nums = %d, edge_nums = %d\n", sub_G->GetNodes(), sub_G->GetEdges());
			/*printf("sub_core: \nnode_nums = %d, edge_nums = %d\n", sub_core->GetNodes(), sub_core->GetEdges());*/
			TCliqueOverlap::GetMaxCliques(sub_core, k - i + 1, cliques);
			//TCliqueOverlap::GetCliquesSizeof(sub_core, k - i + 1, cliques);
			/*cout <<"i=" << i << ":" << cliques.Len() << endl;*/

			for (int clique_idx = 0; clique_idx < cliques.Len(); clique_idx++) {
				neigh_Union.Clr();
				int flag = 0;
				for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {
					int idx = in_neighs.GetDat(*TI).Len();
					if (idx == 0) {
						flag = 1;
						break;
					}
					/*if (!S_total.IsIn(*TI)) {
						flag = 1;
						break;
					}*/
					/*cout << *TI <<" ";
					cout << "in_neigh" << endl;
					if (*TI == 7) {
						cout << "here" << in_neighs.GetDat(*TI).Len() << endl;;
					}
					for (TIntV::TIter nb = in_neighs.GetDat(*TI).BegI(); nb < in_neighs.GetDat(*TI).EndI(); nb++) {
						cout << *nb << " ";
					}
					cout << endl;*/
					neigh_Union.AddV(in_neighs.GetDat(*TI));
					neigh_Union.Merge();
					/*if (neigh_Union.Len() >= k) break;*/
				}
				/*cout << endl;*/
				if (neigh_Union.Len() >= k && flag == 0) {
					for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {

						res.Add(*TI);
						/*if (*TI == 50825)
							cout << i <<" 50825_in_nei:" << in_neighs.GetDat(*TI).Len() << endl;*/
						int idx = in_neighs.GetDat(*TI).Len();
						/*if (idx == 0) break;*/
						if (idx > k) idx = k;
						in_neighs.GetDat(*TI) = {};
						//cout << *TI << endl;
						//cout << idx << endl;
						S[idx].DelIfIn(*TI);
						S_total.DelIfIn(*TI);


						/*if (res[20] == 50825) {
							for (TIntV::TIter NI = res.BegI(); NI < res.EndI(); NI++) {
								cout << *NI << " ";
							}
							cout << endl;
						}*/

					}
					for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {

						update_neighbour(S, in_neighs, out_neighs, *TI, res);
						/*if (*TI == 4572) cout << "here 1//////////////" << endl;*/
					}

					/*num += cliques[clique_idx].Len();*/
				}
			}
			//S_remain.AddVMerged(S[i]);
			/*for (TIntV::TIter TI = S[k - i].BegI(); TI < S[k - i].EndI(); TI++) {

			}*/
		} while ((S_i_temp != S[i]));

		
	}
	/*cout << "expanding_num:" << num << endl;*/
	/*if (num != 0) {
		cout << "expanding_num:" << res.Len() << endl;
	}*/
	
	/*cout<<"1: " << res.Len() << endl;
	res.Merge();
	cout << "2: " << res.Len() << endl;*/
	TIntV G_before = G_S;
#pragma omp critical
	{
		G_S.AddV(res);
	}
	
	
	//if (G_S.Len() > 4540) {
	//	cout << G_before.Len() << " " << G_S.Len() << " " << res.Len() << endl;
	//	/*for (TIntV::TIter NI = res.BegI(); NI < res.EndI(); NI++) {
	//		cout << *NI << " ";
	//	}
	//cout << endl;*/
	//}
	return res.Len();
}

//Parallel Expanding
//void BkVCC::Expanding(int k, TIntVIntV& G_S) {
//	clock_t t2 = clock();
//	//int u;
//	
//	
//	
//	
//	cout << G_S.Len() << endl;
//	G_S.Merge();
//	_time2 += (double)(clock() - t2) * 1.0 / (double)CLOCKS_PER_SEC;
//
//	return;
//
//
//}


int BkVCC::GetCand(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	TIntV res = {};
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;
	int itr = 2;//������10��


	//ԭ�е�expand��ʽ
	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = G->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (!nb_u.IsIn(CtrNode.GetInNId(i)))
				nb_u.Add(CtrNode.GetInNId(i));
		}
		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
			res.AddUnique(*TI);
		}
	}

	//��¼ÿ���ڵ��max_flowֵ
	TIntH flow;
	//Դ�ڵ㣬Ŀ��ڵ�
	int s = -1, t = -1;

	//Դ�ڵ�����ǷǱ�Ե��
	for (TIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {

		if (!delta_S.IsIn(*TI)) { s = *TI; break; }
	}

	//�����������Ľڵ�ֱ��expand��kvcc��ͼ��
	//Attention:���������ȷ��source vertex֮���ٽ�����expand��
	//��Ȼexpand�ĵ�Ҳ������delta_S,����ѡΪsource vertex����������
	G_S.AddVMerged(res);

	//if (G_S.Len() >= 100) return res.Len(); //̫��Ͳ���maxflow�жϣ���ʱ̫��

	//�����ڷǱ�Ե��ʱӦ����δ���������ֱ�ӽ�����Ӧ������һ������
	if (s == -1) { //cout << "end" << endl; 
		return res.Len();
	}

	//Ϊ������������㣬�����������Ľڵ��delta_S_barɾ��
	for (TIntV::TIter TI = res.BegI(); TI < res.EndI(); TI++) {
		delta_S_bar.DelIfIn(*TI);
	}




	TIntV G_U = G_S;
	G_U.AddVMerged(delta_S_bar);





	//cout << s << endl;
	TIntV delta_S_bar_star;//��¼��ѡ����
	int it = 0;
	while (true) {
		delta_S_bar_star.Clr();
		TIntV ResNet;

		PUNGraph Gra = TSnap::GetSubGraph(G, G_U);

		PNEANet DG = Construct_DG(Gra);

		int offset = Gra->GetMxNId();
		//cout << s + offset << endl;
		for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
			t = *TI;
			int lambda = TSnap::MyGetMaxFlowIntEK(DG, s + offset, t, ResNet, k);
			if (lambda >= k) delta_S_bar_star.Add(t);
			else G_U.DelIfIn(*TI);//�Ƴ������С��k�Ķ���
			//flow.AddDat(*TI, lambda);
		}
		/*cout << "delta_S_bar:";
		for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
			cout << *TI << " ";
		}
		cout << endl;

		cout << "delta_S_bar_star:";
		for (TIntV::TIter TI = delta_S_bar_star.BegI(); TI < delta_S_bar_star.EndI(); TI++) {
			cout << *TI << " ";
		}
		cout << endl;*/
		it++;
		//if (delta_S_bar == delta_S_bar_star || it >= itr) break;

		if (delta_S_bar == delta_S_bar_star) break;

		delta_S_bar = delta_S_bar_star;
	}

	/*std::cout << "G_S(before): " << G_S.Len() << endl;*/
	G_S.AddVMerged(delta_S_bar_star);

	/*std::cout << "G_S(after): " << G_S.Len() << endl;*/
	//std::cout << delta_S_bar_star.Len() << endl;

	return res.Len();
}

TIntV BkVCC::GetBoundary(TIntV G_S, TIntV& delta_S_bar)
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


int BkVCC::flag3(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	TIntV res = {};

	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;

	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = G->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (!nb_u.IsIn(CtrNode.GetInNId(i)))
				nb_u.Add(CtrNode.GetInNId(i));
		}
		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
			res.AddUnique(*TI);
		}
	}

	G_S.AddVMerged(res);
	return res.Len();
}

bool BkVCC::flag3(TIntV G_S, int& u, TIntV& delta_S, TIntV& delta_S_bar)
{
	u = -1;
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;

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
		/*if (G_S.IsIn(0)) {
			cout << "u = " << *TI << ":" << endl;

			cout << "G_S:";
			for (TIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {
				cout << *TI << " ";
			}
			cout << endl;
			cout << "delta_S: ";
			for (TIntV::TIter TI = delta_S.BegI(); TI < delta_S.EndI(); TI++) {
				cout << *TI << " ";
			}
			cout << endl;
			cout << "delta_S_bar: ";
			for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
				cout << *TI << " ";
			}
			cout << endl;
			cout << delta_S.Len() << endl;
		}*/
		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
			u = *TI;
			return true;
		}

	}
	return false;
}





void BkVCC::Merging(int k, TIntVIntV& G_S, TIntVIntV& G_R)
{
	clock_t t3 = clock();
	//cout << G_S.Len() << endl;
	
	// �������С������seedɾ��
	for (int i = G_S.Len() - 1; i >= 0 ; i--) {
		if (G_S[i].Len() <= k) {
			G_S.Del(i);
		}
	}
	TIntVIntV G_S_temp = G_S;

	//���ж���Ϊ���ж����Ա�֤û���Ӽ�������
	//for (TIntVIntV::TIter GI = G_S_temp.BegI(); GI < G_S_temp.EndI(); GI++) {
	//	/*cout << "GI: ";
	//	for (TIntV::TIter TI = GI->BegI(); TI < GI->EndI(); TI++) {
	//		cout << *TI << " ";
	//	}
	//	cout << endl;*/
	//	if (IskVCC(*GI, k)) {
	//		G_R.Add(*GI);
	//		G_S.DelIfIn(*GI);
	//	}
	//}
	// 
	//G_S_temp = G_S;

	//
	//TPrVIntV G_Merge_cand = flag4(G_S);
	////cout << "Len: " << G_Merge_cand.Len() << endl;

	//for (TPrVIntV::TIter PI = G_Merge_cand.BegI(); PI < G_Merge_cand.EndI(); PI++) {
	//	GI1 = PI->GetVal1();
	//	GI2 = PI->GetVal2();
	//	if (!G_S.IsIn(GI1) || !G_S.IsIn(GI2)) continue;
	//	//cout << "GI1: ";
	//	//for (TIntV::TIter TI = GI1.BegI(); TI < GI1.EndI(); TI++) {
	//	//	cout << *TI <<" ";
	//	//}
	//	//cout << endl;
	//	//
	//	//cout << "GI2: ";
	//	//for (TIntV::TIter TI = GI2.BegI(); TI < GI2.EndI(); TI++) {
	//	//	cout << *TI << " ";
	//	//}
	//	//cout << endl;
	//	//exist GI1, GI2 satisfied IsMergeValid
	//	//PUNGraph G_Union = Merge2Graph(G, GI1, GI2);
	//	TIntV G_Union = {};
	//	G_Union.AddVMerged(GI1);
	//	G_Union.AddVMerged(GI2);
	//	//cout << "G_Union: ";
	//	//for (TIntV::TIter TI = G_Union.BegI(); TI < G_Union.EndI(); TI++) {
	//	//	cout << *TI << " ";
	//	//}
	//	//cout << endl;
	//	if (IskVCC(G_Union, k)) {
	//		G_R.AddMerged(G_Union);
	//		//G_R.Add(G_Union);
	//		G_S.DelIfIn(GI1);
	//		G_S.DelIfIn(GI2);
	//	}
	//	else {
	//		G_S.AddMerged(G_Union);
	//		//G_S.Add(G_Union);
	//		G_S.DelIfIn(GI1);
	//		G_S.DelIfIn(GI2);
	//	}

	//}
	if (threads >= 2) {
		int i;
		TIntV GI1, GI2;
#pragma omp parallel shared(G_S, G_R) private(i, GI1, GI2)
		{
#pragma omp for schedule(dynamic)
		
		for (int i = 0; i < G_S.Len(); i++) {
			int j = i + 1;
			while (j < G_S.Len()) {
#pragma omp critical
				{
					GI1 = G_S[i];
					GI2 = G_S[j];
				}
				if (strcmp(mergeMethod.c_str(), "flow") == 0) {
					if (IsMergeValid_maxflow(GI1, GI2)) {
						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);
		#pragma omp critical
						{
							G_S.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
						}
						std::cout << "After Merging:" << G_Union.Len() << endl;
						std::cout << "G_S.Len:" << G_S.Len() << endl;
						/*i--;
						j = i + 1;*/
						continue;
					}
					else {
						j++;
					}
				}
				else {
					if (IsMergeValid(GI1, GI2)) {


						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);

#pragma omp critical
						{
							if (IskVCC(G_Union, k)) {
								G_R.Add(G_Union);
								G_S.DelIfIn(GI1);
								G_S.DelIfIn(GI2);
								//G_R.Merge();
							}
							else {
								G_S.Add(G_Union);
								G_S.DelIfIn(GI1);
								G_S.DelIfIn(GI2);
								//G_S.Merge();
							}
						}
						std::cout << "After Merging:" << G_Union.Len() << endl;
						std::cout << "G_S.Len:" << G_S.Len() << endl;
						/*i--;
						j = i + 1;*/
						continue;

					}
					else {
						j++;
					}
				}
			}
		}


		}
	}
	else if (threads <= 1) {
	
		//��򵥵ı�������û��ʱ�临�Ӷȸ��͵��㷨
		for (int i = 0; i < G_S.Len(); ) {
			for (int j = i + 1; j < G_S.Len();) {
				TIntV GI1 = G_S[i];
				TIntV GI2 = G_S[j];
				/*cout << "G_S.Len:" << G_S.Len()<<endl;
				cout << "i:" << i << endl;
				cout << "j:" << j << endl;*/
				if (strcmp(mergeMethod.c_str(), "flow") == 0) {
					if (IsMergeValid_maxflow(GI1, GI2)) {


						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);
						/*if (G_Union.Len() > 200) {
							cout << "here" << endl;
						}*/

						/*if (G_Union.Len() > 4540) {
							for (TIntV::TIter NI = GI1.BegI(); NI < GI1.EndI(); NI++) {
								cout << NI << " ";
							}
							cout << endl;
							for (TIntV::TIter NI = GI2.BegI(); NI < GI2.EndI(); NI++) {
								cout << NI << " ";
							}
							cout << endl;
						}*/
						//if (IskVCC(G_Union, k)) 
						if (false) {
							G_R.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_R.Merge();
						}
						else {
							G_S.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_S.Merge();
						}
						std::cout << "After Merging:" << G_Union.Len() << endl;
						std::cout << "G_S.Len:" << G_S.Len() << endl;
						continue;
					}
					j++;
				}
				else {
					if (IsMergeValid(GI1, GI2)) {


						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);
						/*if (G_Union.Len() > 200) {
							cout << "here" << endl;
						}*/
						if (IskVCC(G_Union, k)) {
							G_R.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_R.Merge();
						}
						else {
							G_S.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_S.Merge();
						}
						std::cout << "After Merging:" << G_Union.Len() << endl;
						std::cout << "G_S.Len:" << G_S.Len() << endl;
						continue;
					}
					j++;
				}

			}
			i++;
		}
	}
	if(strcmp(mergeMethod.c_str(), "old") == 0) {
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
	}
	

	_time3 += (double)(clock() - t3) * 1.0 / (double)CLOCKS_PER_SEC;
	return;
}

//Ϊʲô����ͬ���Ĵ����ܳ��ֲ�һ���Ľ����laugh_cry..
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
	TIntV nb_1 = {}, nb_2 = {};
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
		//��������>=k��ֱ�ӷ���
		if (S_ints_S_prime.Len() >= k) return true;
	}

	//�Ѽ���ĳ�ɾ��
	S_sub_S_prime = S_Nodes;
	S_prime_sub_S = S_prime_Nodes;
	for (TIntV::TIter TI = S_ints_S_prime.BegI(); TI < S_ints_S_prime.EndI(); TI++) {
		S_sub_S_prime.DelIfIn(*TI);
		S_prime_sub_S.DelIfIn(*TI);
	}

	/*for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
		if (!S_ints_S_prime.IsIn(*TI)) {
			S_sub_S_prime.Add(*TI);
		}
	}
	for (TIntV::TIter TI = S_prime_Nodes.BegI(); TI < S_prime_Nodes.EndI(); TI++) {
		if (!S_ints_S_prime.IsIn(*TI)) {
			S_prime_sub_S.Add(*TI);
		}
	}*/

	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		TUNGraph::TNodeI NI = G->GetNI(*TI);
		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
				nb_2.AddUnique(*TI);
				nb_1.AddUnique(NI.GetNbrNId(i));
				//���nb_1����Ͳ���Ҫ�������㣬nb_1��������������
				//if (nb_1.Len() >= k - S_ints_S_prime.Len()) break;
			}
		}
		//����˫��ѭ��
		//if (nb_1.Len() >= k - S_ints_S_prime.Len()) break;
	}

	if (S_ints_S_prime.Len() + min(nb_1.Len(), nb_2.Len()) >= k) {
		return true;
	}

	return false;
}

bool BkVCC::IsMergeValid_maxflow(TIntV G_S, TIntV G_S_prime) {

	TIntV S_Nodes, S_prime_Nodes, S_ints_S_prime, S_sub_S_prime, S_prime_sub_S, delta_S, delta_S_prime;
	TIntV nb_1 = {}, nb_2 = {};
	S_Nodes = G_S;
	S_prime_Nodes = G_S_prime;
	int s = -1, t = -1;


	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
		if (S_prime_Nodes.IsIn(*TI)) {
			S_ints_S_prime.Add(*TI);
		}
		//��������>=k��ֱ�ӷ���
		if (S_ints_S_prime.Len() >= k) return true;
	}



	S_sub_S_prime = S_Nodes;
	S_prime_sub_S = S_prime_Nodes;
	for (TIntV::TIter TI = S_ints_S_prime.BegI(); TI < S_ints_S_prime.EndI(); TI++) {
		S_sub_S_prime.DelIfIn(*TI);
		S_prime_sub_S.DelIfIn(*TI);
	}
	//�ҵ�delta_S �� delta_S_prime
	//�����delta_Sָ��������һ����ͼ�����ھӵĶ��㼯��
	//10/18 �����б�Ҫȫ���ҵ����ǲ����ҵ�һ��û���ھ��ڶԷ������еľͿ���ֹͣ
	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		TUNGraph::TNodeI NI = G->GetNI(*TI);
		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
				delta_S.AddUnique(*TI);
				delta_S_prime.AddUnique(NI.GetNbrNId(i));
			}
		}
	}

	if (min(delta_S.Len(), delta_S_prime.Len()) + S_ints_S_prime.Len() < k) return false;

	for (TIntV::TIter TI = S_prime_sub_S.BegI(); TI < S_prime_sub_S.EndI(); TI++) {
		if (!delta_S_prime.IsIn(*TI)) {
			s = *TI;
			break;
		}
	}


	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		if (!delta_S.IsIn(*TI)) {
			t = *TI;
			break;
		}

	}


	TIntV G_U = {};
	G_U.AddVMerged(G_S);
	G_U.AddVMerged(G_S_prime);
	TIntV ResNet = {};

	PUNGraph Gra = TSnap::GetSubGraph(G, G_U);
	// [24] Lutz Volkmann, On local connectivity of graphs, Appl. Math. Lett. 21 (1) (2008) 63�C66. 
	//cout << Gra->GetNI(s).GetDeg() + Gra->GetNI(t).GetDeg() + 2 - Gra->GetNodes() << endl;
	//if (Gra->GetNI(s).GetDeg() + Gra->GetNI(t).GetDeg() + 2 - Gra->GetNodes() >= k) { //Lower-bound-based reduction rule 

	//	cout << "here" << endl;
	//	return true;
	//}
		

	PNEANet DG = Construct_DG(Gra);



	if (s != -1 && t != -1) {
		int offset = Gra->GetMxNId();
		int lambda = TSnap::MyGetMaxFlowIntEK(DG, s + offset, t, ResNet, k);
		if (lambda + S_ints_S_prime.Len() >= k) {
			return true;
		}
	}

	return false;

}



PNEANet BkVCC::Construct_DG(PUNGraph G)
{
	//Construct corresponding attribute directed graph,
	//prepare for maximum flow calculate.(According to 'Effective K-Vertex connected 
	//component detection in large-scale networks')
	PNEANet DG = TNEANet::New();
	DG->AddIntAttrE("capacity", 0);
	int offset = G->GetMxNId();
	int i, eid;
	//step 1

	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		i = NI.GetId();
		DG->AddNode(i); //v
		DG->AddNode(i + offset); //v'
		eid = DG->AddEdge(i, i + offset);
		DG->AddIntAttrDatE(eid, 1, "capacity");

	}

	//step 2
	for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
		// u -> v
		eid = DG->AddEdge(EI.GetSrcNId() + offset, EI.GetDstNId());  //u''->v'
		DG->AddIntAttrDatE(eid, 1, "capacity");
		eid = DG->AddEdge(EI.GetDstNId() + offset, EI.GetSrcNId()); //v''->u'
		DG->AddIntAttrDatE(eid, 1, "capacity");
	}

	return  DG;
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

