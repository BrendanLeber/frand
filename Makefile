#CPPFLAGS=-std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
#DEBUG=-O2

CPPFLAGS=-std=c++14 -Weverything -Weffc++ -pedantic -Wno-c++98-compat
DEBUG=-g -O0

frand: frand.cpp appinfo.h options.o
	clang++ $(CPPFLAGS) $(DEBUG) -o frand frand.cpp options.o

options.o: options.hpp options.cpp
	clang++ $(CPPFLAGS) $(DEBUG) -c options.cpp


clean:
	rm frand options.o


test: frand
	./frand --debug --base=test --seed 1 10
