//
//  gmathmatrix.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "GMathMatrix.h"

#include "GMath.h"
#include "GAssert.h"

/*
	< m, n > * < p, m > = < p, n >
*/
template<typename IN_TYPE>
/*static*/ void GMathMatrix<IN_TYPE>::MatrixMul( 
	const IN_TYPE* const in_lhs, 
	const IN_TYPE* const in_rhs, 
	const GS32 in_m, 
	const GS32 in_n, 
	const GS32 in_p, 
	IN_TYPE* const out_data 
	)
{
	GS32 trace = 0;
	IN_TYPE value;

	for (GS32 indexN = 0; indexN < in_n; ++indexN)
	{
		for (GS32 indexP = 0; indexP < in_p; ++indexP)
		{
			//for( r = 0 to m ) sum A[n, r] * B[r, p]
			value = 0;
			for (GS32 indexM = 0; indexM < in_m; ++indexM)
			{
				//[ indexM, indexP ] * [ indexP, indexM ]
				value += (in_lhs[(indexN * in_m) + indexM]) * (in_rhs[(indexM * in_p) + indexP]);
			}

			out_data[trace] = value;
			trace += 1;
		}
	}

	return;
}


template<typename IN_TYPE>
/*static*/ const IN_TYPE GMathMatrix<IN_TYPE>::Determinant2(
	const IN_TYPE in_d0_0, 
	const IN_TYPE in_d0_1, 
	const IN_TYPE in_d1_0, 
	const IN_TYPE in_d1_1
	)
{
	return ((in_d0_0 * in_d1_1) - (in_d0_1 * in_d1_0));
}

template<typename IN_TYPE>
/*static*/ const IN_TYPE GMathMatrix<IN_TYPE>::Determinant3(
	const IN_TYPE in_d0_0,
	const IN_TYPE in_d0_1,
	const IN_TYPE in_d0_2,
	const IN_TYPE in_d1_0,
	const IN_TYPE in_d1_1,
	const IN_TYPE in_d1_2,
	const IN_TYPE in_d2_0,
	const IN_TYPE in_d2_1,
	const IN_TYPE in_d2_2
	)
{
	GR32 value = 0;
	
	value += in_d0_0 * Determinant2(in_d1_1, in_d1_2, in_d2_1, in_d2_2);
	value -= in_d1_0 * Determinant2(in_d0_1, in_d0_2, in_d2_1, in_d2_2);
	value += in_d2_0 * Determinant2(in_d0_1, in_d0_2, in_d1_1, in_d1_2);
	
	return value;
}

template<typename IN_TYPE>
/*static*/ const IN_TYPE GMathMatrix<IN_TYPE>::Determinant4(
	const IN_TYPE in_d0_0,
	const IN_TYPE in_d0_1,
	const IN_TYPE in_d0_2,
	const IN_TYPE in_d0_3,
	const IN_TYPE in_d1_0,
	const IN_TYPE in_d1_1,
	const IN_TYPE in_d1_2,
	const IN_TYPE in_d1_3,
	const IN_TYPE in_d2_0,
	const IN_TYPE in_d2_1,
	const IN_TYPE in_d2_2,
	const IN_TYPE in_d2_3,
	const IN_TYPE in_d3_0,
	const IN_TYPE in_d3_1,
	const IN_TYPE in_d3_2,
	const IN_TYPE in_d3_3
	)
{
	GR32 value = 0.0F;
	
	value += in_d0_0 * Determinant3(in_d1_1, in_d1_2, in_d1_3, 
									in_d2_1, in_d2_2, in_d2_3, 
									in_d3_1, in_d3_2, in_d3_3);
	value -= in_d1_0 * Determinant3(in_d0_1, in_d0_2, in_d0_3, 
									in_d2_1, in_d2_2, in_d2_3, 
									in_d3_1, in_d3_2, in_d3_3);
	value += in_d2_0 * Determinant3(in_d0_1, in_d0_2, in_d0_3,
									in_d1_1, in_d1_2, in_d1_3, 
									in_d3_1, in_d3_2, in_d3_3);
	value -= in_d3_0 * Determinant3(in_d0_1, in_d0_2, in_d0_3,
									in_d1_1, in_d1_2, in_d1_3,
									in_d2_1, in_d2_2, in_d2_3);
	
	return value;
}

