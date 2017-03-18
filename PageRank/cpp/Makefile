CC=g++
PageRank=build/pagerank

main: ./PageRank.cpp
	${CC} -std=c++11 -o build/pagerank ./PageRank.cpp
config:
	mkdir build
clean:
	if [ -d build ]; then rm -r build; fi;
