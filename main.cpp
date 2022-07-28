#include"Sweep_Algorithm.h"
#include "Heuristic_Algorithm.h"
#include"Utility.h"
#include<stdio.h>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")

using namespace std;

void printNodesNum(PUNGraph G, FILE* outfile, int j) {
	TIntV Nodes;
	G->GetNIdV(Nodes);
	Nodes.Sort();
	for (TIntV::TIter TI = Nodes.BegI(); TI < Nodes.EndI(); TI++) {
		//cout << "Node_ID:" << *TI << endl;
		fprintf(outfile, "%d	%d\n", *TI, j);
	}
	
	
}


int main() {
	//<Dataset>            <max K-VCC>
	//CA-AstroPh.txt			56
	//Stanford.txt        71	14 28 43 57
	//DBLP.txt				113	
	//Cit.txt				64
	//LiveJournal.txt			
	//CA-CondMat.txt			25

	//TIntV nums = TIntV();
	//for (int i = 1; i <= 321; i++) {
	//	nums.Add(i);
	//}
	//TVec<TIntV> res = subsets(nums, 5, 1000);
	//cout << res.Len();
	TStr dataset = "smalltestforkvcc";//BkFig
	PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/"+ dataset +".txt", 0, 1);
	string dataset_name = dataset.CStr();
	
	

	int K_Arr[] = {4}; 
	
	for (int i = 0; i < sizeof(K_Arr) / sizeof(int); i += 1) {

		int k = K_Arr[i];
		int alpha = 1000;
		clock_t t0;
		int j = 0;
		char* k_str = new char[5];
		char* alpha_str = new char[10];
		
		itoa(k, k_str, 10);
		itoa(alpha, alpha_str, 10);

		/*string filename = "./output/" + dataset_name + "_k=" + k_str + "_alpha=" + alpha_str + ".txt";*/

		string filename = "./output/" + dataset_name + "_k=" + k_str + ".txt";
		FILE* outFile = fopen(filename.c_str(), "w");
		printf("G: \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());

		//BkVCC
		t0 = clock();
		BkVCC BkVCC(G, k);
		BkVCC.dataset = dataset;
		TIntVIntV BkVCC_res = BkVCC.BkVCC_ENUM(BkVCC.G, BkVCC.k, alpha);
		
		fprintf(outFile, "VCC_Num:%d\n"
			"Time for Seeding: %fs\n"
			"Time for Expanding: %fs\n"
			"Time for Merging: %fs\n"
			"Total Time: %fs\n", BkVCC_res.Len(), BkVCC._time, BkVCC._time2, BkVCC._time3, (clock() - t0) * 1.0 / CLOCKS_PER_SEC);

		for (TIntVIntV::TIter GI = BkVCC_res.BegI(); GI < BkVCC_res.EndI(); GI++) {
			PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
			fprintf(outFile, "K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				//cout << *NI<<" ";
				fprintf(outFile, "%d ", *NI);
			}
			fprintf(outFile, "\n");
			//cout << endl;

		}


		//kVCC-ENUM_Sweep
	//	t0 = clock();
	//	j = 0;
	//	/*fprintf(outFile, "## dataset: %s k = %d\n", dataset_name.c_str(), k);
	//	printf("k = %d\n", k);
	//	printf("VCCE*:\n");
	//	fprintf(outFile, "VCCE*:\n");*/
	//	VCCE_S VCCE_S(G, k, 1);
	//	TUNGraV VCCE_S_res = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);

	//	/*if (VCCE_S_res.Len() == 0) break;*/

	///*	fprintf(outFile, "VCC_Num:%d\n"
	//		"time for LOC_CUT(flow>=k): %fs\n"
	//		"Call for LOC_CUT(flow>=k): %d\n"
	//		"time for LOC_CUT(flow<k): %fs\n"
	//		"Call for LOC_CUT(flow<k): %d\n"
	//		"time for LOC_CUT(fake): %fs\n"
	//		"Call for LOC_CUT(fake): %d\n"
	//		"Total Time: %fs\n", VCCE_S_res.Len(), VCCE_S._time2, VCCE_S.m2, VCCE_S._time3, VCCE_S.m3, VCCE_S._time4, VCCE_S.m4, (clock() - t0) * 1.0 / CLOCKS_PER_SEC);*/

	//	printf("VCC_Num:%d\n"
	//		"time for LOC_CUT(flow>=k): %fs\n"
	//		"Call for LOC_CUT(flow>=k): %d\n"
	//		"time for LOC_CUT(flow<k): %fs\n"
	//		"Call for LOC_CUT(flow<k): %d\n"
	//		"time for LOC_CUT(fake): %fs\n"
	//		"Call for LOC_CUT(fake): %d\n"
	//		"Total Time: %fs\n", VCCE_S_res.Len(), VCCE_S._time2, VCCE_S.m2, VCCE_S._time3, VCCE_S.m3, VCCE_S._time4, VCCE_S.m4, (clock() - t0) * 1.0 / CLOCKS_PER_SEC);

	//	VCCE_S_res.Sort();
	//	fprintf(outFile, "Node	KVCC_id\n");
	//	for (TUNGraV::TIter GI = VCCE_S_res.BegI(); GI < VCCE_S_res.EndI(); GI++) {

	//		/*printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));

	//		cout << endl;*/
	//		//fprintf(outFile, "K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
	//		printNodesNum(*GI, outFile, ++j);
	//		fprintf(outFile, "\n");


	//	}
		fclose(outFile);
	}
	
		


	//	fprintf(outFile, "VCCE:\n");
	//	printf("VCCE:\n");
	//	VCCE VCCE(G, k);
	//	clock_t t2 = clock();
	//	TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);

	//	fprintf(outFile, "VCC_Num:%d\n"
	//		"time for LOC_CUT(flow>=k): %fs\n"
	//		"Call for LOC_CUT(flow>=k): %d\n"
	//		"time for LOC_CUT(flow<k): %fs\n"
	//		"Call for LOC_CUT(flow<k): %d\n"
	//		"time for LOC_CUT(fake): %fs\n"
	//		"Call for LOC_CUT(fake): %d\n"
	//		"Total Time: %fs\n", VCC2.Len(), VCCE._time2, VCCE.m2, VCCE._time3, VCCE.m3, VCCE._time4, VCCE.m4, (clock() - t2) * 1.0 / CLOCKS_PER_SEC);

	//	printf("VCC_Num:%d\n"
	//		"time for LOC_CUT(flow>=k): %fs\n"
	//		"Call for LOC_CUT(flow>=k): %d\n"
	//		"time for LOC_CUT(flow<k): %fs\n"
	//		"Call for LOC_CUT(flow<k): %d\n"
	//		"time for LOC_CUT(fake): %fs\n"
	//		"Call for LOC_CUT(fake): %d\n"
	//		"Total Time: %fs\n", VCC2.Len(), VCCE._time2, VCCE.m2, VCCE._time3, VCCE.m3, VCCE._time4, VCCE.m4, (clock() - t2) * 1.0 / CLOCKS_PER_SEC);

	//	
	//	//for (TUNGraV::TIter GI = VCC2.BegI(); GI < VCC2.EndI(); GI++) {

	//	//	printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++h, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
	//	//	printNodesNum(*GI, outFile);
	//	//	cout << endl;
	//	//}
	//	//printf("\n");
	//




	//}
	//fclose(outFile);
	//
}




