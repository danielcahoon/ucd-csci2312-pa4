// Game.cpp

#include <iostream>

#include "Game.h"
#include "Piece.h"
#include "Strategic.h"
#include "Simple.h"
#include "Food.h"
#include "Advantage.h"
#include "Exceptions.h"

namespace Gaming
{
	const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
	const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;

	PositionRandomizer Game::__posRandomizer;

	const unsigned Game::MIN_WIDTH = 3;
	const unsigned Game::MIN_HEIGHT = 3;
	const double Game::STARTING_AGENT_ENERGY = 20;
	const double Game::STARTING_RESOURCE_CAPACITY = 10;

	
	void Game::populate()
	{
		unsigned int numStrategic = __numInitAgents / 2;
		unsigned int numSimple = __numInitAgents - numStrategic;
		unsigned int numAdvantages = __numInitResources / 4;
		unsigned int numFoods = __numInitResources - numAdvantages;

		std::default_random_engine gen;
		std::uniform_int_distribution<int> d(0, __width * __height);

		// populate Strategic agents
		while (numStrategic > 0) 
		{
		    int i = d(gen); // random index in the grid vector
		    if (__grid[i] == nullptr) 
		    {
		        Position pos(i / __width, i % __width);
		        __grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);
		        numStrategic --;
		    }
		}
		// populate Simple agents
		while (numSimple > 0)
		{
			int i = d(gen); // random index in the grid vector
		    if (__grid[i] == nullptr) 
		    {
		        Position pos(i / __width, i % __width);
		        __grid[i] = new Simple(*this, pos, Game::STARTING_AGENT_ENERGY);
		        numSimple --;
		    }
		}
		// populate Advantages
		while (numAdvantages > 0)
		{
			int i = d(gen); // random index in the grid vector
		    if (__grid[i] == nullptr) 
		    {
		        Position pos(i / __width, i % __width);
		        __grid[i] = new Advantage(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
		        numAdvantages --;
		    }
		}
		// populate Foods
		int j = 0;
		while (numFoods > 0)
		{
			
			int i = d(gen); // random index in the grid vector
		    if (__grid[i] == nullptr) 
		    {
		        Position pos(i / __width, i % __width);
		        __grid[i] = new Food(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
		        // std::cout << std::endl << "Food to be added " << ++j << ": " << *__grid[i] << " i = " << i << std::endl;
		        // std::cout << *this << std::endl;
		        numFoods --;
		    }

		}
	}

	Game::Game() : __status(NOT_STARTED), __width(MIN_WIDTH), __height(MIN_HEIGHT), __round(0) 
	{
		for (int i = 0; i < __width * __height; ++i)
		{
			__grid.push_back(nullptr);
		}

		__numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
		__numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;
	}

	Game::Game(unsigned width, unsigned height, bool manual) : __height(height), __width(width), __status(NOT_STARTED), __round(0)
	{
		if (width < MIN_WIDTH || height < MIN_HEIGHT)
		{
			throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);
		}
		
		__numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
		__numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;

		for (int i = 0; i < __width * __height; ++i)
		{
			__grid.push_back(nullptr);
		}
		if (manual == false)
		{
			populate();
		}
	}

	Game::Game(const Game &another) : __round(another.__round)
	{
		for (int i = 0; i < another.__grid.size(); ++i)
		{
			__grid[i] = another.__grid[i];
		}
	}

	Game::~Game()
	{
		for (int i = 0; i < __grid.size(); ++i)
		{
			delete __grid[i];
		}
	}

	// getters
	unsigned int Game::getNumPieces() const
	{
		unsigned int numPieces = 0;
		for (int i = 0; i < (__width * __height); ++i)
		{
			if (__grid[i] != nullptr)
			{
				numPieces++;
			}
		}

		return numPieces;
	}
	unsigned int Game::getNumAgents() const
	{
		unsigned int numAgents = 0;
		for (int i = 0; i < __grid.size(); ++i)
		{
			if (__grid[i] != nullptr)
			{
				if (__grid[i]->getType() == PieceType::SIMPLE || __grid[i]->getType() == PieceType::STRATEGIC)
				{
					++numAgents;
				}
			}
		}

		return numAgents;
	}

