CXX = g++
CXXFLAGS	= -std=c++17 -Wall
CXXTESTFLAGS = --coverage # can be used to generate files that help calculate unit test coverage
CXXGDB = -ggdb

all: run_target

clean:
	rm test Circle.o

run_target: Entity.o Game.o Board.o
	$(CXX) $(CXXFLAGS) Entity.o Game.o Board.o Board.h Entity.h Game.h -o game

test_target: Circle.o main.cpp
	$(CXX) $(CXXFLAGS) main.cpp Circle.o -o main


Entity.o: Entity.cpp
	$(CXX) $(CXXFLAGS) -c Entity.cpp

Game.o: Game.cpp
	$(CXX) $(CXXFLAGS) -c Game.cpp

Board.o: Board.cpp
	$(CXX) $(CXXFLAGS) -c Board.cpp