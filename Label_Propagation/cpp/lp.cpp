#include "lp.hpp"
#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
    //string edgefile = "../dataset/Amazon/com-amazon.ungraph.e";
    //string edgefile = "../dataset/facebook/facebook_combined.e";
    //string edgefile = "../dataset/polbooks/polbooks.e";
    string edgefile = "../dataset/karate/karate.e";
    //string edgefile = "../dataset/test.e";
    string result_file = "./result.dat";
    LP lp(edgefile,result_file);
    
    lp.LabelPropagation();
    
    return 0;
}
