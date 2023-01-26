#include "Heuristic_Algorithm.h"
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




	//加载已经生成的结果
	/*try
	{
		TFIn inFile("D:/Git Project/KVCC-ENUM//community/" + dataset + "_k=" + TStr(k_str) + ".kvcc");
		G_R.Load(inFile);
	}
	catch (TPt<TExcept>)
	{
		cout << endl << "***kvcc result does not exist, running BkVCC function...***" << endl;
	}*/

	/*if (G_R.Len() != 0) return G_R;
	int G1[] = { 19885, 6158, 184606, 13907, 184607, 18395, 19990, 21134, 119540, 126780, 193053 };
	int G2[] = {665, 796, 1442, 2027, 2571, 2656, 2711, 2805, 2962, 3051, 3127, 3137, 3331, 3420, 3668, 3673, 4316, 4446, 4460, 4496, 4533, 4749, 4769, 4912, 4913, 5052, 5184, 5209, 5470, 5784, 5924, 6433, 6772, 6906, 6925, 6947, 6959, 7008, 7059, 7060, 7093, 7168, 7236, 7280, 7404, 7440, 7585, 7630, 7691, 7934, 8074, 8093, 8310, 8566, 8604, 8741, 8809, 8826, 8889, 8930, 9084, 9224, 9449, 9462, 9475, 9726, 9809, 9812, 9943, 10516, 11114, 11123, 11255, 11425, 11426, 11427, 11457, 11492, 11567, 11959, 11983, 12271, 12300, 12447, 12751, 12811, 13007, 13076, 13105, 13263, 13304, 13451, 13627, 13723, 13763, 13885, 14041, 14230, 14442, 14717, 14797, 15192, 15365, 15442, 15568, 15737, 15760, 15954, 15973, 16276, 16373, 16393, 16683, 16833, 16835, 16876, 16986, 16996, 16997, 17217, 17393, 17501, 17505, 17930, 18114, 18280, 18324, 18519, 18538, 18741, 18797, 18823, 19114, 19306, 19610, 19661, 19831, 19909, 20152, 20167, 20320, 20435, 20450, 20488, 20503, 20689, 20696, 20777, 21047, 21055, 21114, 21158, 21250, 21570, 21634, 21708, 21759, 22015, 22159, 22559, 22625, 22853, 22904, 23096, 23156, 23184, 23302, 23305, 23566, 23572, 23573, 23574, 23575, 24046, 24074, 24198, 24257, 24476, 24675, 24677, 24817, 24947, 25347, 25670, 25903, 26144, 26208, 26376, 26497, 26613, 26626, 26854, 26970, 27213, 27264, 27309, 27627, 27715, 27777, 27925, 28290, 28293, 28432, 28888, 29515, 29830, 30012, 30023, 30028, 30030, 30141, 30271, 30312, 30628, 30815, 30925, 31014, 31074, 31089, 31154, 31470, 31519, 31539, 31560, 32534, 32535, 32707, 32708, 32876, 33148, 33274, 33277, 33782, 34135, 34355, 34561, 34793, 34877, 34975, 34990, 35151, 35213, 35239, 35249, 35376, 35481, 35491, 35521, 35534, 35736, 35860, 36082, 36149, 36172, 36241, 36407, 36447, 36485, 36799, 36868, 37174, 37394, 37572, 38154, 38183, 38508, 38546, 38672, 38700, 38868, 38911, 39363, 39387, 39420, 39726, 39744, 39966, 40108, 40215, 40456, 40579, 40589, 40608, 41089, 41110, 41119, 41329, 41549, 41815, 41825, 41847, 41929, 41947, 41990, 42450, 42671, 42857, 43227, 43377, 43763, 43871, 44187, 44544, 44646, 44708, 44729, 44782, 44783, 44873, 44888, 45116, 45164, 45289, 45290, 45291, 45292, 45293, 45479, 45686, 46034, 46124, 46276, 46397, 46577, 46911, 46964, 47541, 47938, 48047, 48163, 48525, 48555, 48792, 48793, 48794, 48819, 49054, 49190, 49246, 49799, 49906, 50153, 50458, 50730, 50788, 50825, 50878, 50969, 51133, 52491, 52539, 52650, 53039, 53240, 55019, 55049, 55155, 55885, 56036, 56444, 56936, 56996, 57215, 57571, 59029, 59487, 60125, 60168, 61074, 61506, 61615, 61708, 61780, 62152, 63003, 63227, 63241, 63472, 63577, 63684, 63985, 64475, 64673, 64715, 64988, 65107, 65241, 65311, 65676, 65957, 66095, 66338, 66420, 66993, 67024, 67140, 67483, 67677, 67706, 67739, 68242, 68263, 68390, 68391, 68740, 68760, 69054, 69406, 70089, 70241, 70248, 71267, 71322, 71507, 71906, 72070, 72210, 72704, 73194, 73468, 73729, 73973, 74064, 75084, 75105, 75507, 75671, 75934, 75984, 75985, 76297, 76323, 76324, 76434, 76524, 76746, 77172, 77324, 77813, 78219, 78220, 78221, 78222, 78223, 78224, 78341, 79279, 80253, 80271, 80466, 81262, 81496, 81504, 81683, 81856, 81939, 82316, 82830, 83076, 83315, 83568, 83570, 84657, 84775, 85141, 85153, 85397, 86439, 86463, 87049, 87319, 88381, 88504, 88766, 88795, 89460, 89558, 89910, 90027, 90513, 91011, 91910, 92582, 92634, 93217, 93427, 95534, 95744, 95828, 96246, 97422, 97423, 97424, 97670, 97734, 97752, 97838, 97839, 97984, 98202, 99746, 100585, 100588, 100722, 100723, 100861, 101299, 101840, 102278, 103632, 105049, 105486, 105488, 105562, 105644, 105873, 106008, 106561, 106610, 107555, 108168, 108369, 108606, 109825, 110291, 110659, 111307, 112848, 112863, 112873, 113097, 113288, 113535, 113716, 113833, 114253, 114575, 114804, 114816, 114914, 115457, 115586, 115728, 115911, 115985, 116246, 116555, 116969, 116970, 117446, 117657, 117888, 119386, 119707, 121117, 121313, 121728, 121742, 121871, 122123, 123582, 123895, 124137, 124337, 124395, 124411, 124415, 124483, 124535, 125678, 128134, 129444, 130251, 131243, 131522, 131740, 134360, 134619, 135307, 136180, 137772, 139431, 139600, 139822, 140707, 141222, 141794, 141841, 141911, 142076, 142571, 143736, 144488, 144678, 146115, 146261, 146604, 147373, 147477, 148255, 149041, 149143, 149545, 149805, 149806, 150043, 150703, 153714, 153885, 154161, 155494, 156384, 161850, 162521, 162781, 162782, 162903, 162943, 163015, 163023, 163636, 163771, 164170, 164224, 164465, 164711, 165126, 165162, 165355, 167700, 168916, 169078, 174083, 174279, 174700, 175116, 178333, 179483, 180356, 180399, 181479, 182559, 184038, 185173, 185262, 185919, 186318, 186376, 187361, 191845, 192103, 193651, 193978, 197163, 198074, 199059, 199407, 199804, 201324, 203626, 203738, 203790, 205395, 205756, 205757, 206388, 206389, 208249, 209669, 209670, 209692, 209722, 209794, 211602, 211757, 212559, 212782, 217431, 219725, 220731, 220936, 223457, 225221, 225902, 227233, 227569, 229756, 229929, 229989, 230620, 230621, 230779, 230780, 230781, 231182, 231184, 231983, 232550, 232777, 234780, 235243, 235244, 237109, 237142, 238123, 238302, 238310, 238793, 238871, 241405, 243032, 243335, 243564, 244063, 244893, 245201, 246013, 249331, 250243, 250558, 250730, 251386, 252106, 252413, 252697, 255210, 256664, 256807, 256943, 257012, 257013, 257014, 257015, 257016, 257017, 257018, 257019, 257020, 257021, 257022, 257023, 257024, 257025, 257533, 257717, 258204, 259032, 260255, 260256, 260257, 260676, 261762, 262459, 264353, 265003, 269413, 270049, 270050, 270051, 270519, 272551, 272552, 273591, 273592, 273687, 274213, 274748, 275945, 278141, 278504, 279245, 279265, 280608, 280671, 281808, 282970, 282989, 283362, 283981, 284923, 286303, 286304, 286305, 286306, 286307, 286308, 286309, 286310, 286311, 286312, 286313, 286314, 286315, 286316, 286317, 286318, 286319, 286320, 286321, 286322, 286323, 286324, 286325, 286326, 286327, 286328, 286339, 286340, 286341, 286342, 286343, 286344, 286345, 286346, 286347, 286348, 286349, 286350, 286351, 286352, 286353, 286354, 286355, 286356, 286357, 286358, 286359, 286360, 286397, 286509, 286510, 286511, 286724, 286725, 286932, 287674, 287845, 287846, 288323, 288324, 288325, 289261, 289748, 291410, 291411, 293497, 294284, 294935, 294936, 294937, 295029, 295715, 295716, 295966, 295967, 297506, 297636, 298065, 298066, 298337, 298902, 302242, 305251, 305407, 305408, 306215, 307088, 307089, 307090, 307439, 308275, 308639, 308943, 311316, 313067, 313480, 315101, 315769, 317339, 317913, 318972, 319279, 319280, 319281, 319282, 319283, 319357, 319822, 319823, 319824, 319825, 319826, 319827, 319828, 319829, 319830, 319831, 319832, 319833, 319834, 319835, 319836, 320557, 320558, 320559, 320839, 320840, 320841, 320842, 320843, 320905, 320906, 320907, 321181, 321182, 321420, 321670, 321750, 321751, 321752, 321753, 321754, 321755, 321771, 323387, 323390, 324166, 324478, 325271, 326452, 328201, 329279, 329280, 333294, 333469, 333470, 333471, 333472, 333473, 334605, 334607, 335560, 335561, 335920, 335921, 335922, 346257, 349190, 353858, 353943, 354756, 354757, 355563, 358759, 359819, 360069, 360070, 360071, 360938, 361633, 361634, 361635, 361636, 361637, 361638, 361639, 361640, 361641, 361642, 361643, 361644, 361645, 361646, 361647, 361648, 361649, 361650, 361651, 361652, 361653, 361654, 361655, 361656, 361657, 361658, 361659, 361660, 361661, 361662, 361663, 361664, 361665, 361666, 362583, 363886, 364248, 364570, 366743, 366744, 366745, 366746, 369080, 369091, 369092, 369545, 369546, 379345, 404632, 404660, 404914, 407890, 407891, 416144, 419693, 419694
};
	TIntV GI1, GI2;
	for (int i = 0; i < sizeof(G1) / sizeof(int); i++) {
		GI1.Add(G1[i]);
	}
	for (int i = 0; i < sizeof(G2) / sizeof(int); i++) {
		GI2.Add(G2[i]);
	}*/
	/*IsMergeValid_maxflow(GI1, GI2);
	IsMergeValid(GI1, GI2);*/
	G_S = Seeding(G, k, alpha);
	//for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
	//	PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
	//	printf("seed-subgraph(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
	//	for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
	//		//cout << *NI<<" ";
	//		printf("%d ", *NI);
	//	}
	//	printf("\n");
	//}

	cout << "Seeding subgraph num:" << G_S.Len() << endl;
	_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;

	do{
		
		G_S_prime = G_S;
		//j = 0;
		//merging G_S		
		if (strcmp(mergeMethod.c_str(), "") != 0) {
			//打乱顺序
			//G_S.Reverse();
			Merging(k, G_S, G_R);
			/*cout << "after Merging subgraph num:" << G_S.Len() << endl;*/
		}
			
		//
		//for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
		//	PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
		//	printf("merging-subgraph(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
		//	for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
		//		//cout << *NI<<" ";
		//		printf("%d ", *NI);
		//	}
		//	printf("\n");
		//}
		//G_R.Merge();

		//expanding G_S
		if (strcmp(expandMethod.c_str(), "") != 0)
			Expanding(k, G_S);

		//cout << "after Expanding subgraph num:" << G_S.Len() << endl;
		//j = 0;
		//for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
		//	PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
		//	printf("expanding-subgraph(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
		//	for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
		//		//cout << *NI<<" ";
		//		printf("%d ", *NI);
		//	}
		//	printf("\n");
		//}

		/*G_S.Merge();

		G_R.Sort();
		G_S.Sort();
		cout << "G_R num:" << G_R.Len() << endl;
		cout << G_R[G_R.Len() - 1].Len() << endl;
		cout << "G_S num:" << G_S.Len() << endl;
		cout << G_S[G_S.Len() - 1].Len() << endl;*/
	} while (G_S != G_S_prime); // TODO: 如何判断是否发生变化，仅靠长度肯定是不行的 FINISH:用顶点数组表示一张图，对比数组即可


	// G_R = G_R Union G_S
	// 
	// error can not use this function
	// 无法比较两个图是否相同，所以sort相关的函数比如AddMergedV(), Merge()都不能使用，除非重载运算符

	//09/04 在图的内部也进行排序操作 比如其中有两个元素[3,5,7,9,22]与[3,9,5,22,7]，
	//这两个是不会合并的，因为merge判断需要完全相同，所以需要在每个元素内部在进行一次merging操作
	G_R.AddVMerged(G_S);
	for (TIntVIntV::TIter TI = G_R.BegI(); TI < G_R.EndI(); TI++) {
		TI->Sort();
	}
	G_R.Merge();
	//保存结果
	TFOut outFile("D:/Git Project/KVCC-ENUM/community/" + dataset + "_k=" + TStr(k_str) + 
		"_alpha=" + TStr(alpha_str) + "_algorithm=" + TStr(alg.c_str()) + "_expand=" + expandMethod.c_str()
		+ "_merge=" + mergeMethod.c_str() +"_t=" + std::to_string(threads).c_str() + ".kvcc");
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

	TIntVIntV res = randSample(nb_set, k, alpha);

	//cout << "res.len:" << res.Len() << endl;
	/*cout << "u = " << u << ":" << endl;*/
	/*cout << "P_Prime:";
	for (TUNGraph::TNodeI NI1 = P_prime->BegNI(); NI1 < P_prime->EndNI(); NI1++) {
		cout << NI1.GetId() << " ";
	}
	cout << endl;
	cout << "nb_set:";
	for (TIntV::TIter TI = nb_set.BegI(); TI < nb_set.EndI(); TI++) {
		cout << *TI << " ";
	}
	cout << endl;
	cout << "res.len:" << res.Len() << endl;
	for (int i = 0; i < res.Len() && i <= alpha; i++) {
		for (TIntV::TIter TI = res[i].BegI(); TI < res[i].EndI(); TI++) {
			cout << *TI << " ";
		}
		cout << endl;
	}*/



	//nb_set.Add(u);

	//nb_set.Gen(k);

	for (int i = 0; i < res.Len() && i <= alpha; i++) {

		TIntV R = res[i];
		R.AddUnique(u); // R = R Union u
		R.Merge();
		/*if (u == 0 || u == 5 || u == 17 || u == 29 || u == 55) {
			for (TIntV::TIter TI = R.BegI(); TI < R.EndI(); TI++) {
				cout << *TI << " ";
			}
			cout << endl;
		}*/

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

			if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId()) < k) {
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

TIntVIntV BkVCC::Seeding(PUNGraph G, int k, int alpha)
{

	TIntVIntV G_S;
	TIntV G_C;
	TIntH CandMaintain, deg;
	int i = 0;


	try
	{
		TFIn inFile("./seedgraph/" + dataset + "_k=" + TStr(k_str) + "_alpha=" + TStr(alpha_str) + "_seed="+ TStr(seed.c_str()) + ".seed");
		//TFIn inFile("D:/Git Project/KVCC-ENUM/seedgraph/DBLP_k=5_alpha=1000_algorithm=BkVCC.seed");
		G_S.Load(inFile);
	}
	catch (TPt<TExcept>)
	{
		cout << endl << "***seedgraph does not exist, running seeding function...***" << endl;
	}

	if (G_S.Len() != 0) return G_S;


	if (strcmp(seed.c_str(), "rand") == 0) {
		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}
		deg.SortByDat(); //non-decreasing order

		for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
			//cout << HI.GetKey() << " " << HI.GetDat() << endl;

			int v = HI.GetKey();
			if (CandMaintain.GetDat(v) == 0) {
				//cout << HI.GetKey() << " " << HI.GetDat() << endl;
				G_C = LkVCS(G, k, v, alpha);
				//cout<< ++i <<endl;
				if (G_C.Empty()) continue;

				G_S.Add(G_C); //怎么判断是否相等（Unique）？


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
	else if (strcmp(seed.c_str(), "clique") == 0) {
		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);
	}
	else if (strcmp(seed.c_str(), "k+1-clique") == 0) {
		TCliqueOverlap::GetCliquesSizeof(G, k + 1, G_S);
	}
	else if (strcmp(seed.c_str(), "mix") == 0) {
		//先用clique生成seed，在使用rand的方法选中不属于任何seed的点作为中心来生成新的seed

		//step1.先生成max_cliques
		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);
		//cout << G_S.Len()<< endl;

		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		//step2.将处于max_cliques的点从CandMaintain设为1，表示已被选入子图
		//TODO：直接将max_cliques中的点删除
		//TUNGraph G_temp = *G;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				//if(G_temp.IsNode(*NI)) G_temp.DelNode(*NI);
				
				//cout << i++ << ": " << CandMaintain.GetDat(*NI) << endl;
				
			}
		}
		//cout << G_temp.GetNodes() << endl;
		//cout << G_S.Len() << endl;
		
		int i = 0;

		deg.SortByDat(); //non-decreasing order

		for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
			//cout << HI.GetKey() << " " << HI.GetDat() << endl;

			int v = HI.GetKey();
			if (CandMaintain.GetDat(v) == 0) {
				//cout << HI.GetKey() << " " << HI.GetDat() << endl;
				G_C = LkVCS(G, k, v, alpha);
				//cout<< ++i <<endl;
				if (G_C.Empty()) continue;

				G_S.Add(G_C); //怎么判断是否相等（Unique）？


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

			//TODO:修改获取cand的方式
			// 通过IMF算法来将多个点同时expand,而这些点单独expand时是不满足条件的
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
	else if(threads >= 2) {
		#pragma omp parallel
		{
			int threadId = omp_get_thread_num();
			#pragma omp for
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

//用于ring_expanding中节点加入到local-kvcc中， 其邻居更新到local-kvcc边数

void update_neighbour(TIntVIntV& S, TIntIntVH in_neighs, TIntIntVH out_neighs, int v, TIntV& res) {
	TIntV neigh = out_neighs.GetDat(v);
	//cout << neigh.Len() << endl;
	for (TIntV::TIter NI = neigh.BegI(); NI < neigh.EndI(); NI++) {
		int idx = in_neighs.GetDat(*NI).Len();
		//cout << idx << endl;
		if (idx < 1) continue;
		if (idx == k - 1) {
			S[idx].DelIfIn(*NI);
			S[idx + 1].AddMerged(*NI);
			res.Add(*NI);
			//update_neighbour(S, in_neighs, out_neighs, *NI, res);
		}
		else if (idx < k - 1) {
			if(S[idx].IsIn(*NI)) S[idx].DelIfIn(*NI);		
			S[idx + 1].AddMerged(*NI);
		}
		
	}
}

int BkVCC::ring_expanding(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	//step1. 仅考虑每个local-kvcc的节点和一跳邻居节点之间的边(诱导子图)
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV G_sub = G_S;
	G_sub.AddVMerged(delta_S_bar);
	PUNGraph Gra = TSnap::GetSubGraph(G, G_sub);

	//step2. 每个节点计算自己到local-kvcc的边个数，分成集合S_(k+), S_(k-1), S_(k-2), ...
	// 使用二维数组存储集合
	TIntVIntV S;
	for (int i = 0; i <= k; i++) {
		S.Add({});
	}
	TIntV res = {};
	TIntV nb_u1 = {}, nb_u2 = {};
	//in_neighs存储下每个候选节点的属于local-kvcc的邻居，节省计算时间
	//out_neighs存储下每个候选节点的属于候选集中的邻居
	TIntIntVH in_neighs, out_neighs;

	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u1.Clr();
		nb_u2.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = Gra->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (delta_S.IsIn(CtrNode.GetInNId(i))) //只把local-kvcc中的邻居加进去
				nb_u1.AddMerged(CtrNode.GetInNId(i));
			else
				nb_u2.AddMerged(CtrNode.GetInNId(i));
				

		}
		in_neighs.AddDat(*TI, nb_u1);
		out_neighs.AddDat(*TI, nb_u2);
		int index = nb_u1.Len();
		
		//index为S集合下标索引
		if (index >= k) index = k; //这里k统一存储k+的节点
		S[index].AddMerged(*TI);

	}

	//step3. S_(k+)的点首先加入到kvcc中，其邻居更新到kvcc的边数，更新S
	int S_Len;

	for (TIntV::TIter TI = S[k].BegI(); TI < S[k].EndI(); TI++) {
		res.Add(*TI);
		update_neighbour(S, in_neighs, out_neighs, *TI, res);
	}

	//好像没有必要while循环
	/*
	S_Len = S[k].Len();
	do {
		S_Len = S[k].Len();
		for (TIntV::TIter TI = S[k].BegI(); TI < S[k].EndI(); TI++) {
			res.Add(*TI);
			update_neighbour(S, neighs, *TI);
		}
	} while (S[k].Len() != S_Len);*/

	//step4. Sk-1中的点，查找邻居中是否有Sk-1中的点与其与kvcc中交点集合不完全相同，
	//若有，两点同时加入kvcc，其邻居更新与kvcc连边数量，
	//继续此过程，直到剩余的Sk-1中的点都不满足条件
	//TODO: 这里的问题是一遍for循环能保证剩余的点都不满足条件吗？
	//另外如果存在点后加入k-1集合，就有可能出现*TI在\*NI后面的情况，这时TI--两次
	//cout << 1 << endl;
	//for (TIntV::TIter TI = S[k - 1].BegI(); TI < S[k - 1].EndI(); TI++) {
	//	
	//	TIntV neigh = out_neighs.GetDat(*TI);

	//	for (TIntV::TIter NI = neigh.BegI(); NI < neigh.EndI(); NI++) {

	//		if (in_neighs.GetDat(*NI).Len() == k - 1) {
	//			int common_neighs = TSnap::GetCommon(in_neighs.GetDat(*TI), in_neighs.GetDat(*NI));
	//			if (common_neighs < k - 1) {
	//				//这两个节点可以加入kvcc
	//				res.Add(*TI);
	//				res.Add(*NI);
	//				update_neighbour(S, in_neighs, out_neighs, *TI, res);
	//				update_neighbour(S, in_neighs, out_neighs, *NI, res);
	//				S[k - 1].DelIfIn(*TI);
	//				S[k - 1].DelIfIn(*NI);
	//				//if(S[k - 1].get)
	//				TI--; //因为删除*TI导致后面的元素前移，所以迭代器索引-1，删除*NI不会受影响，因为NI肯定出现在*TI后面（如果在前面，应该已经更新了）
	//				break;
	//			}
	//		}
	//	}
	//}
	//cout << 2 << endl;

	/*int flag = 1;
	do {
		S_Len = S[k - 1].Len();
		

	} while (flag == 1);*/

	//step5. 从S_(k-2)开始，找clique，具体来讲S_(k-2)找3-clique，S_(k-3)找4-clique，...,S_(k-i)找size>= (i+1)的maximal clique
	//然后判断这些clique在local-kvcc中并集的邻居的个数是否$\geq k$
	//若是，则同时加入到local-kvcc中，并更新其周围邻居。
	int num = 0;
	for (int i = k - 1; i > 0; i--) {
		TIntVIntV cliques;
		TIntV neigh_Union;
		if (S[i].Len() < k - i + 1) continue;
		PUNGraph sub_G = TSnap::GetSubGraph(Gra, S[i]);
		/*PUNGraph sub_core = TSnap::GetKCore(sub_G, k - i);*/
		//printf("sub_G: \nnode_nums = %d, edge_nums = %d\n", sub_G->GetNodes(), sub_G->GetEdges());
		/*printf("sub_core: \nnode_nums = %d, edge_nums = %d\n", sub_core->GetNodes(), sub_core->GetEdges());*/
		TCliqueOverlap::GetMaxCliques(sub_G, k - i + 1, cliques);
		//TCliqueOverlap::GetCliquesSizeof(sub_core, k - i + 1, cliques);
		//cout << i << ":" << cliques.Len() << endl;
		for (int clique_idx = 0; clique_idx < cliques.Len(); clique_idx++) {
			neigh_Union.Clr();
			for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {
				if (!S[i].IsIn(*TI)) break;
				neigh_Union.AddV(in_neighs.GetDat(*TI));
				neigh_Union.Merge();
				if (neigh_Union.Len() >= k) break;
			}
			if (neigh_Union.Len() >= k) {
				for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {
					res.Add(*TI);
					S[i].DelIfIn(*TI);
					update_neighbour(S, in_neighs, out_neighs, *TI, res);
				}

				num += cliques[clique_idx].Len();
			}			
		}
		/*for (TIntV::TIter TI = S[k - i].BegI(); TI < S[k - i].EndI(); TI++) {

		}*/
	}
	if (num != 0) {
		cout << "expanding_num:" << num << endl;
	}
	

	G_S.AddV(res);
	G_S.Merge();
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


int BkVCC::GetCand(TIntV &G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	TIntV res = {};
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;
	int itr = 2;//最多迭代10次


	//原有的expand方式
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

	//记录每个节点的max_flow值
	TIntH flow;
	//源节点，目标节点
	int s = -1, t = -1;
	
	//源节点必须是非边缘点
	for (TIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {

		if (!delta_S.IsIn(*TI)) { s = *TI; break; }
	}

	//将满足条件的节点直接expand到kvcc子图中
	//Attention:这里必须先确定source vertex之后再将进行expand，
	//不然expand的点也不属于delta_S,会误选为source vertex，结果会出错
	G_S.AddVMerged(res);

	if (G_S.Len() >= 100) return res.Len(); //太大就不用maxflow判断，耗时太长

	//不存在非边缘点时应该如何处理？这里是直接结束，应该做进一步考虑
	if (s == -1) { //cout << "end" << endl; 
		return res.Len();
	}

	//为便于最大流计算，将满足条件的节点从delta_S_bar删除
	for (TIntV::TIter TI = res.BegI(); TI < res.EndI(); TI++) {
		delta_S_bar.DelIfIn(*TI);
	}



	
	TIntV G_U = G_S;
	G_U.AddVMerged(delta_S_bar);
	
	

	

	//cout << s << endl;
	TIntV delta_S_bar_star;//记录候选顶点
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
			else G_U.DelIfIn(*TI);//移除最大流小于k的顶点
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
		if (delta_S_bar == delta_S_bar_star || it >= itr) break;
		
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


int BkVCC::flag3(TIntV &G_S, TIntV& delta_S, TIntV& delta_S_bar) {
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
		G_S_temp = G_S;
		TVec<TIntVIntV> G_S_set = {};
		for (int t = 0; t < threads; t++) {
			G_S_set.Add({});
		}
		/*cout << G_S_set.Len() << endl;*/
		for (int i = 0; i < G_S.Len(); i++) {
			G_S_set[i % threads].Add(G_S[i]);
		}
		do {
			#pragma omp parallel 
			{
				//G_S_temp = G_S;
				int threadId = omp_get_thread_num();
			#pragma omp for
				for (int t = 0; t < threads; t++) {

					for (int i = 0; i < G_S_set[t].Len(); i++) {
						for (int j = i + 1; j < G_S_set[t].Len();) {
							TIntV GI1 = G_S_set[t][i];
							TIntV GI2 = G_S_set[t][j];
							if (strcmp(mergeMethod.c_str(), "flow") == 0) {
								if (IsMergeValid_maxflow(GI1, GI2)) {
									TIntV G_Union = {};
									G_Union.AddVMerged(GI1);
									G_Union.AddVMerged(GI2);

								
									if (IskVCC(G_Union, k)) {
										#pragma omp critical			
										G_R.Add(G_Union);
										G_S_set[t].DelIfIn(GI1);
										G_S_set[t].DelIfIn(GI2);
									}
									else {
										G_S_set[t].Add(G_Union);
										G_S_set[t].DelIfIn(GI1);
										G_S_set[t].DelIfIn(GI2);
									}


									//cout << "thread: " << threadId << endl;
									//std::cout << "After Merging:" << G_Union.Len() << endl;
									//std::cout << "G_S_set[" << t << "].Len:" << G_S_set[t].Len() << endl;
									continue;
								}
								j++;
							}
							else {
								if (IsMergeValid(GI1, GI2)) {
									TIntV G_Union = {};
									G_Union.AddVMerged(GI1);
									G_Union.AddVMerged(GI2);

									if (IskVCC(G_Union, k)) {
										#pragma omp critical
										G_R.Add(G_Union);
										G_S_set[t].DelIfIn(GI1);
										G_S_set[t].DelIfIn(GI2);
									}
									else {
										G_S_set[t].Add(G_Union);
										G_S_set[t].DelIfIn(GI1);
										G_S_set[t].DelIfIn(GI2);
									}
									//std::cout << "After Merging:" << G_Union.Len() << endl;
									//std::cout << "G_S.Len:" << G_S_temp.Len() << endl;
									continue;
								}
								j++;
							}
						}
					}
				}

			}
			//合并
			/*int half_length = G_S_set.Len() / 2;
			for (int t = 0; t < half_length; t++) {
				G_S_set[t].AddVMerged(G_S_set[G_S_set.Len() - 1 - t]);
				G_S_set.DelIfIn(G_S_set[G_S_set.Len() - 1 - t]);
			}*/
			int half_length = G_S_set.Len() / 2;
			for (int t = 0; t < half_length; t++) {
				G_S_set[t].AddVMerged(G_S_set[t + half_length]);				
			}
			for (int t = G_S_set.Len() - 1; t >= half_length; t--) {
				G_S_set.DelIfIn(G_S_set[t]);
				//cout << t << endl;
			}
			//TVec<TIntVIntV> G_S_set_temp;
			//for (int t = 0; t < G_S_set.Len() / 2; t++) {
			//	G_S_set_temp.Add({});
			//}
			//int len = G_S_set_temp.Len();
			//for (int t = 0; t < len - 1; t++) {
			//	if (t % 2 == 0) {
			//		//cout << t << endl;
			//		G_S_set_temp[t/2].AddVMerged(G_S_set[t]);
			//		G_S_set_temp[t/2].AddVMerged(G_S_set_temp[t + 1]);
			//	}
			//	
			//}
			//G_S_set = G_S_set_temp;
			cout << G_S_set.Len() << endl;
			
		} while (G_S_set.Len() != 1);
		G_S = G_S_set[0];
		
	}
	else if (threads <= 1) {
		//int Len = G_S.Len();
		//vector<int>::iterator it;
		//vector<int> visited(Len, 0);

		////Preprocess
		//vector<vector<int>> Hyper_Graph(Len);
		//for (int i = 0; i < Hyper_Graph.size(); i++)
		//	Hyper_Graph[i].resize(Len);


		//for (int i = 0; i < G_S.Len(); i++) {
		//	for (int j = i + 1; j < G_S.Len(); j++) {
		//		TIntV GI1 = G_S[i];
		//		TIntV GI2 = G_S[j];
		//		if (IsMergeValid_maxflow(GI1, GI2)) {
		//			Hyper_Graph[i][j] = 1;
		//			//cout << "here" << endl;
		//		}
		//		else {
		//			Hyper_Graph[i][j] = 0;
		//		}
		//	}
		//}

		//for (int v = 0; v < Len; v++) {
		//	if (visited[v]) continue;

		//	queue<int> tempQueue;
		//	TIntV G_Union = {};
		//	tempQueue.push(v);
		//	visited[v] = 1;
		//	while (!tempQueue.empty())
		//	{
		//		int v = tempQueue.front();
		//		G_Union.AddV(G_S[v]);
		//		G_S.DelIfIn(G_S[v]);
		//		tempQueue.pop();
		//		for (it = Hyper_Graph[v].begin(); it != Hyper_Graph[v].end(); ++it)
		//		{
		//			if (!visited[*it] && Hyper_Graph[v][*it] == 1)
		//			{
		//				// process node
		//				//std::cout << *it << " ";
		//				tempQueue.push(*it);
		//				visited[*it] = 1;
		//			}
		//		}
		//	}
		//	G_Union.Merge();
		//	if (IskVCC(G_Union, k)) {
		//		G_R.Add(G_Union);
		//	}
		//	else {
		//		G_S.Add(G_Union);
		//		//G_S.Merge();
		//	}
		//}
		

		//std::cout << std::endl;

		//for (int i = 0; i < Len; i++) {
		//	TIntV G_Union = {};
		//	G_Union.AddV(G_S[i]);
		//	G_S.DelIfIn(G_S[i]);
		//	for (int j = i + 1; j < Len; j++) {
		//		//cout << i << " " << j << endl;
		//		if (Hyper_Graph[i][j] == 1) {
		//			G_Union.AddV(G_S[j]);
		//			G_S.DelIfIn(G_S[j]);
		//		}
		//	}
		//	G_Union.Merge();
		//	if (IskVCC(G_Union, k)) {
		//		G_R.Add(G_Union);
		//	}
		//	else {
		//		G_S.Add(G_Union);
		//		//G_S.Merge();
		//	}

		//}
	//}


		//最简单的遍历，有没有时间复杂度更低的算法
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
	

	_time3 += (double)(clock() - t3) * 1.0 / (double)CLOCKS_PER_SEC;
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
		//公共顶点>=k，直接返回
		if (S_ints_S_prime.Len() >= k) return true;
	}

	//把加入改成删除
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
				//如果nb_1够大就不需要继续计算，nb_1不会是限制条件
				//if (nb_1.Len() >= k - S_ints_S_prime.Len()) break;
			}
		}
		//跳出双重循环
		//if (nb_1.Len() >= k - S_ints_S_prime.Len()) break;
	}

	//for (TIntV::TIter TI = S_prime_sub_S.BegI(); TI < S_prime_sub_S.EndI(); TI++) {
	//	TUNGraph::TNodeI NI = G->GetNI(*TI);
	//	for (int i = 0; i < NI.GetInDeg(); i++) {
	//		if (S_sub_S_prime.IsIn(NI.GetNbrNId(i))) {
	//			nb_2.AddUnique(NI.GetNbrNId(i));
//			//如果nb_2够大就不需要继续计算，nb_2不会是限制条件
//			if (nb_2.Len() >= k - S_ints_S_prime.Len()) break;
//		}
//	}
//	//跳出双重循环
//	if (nb_2.Len() >= k - S_ints_S_prime.Len()) break;
//}

/*cout << "G_S: ";
for (TIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {
	cout << *TI << " ";
}
cout << endl;

cout << "G_S_prime: ";
for (TIntV::TIter TI = G_S_prime.BegI(); TI < G_S_prime.EndI(); TI++) {
	cout << *TI << " ";
}
cout << endl;
cout << "S_ints_S_prime.Len():" << S_ints_S_prime.Len() << endl;
cout << "min(nb_1, nb_2):" << min(nb_1.Len(), nb_2.Len()) << endl;*/

/*for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
	if (*TI == 2045 || *TI == 2093 || *TI == 2096) {
		cout << "S_ints_S_prime.Len():" << S_ints_S_prime.Len() << endl;
		cout << "min(nb_1, nb_2):" << min(nb_1.Len(), nb_2.Len()) << endl;
	}
}*/
//cout << S_ints_S_prime.Len() + min(nb_1.Len(), nb_2.Len()) << endl;

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
		//公共顶点>=k，直接返回
		if (S_ints_S_prime.Len() >= k) return true;
	}



	S_sub_S_prime = S_Nodes;
	S_prime_sub_S = S_prime_Nodes;
	for (TIntV::TIter TI = S_ints_S_prime.BegI(); TI < S_ints_S_prime.EndI(); TI++) {
		S_sub_S_prime.DelIfIn(*TI);
		S_prime_sub_S.DelIfIn(*TI);
	}
	//找到delta_S 和 delta_S_prime
	//这里的delta_S指的是在另一个子图中有邻居的顶点集合
	//10/18 这里有必要全部找到吗，是不是找到一个没有邻居在对方集合中的就可以停止
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


	/*for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		TUNGraph::TNodeI NI = G->GetNI(*TI);
		int flag = 0;
		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
				flag = 1;
				break;
			}
		}
		if(flag == 0){
			s = *TI;
			break;
		}
	}

	for (TIntV::TIter TI = S_prime_sub_S.BegI(); TI < S_prime_sub_S.EndI(); TI++) {
		TUNGraph::TNodeI NI = G->GetNI(*TI);
		int flag = 0;
		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_sub_S_prime.IsIn(NI.GetNbrNId(i))) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			t = *TI;
			break;
		}
	}*/


	

	TIntV G_U = {};
	G_U.AddVMerged(G_S);
	G_U.AddVMerged(G_S_prime);
	TIntV ResNet = {};

	PUNGraph Gra = TSnap::GetSubGraph(G, G_U);

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

