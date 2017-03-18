#include "PageRank.hpp"

using namespace std;

int main(int argc, char *argv[]){
  string efile = "../dataset/web-Stanford.txt";
  string result_file  = "./ranks.dat";
  int max_step = 100;
  double alpha = 0.15;
  double delt = 0.000001;
  PageRank pagerank(efile);
  pagerank.Run(max_step, alpha, delt);
  pagerank.OutputResult(result_file);
}
