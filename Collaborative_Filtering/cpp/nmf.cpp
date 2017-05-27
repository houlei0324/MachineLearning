#include "NMF.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
	int max_iter = 100;
	int dim = atoi(argv[1]);
	string edgefile = argv[2];
	string vertexfile = argv[3];
	double alpha = 0.00005;
	double beta = 0.1;
	double min_err = 0.9;
	NMF nmf(edgefile, vertexfile, max_iter, dim, alpha, beta, min_err);
	unordered_map<int, vector<double>> userP;
	unordered_map<int, vector<double>> itemQ;
	
	nmf.runNMF(userP, itemQ);
	
	return 0;
}
