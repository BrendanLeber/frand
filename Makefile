frand: frand.cpp appinfo.h options.h options.cpp display.h display.cpp utility.h utility.cpp xplat.h xplat.cpp
	clang++ -pedantic -std=c++11 -DNDEBUG frand.cpp options.cpp display.cpp utility.cpp xplat.cpp -o frand

analyze: frand.cpp appinfo.h options.h options.cpp display.h display.cpp utility.h utility.cpp xplat.h xplat.cpp
	clang++ --analyze -pedantic -std=c++11 -DNDEBUG --analyze frand.cpp options.cpp display.cpp utility.cpp xplat.cpp

test: frand
	./frand --debug --base=test --seed 1 10
