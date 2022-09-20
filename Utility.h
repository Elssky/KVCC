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

void usage();
TVec<TIntV> subsets(TIntV nums, int k, int alpha);
int get_vertex_id(int u, int& num, int* vertex_map_);
void format_graph(string src);
TVec<TIntV> randSample(TIntV nums, int k, int alpha);

//全局变量，用于接收命令行参数
extern int k; 
extern string alg, seed, useFlow; 
extern TStr dataset;
