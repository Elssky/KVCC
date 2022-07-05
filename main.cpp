#include"Sweep_Algorithm.h"
#include "Heuristic_Algorithm.h"
#include"Utility.h"
#include<stdio.h>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")

using namespace std;

void printNodesNum(PUNGraph G, FILE* outfile) {
	TIntV Nodes;
	G->GetNIdV(Nodes);
	Nodes.Sort();
	for (TIntV::TIter TI = Nodes.BegI(); TI < Nodes.EndI(); TI++) {
		cout << "Node_ID:" << *TI << endl;
		fprintf(outfile, "Node_ID: %d\n", *TI);
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
	TStr dataset = "DBLP";//BkFig
	PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/"+ dataset +".txt", 0, 1);
	string dataset_name = dataset.CStr();
	string filename = "./output/"+ dataset_name + "_alpha=10000.txt";
	FILE* outFile = fopen(filename.c_str(), "w");
	printf("G: \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
	clock_t t1 = clock();
	BkVCC BkVCC(G, 20);
	BkVCC.dataset = dataset;
	TIntVIntV VCC1 = BkVCC.BkVCC_ENUM(BkVCC.G, BkVCC.k, 10000);
	int j = 0;
	fprintf(outFile, "Total Time: %fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
	
	for (TIntVIntV::TIter GI = VCC1.BegI(); GI < VCC1.EndI(); GI++) {
		PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
		fprintf(outFile, "K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
		for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
			//cout << *NI<<" ";
			fprintf(outFile, "%d ",*NI);
		}
		fprintf(outFile, "\n");
		//cout << endl;

	}

	int K_Arr[] = {20};
	j = 0;
	for (int i = 0; i < sizeof(K_Arr) / sizeof(int); i += 1) {
		int k = K_Arr[i];
		clock_t t1 = clock();
		fprintf(outFile, "## dataset: %s k = %d\n", dataset_name, k);
		printf("k = %d\n", k);
		printf("VCCE*:\n");
		fprintf(outFile, "VCCE*:\n");
		VCCE_S VCCE_S(G, k, 1);
		TUNGraV VCC1 = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
		if (VCC1.Len() == 0) break;
		fprintf(outFile, "VCC_Num:%d\n"
			"time for LOC_CUT(flow>=k): %fs\n"
			"Call for LOC_CUT(flow>=k): %d\n"
			"time for LOC_CUT(flow<k): %fs\n"
			"Call for LOC_CUT(flow<k): %d\n"
			"time for LOC_CUT(fake): %fs\n"
			"Call for LOC_CUT(fake): %d\n"
			"Total Time: %fs\n", VCC1.Len(), VCCE_S._time2, VCCE_S.m2, VCCE_S._time3, VCCE_S.m3, VCCE_S._time4, VCCE_S.m4, (clock() - t1) * 1.0 / CLOCKS_PER_SEC);

		printf("VCC_Num:%d\n"
			"time for LOC_CUT(flow>=k): %fs\n"
			"Call for LOC_CUT(flow>=k): %d\n"
			"time for LOC_CUT(flow<k): %fs\n"
			"Call for LOC_CUT(flow<k): %d\n"
			"time for LOC_CUT(fake): %fs\n"
			"Call for LOC_CUT(fake): %d\n"
			"Total Time: %fs\n", VCC1.Len(), VCCE_S._time2, VCCE_S.m2, VCCE_S._time3, VCCE_S.m3, VCCE_S._time4, VCCE_S.m4, (clock() - t1) * 1.0 / CLOCKS_PER_SEC);

		VCC1.Sort();
		for (TUNGraV::TIter GI = VCC1.BegI(); GI < VCC1.EndI(); GI++) {

			/*printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));

			cout << endl;*/
			fprintf(outFile, "K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
			//printNodesNum(*GI, outFile);
			fprintf(outFile, "\n");

		}
	}
	fclose(outFile);
		


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




