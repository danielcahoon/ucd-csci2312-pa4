// Piece.cpp

#include "Piece.h"


namespace Gaming
{
	unsigned int Piece::__idGen = 0;

	Piece::Piece(const Game &g, const Position &p) : __position(p), __turned(false), __finished(false), __game(g)
	{
		__id = __idGen;
		++__idGen;
	}

	Piece::~Piece()
	{
	}

	std::ostream& operator<<(std::ostream &os, const Piece &piece)
	{
		piece.print(os);
		os << piece.__id;
		return os;
	}
}