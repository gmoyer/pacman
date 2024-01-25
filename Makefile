CXX = g++
CXXFLAGS	= -std=c++17 -Wall
CXXTESTFLAGS = --coverage # can be used to generate files that help calculate unit test coverage
CXXGDB = -ggdb

all: run_target

clean:
	rm game test Game.o Entity.o

run_target: Entity.o Game.o main.cpp
	$(CXX) $(CXXFLAGS) Entity.o Game.o Board.h Entity.h Game.h main.cpp -o game

test_target: Entity.o Game.o test.cpp
	$(CXX) $(CXXFLAGS) Entity.o Game.o Board.h Entity.h Game.h test.cpp -o test

Entity.o: Entity.cpp
	$(CXX) $(CXXFLAGS) -c Entity.cpp

Game.o: Game.cpp
	$(CXX) $(CXXFLAGS) -c Game.cpp