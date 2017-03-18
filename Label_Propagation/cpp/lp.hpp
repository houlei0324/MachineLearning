/*
 * lp.cpp
 * 
 * Copyright 2017 houlei <houlei@OptiPlex-9010>
 * 
 * Date: 01/09/2017
 * Author: LeiHou
 * Function: define the class Non-negative Matrix Factorization
 */

#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include <limits.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <utility>

#include "Edge.hpp"

using namespace std;

class LP {
public:
  //vi file
  LP(string _edgefile,string _result_file): edgefile(_edgefile),result_file(_result_file){
		LoadData();
		InitLabel();
  }
   
  //read edgefile
  void LoadData(){	
		fstream edge_file;
		edge_file.open(edgefile.c_str());
		string eline;
		while(!edge_file.eof()){
	    getline(edge_file, eline);
	    if (edge_file.fail())
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
	    if (adj_list.find(src) == adj_list.end()){
				unordered_map<int, int> adj_node;
				adj_node[dst] = attr;
				adj_list[src] = adj_node;
	    }else{
				adj_list[src].insert(make_pair(dst,attr));
	    }
	    if (adj_list.find(dst) == adj_list.end()){
				unordered_map<int, int> adj_node;
				adj_node[src] = attr;
				adj_list[dst] = adj_node;
	    }else{
				adj_list[dst].insert(make_pair(src,attr));
	    }
	    edges.push_back(e);	
		}
		edge_file.close();
  }
    
	void InitLabel(){
		for (auto v : adj_list){
	    vertex_label[v.first] = v.first;
		}
  }
    
	void GetMaxCommunityLabel(int src){
		unordered_map<int, int> label_weight;
		for (auto label : adj_list.at(src)){
	    if (label_weight.find(vertex_label[label.first]) == label_weight.end()){
				label_weight[vertex_label[label.first]] = label.second;
	    }else{
			label_weight[vertex_label[label.first]] += label.second;
			}
		}
		//find the max label
		int max_weight = 0;
		int new_label = -1;
		for (auto item : label_weight){
	    if (item.second > max_weight){
				max_weight = item.second;
				new_label = item.first;
	    }
		}
		vertex_label[src] = new_label;
		cout << src<< " : " <<vertex_label[src]<<endl;
  }
    
	int CheckToStop(){
		for (auto src : vertex_label){
	    unordered_map<int, int> label_weight;
	    for (auto label : adj_list.at(src.first)){
				if (label_weight.find(vertex_label[label.first]) == label_weight.end()){
					label_weight[vertex_label[label.first]] = label.second;
				}else{
					label_weight[vertex_label[label.first]] += label.second;
				}
			}
	    //find the max label
	    int max_weight = 0;
	    int new_label = -1;
	    for (auto item : label_weight){
				if (item.second > max_weight){
					max_weight = item.second;
					new_label = item.first;
				}
	    }
	    if (vertex_label[src.first] == new_label){
				continue;
	    }else{
				return 0;
	    }
		}
		return 1;
  }
    
	void LabelPropagation(){
		int t = 0;
		while(1){
	    if (CheckToStop() == 0){
				t = t + 1;
				cout << "iteration: "<<t<<endl;
				for (auto v : vertex_label){
					GetMaxCommunityLabel(v.first);
				}
	    }else{
				break;
	    }
		}
		cout << t << endl;
		map<int, int> result;
		unordered_map<int, int> labels;
		for (auto item : vertex_label) {
	    auto v = item.first;
	    auto label = item.second;
	    result[v] = label;
	    if(labels.find(label) == labels.end()){
				labels[label] = label;
	    }
		}
		ofstream fout;
		fout.open(result_file);
		fout << labels.size() << endl;
		for (auto item : result) {
	    fout << item.first << '\t' << item.second << endl;
		}
		fout.close();
  }
public:
    string edgefile;//the readin file about ratings
    string result_file;
    vector<Edge> edges;
    unordered_map<int, int> vertex_label;
    unordered_map<int, unordered_map<int ,int>> adj_list;
};

