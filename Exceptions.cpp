// Exceptions.cpp
#include <iostream>

#include "Exceptions.h"

namespace Gaming
{
	// GamingException
	void GamingException::setName(std::string name)
	{
		__name = name;
	}

	std::ostream &operator<<(std::ostream &os, const GamingException &ex)
	{
		os << ex.__name << ": ";
		ex.__print_args(os);
		return os;
	}

	// DimensionEx
	DimensionEx::DimensionEx(unsigned expWidth, unsigned expHeight, unsigned width, unsigned height) :
		__exp_width(expWidth),
		__exp_height(expHeight),
		__width(width),
		__height(height)
	{
		setName("DimensionEx");
	}

	unsigned DimensionEx::getExpWidth() const
	{
		return __exp_width;
	}
	unsigned DimensionEx::getExpHeight() const
	{
		return __exp_height;
	}
	unsigned DimensionEx::getWidth() const
	{
		return __width;
	}
	unsigned DimensionEx::getHeight() const
	{
		return __height;
	}

	// InsufficientDimensionsEx

	InsufficientDimensionsEx::InsufficientDimensionsEx(unsigned minWidth, unsigned minHeight, unsigned width, unsigned height) : DimensionEx(minWidth, minHeight, width, height)
	{
		setName("InsufficientDimensionsEx");
	}

	void InsufficientDimensionsEx::__print_args(std::ostream &os) const
	{
		os << "minWidth = " << getExpWidth() << " minHeight = " << getExpHeight() << " width = " << getWidth() << " height = " << getHeight();
	}

	// OutOfBoundsEx

	OutOfBoundsEx::OutOfBoundsEx(unsigned maxWidth, unsigned maxHeight, unsigned width, unsigned height) : DimensionEx(maxWidth, maxHeight, width, height)
	{
		setName("OutOfBoundsEx");
	}
	void OutOfBoundsEx::__print_args(std::ostream &os) const
	{
		os << "maxWidth = " << getExpWidth() << " maxHeight = " << getExpHeight() << " width = " << getWidth() << " height = " << getHeight();
	}

	// PositionEx

	PositionEx::PositionEx(unsigned x, unsigned y) : __x(x), __y(y)
	{
		setName("PositionEx");
	}

	void PositionEx::__print_args(std::ostream &os) const
	{
		os << "__x = " << __x << " __y = " << __y;
	}

	// PositionNonEmptyEx

	PositionNonemptyEx::PositionNonemptyEx(unsigned x, unsigned y) : PositionEx(x, y)
	{
		setName("PositionNonemptyEx");
	}

	// PositionEmptyEx

	PositionEmptyEx::PositionEmptyEx(unsigned x, unsigned y) : PositionEx(x, y)
	{
		setName("PositionEmptyEx");
	}

	// PosVectorEmptyEx

	PosVectorEmptyEx::PosVectorEmptyEx()
	{
		setName("PosVectorEmptyEx");
	}

	void PosVectorEmptyEx::__print_args(std::ostream &os) const
	{
		os << "PosVector is empty";
	}
}