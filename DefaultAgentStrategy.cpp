// DefaultAgentStrategy.cpp



#include "DefaultAgentStrategy.h"
#include "Game.h"
#include "Piece.h"

namespace Gaming
{
	DefaultAgentStrategy::DefaultAgentStrategy()
	{
	}
	DefaultAgentStrategy::~DefaultAgentStrategy()
	{
	}

	ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const
	{
		ActionType ac = S;

		return ac;
	}
}