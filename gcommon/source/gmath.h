//
//  gmath.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmath_h_
#define _gmath_h_

#include "gcommon.h"

class GMath
{
private:
	~GMath();

public:
	static const GR32 GetPI();
	static const GR32 GetHalfPI();
	static const GR32 GetTwoPI();

	static const GBOOL Valid(const GR32 in_value);

	//return true if valid GR32 at or within epsilon of zero
	static const GBOOL AlmostZero(const GR32 in_value);
	static const GBOOL AlmostZero(const GR32 in_value, const GR32 in_epsilon);

	//return true if valid GR32 epsilon or more away from zero
	static const GBOOL NotAlmostZero(const GR32 in_value);
	static const GBOOL NotAlmostZero(const GR32 in_value, const GR32 in_epsilon);

	static const GS32 Abs(const GS32 in_value);
	static const GR32 Abs(const GR32 in_value);
	static const GR32 SquareRoot(const GR32 in_value);
	static const GR32 Square(const GR32 in_value);
	static const GR32 Cos(const GR32 in_angleRad);
	static const GR32 Sin(const GR32 in_angleRad);
	static const GR32 Tan(const GR32 in_angleRad);
	//return angle radian
	static const GR32 ATan(const GR32 in_rize, const GR32 in_run);
	static const GR32 ACos(const GR32 in_ratio);
	static const GR32 ASin(const GR32 in_ratio);
	static const GR32 Log10(const GR32 in_x); //10^y = x , log10(x) = y
	static const GR32 Pow(const GR32 in_x, const GR32 in_y); //x^y

	static const GR32 Minimum(const GR32 in_valueLhs, const GR32 in_valueRhs);
	static const GS32 Minimum(const GS32 in_valueLhs, const GS32 in_valueRhs);
	static const GU32 Minimum(const GU32 in_valueLhs, const GU32 in_valueRhs);

	static const GR32 Maximum(const GR32 in_valueLhs, const GR32 in_valueRhs);
	static const GS32 Maximum(const GS32 in_valueLhs, const GS32 in_valueRhs);
	static const GU32 Maximum(const GU32 in_valueLhs, const GU32 in_valueRhs);

	static const GR32 Clamp(const GR32 in_value, const GR32 in_boundLow, const GR32 in_boundHigh);
	static const GS32 Clamp(const GS32 in_value, const GS32 in_boundLow, const GS32 in_boundHigh);

	static const GR32 GetRatio(
		const GR32 in_value,
		const GR32 in_rangeLow,
		const GR32 in_rangeHigh
		);
	static const GR32 Lerp(
		const GR32 in_ratio,
		const GR32 in_valueLow,
		const GR32 in_valueHigh
		);
	static const GS32 Lerp(
		const GR32 in_ratio,
		const GS32 in_valueLow,
		const GS32 in_valueHigh
		);

	static const GR32 ClampRadian(const GR32 in_radianValue);

	//3x^2-2x^3
	static const GR32 RatioEaseInOut(const GR32 in_ratio);
	//d/dx(3x^2-2x^3) = 6x-6x^2
	static const GR32 RatioEaseInOutRateOfChange(const GR32 in_ratio);

	static void RandomSeed(const GU32 in_seed);
	static const GS32 RandomInt();
	//return [0.0 ... 1.0] (inclusive of upper limit)
	static const GR32 RandomUnit();
	static const GR32 RandomUnitPlusMinius();

	static const GR32 SquareRootAprox(const GR32 in_value); //+- 3.5%
	static const GR32 InvertSquareRootAprox(const GR32 in_value);

	//return the smallest number equal or greator to in_value which is a multiple of given divisor
	static const GS32 Pad(const GS32 in_value, const GS32 in_divisor);

	static const GBOOL QuadraticRoots(
		GR32& out_rootA, 
		GR32& out_rootB, 
		const GR32 in_constantA,
		const GR32 in_constantB,
		const GR32 in_constantC
		);

	static const GU32 NextPow2(const GU32 in_value);

};

#endif //_gmath_h_