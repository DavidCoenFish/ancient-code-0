//
//  gmathmatrix.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmathmatrix_h_
#define _gmathmatrix_h_

#include "gcommon.h"

template< typename IN_TYPE > class GMathMatrix
{
private:
	~GMathMatrix();

public:
	// < m, n > * < n, p > = < m, p >
	static void MatrixMul( 
		const IN_TYPE* const in_lhs, 
		const IN_TYPE* const in_rhs, 
		const GS32 in_m, 
		const GS32 in_n, 
		const GS32 in_p, 
		IN_TYPE* const out_data 
		);

	static const IN_TYPE Determinant2(
		const IN_TYPE in_d0_0,
		const IN_TYPE in_d0_1,
		const IN_TYPE in_d1_0,
		const IN_TYPE in_d1_1
		);
	static const IN_TYPE Determinant3(
		const IN_TYPE in_d0_0,
		const IN_TYPE in_d0_1,
		const IN_TYPE in_d0_2,
		const IN_TYPE in_d1_0,
		const IN_TYPE in_d1_1,
		const IN_TYPE in_d1_2,
		const IN_TYPE in_d2_0,
		const IN_TYPE in_d2_1,
		const IN_TYPE in_d2_2
		);
	static const IN_TYPE Determinant4(
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
		);

	static void Inverse4( 
		IN_TYPE* const out_data,
		const IN_TYPE* const in_src
		);

	static void Inverse3( 
		IN_TYPE* const out_data,
		const IN_TYPE* const in_src
		);

};

#endif //_gmathmatrix_h_