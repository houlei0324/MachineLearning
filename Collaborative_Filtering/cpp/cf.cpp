/*
 * cf.cpp
 * 
 * Copyright 2017 houlei <houlei@OptiPlex-9010>
 * Author: LeiHou
 * Date: 01/09/2017
 * E-mail: houlei@act.buaa.edu.cn
 * Function: an implement of Collaborative Filtering via Non-negative Matrix Factorization using SGD
 * 
 */
#include "NMF.hpp"
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
using namespace std;
class CF{
public:
	CF(){	
	}
	
	CF(string _edgefile, string _vertexfile, int _max_iter, int _dim, double _alpha, double _beta, double _min_err): edgefile(_edgefile), vertexfile(_vertexfile), max_iter(_max_iter), dim(_dim), alpha(_alpha), beta(_beta), min_err(_min_err){
		nmf.edgefile = this->edgefile;
		nmf.vertexfile = this->vertexfile;
		nmf.max_iter = this->max_iter;
		nmf.dim = this->dim;
		nmf.alpha = this->alpha;
		nmf.beta = this->beta;
		nmf.min_err = this->min_err;
		nmf.readfile();
		nmf.initPQ();
		nmf.runNMF(userP, itemQ);
	}
	
	//calculate the Euclidean Distance sim of the id node to other users or items
	vector<pair<int, double>> sim_ED(VertexID id, unordered_map<int, vector<double>> &matrix){
		vector<pair<int, double>> sim_values;
		for (auto v: matrix){
			double sim = 0;
			for (int k = 0; k < nmf.dim; k ++){
				sim += pow((matrix[id][k] - matrix[v.first][k]), 2);
			}
			sim = 1 / (1 + sqrt(sim));
			sim_values.push_back(make_pair(v.first, sim));
		}
		return sim_values;
	}
	
	//calculate the Pearson Correlation Coefficient sim of the id node to other users or items
	vector<pair<int, double>> sim_PCC(VertexID id, unordered_map<int, vector<double>> &matrix){
		vector<pair<int, double>> sim_values;
		for (auto v: matrix){
			double sim = 0;
			double temp1 = 0;
			double temp2 = 0;
			double temp3 = 0;
			double temp4 = 0;
			for (int k = 0; k < nmf.dim; k ++){
				sim += matrix[id][k] * matrix[v.first][k];
				temp1 += matrix[id][k];
				temp2 += matrix[v.first][k];
				temp3 += pow(matrix[id][k], 2);
				temp4 += pow(matrix[v.first][k], 2);
			}
			sim = sim - (temp1 * temp2) / nmf.dim;
			sim = sim / sqrt((temp3 - (pow(temp1, 2) / nmf.dim)) * (temp4 - (pow(temp2, 2) / nmf.dim)));
			sim_values.push_back(make_pair(v.first, sim));
		}
		return sim_values;	
	}
	
	//calculate the Cosine sim of the id node to other users or items
	vector<pair<int, double>> sim_COS(VertexID id, unordered_map<int, vector<double>> &matrix){
		vector<pair<int, double>> sim_values;
		for (auto v: matrix){
			double sim = 0;
			double temp1 = 0;
			double temp2 = 0;
			for (int k = 0; k < nmf.dim; k ++){
				sim += matrix[id][k] * matrix[v.first][k];
				temp1 += pow(matrix[id][k], 2);
				temp2 += pow(matrix[v.first][k], 2);
			}
			sim = sim / (sqrt(temp1) * sqrt(temp2));
			sim_values.push_back(make_pair(v.first, sim));
		}
		return sim_values;	
	}
	
	//calculate the Tanimoto (Jaccard) Coefficient sim of the id node to other users or items
	vector<pair<int, double>> sim_TAN(VertexID id, unordered_map<int, vector<double>> &matrix){
		vector<pair<int, double>> sim_values;
		for (auto v: matrix){
			double sim = 0;
			double temp1 = 0;
			double temp2 = 0;
			for (int k = 0; k < nmf.dim; k ++){
				sim += matrix[id][k] * matrix[v.first][k];
				temp1 += pow(matrix[id][k], 2);
				temp2 += pow(matrix[v.first][k], 2);
			}
			sim = sim / (sqrt(temp1) + sqrt(temp2) - sim);
			sim_values.push_back(make_pair(v.first, sim));
			cout<< sim <<endl;
		}
		return sim_values;	
	}
	
