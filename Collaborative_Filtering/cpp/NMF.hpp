/*
 * NMF.hpp
 * 
 * Copyright 2017 houlei <houlei@OptiPlex-9010>
 * Date: 01/09/2017
 * Author: LeiHou
 * Function: define the class Non-negative Matrix Factorization
 * 
 */
#ifndef NMF_HPP_
#define NMF_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <stdlib.h> 
#include <time.h>
#include "Edge.hpp"
using namespace std;
class NMF {
public:
	NMF(){
	}
	
	//vi file
	NMF(string _edgefile, string _vertexfile, int _max_iter, int _dim, double _alpha, double _beta, double _min_err): edgefile(_edgefile), vertexfile(_vertexfile), max_iter(_max_iter), dim(_dim), alpha(_alpha), beta(_beta), min_err(_min_err){
		clock_t start, end;
		start = clock();
		readfile();
		end = clock();
		cout << "Loading time is " <<
		     (double)(end - start) / (double)CLOCKS_PER_SEC << "s" << endl;
		initPQ();
		end = clock();
		cout << "Init time is " <<
		     (double)(end - start) / (double)CLOCKS_PER_SEC << "s" << endl;
	}
	
	//param edges directly
	NMF(vector<Edge> &_edges, vector<VertexID> &_users, vector<VertexID> &_items, int _max_iter, int _dim, double _alpha, double _beta, double _min_err): edges(_edges), users(_users), items(_items), max_iter(_max_iter), dim(_dim), alpha(_alpha), beta(_beta), min_err(_min_err){
		initPQ();
	}
	//read edgefile
	void readfile(){
		fstream vfile;
		vfile.open(vertexfile.c_str());
		string line;
		while(!vfile.eof()){
			getline(vfile, line);
			if (vfile.fail())
				break;
			char *val;
			const char *split = "\t";
			char *l = (char*)line.c_str();
			val = strtok (l, split);
			VertexID src = atoi(val);
			val = strtok (NULL, "\t");
			int label = atoi(val);
			if (label == 0){
				users.push_back(src);
			}else{
				items.push_back(src);
			}
		}
		vfile.close();
		usernum = users.size();
		itemnum = items.size();
		
		fstream infile;
		infile.open(edgefile.c_str());
		string eline;
		while(!infile.eof()){
			getline(infile, eline);
			if (infile.fail())
				break;
			char *val;
			const char *split = "\t";
			char *l = (char*)eline.c_str();
			val = strtok (l, split);
			VertexID src = atoi(val);
			val = strtok (NULL, "\t");
			VertexID dst = atoi(val);
			val = strtok (NULL, "\t");
			ED attr = atof(val);
			Edge e(src, dst, attr);
			if(user_items.find(src) == user_items.end()){
				vector<int> temp;
				temp.push_back(dst);
				user_items[src] = temp;
			}else{
				user_items[src].push_back(dst);
			}
			edges.push_back(e);	
		}
		infile.close();
	}
	
	//init userP and itemQ
	void initPQ(){
		for (int i = 0; i < usernum; i++){
			vector<double> temp;
			for (int j = 0;j < dim; j++){
				temp.push_back((double)rand()/(double)RAND_MAX);
				//temp.push_back(1);
			}
			userP.insert({users[i], temp});
		}
		for (int i = 0; i < itemnum; i++){
			vector<double> temp;
			for (int j = 0;j < dim; j++){
				temp.push_back((double)rand()/(double)RAND_MAX);
				//temp.push_back(1);
			}
			itemQ.insert({items[i],temp});
		}
	}
	
	//update userP and itemQ, using stochastic gradient descent(SGD)
	void updatePQ(){
		for (int i = 0; i < edges.size(); i++){
			double PQ = 0;
			for (int k = 0; k < dim; k++){
				PQ += userP[edges[i].src][k] * itemQ[edges[i].dst][k];
			}
			double eij = edges[i].attr - PQ;
			//update Pik and Qjk
				
			for (int k = 0; k < dim; k++){
				double oldPik = userP[edges[i].src][k];
				userP[edges[i].src][k] += alpha * (eij * itemQ[edges[i].dst][k] - beta * userP[edges[i].src][k]);
				itemQ[edges[i].dst][k] += alpha * (eij * oldPik - beta * itemQ[edges[i].dst][k]);
			}
		}
	}
	
	//calculate the error after each iter
	double getErr(){
		double err = 0;\
		double train_err = 0;
		vector<double> train;
		for (int i = 0; i < edges.size(); i ++){
			double PQ = 0;
			for (int k = 0; k < dim; k++){
				PQ += userP[edges[i].src][k] * itemQ[edges[i].dst][k];
			}
			err += pow((edges[i].attr - PQ), 2);
			PQ = min(5.0, PQ);
			PQ = max(1.0, PQ);
			train.push_back(edges[i].attr - PQ);
		}
		
		for (int i = 0; i < usernum; i++){
			for (int k = 0; k < dim; k++){
				err += (beta* pow(userP[users[i]][k], 2));
			}
		}
		
		for (int i = 0; i < itemnum; i++){
			for (int k = 0; k < dim; k++){
				err += (beta * pow(itemQ[items[i]][k], 2));
			}
		}
		
		double sum = 0;
		for (int i = 0; i < train.size(); i ++){
			sum += train[i] * train[i];
		}
	/*	double mean = sum / train.size();
		double accum = 0;
		for (int i = 0; i < train.size(); i ++){
			accum += (train[i] - mean) * (train[i] - mean);
		}*/
		train_err = sqrt(sum / train.size());
		cout<<"train_err : " << train_err<<endl;
		return err;
	}
	
	//run NMF and return the result of userP and itemQ
	void runNMF(unordered_map<int, vector<double>> &_userP, unordered_map<int, vector<double>> &_itemQ){
		int step = 1;
		double err = INT_MAX;
		clock_t start, end;
		start = clock();
		cout << "=========Iter has started========="<< endl;
		while(step <= max_iter && err > min_err){
			updatePQ();
			if (step == max_iter) {
				err = getErr();
			}
			cout<<step<<" : "<<err<<endl;
			step += 1;
			end = clock();
			cout << "run time is " <<
		     (double)(end - start) / (double)CLOCKS_PER_SEC << "s" << endl;
		}
		cout << "========Iter has stopped==========" << endl;
		_userP = userP;
		_itemQ = itemQ;
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
	vector<VertexID> users;
	vector<VertexID> items;
	unordered_map<int, vector<int>> user_items;
	unordered_map<int, vector<double>> userP;
	unordered_map<int, vector<double>> itemQ;
};

#endif /*NMF_HPP_*/
