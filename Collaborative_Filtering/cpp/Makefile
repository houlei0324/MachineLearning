CC=g++
NMF=build/NMF

main: ./nmf.cpp
	${CC} -std=c++11 -o build/nmf ./nmf.cpp
cf: ./cf.cpp
	${CC} -std=c++11 -o build/cf ./cf.cpp
config:
	mkdir build
clean:
	if [ -d build ]; then rm -r build; fi;
