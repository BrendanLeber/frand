frand: frand.cpp options.h options.cpp display.h display.cpp utility.h utility.cpp xplat.h xplat.cpp
	g++ -Wall -O0 -g -std=c++11 frand.cpp options.cpp display.cpp utility.cpp xplat.cpp -o frand

test: frand
	./frand --debug --base=test --seed 1 10
