// Strategic.cpp

#include "Strategic.h"
#include "DefaultAgentStrategy.h"
#include "AggressiveAgentStrategy.h"
#include "Game.h"
#include "Gaming.h"

namespace Gaming
{
	const char Strategic::STRATEGIC_ID = 'T';

	Strategic::Strategic(const Game &g, const Position &p, double energy, Strategy *s) : Agent(g, p, energy), __strategy(s)
	{
	}

	Strategic::~Strategic()
	{
		delete __strategy;
	}

	void Strategic::print(std::ostream &os) const
	{
		os << STRATEGIC_ID;
	}

	ActionType Strategic::takeTurn(const Surroundings &s) const
	{
		ActionType strat = (*__strategy)(s);

		std::vector<int> simple;
		std::vector<int> food;
		std::vector<int> advantage;
		std::vector<int> empty;
		std::vector<int> agent;

		Position p;
		ActionType ac = STAY;

		for (int i = 0; i < 9; ++i)
		{
			if (s.array[i] == SIMPLE)
			{
				simple.push_back(i);
			}
			else if (s.array[i] == FOOD)
			{
				food.push_back(i);
			}
			else if (s.array[i] == ADVANTAGE)
			{
				advantage.push_back(i);
			}
			else if (s.array[i] == EMPTY)
			{
				empty.push_back(i);
			}
			if (s.array[i] == STRATEGIC || s.array[i] == SIMPLE)
			{
				agent.push_back(i);
			}
		}

		if (strat == N)
		{
			if (__energy >= AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD && !agent.empty())
			{
				p = Game::randomPosition(agent);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			if (!advantage.empty())
			{
				p = Game::randomPosition(advantage);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			else if (!food.empty())
			{
				p = Game::randomPosition(food);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			else if (!empty.empty())
			{
				p = Game::randomPosition(empty);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			else
			{
				ac = STAY;
			}
		}
		else if (strat == S)
		{
			if (!advantage.empty())
			{
				p = Game::randomPosition(advantage);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			else if (!food.empty())
			{
				p = Game::randomPosition(food);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			else if (!empty.empty())
			{
				p = Game::randomPosition(empty);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			else if (!simple.empty())
			{
				p = Game::randomPosition(simple);
				ac = Game::reachSurroundings(Piece::getPosition(), p);
			}
			else
			{
				ac = STAY;
			}
		}
		return ac;
	}

}