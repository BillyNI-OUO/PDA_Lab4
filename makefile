all: Lab4

CXX = g++

LINKFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -g

Lab4: main.o io.o entity.o algor.o
	$(CXX) $(LINKFLAGS) main.o -o Lab4

main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c

io.o: ./src/io.cpp ./inc/io.hpp
	$(CXX) $(LINKFLAGS) ./src/io.cpp -c

entity.o: ./src/entity.cpp ./inc/entity.hpp
	$(CXX) $(LINKFLAGS) ./src/entity.cpp -c

algor.o: ./src/algorihm.cpp ./inc/algorithm.hpp
	$(CXX) $(LINKFLAGS) ./src/algorihm.cpp -c

clean:
	rm -rf *.o *.gch Lab4






