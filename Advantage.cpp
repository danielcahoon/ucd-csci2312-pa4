// Advantage.cpp

#include "Advantage.h"

namespace Gaming
{
	const char Advantage::ADVANTAGE_ID = 'D';
	const double Advantage::ADVANTAGE_MULT_FACTOR = 2.0;

	Advantage::Advantage(const Game &g, const Position &p, double capacity) : Resource(g, p, capacity)
	{
	}

	Advantage::~Advantage()
	{
	}

	void Advantage::print(std::ostream &os) const
	{
		os << ADVANTAGE_ID;
	}

	double Advantage::getCapacity() const
	{
		return (__capacity * ADVANTAGE_MULT_FACTOR);
	}
	double Advantage::consume()
	{
		finish();
		return (__capacity * ADVANTAGE_MULT_FACTOR);
	}
}