//
//  gmath.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "GMath.h"
#include <math.h>
#include <limits>
#include <stdlib.h>

#define PI 3.14159265358979f
#define HALF_PI 1.5707963f
#define TWO_PI 6.28318530f

/*static*/ const GR32 GMath::GetPI()
{
	return PI;
}

/*static*/ const GR32 GMath::GetHalfPI()
{
	return HALF_PI;
}

/*static*/ const GR32 GMath::GetTwoPI()
{
	return TWO_PI;
}

/**/
/*static*/ const GBOOL GMath::Valid(const GR32 in_value)
{
	return ((in_value < 0.0F) || (in_value == 0.0F) || (0.0F < in_value));
}

/*static*/ const GBOOL GMath::AlmostZero(const GR32 in_value)
{
	return (Abs(in_value) <= std::numeric_limits<GR32>::epsilon());
}

/*static*/ const GBOOL GMath::AlmostZero(const GR32 in_value, const GR32 in_epsilon)
{
	return (Abs(in_value) <= in_epsilon);
}

/*static*/ const GBOOL GMath::NotAlmostZero(const GR32 in_value)
{
	return (std::numeric_limits<GR32>::epsilon() < Abs(in_value));
}

/*static*/ const GBOOL GMath::NotAlmostZero(const GR32 in_value, const GR32 in_epsilon)
{
	return (in_epsilon < Abs(in_value));
}
/*static*/ const GS32 GMath::Abs(const GS32 in_value)
{
	if (in_value < 0)
	{
		return -in_value;
	}
	return in_value;
}

/*static*/ const GR32 GMath::Abs(const GR32 in_value)
{
	if (in_value < 0)
	{
		return -in_value;
	}
	return in_value;
}


/*static*/ const GR32 GMath::SquareRoot(const GR32 in_value)
{
	if (0 < in_value)
	{
		return (GR32)sqrt((GR32)in_value);
	}
	return 0;
}

	
/*static*/ const GR32 GMath::Square(const GR32 in_value)
{
	return (in_value * in_value);
}


/*static*/ const GR32 GMath::Cos(const GR32 in_angleRad)
{
	return cos(in_angleRad);
}


/*static*/ const GR32 GMath::Sin(const GR32 in_angleRad)
{
	return sin(in_angleRad);
}

/*static*/ const GR32 GMath::Tan(const GR32 in_angleRad)
{
	return tan(in_angleRad);
}

/*static*/ const GR32 GMath::ATan(const GR32 in_rize, const GR32 in_run)
{
	return atan2(in_rize, in_run);
}

/*static*/ const GR32 GMath::ACos(const GR32 in_ratio)
{
	return acos(in_ratio);
}

/*static*/ const GR32 GMath::ASin(const GR32 in_ratio)
{
	return asin(in_ratio);
}

/*static*/ const GR32 GMath::Log10(const GR32 in_x)
{
	return log10(in_x);
}

/*static*/ const GR32 GMath::Pow(const GR32 in_x, const GR32 in_y)
{
	return pow(in_x, in_y);
}

/*static*/ const GR32 GMath::Minimum(const GR32 in_valueLhs, const GR32 in_valueRhs)
{
	if (in_valueLhs <= in_valueRhs)
	{
		return in_valueLhs;
	}
	return in_valueRhs;
}
/*static*/ const GS32 GMath::Minimum(const GS32 in_valueLhs, const GS32 in_valueRhs)
{
	if (in_valueLhs <= in_valueRhs)
	{
		return in_valueLhs;
	}
	return in_valueRhs;
}
/*static*/ const GU32 GMath::Minimum(const GU32 in_valueLhs, const GU32 in_valueRhs)
{
	if (in_valueLhs <= in_valueRhs)
	{
		return in_valueLhs;
	}
	return in_valueRhs;
}
/*static*/ const GR32 GMath::Maximum(const GR32 in_valueLhs, const GR32 in_valueRhs)
{
	if (in_valueLhs <= in_valueRhs)
	{
		return in_valueRhs;
	}
	return in_valueLhs;
}
/*static*/ const GS32 GMath::Maximum(const GS32 in_valueLhs, const GS32 in_valueRhs)
{
	if (in_valueLhs <= in_valueRhs)
	{
		return in_valueRhs;
	}
	return in_valueLhs;
}
/*static*/ const GU32 GMath::Maximum(const GU32 in_valueLhs, const GU32 in_valueRhs)
{
	if (in_valueLhs <= in_valueRhs)
	{
		return in_valueRhs;
	}
	return in_valueLhs;
}
/*static*/ const GR32 GMath::Clamp(const GR32 in_value, const GR32 in_boundLow, const GR32 in_boundHigh)
{
	if (in_value < in_boundLow)
	{
		return in_boundLow;
	}
	if (in_boundHigh < in_value)
	{
		return in_boundHigh;
	}

	return in_value;
}
/*static*/ const GS32 GMath::Clamp(const GS32 in_value, const GS32 in_boundLow, const GS32 in_boundHigh)
{
	if (in_value < in_boundLow)
	{
		return in_boundLow;
	}
	if (in_boundHigh < in_value)
	{
		return in_boundHigh;
	}

	return in_value;
}

/*static*/ const GR32 GMath::GetRatio(
	const GR32 in_value,
	const GR32 in_rangeLow,
	const GR32 in_rangeHigh
	)
{
	const GR32 result = (in_value - in_rangeLow) / (in_rangeHigh - in_rangeLow);
	return result;
}