template<typename IN_TYPE>
/*static*/ void GMathMatrix<IN_TYPE>::Inverse4( 
	IN_TYPE* const out_data,
	const IN_TYPE* const in_src
	)
{
	GASSERT(out_data != in_src, "Don't have src and dest the same");

	const IN_TYPE determinant = Determinant4(in_src[0], in_src[1], in_src[2], in_src[3], 
											 in_src[4], in_src[5], in_src[6], in_src[7],
											 in_src[8], in_src[9], in_src[10], in_src[11],
											 in_src[12], in_src[13], in_src[14], in_src[15]);
	if (GMath::AlmostZero(determinant))
	{
		//out_value = HMatrixUtility::IdentityGet< TMatrix >();
		return;
	}
	const IN_TYPE invertedDeterminant = ((IN_TYPE)1) / determinant;

	for (GS32 x = 0; x < 4; ++x)
	{
		for (GS32 y = 0; y < 4; ++y)
		{
			const GS32 xGT0 = ( x <= 0) ? 1 : 0;
			const GS32 xGT1 = ( x <= 1) ? 1 : 0;
			const GS32 xGT2 = ( x <= 2) ? 1 : 0;
			const GS32 yGT0 = ( y <= 0) ? 4 : 0;
			const GS32 yGT1 = ( y <= 1) ? 4 : 0;
			const GS32 yGT2 = ( y <= 2) ? 4 : 0;
			const IN_TYPE subDeterminant = Determinant3(
				in_src[0 + xGT0 + yGT0], in_src[1 + xGT1 + yGT0], in_src[2 + xGT2 + yGT0], 
				in_src[4 + xGT0 + yGT1], in_src[5 + xGT1 + yGT1], in_src[6 + xGT2 + yGT1], 
				in_src[8 + xGT0 + yGT2], in_src[9 + xGT1 + yGT2], in_src[10 + xGT2 + yGT2]
				);
			if(( x + y ) & 1)
			{
				out_data[(x * 4) + y] = -( subDeterminant * invertedDeterminant );
			}
			else
			{
				out_data[(x * 4) + y] = ( subDeterminant * invertedDeterminant );
			}
		}
	}

	return;
}

template<typename IN_TYPE>
/*static*/ void GMathMatrix<IN_TYPE>::Inverse3( 
	IN_TYPE* const out_data,
	const IN_TYPE* const in_src
	)
{
	GASSERT(out_data != in_src, "Don't have src and dest the same");

	const IN_TYPE determinant = Determinant3(
		in_src[0], in_src[1], in_src[2],
		in_src[3], in_src[4], in_src[5], 
		in_src[6], in_src[7], in_src[8]
		);

	if (GMath::AlmostZero(determinant))
	{
		//out_value = HMatrixUtility::IdentityGet< TMatrix >();
		return;
	}
	const IN_TYPE invertedDeterminant = ((IN_TYPE)1) / determinant;

	for (GS32 x = 0; x < 3; ++x)
	{
		for (GS32 y = 0; y < 3; ++y)
		{
			const GS32 xGT0 = ( x <= 0) ? 1 : 0;
			const GS32 xGT1 = ( x <= 1) ? 1 : 0;
			const GS32 yGT0 = ( y <= 0) ? 4 : 0;
			const GS32 yGT1 = ( y <= 1) ? 4 : 0;
			const IN_TYPE subDeterminant = Determinant2(
				in_src[0 + xGT0 + yGT0], in_src[1 + xGT1 + yGT0], 
				in_src[2 + xGT0 + yGT1], in_src[3 + xGT1 + yGT1]
				);
			if(( x + y ) & 1)
			{
				out_data[(x * 4) + y] = -( subDeterminant * invertedDeterminant );
			}
			else
			{
				out_data[(x * 4) + y] = ( subDeterminant * invertedDeterminant );
			}
		}
	}

	return;
}

/**/
template class GMathMatrix<GR32>;

/**/