//bool BkVCC::IsMergeValid_maxflow(TIntV G_S, TIntV G_S_prime) {
//
//	TIntV S_Nodes, S_prime_Nodes, S_ints_S_prime, S_sub_S_prime, S_prime_sub_S, delta_S, delta_S_prime;
//	TIntV nb_1 = {}, nb_2 = {};
//	S_Nodes = G_S;
//	S_prime_Nodes = G_S_prime;
//	int s = 0, t = 0;
//
//	/*cout << "GI1:[";
//	for (TIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {
//		cout << *TI << ",";
//	}
//	cout << "]" << endl << "GI2:[";
//	for (TIntV::TIter TI = G_S_prime.BegI(); TI < G_S_prime.EndI(); TI++) {
//		cout << * TI << ",";
//	}
//	cout<< "]" << endl;*/
//
//
//
//	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
//		if (S_prime_Nodes.IsIn(*TI)) {
//			S_ints_S_prime.Add(*TI);
//		}
//		//公共顶点>=k，直接返回
//		if (S_ints_S_prime.Len() >= k) return true;
//	}
//
//
//
//	S_sub_S_prime = S_Nodes;
//	S_prime_sub_S = S_prime_Nodes;
//	for (TIntV::TIter TI = S_ints_S_prime.BegI(); TI < S_ints_S_prime.EndI(); TI++) {
//		S_sub_S_prime.DelIfIn(*TI);
//		S_prime_sub_S.DelIfIn(*TI);
//	}
//	//找到delta_S 和 delta_S_prime
//	//这里的delta_S指的是在另一个子图中有邻居的顶点集合
//	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
//		TUNGraph::TNodeI NI = G->GetNI(*TI);
//		for (int i = 0; i < NI.GetInDeg(); i++) {
//			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
//				delta_S.AddUnique(*TI);
//				delta_S_prime.AddUnique(NI.GetNbrNId(i));
//			}
//		}
//	}
//
//
//	if (min(delta_S.Len(), delta_S_prime.Len()) + S_ints_S_prime.Len() < k) return false;
//
//	TIntV G_U = {};
//	G_U.AddVMerged(G_S);
//	G_U.AddVMerged(G_S_prime);
//	TIntV ResNet = {};
//
//	PUNGraph Gra = TSnap::GetSubGraph(G, G_U);
//
//	PNEANet DG = Construct_DG(Gra);
//
//	for (TIntV::TIter TI = S_prime_sub_S.BegI(); TI < S_prime_sub_S.EndI(); TI++) {
//		if (!delta_S.IsIn(*TI)) s = *TI;
//	}
//
//	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
//		if (!delta_S_prime.IsIn(*TI)) t = *TI;
//	}
//
//	int offset = Gra->GetMxNId();
//	int lambda = TSnap::MyGetMaxFlowIntEK(DG, s + offset, t, ResNet, k);
//
//	//cout << lambda + S_ints_S_prime.Len() << endl;
//
//	if (lambda + S_ints_S_prime.Len() >= k) {
//		return true;
//	}
//	else return false;
//
//
//}

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

