#include "NMF.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
	string edgefile = "../datasets/ml-latest-small/user-movie.e";
	string vertexfile = "../datasets/ml-latest-small/user-movie.v";
	int max_iter = 1000;
	int dim = atoi(argv[1]);
	double alpha = 0.002;
	double beta = 0.001;
	double min_err = 0.5;
	NMF nmf(edgefile, vertexfile, max_iter, dim, alpha, beta, min_err);
	unordered_map<int, vector<double>> userP;
	unordered_map<int, vector<double>> itemQ;
	
	nmf.runNMF(userP, itemQ);
	
	return 0;
}