	unsigned int Game::getNumSimple() const
	{
		unsigned int numSimple = 0;
		for (int i = 0; i < __grid.size(); ++i)
		{
			if (__grid[i] != nullptr)
			{
				if (__grid[i]->getType() == PieceType::SIMPLE)
				{
					++numSimple;
				}
			}
		}

		return numSimple;
	}

	unsigned int Game::getNumStrategic() const
	{
		unsigned int numStrategic = 0;
		for (int i = 0; i < __grid.size(); ++i)
		{
			if (__grid[i] != nullptr)
			{
				if (__grid[i]->getType() == PieceType::STRATEGIC)
				{
					++numStrategic;
				}
			}
		}

		return numStrategic;
	}

	unsigned int Game::getNumResources() const
	{
		unsigned int numResoureces = 0;
		for (int i = 0; i < __grid.size(); ++i)
		{
			if (__grid[i] != nullptr)
			{
				if (__grid[i]->getType() == PieceType::FOOD || __grid[i]->getType() == PieceType::ADVANTAGE)
				{
					++numResoureces;
				}
			}
		}
		return numResoureces;
	}

	const Piece * Game::getPiece(unsigned int x, unsigned int y) const
	{
		if(__grid[(x * __width) + y] == nullptr)
		{
			throw PositionEmptyEx(x, y);
		}
		return __grid[(x * __width) + y];
	}

	// Grid Population Methods
	void Game::addSimple(const Position &position)
	{
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}
		if (__grid[(position.x * __width) + position.y] != nullptr)
		{
			throw PositionNonemptyEx(position.x, position.y);
		}

