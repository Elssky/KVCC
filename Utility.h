#pragma once
#include<vector>
#include<iostream>
#include<algorithm>
#include"Heuristic_Algorithm.h"
//#include "omp.h"

using namespace std;
//typedef TVec<TInt> TIntV;
//typedef TVec<PUNGraph> TUNGraV;
//typedef TPt<TUNGraph> PUNGraph;

TVec<TIntV> subsets(TIntV nums, int k, int alpha);
int get_vertex_id(int u, int& num, int* vertex_map_);
void format_graph(string src);
TVec<TIntV> randSample(TIntV nums, int k, int alpha);

extern int a; //用a来选择运行的算法
