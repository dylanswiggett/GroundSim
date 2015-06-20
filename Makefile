CXX = g++
LINKERS = -lSDL2 -lGLEW -lGL
FLAGS = -Wall -g -std=c++11

main: main.o GroundSim.o Ground.o
	$(CXX) $(FLAGS) -o main main.o GroundSim.o Ground.o $(LINKERS)

main.o: main.cpp GroundSim.hpp Ground.hpp
	$(CXX) $(FLAGS) -c -o main.o main.cpp

GroundSim.o: GroundSim.cpp GroundSim.hpp Ground.hpp
	$(CXX) $(FLAGS) -c -o GroundSim.o GroundSim.cpp

Ground.o: Ground.cpp Ground.hpp
	$(CXX) $(FLAGS) -c -o Ground.o Ground.cpp
