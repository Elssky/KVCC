#include"Utility.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>  
#include <set>
using namespace std;
//Preprocessing function

int a;//用a来选择运行的算法

int get_vertex_id(int u, int& num, int* vertex_map_) {
	if (vertex_map_[u] < 0) {
		vertex_map_[u] = num++;
	}
	return vertex_map_[u];
}

void format_graph(string src)
{
	printf("Create binary graph from .txt file: %s\n", src.c_str());

	// input example "./dataset/stanford.txt"
	string::size_type pos = src.rfind("/");

	// get file direcotry
	// e.g. "./dataset/"
	string file_dir = src.substr(0, pos + 1);

	// get graph_name
	// e.g. "stanford"
	string graph_name = src.substr(pos + 1, src.rfind(".") - pos - 1);

	// create out file 
	// e.g. "./dataset/stanford"
	string out = file_dir +   graph_name + "_Processed" + ".txt";

	ifstream fp;
	ofstream fp2;
	fp.open(src.c_str());
	fp2.open(out.c_str());
	//FILE* fp = open_file(src.c_str(), "r");
	//FILE* fp2 = open_file(out.c_str(), "w");
	string line;
	int nodes = 0;
	int u, v;
	int num = 0;

	while (getline(fp, line)) {
		fp2 << line << endl;
		if (line[2] == 'F')
			break;
		if (line[2] == 'N') {
			string::size_type pos1 = line.find("Nodes: ");
			string file_dir1 = line.substr(pos1 + 7);
			cout << file_dir1 << endl;
			stringstream ss(file_dir1);
			ss >> nodes;
			cout << "Node_Nums:" << nodes << endl;
			
			
		}
		
		
	}
	int *vertex_map_ = new int[10000000];
	::memset(vertex_map_, -1, sizeof(int) * 10000000);
	
	
	while (getline(fp, line)) {

		if (line[0] < '0' || line[0] > '9')
			continue;

		sscanf_s(line.c_str(), "%d\t%d", &u, &v);

		if (u == v) continue;


		u = get_vertex_id(u, num, vertex_map_);
		v = get_vertex_id(v, num, vertex_map_);

		
		//fprintf(fp2, "%d\t%d", u, v);
		fp2 << u << "\t" << v << endl;

	}

	int* new2old = new int[num];
	int cur = 0;
	int i = 0;
	while (cur < num) {
		if (vertex_map_[i] == -1) {
			++i;
			continue;
		}
		new2old[vertex_map_[i]] = i;
		++cur;
		++i;
	}
	string mts = "./dataset/match.st";
	errno_t err;
	FILE *mch;
	err = fopen_s(&mch, mts.c_str(), "wb");
	//FILE* mch = fopen_s(mts.c_str(), "wb");
	if (mch) {
		fwrite(new2old, sizeof(int), num, mch);
		fclose(mch);
	}

	delete[] new2old;
	delete[] vertex_map_;



	/*read part*/
	//FILE *mch2;
	//int* new2old = new int[num];
	//err = fopen_s(&mch2, mts.c_str(), "rb");
	//fread(new2, sizeof(int), num, mch);
	//fclose(mch);

	//ifstream fp3;
	//fp3.open(out.c_str());
	//while (getline(fp3, line)) {//fgets(line, 100, fp)

	//	if (line[0] < '0' || line[0] > '9')
	//		continue;

	//	sscanf_s(line.c_str(), "%d\t%d", &u, &v);

	//	if (u == v) continue;
	//	u = new2old[u];
	//	v = new2old[v];
	//	cout << u << "\t" << v << endl;
	//}


}



//Others

TVec<TIntV> res;
TIntV track;
//int conb[51][51] = {};

void backtrack(TIntV nums, int start, int k, int alpha);

TVec<TIntV> randSample(TIntV nums, int k, int alpha)
{
	res.Clr();
	int n = nums.Len();
	int m = k;
	int flag = 0;//标记组合数是否足够大，0则调用子集，1则调用随机生成

	if(n < 50 ) return subsets(nums, k, alpha);
	
	//计算理论的子集个数


	/*int i, j;
	for (i = 0; i < n + 1; i++)
	{
		for (j = 0; j <= i; j++) {
			if (i == j || j == 0)
				conb[i][j] = 1;
			else conb[i][j] = conb[i - 1][j] + conb[i - 1][j - 1];
		}
		if (conb[i][j] > alpha) { flag = 1; break; }
	}

	if (flag == 0) {
		return subsets(nums, k, alpha);
	}*/

	
	//随机生成子集
	while (res.Len() < alpha) {
		track.Clr();

		while (track.Len() < k) {
			int t = rand() % (n + 1);
			track.AddUnique(nums[t]);
		}
		//for (int i = 0; i < n; ++i) {
		//	if (rand() % (n - i) < m) {
		//		//cout << i << endl;
		//		m--;
		//	}
		//}
		/*for (int i = n - m; i < n; ++i)
		{
			int t = rand() % (i + 1);
			if (track.IsIn(nums[t])) {
				track.Add(nums[t]);
			}
			else {
				track.Add(nums[i]);
			}
		}*/
		/*for (TIntV::TIter TI = track.BegI(); TI < track.EndI(); TI++) {
			cout << *TI << " ";
		}
		cout << endl;*/
		res.Add(TIntV(track));
		
		
	}
	res.Merge();
	return res;
}


TVec<TIntV> subsets(TIntV nums, int k, int alpha) {
	res.Clr();
	track.Clr();
	int n = nums.Len();
	backtrack(nums, 0, k, alpha);
	/*if(n < 50) backtrack(nums, 0, k, alpha);
	else {
		while (res.Len() < alpha) {
			track.Clr();
			while (track.Len() < k) {
				int t = rand() % n;
				track.Add(nums[t]);
			}
			res.Add(track);
		}
		res.Merge();
	}*/
	return res;
}

void backtrack(TIntV nums, int start, int k, int alpha) {
	
	if (res.Len() >= alpha) {
		return;
	}

	if (track.Len() == k) {
		res.Add(TIntV (track));
		return;
	}
	
	for (int i = start; i < nums.Len(); i++) {
		track.Add(nums[i]);
		backtrack(nums, i + 1, k, alpha);
		track.DelLast();
	}
}


