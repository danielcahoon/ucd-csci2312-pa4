// AggressiveAgentStrategy.cpp

#include "AggressiveAgentStrategy.h"
#include "Game.h"


namespace Gaming
{
	const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

	AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy) : __agentEnergy(agentEnergy)
	{
	}

	AggressiveAgentStrategy::~AggressiveAgentStrategy()
	{
	}

	ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const
	{
		ActionType ac = N;
		return ac;
	}
}