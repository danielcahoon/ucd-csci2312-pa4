CXX := c++
CXXFLAGS := -g -std=gnu++11

ucd-csci2312-pa4: ErrorContext.o GamingTests.o Advantage.o Agent.o DefaultAgentStrategy.o Exceptions.o Food.o Game.o Gaming.o Piece.o Resource.o Simple.o Strategic.o Strategy.o main.o
		$(CXX) $(CXXFLAGS) -o ucd-csci2312-pa4 ErrorContext.o GamingTests.o Advantage.o Agent.o DefaultAgentStrategy.o Exceptions.o Food.o Game.o Gaming.o Piece.o Resource.o Simple.o Strategic.o Strategy.o main.o

main.o: main.cpp ErrorContext.h GamingTests.h
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp

ErrorContext.o: ErrorContext.h ErrorContext.cpp
	$(CXX) $(CXXFLAGS) -c -o ErrorContext.o ErrorContext.cpp

GamingTests.o: GamingTests.h GamingTests.cpp
	$(CXX) $(CXXFLAGS) -c -o GamingTests.o GamingTests.cpp

Advantage.o: Resource.h Game.h Advantage.h Advantage.cpp
	$(CXX) $(CXXFLAGS) -c -o Advantage.o Advantage.cpp

Agent.o: Game.h Piece.h Agent.h Agent.cpp
	$(CXX) $(CXXFLAGS) -c -o Agent.o Agent.cpp

DefaultAgentStrategy.o: Strategy.h DefaultAgentStrategy.h DefaultAgentStrategy.cpp
	$(CXX) $(CXXFLAGS) -c -o DefaultAgentStrategy.o DefaultAgentStrategy.cpp

Exceptions.o: Exceptions.h Exceptions.cpp
	$(CXX) $(CXXFLAGS) -c -o Exceptions.o Exceptions.cpp

Food.o: Game.h Resource.h Food.h Food.cpp
	$(CXX) $(CXXFLAGS) -c -o Food.o Food.cpp

Game.o: Gaming.h DefaultAgentStrategy.h Game.h Game.cpp
	$(CXX) $(CXXFLAGS) -c -o Game.o Game.cpp

Gaming.o: Exceptions.h Gaming.h
	$(CXX) $(CXXFLAGS) -c -o Gaming.o Gaming.h

Piece.o: Game.h Piece.h Piece.cpp
	$(CXX) $(CXXFLAGS) -c -o Piece.o Piece.cpp

Resource.o: Game.h Piece.h Resource.h Resource.cpp
	$(CXX) $(CXXFLAGS) -c -o Resource.o Resource.cpp

Simple.o: Game.h Agent.h Simple.h Simple.cpp
	$(CXX) $(CXXFLAGS) -c -o Simple.o Simple.cpp

Strategic.o: Game.h Agent.h Strategy.h DefaultAgentStrategy.h Strategic.h Strategic.cpp
	$(CXX) $(CXXFLAGS) -c -o Strategic.o Strategic.cpp

Strategy.o: Gaming.h Strategy.h Strategy.cpp
	$(CXX) $(CXXFLAGS) -c -o Strategy.o Strategic.cpp