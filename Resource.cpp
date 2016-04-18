// Resource.cpp

#include "Resource.h"
#include "Piece.h"
#include "Agent.h"


namespace Gaming
{
	const double Resource::RESOURCE_SPOIL_FACTOR = 10;

	Resource::Resource(const Game &g, const Position &p, double capacity) : Piece(g, p), __capacity(capacity)
	{
	}

	Resource::~Resource()
	{
	}

	double Resource::consume()
	{
		finish();
		return __capacity;
	}

	void Resource::age()
	{
		__capacity /= RESOURCE_SPOIL_FACTOR;
		if (__capacity < 1)
		{
			finish();
		}
		
	}

	ActionType Resource::takeTurn(const Surroundings &s) const
	{
		return STAY;
	}

	Piece& Resource::operator*(Piece &other)
	{
		return other.interact(this);
	}

	Piece& Resource::interact(Agent *agent)
	{
		return *agent;
	}

	Piece& Resource::interact(Resource *resource)
	{
		return *this;
	}
}