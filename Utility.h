#pragma once
#include<vector>
#include<iostream>
#include<algorithm>
#include"Heuristic_Algorithm.h"

using namespace std;
//typedef TVec<TInt> TIntV;
//typedef TVec<PUNGraph> TUNGraV;
//typedef TPt<TUNGraph> PUNGraph;

//TVec<TIntV> subsets(TIntV nums, int k);
int get_vertex_id(int u, int& num, int* vertex_map_);
//FILE* open_file(const char* file_name, const char* mode);
void format_graph(string src);