// Agent.cpp


#include "Resource.h"
#include "Agent.h"
#include "Advantage.h"

namespace Gaming
{

	const double Agent::AGENT_FATIGUE_RATE = 0.3;

	Agent::Agent(const Game &g, const Position &p, double energy) : Piece(g, p), __energy(energy)
	{

	}

	Agent::~Agent()
	{
	}

	void Agent::age()
	{
		__energy -= AGENT_FATIGUE_RATE;
		if (__energy <= 0)
		{
			finish();
		}
	}


	Piece& Agent::operator*(Piece &other)
	{
		return other.interact(this);
	}

	Piece& Agent::interact(Agent *agent)
	{
		if(__energy > agent->__energy)
		{
			agent->finish();
		}
		else if (__energy < agent->__energy)
		{
			finish();
		}
		else if (__energy == agent->__energy)
		{
			finish();
			agent->finish();
		}
		return *agent;
	}

	Piece& Agent::interact(Resource *resource)
	{
		__energy += resource->consume();
		return *this;
	}


}