#include "Sweep_Algorithm.h"
#include <stdio.h>
using namespace std;
int main() {
	int k = 20;
	//PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/paper_fig.txt", 0, 1);
	PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/DBLP.txt", 0, 1);
	printf("G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
	printf("VCCE_Sweep:\n");
	VCCE_S VCCE_S(G, k);
	clock_t t1 = clock();
	TUNGraV VCC1 = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
	printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
	printf("%d\n", VCC1.Len());
	printf("time for SSV: %fs", VCCE_S._time);

	printf("VCCE:\n");
	VCCE VCCE(G, k);
	clock_t t2 = clock();
	TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);
	printf("%fs\n", (clock() - t2) * 1.0 / CLOCKS_PER_SEC);
	printf("%d\n", VCC2.Len());
	 
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

	//std::map<int, int> deposit;
	//TIntH deposit = {}, dist = {}, pru = {};
	//
	//for (int i = 0; i < 40000; i++) {
	//	deposit.AddKey(i);
	//	deposit[i] = 0;
	//}
	//
	//printf("%d", deposit.Len());
}