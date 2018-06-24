//
//  gmathutility.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmathutility.h"
#include "gvector3float.h"

template <typename IN_TYPE>
/*static*/ const IN_TYPE GMathUtility<IN_TYPE>::Interpolate(
	const IN_TYPE& in_lhs,
	const IN_TYPE& in_rhs,
	const GR32 in_ratio
	)
{
	const IN_TYPE value = (in_lhs) + ((in_rhs - in_lhs) * in_ratio);
	return value;
}

template<typename IN_TYPE>
/*static*/ const IN_TYPE GMathUtility<IN_TYPE>::Spline(
	const IN_TYPE& in_data0,
	const IN_TYPE& in_data1,
	const IN_TYPE& in_data2,
	const IN_TYPE& in_data3,
	const GR32 in_ratio
	)
{
	const GR32 weight0 = ((in_ratio - 0.5F) * (in_ratio - 0.5F));
	const GR32 weight1 = 1.0F - weight0;

	IN_TYPE value = Interpolate(in_data1, in_data2, in_ratio) * weight1;

	if (in_ratio < 0.5F)
	{
		value += (in_data1 + ((in_data2 - in_data0) * in_ratio)) * weight0;
	}
	else
	{
		value += (in_data2 - ((in_data3 - in_data1) * (1.0F - in_ratio))) * weight0;
	}

	return value;
}


/**/
template class GMathUtility<GR32>;
template class GMathUtility<GVector3Float>;
