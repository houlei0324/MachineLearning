#include "PageRank.hpp"

using namespace std;

int main(int argc, char *argv[]){
  string efile = argv[1];
  int max_step = atoi(argv[2]);
  string result_file  = "./pr_ranks.dat";
  double alpha = 0.15;
  double delt = 0.001;
  PageRank pagerank(efile);
  pagerank.Run(max_step, alpha, delt);
  pagerank.OutputResult(result_file);
}