		__grid[(position.x *__width) + position.y] = new Simple(*this, position, Game::STARTING_AGENT_ENERGY);
	}
	void Game::addSimple(const Position &position, double energy)
	{
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}

		if (__grid[(position.x * __width) + position.y] != nullptr)
		{
			throw PositionNonemptyEx(position.x, position.y);
		}

		__grid[(position.x * __width) + position.y] = new Simple(*this, position, energy);
	}
	void Game::addSimple(unsigned x, unsigned y)
	{
		Position position(x, y);
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}

		if (__grid[(x * __width) + y] != nullptr)
		{
			throw PositionNonemptyEx(x, y);
		}
	
		__grid[(x * __width) + y] = new Simple(*this, position, Game::STARTING_AGENT_ENERGY);	
	}
	void Game::addSimple(unsigned x, unsigned y, double energy)
	{
		Position position(x, y);
		
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}

		if (__grid[(x * __width) + y] != nullptr)
		{
			throw PositionNonemptyEx(x, y);
		}

		__grid[(x * __width) + y] = new Simple(*this, position, energy);
	}
	void Game::addStrategic(const Position &position, Strategy *s)
	{
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}
		if (__grid[(position.x * __width) + position.y] != nullptr)
		{
			throw PositionNonemptyEx(position.x, position.y);
		}

		__grid[(position.x * __width) + position.y] = new Strategic(*this, position, Game::STARTING_AGENT_ENERGY, s);
	}
	void Game::addStrategic(unsigned x, unsigned y, Strategy *s)
	{
		Position position(x, y);
		
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}
		if (__grid[(x * __width) + y] != nullptr)
		{
			throw PositionNonemptyEx(x, y - 2);
		}
		__grid[(x * __width) + y] = new Strategic(*this, position, Game::STARTING_AGENT_ENERGY, s);
	}
	void Game::addFood(const Position &position)
	{
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}
		if (__grid[(position.x * __width) + position.y] != nullptr)
		{
			throw PositionNonemptyEx(position.x, position.y - 1);
		}

		__grid[(position.x * __width) + position.y] = new Food(*this, position, Game::STARTING_RESOURCE_CAPACITY);
	}
	void Game::addFood(unsigned x, unsigned y)
	{
		Position position(x, y);
		
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}		
		if (__grid[(x * __width) + y] != nullptr)
		{
			throw PositionNonemptyEx(x, y - 1);
		}

		__grid[(x * __width) + y] = new Food(*this, position, Game::STARTING_RESOURCE_CAPACITY);
	}
	void Game::addAdvantage(const Position &position)
	{
		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}		
		if (__grid[(position.x * __width) + position.y] != nullptr)
		{
			throw PositionNonemptyEx(position.x, position.y - 1);
		}
		__grid[(position.x * __width) + position.y] = new Advantage(*this, position, Game::STARTING_RESOURCE_CAPACITY);
	}
	void Game::addAdvantage(unsigned x, unsigned y)
	{
		Position position(x, y);

		if (position.x >= __height || position.y >= __width)
		{
			throw OutOfBoundsEx(__width - 1, __height - 1, position.x, position.y);
		}
		if (__grid[(x * __width) + y] != nullptr)
		{
			throw PositionNonemptyEx(x, y);
		}

		__grid[(x * __width) + y] = new Advantage(*this, position, Game::STARTING_RESOURCE_CAPACITY);
	}

	const Surroundings Game::getSurroundings(const Position &p) const
	{
		Surroundings s;
		// Top Left Corner
		if (p.x == 0 && p.y == 0)
		{
			s.array[0] = INACCESSIBLE;
			s.array[1] = INACCESSIBLE;
			s.array[2] = INACCESSIBLE;
			s.array[3] = INACCESSIBLE;
			s.array[4] = SELF;
			if (__grid[1] == nullptr)
			{
				s.array[5] = EMPTY;
			}
			else
			{
				s.array[5] = __grid[1]->getType();	
			}
			s.array[6] = INACCESSIBLE;
			if (__grid[__width] == nullptr)
			{
				s.array[7] = EMPTY;
			}
			else
			{
				s.array[7] = __grid[__width]->getType();
			}
			if (__grid[__width + 1] == nullptr)
			{
				s.array[8] = EMPTY;
			}
			else
			{
				s.array[8] = __grid[__width + 1]->getType();
			}
		}
		// Top Row Not Corner
		else if (p.x == 0 && p.y > 0 && p.y < __width)
		{
			s.array[0] = INACCESSIBLE;
			s.array[1] = INACCESSIBLE;
			s.array[2] = INACCESSIBLE;
			if (__grid[p.y - 1] == nullptr)
			{
				s.array[3] = EMPTY;
			}
			else
			{
				s.array[3] = __grid[p.y - 1]->getType();
			}
			s.array[4] = SELF;
			if(__grid[p.y + 1] == nullptr)
			{
				s.array[5] = EMPTY;
			}
			else
			{
				s.array[5] = __grid[p.y + 1]->getType();
			}
			if (__grid[((p.x + 1) * __width) + (p.y - 1)] == nullptr)
			{
				s.array[6] = EMPTY;
			}
			else
			{
				s.array[6] = __grid[((p.x + 1) * __width) + (p.y - 1)]->getType();
			}
			if (__grid[((p.x + 1) * __width) + p.y] == nullptr)
			{
				s.array[7] = EMPTY;
			}
			else
			{
				s.array[7] = __grid[((p.x + 1) * __width) + p.y]->getType();
			}
			if (__grid[((p.x + 1) * __width) + (p.y + 1)] == nullptr)
			{
				s.array[8] = EMPTY;
			}
			else
			{
				s.array[8] = __grid[((p.x + 1) * __width) + (p.y + 1)]->getType();
			}
		}
		// Top Right Corner
		else if (p.x == 0 && p.y == __width - 1)
		{
			s.array[0] = INACCESSIBLE;
			s.array[1] = INACCESSIBLE;
			s.array[2] = INACCESSIBLE;
			if (__grid[p.y - 1] == nullptr)
			{
				s.array[3] = EMPTY;
			}	
			else
			{
				s.array[3] = __grid[p.y - 1]->getType();
			}
			s.array[4] = SELF;
			s.array[5] = INACCESSIBLE;
			if (__grid[((p.x + 1) * __width) + (p.y - 1)] == nullptr)
			{
				s.array[6] = EMPTY;
			}
			else
			{
				s.array[6] = __grid[((p.x + 1) * __width) + (p.y - 1)]->getType();
			}
			if (__grid[((p.x + 1) * __width) + p.y] == nullptr)
			{
				s.array[7] = EMPTY;
			}
			else
			{
				s.array[7] = __grid[((p.x + 1) * __width) + p.y]->getType();
			}
			s.array[8] = INACCESSIBLE;
		}
		// Left Column Not Corner
		else if(p.x > 0 && p.x < __height - 1 && p.y == 0)
		{
			s.array[0] = INACCESSIBLE;
			if (__grid[(p.x - 1) * __width] == nullptr)
			{
				s.array[1] = EMPTY;
			}
			else
			{
				s.array[1] = __grid[(p.x - 1) * __width]->getType();
			}
			if (__grid[((p.x - 1) * __width) + 1] == nullptr)
			{
				s.array[2] = EMPTY;
			}
			else
			{
				s.array[2] = __grid[((p.x - 1) * __width) + 1]->getType();
			}
			s.array[3] = INACCESSIBLE;
			s.array[4] = SELF;
			if (__grid[(p.x * __width) + 1] == nullptr)
			{
				s.array[5] = EMPTY;
			}
			else
			{
				s.array[5] = __grid[(p.x * __width) + 1]->getType();
			}
			s.array[6] = INACCESSIBLE;
			if (__grid[((p.x + 1) * __width)] == nullptr)
			{
				s.array[7] = EMPTY;
			}
			else
			{
				s.array[7] = __grid[((p.x + 1) * __width)]->getType();
			}
			if (__grid[((p.x + 1) * __width) + 1] == nullptr)
			{
				s.array[8] = EMPTY;
			}
			else
			{
				s.array[8] = __grid[((p.x + 1) * __width) + 1]->getType();
			}
		}
		// Middle of Grid
		else if (p.x > 0 && p.x < __height - 1 && p.y > 0 && p.y < __width - 1)
		{
			if (__grid[((p.x - 1) * __width) + p.y - 1] == nullptr)
			{
				s.array[0] = EMPTY;
			}
			else
			{
				s.array[0] = __grid[((p.x - 1) * __width) + p.y - 1]->getType();
			}
			if (__grid[((p.x - 1) * __width) + p.y] == nullptr)
			{
				s.array[1] = EMPTY;
			}
			else
			{
				s.array[1] = __grid[((p.x - 1) * __width) + p.y]->getType();
			}
			if (__grid[((p.x - 1) * __width) + p.y + 1] == nullptr)
			{
				s.array[2] = EMPTY;
			}
			else
			{
				s.array[2] = __grid[((p.x - 1) * __width) + p.y + 1]->getType();
			}
			if (__grid[(p.x * __width) + p.y - 1] == nullptr)
			{
				s.array[3] = EMPTY;
			}
			else
			{
				s.array[3] = __grid[(p.x * __width) + p.y - 1]->getType();
			}
			s.array[4] = SELF;
			if (__grid[(p.x * __width) + p.y + 1] == nullptr)
			{
				s.array[5] = EMPTY;
			}
			else
			{
				s.array[5] = __grid[(p.x * __width) + p.y + 1]->getType();
			}
			if (__grid[((p.x + 1) * __width) + p.y - 1] == nullptr)
			{
				s.array[6] = EMPTY;
			}
			else
			{
				s.array[6] = __grid[((p.x + 1) * __width) + p.y - 1]->getType();
			}
			if (__grid[((p.x + 1) * __width) + p.y] == nullptr)
			{
				s.array[7] = EMPTY;
			}
			else
			{
				s.array[7] = __grid[((p.x + 1) * __width) + p.y]->getType();
			}
			if (__grid[((p.x + 1) * __width) + p.y + 1] == nullptr)
			{
				s.array[8] = EMPTY;
			}
			else
			{
				s.array[8] = __grid[((p.x + 1) * __width) + p.y + 1]->getType();
			}
		}
		// Right Column Not Corner
		else if (p.x > 0 && p.x < __height - 1 && p.y == __width - 1)
		{
			if (__grid[(p.x - 1) * __height + p.y - 1] == nullptr)
			{
				s.array[0] = EMPTY;
			}
			else
			{
				s.array[0] = __grid[(p.x - 1) * __height + p.y - 1]->getType();
			}
			if (__grid[(p.x - 1) * __height + p.y] == nullptr)
			{
				s.array[1] = EMPTY;
			}
			else
			{
				s.array[1] = __grid[(p.x - 1) * __height + p.y]->getType();
			}
			s.array[2] = INACCESSIBLE;
			if (__grid[(p.x * __height) + p.y - 1] == nullptr)
			{
				s.array[3] = EMPTY;
			}
			else
			{
				s.array[3] = __grid[(p.x * __height) + p.y - 1]->getType();
			}
			s.array[4] = SELF;
			s.array[5] = INACCESSIBLE;
			if (__grid[(p.x + 1) * __height + p.y - 1] == nullptr)
			{
				s.array[6] = EMPTY;
			}
			else
			{
				s.array[6] = __grid[(p.x + 1) * __height + p.y - 1]->getType();
			}
			if (__grid[(p.x + 1) * __height + p.y] == nullptr)
			{
				s.array[7] = EMPTY;
			}
			else
			{
				s.array[7] = __grid[(p.x + 1) * __height + p.y]->getType();
			}
			s.array[8] = INACCESSIBLE;
		}
		// Bottom Left Corner
		else if (p.x == __height - 1 && p.y == 0)
		{
			s.array[0] = INACCESSIBLE;
			if (__grid[((p.x - 1) * __width) + p.y] == nullptr)
			{
				s.array[1] = EMPTY;
			}
			else
			{
				s.array[1] = __grid[((p.x - 1) * __width) + p.y]->getType();
			}
			if(__grid[((p.x - 1) * __width) + p.y + 1] == nullptr)
			{
				s.array[2] = EMPTY;
			}
			else
			{
				s.array[2] = __grid[((p.x - 1) * __width) + p.y + 1]->getType();
			}
			s.array[3] = INACCESSIBLE;
			s.array[4] = SELF;
			if (__grid[(p.x * __width) + p.y + 1] == nullptr)
			{
				s.array[5] = EMPTY;
			}
			else
			{
				s.array[5] = __grid[(p.x * __width) + p.y + 1]->getType();
			}
			s.array[6] = INACCESSIBLE;
			s.array[7] = INACCESSIBLE;
			s.array[8] = INACCESSIBLE;
		}
		// Bottom Row Not Corner
		else if (p.x == __height - 1 && p.y > 0 && p.y < __width - 1)
		{
			if (__grid[((p.x - 1) * __width) + p.y - 1] == nullptr)
			{
				s.array[0] = EMPTY;
			}
			else
			{
				s.array[0] = __grid[((p.x - 1) * __width) + p.y - 1]->getType();
			}
			if (__grid[((p.x - 1) * __width) + p.y] == nullptr)
			{
				s.array[1] = EMPTY; 
			}
			else
			{
				s.array[1] = __grid[((p.x - 1) * __width) + p.y]->getType();
			}
			if (__grid[((p.x - 1) * __width) + p.y + 1] == nullptr)
			{
				s.array[2] = EMPTY;
			}
			else
			{
				s.array[2] = __grid[((p.x - 1) * __width) + p.y + 1]->getType();
			}
			if (__grid[(p.x * __width) + p.y - 1] == nullptr)
			{
				s.array[3] = EMPTY;
			}
			else
			{
				s.array[3] = __grid[(p.x * __width) + p.y - 1]->getType();
			}
			s.array[4] = SELF;
			if (__grid[(p.x * __width) + p.y + 1] == nullptr)
			{
				s.array[5] = EMPTY;
			}
			else
			{
				s.array[5] = __grid[(p.x * __width) + p.y + 1]->getType();
			}
			s.array[6] = INACCESSIBLE;
			s.array[7] = INACCESSIBLE;
			s.array[8] = INACCESSIBLE;
		}
		// Bottom Right Corner
		else if (p.x == __height - 1 && p.y == __width - 1)
		{
			if (__grid[((p.x - 1) * __width) + p.y - 1] == nullptr)
			{
				s.array[0] = EMPTY;
			}
			else
			{
				s.array[0] = __grid[((p.x - 1) * __width) + p.y - 1]->getType();
			}
			if (__grid[((p.x - 1) * __width) + p.y] == nullptr)
			{
				s.array[1] = EMPTY;
			}
			else
			{
				s.array[1] = __grid[((p.x - 1) * __width) + p.y]->getType();
			}
			s.array[2] = INACCESSIBLE;
			if (__grid[(p.x * __width) + p.y - 1] == nullptr)
			{
				s.array[3] = EMPTY;
			}
			else
			{
				s.array[3] = __grid[(p.x * __width) + p.y - 1]->getType();
			}
			s.array[4] = SELF;
			s.array[5] = INACCESSIBLE;
			s.array[6] = INACCESSIBLE;
			s.array[7] = INACCESSIBLE;
			s.array[8] = INACCESSIBLE;
		}
		return s;
	}

	// Gameplay Methods
	const ActionType Game::reachSurroundings(const Position &from, const Position &to)
	{
		ActionType a = STAY;
		if (from.x - to.x == 1 && from.y - to.y == 1)
		{
			a = NW;
		}
		else if(from.x - to.x == 1 && from.y - to.y == 0)
		{
			a = N;
		}
		else if (from.x - to.x == 1 && from.y - to.y == -1)
		{
			a = NE;
		}
		else if (from.x - to.x == 0 && from.y - to.y == 1)
		{
			a = W;
		}
		else if (from.x - to.x == 0 && from.y - to.y == 0)
		{
			a = STAY;
		}
		else if (from.x - to.x == 0 && from.y - to.y == -1)
		{
			a = E;
		}
		else if (from.x - to.x == -1 && from.y - to.y == 1)
		{
			a = SW;
		}
		else if (from.x - to.x == -1 && from.y - to.y == 0)
		{
			a = S;
		}
		else if (from.x - to.x == -1 && from.y - to.y == -1)
		{
			a = SE;
		}
		return a;
	}

	bool Game::isLegal(const ActionType &ac, const Position &pos) const
	{
		Surroundings s = getSurroundings(pos);
		// North West
		if (ac == NW)
		{
			if (s.array[0] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		else if (ac == N)
		{
			if (s.array[1] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		else if (ac == NE)
		{
			if (s.array[2] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		else if (ac == W)
		{
			if (s.array[3] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		else if (ac == STAY)
		{
			return true;
		}
		else if (ac == E)
		{
			if (s.array[5] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		else if (ac == SW)
		{
			if (s.array[6] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		else if (ac == S)
		{
			if (s.array[7] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		else if (ac == SE)
		{
			if (s.array[8] == INACCESSIBLE)
			{
				return false;
			}
			return true;
		}
		return true;
	}

	const Position Game::move(const Position &pos, const ActionType &ac) const
	{
		if (isLegal(ac, pos))
		{
			// North West
			if (ac == NW)
			{
				// std::cout << "~NW" << std::endl;
				Position p((pos.x - 1), (pos.y - 1));
				return p;
			}
			// North
			else if (ac == N)
			{
				// std::cout << "~N" << std::endl;
				Position p((pos.x - 1), pos.y);
				return p;
			}
			// North East
			else if (ac == NE)
			{
				// std::cout << "~NE" << std::endl;
				Position p((pos.x - 1), (pos.y + 1));
				return p;
			}
			else if (ac == W)
			{
				// std::cout << "~W" << std::endl;
				Position p(pos.x, (pos.y - 1));
				return p;
			}
			else if (ac == STAY)
			{
				// std::cout << "~STAY" << std::endl;
				return pos;
			}
			else if (ac == E)
			{
				// std::cout << "~E" << std::endl;
				Position p(pos.x, (pos.y + 1));
				return p;
			}
			else if (ac == SW)
			{
				// std::cout << "~SW" << std::endl;
				Position p((pos.x + 1), (pos.y - 1));
				return p;
			}
			else if (ac == S)
			{
				// std::cout << "~S" << std::endl;
				Position p((pos.x + 1), pos.y);
				return p;
			}
			else if (ac == SE)
			{
				// std::cout << "~SE" << std::endl;
				Position p((pos.x + 1), (pos.y + 1));
				return p;
			}
		}
		return pos;
	}

	void Game::round()
	{
		++__round;
		__status = PLAYING;
		for (int i = 0; i < __grid.size(); ++i)
		{
			if (__grid[i] != nullptr)
			{
				ActionType ac = STAY;
				Position p0(i / __width, i % __width);
				Surroundings surr = getSurroundings(p0);
				ac = __grid[i]->takeTurn(surr);
				// Position p1 = Game::move(p0, ac);
				// if(p1.x != p0.x && p1.y != p0.y)
				// {		
				// 	Piece *q;
				// 	q = __grid[i];
				// 	__grid[(p1.x * __width) + p1.y] = q;
				// 	__grid[i] = nullptr;
				// }
				Position p1 = __grid[i]->getPosition();
				p1.x += 1;
				p1.y += 1; 
				__grid[i]->setPosition(p1);
			}
		}
		for (int i = 0; i < __grid.size(); ++i)
		{
			if(__grid[i] != nullptr)
			{
				__grid[i]->age();
			}
		}
	}

	void Game::play(bool verbose)
	{
		// unsigned resource = __numInitResources;
		// while (resource > 0)
		// {
		// 	round();
		// 	resource - NUM_INIT_RESOURCE_FACTOR;
		// }
	}

	std::ostream& operator<<(std::ostream &os, const Game &game)
	{
		os << "Round " << game.__round << ":\n";
		for (int i = 0; i < (game.__height); ++i)
		{
			for (int j = 0; j < game.__width; ++j)
			{
				if (game.__grid[(game.__width * i ) + j] == nullptr)
				{
					os << "[     ]";
				}
				else
				{
					std::string pieceStr;
					// std::cout << *game.__grid[(game.__width * i ) + j] << std::endl;
					
					os << "[" << *game.__grid[(game.__width * i ) + j];
					if (pieceStr.length() == 2)
					{
						os << "   ]";
					}
					else if (pieceStr.length() == 3)
					{
						os << "  ]";
					}
					else if (pieceStr.length() == 4)
					{
						os << " ]";
					}
					else
					{
						os << "]";
					}
				}
			}
			os << "\n";
		}
		os << "Status:";
		if (game.__status == Game::PLAYING)
		{
			os << " Playing...";
		}
		else if (game.__status == Game::OVER)
		{
			os << " Over!";
		}
		else
		{
			os << "NOT_STARTED";
		}
		os << '\n';
		return os;
	}
}