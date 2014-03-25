frand: frand.cpp options.h options.cpp display.h display.cpp xplat.h xplat.cpp
	g++ -Wall -g -std=c++11 frand.cpp options.cpp display.cpp xplat.cpp -o frand

test: frand
	./frand --debug --base=test --seed 1 10
