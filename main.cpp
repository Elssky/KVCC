#include"Sweep_Algorithm.h"
#include"Utility.h"
#include<stdio.h>
#include <string>
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


	//preprocess
	/*format_graph("./dataset/DBLP.txt");*/
	PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/CA-AstroPh.txt", 0, 1);
	printf("G: \n node_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
	
	int K_Arr[] = {11};//13, 26, 39, 51 
	string filename = "CA-AstroPh_out1.txt";
	FILE* outFile = fopen(filename.c_str(), "a");
	//for (int i = K_Arr[0]; ; i += 1) {
	for (int i = 0; i < sizeof(K_Arr) / sizeof(int); i += 1) {
		int k = K_Arr[i];
		//string filename = "Cit_k=" + to_string(k) + ".txt";A
		//int k = i;
		fprintf(outFile, "## dataset: CA-AstroPh k = %d\n", k);
		printf("k = %d\n", k);
		
		
		int j = 0;
		/*for (int i = 0; i <= 0; i++) {*/
		printf("VCCE*:\n");
		fprintf(outFile, "VCCE*:\n");

		VCCE_S VCCE_S(G, k, 1);
		//printf("Compute_SSV_times = %d\n", VCCE_S.Compute_SSV_times);
		clock_t t1 = clock();
		TUNGraV VCC1 = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
		/*printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);*/
		printf("VCC_Num:%d\n", VCC1.Len());
		if (VCC1.Len() == 0) break;
		fprintf(outFile, "VCC_Num:%d\n", VCC1.Len());
		fprintf(outFile, "time for LOC_CUT(flow>=k): %fs\n", VCCE_S._time2);
		fprintf(outFile, "Call for LOC_CUT(flow>=k): %d\n", VCCE_S.m2);
		fprintf(outFile, "time for LOC_CUT(flow<k): %fs\n", VCCE_S._time3);
		fprintf(outFile, "Call for LOC_CUT(flow<k): %d\n", VCCE_S.m3);
		fprintf(outFile, "time for LOC_CUT(fake): %fs\n", VCCE_S._time4);
		fprintf(outFile, "Call for LOC_CUT(fake): %d\n", VCCE_S.m4);
		printf("time for LOC_CUT(flow>=k): %fs\n", VCCE_S._time2);
		printf("Call for LOC_CUT(flow>=k): %d\n", VCCE_S.m2);
		printf("time for LOC_CUT(flow<k): %fs\n", VCCE_S._time3);
		printf("Call for LOC_CUT(flow<k): %d\n", VCCE_S.m3);
		printf("time for LOC_CUT(fake): %fs\n", VCCE_S._time4);
		printf("Call for LOC_CUT(fake): %d\n", VCCE_S.m4);
		//VCC1.Sort();
		/*printf("time for Get_SSV: %fs\n", VCCE_S._time);
		printf("Call for Get_SSV: %d\n", VCCE_S.m);
		printf("time for LOC_CUT(flow>=k): %fs\n", VCCE_S._time2);
		printf("Call for LOC_CUT(flow>=k): %d\n", VCCE_S.m2);
		printf("time for LOC_CUT(flow<k): %fs\n", VCCE_S._time3);
		printf("Call for LOC_CUT(flow<k): %d\n", VCCE_S.m3);
		printf("time for LOC_CUT(fake): %fs\n", VCCE_S._time4);
		printf("Call for LOC_CUT(fake): %d\n", VCCE_S.m4);
		printf("Call for LOC_CUT(SSV_rule): %d\n", VCCE_S.c);
		printf("pru_node_nums: %d\n", VCCE_S.pru_node);
		printf("non_pru_node_nums: %d\n", VCCE_S.non_pru_node);
		printf("non_pru/all = %f\n", 1.0 * VCCE_S.non_pru_node / (1.0 * (VCCE_S.non_pru_node + VCCE_S.pru_node)));*/
		fprintf(outFile, "%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
		////for (TUNGraV::TIter GI = VCC1.BegI(); GI < VCC1.EndI(); GI++) {

		////	/*printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));

		////	cout << endl;*/			
		////	fprintf(outFile, "K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
		////	//printNodesNum(*GI, outFile);
		////	fprintf(outFile, "\n");
		////	
		////}
		//printf("\n");


		/*}*/
		

		//fprintf(outFile, "VCCE:\n");
		//printf("VCCE:\n");
		//VCCE VCCE(G, k);
		//clock_t t2 = clock();
		//TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);
		//fprintf(outFile, "VCC_Num:%d\n", VCC2.Len());
		//fprintf(outFile, "%fs\n", (clock() - t2) * 1.0 / CLOCKS_PER_SEC);
		//fprintf(outFile, "time for LOC_CUT(flow>=k): %fs\n", VCCE._time2);
		//fprintf(outFile, "Call for LOC_CUT(flow>=k): %d\n", VCCE.m2);
		//fprintf(outFile, "time for LOC_CUT(flow<k): %fs\n", VCCE._time3);
		//fprintf(outFile, "Call for LOC_CUT(flow<k): %d\n", VCCE.m3);
		//fprintf(outFile, "time for LOC_CUT(fake): %fs\n", VCCE._time4);
		//fprintf(outFile, "Call for LOC_CUT(fake): %d\n", VCCE.m4);
		////printf("time for LOC_CUT(flow>=k): %fs\n", VCCE._time2);
		//printf("time for LOC_CUT: %fs\n", VCCE._time2+ VCCE._time3+ VCCE._time4);
		//printf("Call for LOC_CUT: %d\n", VCCE.m2+ VCCE.m3+ VCCE.m4);
		//printf("\n");
		
		//	int h = 0;
		//	printf("VCCE:\n");
		//	VCCE VCCE(G, k);
		//	clock_t t2 = clock();
		//	TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);
		//	/*printf("%fs\n", (clock() - t2) * 1.0 / CLOCKS_PER_SEC);*/
		//	printf("VCC_Num:%d\n", VCC2.Len());
		//	VCC2.Sort();
		//	/*printf("time for GLO_CUT: %fs\n", VCCE._time);
		//	printf("Call for GLO_CUT: %d\n", VCCE.m);*/
		//	/*printf("time for LOC_CUT: %fs\n", VCCE._time2);
		//	printf("Call for LOC_CUT: %d\n", VCCE.m2);*/
		//	for (TUNGraV::TIter GI = VCC2.BegI(); GI < VCC2.EndI(); GI++) {

		//		printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++h, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
		//		printNodesNum(*GI);
		//		cout << endl;
		//	}
		//	printf("\n");
		//}




	}
	fclose(outFile);

	//内存记录
	//HANDLE handle = GetCurrentProcess();
	//PROCESS_MEMORY_COUNTERS pmc;
	//GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	//printf("%d MB\r\n", pmc.WorkingSetSize / (1024 * 1024));  //结果保存单位是MB
	
}







//FILE* outFile = fopen("exp_data.txt", "w");
//fprintf(outFile, "G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
//for (int k = 40; k <= 40; k += 5) {
//	fprintf(outFile, "k = %d\n", k);
//	fprintf(outFile, "VCCE_Sweep:\n");
//	VCCE_S VCCE_S(G, k);
//	clock_t t1 = clock();
//	TUNGraV VCC1 = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
//	fprintf(outFile, "%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
//
//	fprintf(outFile, "VCCE:\n");
//	VCCE VCCE(G, k);
//	clock_t t2 = clock();
//	TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);
//	fprintf(outFile, "%fs\n", (clock() - t2) * 1.0 / CLOCKS_PER_SEC);
//
//	printf("%4f", VCCE_S._time);
//}
//fclose(outFile);