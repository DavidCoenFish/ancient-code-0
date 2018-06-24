//
//  gmathutility.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmathutility_h_
#define _gmathutility_h_

#include "gcommon.h"

template< typename IN_TYPE > class GMathUtility
{
private:
	~GMathUtility();

public:
	static const IN_TYPE Interpolate(
		const IN_TYPE& in_lhs,
		const IN_TYPE& in_rhs,
		const GR32 in_ratio
		);

	static const IN_TYPE Spline(
		const IN_TYPE& in_data0,
		const IN_TYPE& in_data1,
		const IN_TYPE& in_data2,
		const IN_TYPE& in_data3,
		const GR32 in_ratio
		);

};

#endif //_gmathutility_h_