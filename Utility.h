#pragma once
#include<vector>
#include<iostream>
#include<algorithm>
#include"Heuristic_Algorithm.h"

using namespace std;
//typedef TVec<TInt> TIntV;
//typedef TVec<PUNGraph> TUNGraV;
//typedef TPt<TUNGraph> PUNGraph;

TVec<TIntV> subsets(TIntV nums, int k, int alpha);
int get_vertex_id(int u, int& num, int* vertex_map_);
void format_graph(string src);