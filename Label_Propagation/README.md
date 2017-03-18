# a-implement-of-label-propagation-algorithm
Label Propagation Algorithm
===========================

Here is a implement using c++ for label propagation algorithm refering to Raghavan's paper. The following is some understanding about the paper and the method to use those codes.

References
---------------
[1] Raghavan U N, Albert R, Kumara S. Near linear time algorithm to detect community structures in large-scale networks.[J]. Physical Review E Statistical Nonlinear & Soft Matter Physics, 2007, 76(3 Pt 2):036106.

How to start
----------------
1. cd label-propagation/cpp
2. make config
3. open lp.cpp and change the dataset to run
4. make
5. ./build/lp -- to get the result at the file result.dat

Some understanding about the paper and label propagation
-----------------------
Here I want to list some error I met when coding this label propagation algorithm

1. the result of lp algorithm is not unique for one input-dataset. In this paper, Raghavan gives that "Since the algorithm breaks ties uniformly randomly, early on in the iterative process when possibilities of ties are high, a node may vote in favor of a randomly chosen community. As a result, multiple community structures are reachable from the same initial condition." In my understanding, the different traversal orders and the different method to choose new label are also reasons.

2. the multiple community structures can be compared with each other and a evaluate index can be found in the paper. Based on this, we can aggregate those structures together and identify a community structure containing the most useful information.



