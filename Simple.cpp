// Simple.cpp

#include "Simple.h"
#include "Game.h"
#include "Gaming.h"

namespace Gaming
{
	const char Simple::SIMPLE_ID = 'S';

	Simple::Simple(const Game &g, const Position &p, double energy) : Agent(g, p, energy)
	{
	}

	Simple::~Simple()
	{
	}

	void Simple::print(std::ostream &os) const
	{
		os << SIMPLE_ID;
	}

	ActionType Simple::takeTurn(const Surroundings &s) const
	{
		std::vector<int> resources;
		std::vector<int> agents;
		std::vector<int> empty;
		ActionType ac = STAY;
		for (int i = 0; i < 9; ++i)
		{
			if (s.array[i] == STRATEGIC || s.array[i] == SIMPLE)
			{
				agents.push_back(i);
			}
			else if (s.array[i] == FOOD || s.array[i] == ADVANTAGE)
			{
				resources.push_back(i);
			}
			else if (s.array[i] == EMPTY)
			{
				empty.push_back(i);
			}
		}
		if (!resources.empty())
		{
			Position p;
			p = __game.randomPosition(resources);
			ac = Game::reachSurroundings(getPosition(), p);
		}
		else if (!empty.empty())
		{
			Position p;
			p = __game.randomPosition(empty);
			ac = Game::reachSurroundings(getPosition(), p);
		}
		else
		{
			ac = STAY;
		}

		return ac;
	}
}