	//choose the neighboors according knn or Threshold-based neighborhoods
	vector<VertexID> choose_neighboor(VertexID id, unordered_map<int, vector<double>> &matrix, int choose_method, double k, int method){
		vector<pair<int, double>> sim_values;
		switch (method){
			case 0:{//choose the Euclidean Distance simility
				sim_values = sim_ED(id, matrix);
				break;
			}
			case 1:{//choose the Pearson Correlation Coefficient simility
				sim_values = sim_PCC(id, matrix);
				break;
			}
			case 2:{//choose the Cosine simility
				sim_values = sim_COS(id, matrix);
				break;
			}
			case 3:{//choose the the Tanimoto (Jaccard) Coefficient simility
				sim_values = sim_TAN(id, matrix);
				break;
			}
			default:{
				break;
			}
		}
		sort(sim_values.begin(), sim_values.end(), 
			[](const pair<int, double> &x, const pair<int, double> &y) -> int{
			return x.second > y.second;
		});
		
		vector<VertexID> neighbors;
		if(choose_method == 0){//knn
			for (int i = 0; i < (int)k; i++){
				if(sim_values[i].first != id){
					neighbors.push_back(sim_values[i].first);
				}else{
					k ++;
				}
			}
		}else{//Threshold-based neighborhoods
			for (int i = 0; i < sim_values.size(); i++){
				if (sim_values[i].first != id){
					if((1 - sim_values[i].second) > k){
						continue;
					}
					neighbors.push_back(sim_values[i].first);
				}else{
					continue;
				}
			}
		}
		return neighbors;
	}
	
	//recommendation based on items
	void itemCF(VertexID userid, int choose_method, double k, int sim_method){
		vector<VertexID> neighboor_items;
		unordered_set<VertexID> final_items;
		vector<VertexID> itemid = nmf.user_items[userid];
		//find sim items for recommitdation
		int lenresult = 0;
		for(auto id :itemid){
			neighboor_items =choose_neighboor(id, itemQ, choose_method, k, sim_method);
			for(auto neighboor: neighboor_items){
				if(final_items.find(neighboor) == final_items.end()){
					final_items.insert(neighboor);
				}
			}
		}
		cout << "itemCF recommends user "<< userid << " movies count " << final_items.size() <<endl;
	}
	
	//recommendation based on users
	void userCF(VertexID userid, int choose_method, double k, int sim_method){
		unordered_set<VertexID> results;
		vector<VertexID> neighboor_users;
		//find sim users
		neighboor_users =choose_neighboor(userid, userP, choose_method, k, sim_method);
		//find items for recommendation
		int lenresult = 0;
		for (auto id : neighboor_users){
			vector<VertexID> tempitems = nmf.user_items[id];
			for(auto item: tempitems){
				if (results.find(item) == results.end()){
					results.insert(item);
				}
			}
		}
		cout << "userCF recommends user "<< userid << " movies count " << results.size() <<endl;
	}
public:
	string edgefile;//the readin file about ratings
	string vertexfile;
	int max_iter;
	int usernum;
	int itemnum;
	int dim;//the dimension of the factorizated matrix
	double alpha;
	double beta;
	double min_err;
	vector<Edge> edges;
	NMF nmf;
	unordered_map<int, vector<double>> userP;
	unordered_map<int, vector<double>> itemQ;
};

int main(int argc, char *argv[]){
	string edgefile = "../datasets/ml-latest-small/user-movie.e";
	string vertexfile = "../datasets/ml-latest-small/user-movie.v";
	int max_iter = 1000;
	int dim = atoi(argv[1]);
	VertexID user = atoi(argv[2]);
	int choose_method = atoi(argv[3]);
	double k = atof(argv[4]);
	int sim_method = atoi(argv[5]);
	double alpha = 0.002;
	double beta = 0.02;
	double min_err = 1;
	CF cf(edgefile, vertexfile, max_iter, dim, alpha, beta, min_err);
	
	cf.itemCF(user, choose_method, k, sim_method);
	cf.userCF(user, choose_method, k, sim_method);
	
	return 0;
}
