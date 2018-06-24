//
//  GVector2Float.cpp
//
//  Created by David Coen on 23/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GVector2Float.h"

#include "GMath.h"

//static public members
/*static*/ const GVector2Float GVector2Float::sZero(0.0F, 0.0F);
/*static*/ const GVector2Float GVector2Float::sUnit(1.0F, 1.0F);
/*static*/ const GVector2Float GVector2Float::sUnitX(1.0F, 0.0F);
/*static*/ const GVector2Float GVector2Float::sUnitY(0.0F, 1.0F);

//constructors
GVector2Float::GVector2Float(const GR32 in_dataX, const GR32 in_dataY)
{
	SetData(in_dataX, in_dataY);
	return;
}

GVector2Float::GVector2Float(const GVector2Float& in_src)
{
	(*this) = in_src;
	return;
}

GVector2Float::~GVector2Float()
{
	return;
}

//operators
const GVector2Float& GVector2Float::operator=(const GVector2Float& in_rhs)
{
	m_x = in_rhs.m_x;
	m_y = in_rhs.m_y;
	return (*this);
}
const GVector2Float& GVector2Float::operator+=(const GVector2Float& in_rhs)
{
	m_x += in_rhs.m_x;
	m_y += in_rhs.m_y;
	return (*this);
}
const GVector2Float& GVector2Float::operator-=(const GVector2Float& in_rhs)
{
	m_x -= in_rhs.m_x;
	m_y -= in_rhs.m_y;
	return (*this);
}
const GVector2Float& GVector2Float::operator*=(const GR32 in_rhs)
{
	m_x *= in_rhs;
	m_y *= in_rhs;
	return (*this);
}
const GVector2Float& GVector2Float::operator/=(const GR32 in_rhs)
{
	const GR32 mul = 1.0F / in_rhs;
	m_x *= mul;
	m_y *= mul;
	return (*this);
}
	
void GVector2Float::SetData(const GR32 in_dataX, const GR32 in_dataY)
{
	m_x = in_dataX;
	m_y = in_dataY;
	return;
}

//global operators
const GBOOL operator==(const GVector2Float& in_lhs, const GVector2Float& in_rhs)
{
	if (&in_lhs == &in_rhs)
	{
		return true;
	}
	if (!GMath::AlmostZero(in_lhs.m_x - in_rhs.m_x))
	{
		return false;
	}
	if (!GMath::AlmostZero(in_lhs.m_y - in_rhs.m_y))
	{
		return false;
	}
	return true;
}

const GBOOL operator!=(const GVector2Float& in_lhs, const GVector2Float& in_rhs)
{
	return (!(operator==(in_lhs, in_rhs)));
}

const GVector2Float operator+(const GVector2Float& in_lhs, const GVector2Float& in_rhs)
{
	return GVector2Float(
		in_lhs.m_x + in_rhs.m_x,
		in_lhs.m_y + in_rhs.m_y
		);
}

const GVector2Float operator-(const GVector2Float& in_lhs, const GVector2Float& in_rhs)
{
	return GVector2Float(
		in_lhs.m_x - in_rhs.m_x,
		in_lhs.m_y - in_rhs.m_y
		);
}

const GVector2Float operator-(const GVector2Float& in_rhs)
{
	return GVector2Float(
		-in_rhs.m_x,
		-in_rhs.m_y
		);
}


const GVector2Float operator*(const GVector2Float& in_lhs, const GVector2Float& in_rhs)
{
	return GVector2Float(
		in_lhs.m_x * in_rhs.m_x,
		in_lhs.m_y * in_rhs.m_y
		);
}

const GVector2Float operator*(const GVector2Float& in_lhs, const GR32 in_rhs)
{
	return GVector2Float(
		in_lhs.m_x * in_rhs,
		in_lhs.m_y * in_rhs
		);
}

const GVector2Float operator*(const GR32 in_lhs, const GVector2Float& in_rhs)
{
	return GVector2Float(
		in_lhs * in_rhs.m_x,
		in_lhs * in_rhs.m_y
		);
}

const GVector2Float operator/(const GVector2Float& in_lhs, const GR32 in_rhs)
{
	return (in_lhs * (1.0F / in_rhs));
}

const GR32 DotProduct(const GVector2Float& in_lhs, const GVector2Float& in_rhs)
{
	return ((in_lhs.m_x * in_rhs.m_x) + 
		(in_lhs.m_y * in_rhs.m_y));
}

const GR32 Length(const GVector2Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);
	const GR32 length = GMath::SquareRoot(lengthSquared);
	return length;
}

const GR32 LengthSquared(const GVector2Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);
	return lengthSquared;
}


const GR32 LengthAprox(const GVector2Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);
	const GR32 length = GMath::SquareRootAprox(lengthSquared);
	return length;
}

const GVector2Float Normalise(const GVector2Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		return GVector2Float::sZero;
	}

	//already normalised case
	if (GMath::AlmostZero(lengthSquared - 1.0F))
	{
		return in_src;
	}

	const GR32 mul = 1.0F / GMath::SquareRoot(lengthSquared);
	GVector2Float value = in_src * mul;
	return value;
}

const GVector2Float NormaliseAprox(const GVector2Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		return GVector2Float::sZero;
	}

	const GR32 mul = GMath::InvertSquareRootAprox(lengthSquared);
	GVector2Float value = in_src * mul;
	return value;
}

const GVector2Float Normalise(const GVector2Float& in_src, GR32& out_length)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		out_length = 0.0F;
		return GVector2Float::sZero;
	}

	//already normalised case
	if (GMath::AlmostZero(lengthSquared - 1.0F))
	{
		out_length = 1.0F;
		return in_src;
	}

	out_length = GMath::SquareRoot(lengthSquared);
	const GR32 mul = 1.0F / out_length;
	GVector2Float value = in_src * mul;
	return value;
}


const GVector2Float NormaliseAprox(const GVector2Float& in_src, GR32& out_length)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		out_length = 0.0F;
		return GVector2Float::sZero;
	}

	out_length = GMath::SquareRootAprox(lengthSquared);
	const GR32 mul = 1.0F / out_length;
	GVector2Float value = in_src * mul;
	return value;
}

const GBOOL BoundPointOverlap(
    const GVector2Float& in_lhsBoundLow,
    const GVector2Float& in_lhsBoundHigh,
    const GVector2Float& in_point
	)
{
    return ((in_lhsBoundLow.m_x <= in_point.m_x) &&
            (in_point.m_x < in_lhsBoundHigh.m_x) &&
            (in_lhsBoundLow.m_y <= in_point.m_y) &&
            (in_point.m_y < in_lhsBoundHigh.m_y));
}

const GBOOL BoundBoundOverlap(
    const GVector2Float& in_lhsBoundLow,
    const GVector2Float& in_lhsBoundHigh,
    const GVector2Float& in_rhsBoundLow,
    const GVector2Float& in_rhsBoundHigh
	)
{
    return ((in_lhsBoundLow.m_x <= in_rhsBoundHigh.m_x) &&
            (in_rhsBoundLow.m_x < in_lhsBoundHigh.m_x) &&
            (in_lhsBoundLow.m_y <= in_rhsBoundHigh.m_y) &&
            (in_rhsBoundLow.m_y < in_lhsBoundHigh.m_y));
}