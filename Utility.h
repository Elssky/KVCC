
#ifndef UTILITY_H
#define UTILITY_H

#include <Snap.h>
#include <vector>
#include <iostream>
#include <algorithm>
//#include"Heuristic_Algorithm.h"
#include <omp.h>
//#define threads 8
//#include "omp.h"

using namespace std;
//typedef TVec<TInt> TIntV;
//typedef TVec<PUNGraph> TUNGraV;
//typedef TPt<TUNGraph> PUNGraph;

typedef TVec<TInt> TIntV;
typedef TVec<PUNGraph> TUNGraV;
typedef TPt<TUNGraph> PUNGraph;
typedef TVec<TIntV> TIntVIntV;
typedef TVec <TPair<TIntV, TIntV>> TPrVIntV;

void usage();
TVec<TIntV> subsets(TIntV nums, int k, int alpha);
int get_vertex_id(int u, int& num, int* vertex_map_);
void format_graph(string src);
TVec<TIntV> randSample(TIntV nums, int k, int alpha);

double computeFscore(TIntVIntV S, TIntVIntV T);
// For each discovered community S, 
// we compute the F-score with every ground-truth community T of the dataset and choose the largest as Fscore
// use the average value as final F-score

int getDegeneracy(PUNGraph G_in);

//全局变量，用于接收命令行参数
extern int k, threads;
extern string alg, seed, mergeMethod, expandMethod;
extern TStr dataset;

#endif // !UTILITY_H


