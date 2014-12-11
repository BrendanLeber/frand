frand: frand.cpp appinfo.h options.h options.cpp display.h display.cpp utility.h utility.cpp xplat.h xplat.cpp
	g++ -Wall -std=c++11 -DNDEBUG frand.cpp options.cpp display.cpp utility.cpp xplat.cpp -o frand

test: frand
	./frand --debug --base=test --seed 1 10