/*static*/ const GR32 GMath::Lerp(
	const GR32 in_ratio,
	const GR32 in_valueLow,
	const GR32 in_valueHigh
	)
{
	const GR32 result = in_valueLow + ((in_valueHigh - in_valueLow) * in_ratio);
	return result;
}

/*static*/ const GS32 GMath::Lerp(
	const GR32 in_ratio,
	const GS32 in_valueLow,
	const GS32 in_valueHigh
	)
{
	const GS32 result = in_valueLow + (GS32)(((in_valueHigh - in_valueLow) * in_ratio) + 0.5F);
	return result;
}
//take the shortest path by limiting to under abs(value) < pi
/*static*/ const GR32 GMath::ClampRadian(const GR32 in_radianValue)
{
	GR32 returnValue(in_radianValue);
	while(returnValue < (-PI))
	{
		returnValue += TWO_PI;
	}
	while(PI < returnValue)
	{
		returnValue -= TWO_PI;
	}

	return returnValue;
}

//3x^2-2x^3
/*static*/ const GR32 GMath::RatioEaseInOut(const GR32 in_ratio)
{
	const GR32 result = ((3.0F - in_ratio - in_ratio) * in_ratio * in_ratio);
	return result;
}

//d/dx(3x^2-2x^3) = 6x-6x^2
/*static*/ const GR32 GMath::RatioEaseInOutRateOfChange(const GR32 in_ratio)
{
	const GR32 result = ((1.0F - in_ratio) * in_ratio * 6.0F);
	return result;
}

/*
on iphone, arc4random?
*/

/*static*/ void GMath::RandomSeed(const GU32 in_seed)
{
	srand(in_seed);
	return;
}

/*static*/ const GS32 GMath::RandomInt()
{
#ifdef IPHONE
	const GS32 result = Abs((GS32)arc4random());
#else
	const GS32 result = rand();
#endif	
	return result;
}

//return [0.0 ... 1.0] (inclusive of upper limit)
/*static*/ const GR32 GMath::RandomUnit()
{
	const GR32 result = (((GR32)(RandomInt() & 0x7FFF)) / 32767.0F);
	return result;
}

/*static*/ const GR32 GMath::RandomUnitPlusMinius()
{
	const GR32 result = ((GMath::RandomUnit() - 0.5F) * 2.0F);
	return result;
}

/*
http://en.wikipedia.org/wiki/Methods_of_computing_square_roots
*/
/*static*/ const GR32 GMath::SquareRootAprox(const GR32 in_value)
{
	union
	{
		GS32 tmp;
		GR32 val;
	} u;
	u.val = in_value;
	//u.tmp -= 1<<23; /* Remove last bit so 1.0 gives 1.0 */
	///* tmp is now an approximation to logbase2(val) */
	//u.tmp >>= 1; /* divide by 2 */
	//u.tmp += 1<<29; /* add 64 to exponent: (e+127)/2 =(e/2)+63, */
	///* that represents (e/2)-64 but we want e/2 */

	u.tmp = (1<<29) + (u.tmp >> 1) - (1<<22) - 0x4C000;

	return u.val;
}

/*
http://en.wikipedia.org/wiki/Methods_of_computing_square_roots
*/
/*static*/ const GR32 GMath::InvertSquareRootAprox(const GR32 in_value)
{
	const GR32 xhalf = 0.5f*in_value;
	union
	{
		GR32 x;
		GS32 i;
	} u;
	u.x = in_value;
	u.i = 0x5f3759df - (u.i >> 1);
	const GR32 result = u.x * (1.5f - (xhalf * u.x * u.x));
	return result;
}

/*static*/ const GS32 GMath::Pad(const GS32 in_value, const GS32 in_divisor)
{
	const GS32 remainder = in_value % in_divisor;
	if (!remainder)
	{
		return in_value;
	}
	return (in_value + (in_divisor - in_value));
}

/*static*/ const GBOOL GMath::QuadraticRoots(
	GR32& out_rootA, 
	GR32& out_rootB, 
	const GR32 in_constantA,
	const GR32 in_constantB,
	const GR32 in_constantC
	)
{
	const GR32 value = ((in_constantB * in_constantB) - (4.0F * in_constantA * in_constantC));
	if (0.0F <= value)
	{
		const GR32 valueSqrt = GMath::SquareRoot(value);
		out_rootA = (-in_constantB + valueSqrt) / (2.0F * in_constantA); 
		out_rootB = (-in_constantB - valueSqrt) / (2.0F * in_constantA); 

		////http://en.wikipedia.org/wiki/Quadratic_equation  better accuracy for computers
		//GR32 q = 0.0F;
		//if (0.0F < in_constantB)
		//{
		//	q = (-0.5F)*(in_constantB + valueSqrt);
		//}
		//else
		//{
		//	q = (-0.5F)*(in_constantB - valueSqrt);
		//}
		//out_rootA = q / in_constantA;
		//out_rootB = in_constantC / q;

		return true;
	}

	return false;
}

/*static*/ const GU32 GMath::NextPow2(const GU32 in_value)
{
	GU32 x = in_value;
	--x;    
	x |= x >> 1;    
	x |= x >> 2;    
	x |= x >> 4;    
	x |= x >> 8;    
	x |= x >> 16;    
	return ++x;
} 

