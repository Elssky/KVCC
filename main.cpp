#include "Sweep_Algorithm.h"
#include <stdio.h>
using namespace std;
int main() {
	//<Dataset>            <max K-VCC>
	//CA-AstroPh.txt			56
	//web-Stanford.txt        71
	PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/CA-AstroPh.txt", 0, 1);
	
	printf("G: \n node_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
	int K_Arr[] = {23, 34};
	for (int i = 0; i < sizeof(K_Arr)/sizeof(int); i += 1) {
		int k = K_Arr[i];
		printf("k = %d\n", k);
		printf("VCCE_Sweep:\n");
		for (int i = 2; i <= 2; i++) {
			VCCE_S VCCE_S(G, k, i);
			printf("Compute_SSV_times = %d\n", VCCE_S.Compute_SSV_times);
			clock_t t1 = clock();
			TUNGraV VCC1 = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
			printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
			printf("VCC_Num:%d\n", VCC1.Len());
			printf("time for Get_SSV: %fs\n", VCCE_S._time);
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
			printf("non_pru/all = %f\n", 1.0 * VCCE_S.non_pru_node / (1.0 * (VCCE_S.non_pru_node + VCCE_S.pru_node)));

			printf("\n");
		}
		


		//printf("VCCE:\n");
		//VCCE VCCE(G, k);
		//clock_t t2 = clock();
		//TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);
		//printf("%fs\n", (clock() - t2) * 1.0 / CLOCKS_PER_SEC);
		//printf("VCC_Num:%d\n", VCC2.Len());
		///*printf("time for GLO_CUT: %fs\n", VCCE._time);
		//printf("Call for GLO_CUT: %d\n", VCCE.m);*/
		//printf("time for LOC_CUT: %fs\n", VCCE._time2);
		//printf("Call for LOC_CUT: %d\n", VCCE.m2);
	}
	 


}




/*for (TUNGraV::TIter GI = VCC.BegI(); GI < VCC.EndI(); GI++) {
	printf("GI: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
}*/

/*FILE* outFile = fopen("exp_data.txt", "w");
fprintf(outFile, "G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
for (int k = 40; k <= 40; k += 5) {
	fprintf(outFile, "k = %d\n", k);
	fprintf(outFile, "VCCE_Sweep:\n");
	VCCE_S VCCE_S(G, k);
	clock_t t1 = clock();
	TUNGraV VCC1 = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
	fprintf(outFile, "%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);

	fprintf(outFile, "VCCE:\n");
	VCCE VCCE(G, k);
	clock_t t2 = clock();
	TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);
	fprintf(outFile, "%fs\n", (clock() - t2) * 1.0 / CLOCKS_PER_SEC);

	printf("%4f", VCCE_S._time);
}
fclose(outFile);*/