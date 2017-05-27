#ifndef PAGERANK_HPP_
#define PAGERANK_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

#include <string.h>
#include <math.h>
#include <time.h>

#include "Edge.hpp"

using namespace std;

class PageRank {

public:

  PageRank(string efile): efile_(efile) {
    clock_t start, end;
    start = clock();
    LoadDataAndInit();
    end = clock();
    cout << "Loading and init time is " <<
         (double)(end - start) / (double)CLOCKS_PER_SEC << "s" << endl;
  }

  void LoadDataAndInit() {
    cout << "Start to load graph..." << endl;
    fstream file;
    file.open(efile_.c_str());
    string line;
    while (!file.eof()) {
      getline(file, line);
      if (file.fail()) {
        break;
      }
      char *val;
      int n = 10;
      char* buffer = new char[n];
      buffer[n] = 0;
      const char *split = "\t";
      val = strtok (const_cast<char*>(line.c_str()), split);
      VertexID src = atoi(val);
      val = strtok (NULL, "\t");
      VertexID dst = atoi(val);

      ED attr = 1;
      Edge e(src, dst, attr);
      edges_.emplace_back(e);

      if (vertices_.find(src) == vertices_.end()) {
        vertices_[src] = 0;
      }
      if (vertices_.find(dst) == vertices_.end()) {
        vertices_[dst] = 0;
      }

      if (adj_in_list_.find(dst) == adj_in_list_.end()) {
        unordered_map<int, int> adj_node;
        adj_node[src] = attr;
        adj_in_list_[dst] = adj_node;
      } else {
        adj_in_list_[dst].insert(make_pair(src, attr));
      }
      if (adj_out_list_.find(src) == adj_out_list_.end()) {
        unordered_map<int, int> adj_node;
        adj_node[dst] = attr;
        adj_out_list_[src] = adj_node;
      } else {
        adj_out_list_[src].insert(make_pair(dst, attr));
      }
    }

    num_vertices_ = vertices_.size();
    num_edges_ = edges_.size();
    for (auto v : vertices_) {
      vertices_[v.first] = 1.0 / num_vertices_;
    }
    file.close();
    cout << "Loading finish!" << endl;
  }

  void Run(int max_step, double alpha, double delt) {
    int num_step = 1;
    int num_finish = 0;
    double error = 1000;
    clock_t start, end;
    start = clock();
    while (error >= delt && num_step <= max_step) {
      error = 0;
      cout << "iter: " << num_step << endl;
      num_finish = 0;
      unordered_map<int, double> new_vertices = vertices_;
      for (auto v : new_vertices) {
        double temp = v.second;
        new_vertices[v.first] = alpha / num_vertices_;
        for (auto item : adj_in_list_[v.first]) {
          new_vertices[v.first] += (1 - alpha) * vertices_[item.first]
                                / adj_out_list_[item.first].size();
        }
        error += abs(new_vertices[v.first] - vertices_[v.first]);
      }
      cout<<"error : "<<error<<endl;
      vertices_ = new_vertices;
      num_step ++;
      end = clock();
      cout << "Running time is " <<
         (double)(end - start) / (double)CLOCKS_PER_SEC << "s" << endl;
    }
  }

  void OutputResult(string result_file) {
    clock_t start, end;
    start = clock();
    ofstream file;
    file.open(result_file.c_str());
    for (auto v : vertices_) {
      file << v.first << "\t" << v.second << endl;
    }
    file.close();
    end = clock();
    cout << "Output result time is " <<
         (double)(end - start) / (double)CLOCKS_PER_SEC << "s" << endl;
  }

private:
  unordered_map<int, double> vertices_;
  vector<Edge> edges_;
  unordered_map<int, unordered_map<int , int>> adj_in_list_;
  unordered_map<int, unordered_map<int , int>> adj_out_list_;
  string efile_;
  int num_vertices_;
  int num_edges_;
};

#endif