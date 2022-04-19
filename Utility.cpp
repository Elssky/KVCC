#include"Utility.h"
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>  
using namespace std;
//Preprocessing function
//FILE* open_file(const char* file_name, const char* mode) {
//	FILE* f = fopen(file_name, mode);
//
//	if (f == NULL) {
//		printf("Can not open file: %s\n", file_name);
//		exit(1);
//	}
//
//	return f;
//}

int get_vertex_id(int u, int& num, int* vertex_map_) {
	if (vertex_map_[u] < 0) {
		vertex_map_[u] = num++;
	}
	return vertex_map_[u];
}

void format_graph(string src)
{
	printf("Create binary graph from .txt file: %s\n", src.c_str());
	//int mem_edges = 100000000;
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

	while (getline(fp, line)) {//fgets(line, 100, fp)
		if (line[2] == 'N') {
			string::size_type pos1 = line.find("Nodes: ");
			string file_dir1 = line.substr(pos1 + 7);
			cout << file_dir1 << endl;
			stringstream ss(file_dir1);
			ss >> nodes;
			cout << "Node_Nums:" << nodes << endl;
			
			
		}
		fp2 << line << endl;
		if (line[0] != '#')
			break;
	}
	int *vertex_map_ = new int[10000000];
	::memset(vertex_map_, -1, sizeof(int) * 10000000);
	
	
	while (getline(fp, line)) {//fgets(line, 100, fp)

		if (line[0] < '0' || line[0] > '9')
			continue;

		sscanf_s(line.c_str(), "%d\t%d", &u, &v);

		if (u == v) continue;


		u = get_vertex_id(u, num, vertex_map_);
		v = get_vertex_id(v, num, vertex_map_);

		
		//fprintf(fp2, "%d\t%d", u, v);
		fp2 << u << "\t" << v << endl;

	}
}



//Others

//TVec<TIntV> res;
//TIntV track;
//void backtrack(TIntV nums, int start, int k);
//
//TVec<TIntV> subsets(TIntV nums, int k) {
//	backtrack(nums, 0, k);
//	return res;
//}
//
//void backtrack(TIntV nums, int start, int k) {
//	if (track.Len() == k) {
//		res.Add(track);
//		return;
//	}
//	
//	for (int i = start; i < nums.Len(); i++) {
//		track.Add(nums[i]);
//		backtrack(nums, i + 1, k);
//		track.DelLast();
//	}
//}


//int main() {
//	vector<int> nums = { 1,2,3 };
//	subsets(nums);
//	for (int i = 0; i < res.size(); i++) {
//		cout << "[";
//		for (int j = 0; j < res[i].size(); j++) {
//			cout << res[i][j] << " ";
//		}
//		cout << "]";
//		cout << endl;
//	}
//
//}