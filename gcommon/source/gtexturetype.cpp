//  gtexturetype.h

#include "gtexturetype.h"

/////////////////////////////////////////////////////////////////
//static public
/*static*/ const GU32 GTextureType::MakeFlag(
	const TType::TEnum in_type,
	const TMagnifyFilter::TEnum in_magnifyFilter,
	const TWrapMode::TEnum in_wrapMode,
	const TMipmapMagnifyFilter::TEnum in_mipmapMagnifyFilter,
	const TMipmapMinifyFilter::TEnum in_mipmapMinifyFilter		
	)
{
	GU32 result = 0;

	result |= (in_type & TTypeFlag::TTypeMask);

	switch (in_magnifyFilter)
	{
	default:
		break;
	case TMagnifyFilter::TNearest:
		result |= TTypeFlag::TMagnifyFilterNearest;
		break;
	}

	switch (in_wrapMode)
	{
	default:
		break;
	case TWrapMode::TClamp:
		result |= TTypeFlag::TWrapModeClamp;
		break;
	}

	switch (in_mipmapMagnifyFilter)
	{
	default:
		break;
	case TMipmapMagnifyFilter::TNearest:
		result |= TTypeFlag::TMipmapMagnifyFilterNearest;
		break;
	}

	switch (in_mipmapMinifyFilter)
	{
	default:
		break;
	case TMipmapMinifyFilter::TLinearMipmapLinear:
		result |= TTypeFlag::TMipmapMinifyFilterLinearMipmapLinear;
		break;
	case TMipmapMinifyFilter::TLinearMipmapNearest:
		result |= TTypeFlag::TMipmapMinifyFilterLinearMipmapNearest;
		break;
	case TMipmapMinifyFilter::TNearest:
		result |= TTypeFlag::TMipmapMinifyFilterNearest;
		break;
	case TMipmapMinifyFilter::TNearestMipmapLinear:
		result |= TTypeFlag::TMipmapMinifyFilterNearestMipmapLinear;
		break;
	case TMipmapMinifyFilter::TNearestMipmapNearest:
		result |= TTypeFlag::TMipmapMinifyFilterNearestMipmapNearest;
		break;
	}

	return result;
}

/*static*/ GVOID GTextureType::UnMakeFlag(
	TType::TEnum& out_type,
	TMagnifyFilter::TEnum& out_magnifyFilter,
	TWrapMode::TEnum& out_wrapMode,
	TMipmapMagnifyFilter::TEnum& out_mipmapMagnifyFilter,
	TMipmapMinifyFilter::TEnum& out_mipmapMinifyFilter,
	const GU32 in_flag
	)
{
	out_type = GetTypeFromFlag(in_flag);
	out_magnifyFilter = GetMagnifyFilterFromFlag(in_flag);
	out_wrapMode = GetWrapModeFromFlag(in_flag);
	out_mipmapMagnifyFilter = GetMipmapMagnifyFilterFromFlag(in_flag);
	out_mipmapMinifyFilter = GetMipmapMinifyFilterFromFlag(in_flag);

	return;
}
/*static*/ const GTextureType::TType::TEnum GTextureType::GetTypeFromFlag(const GU32 in_flag)
{
	return (TType::TEnum)(in_flag & TTypeFlag::TTypeMask);
}

/*static*/ const GTextureType::TMagnifyFilter::TEnum GTextureType::GetMagnifyFilterFromFlag(const GU32 in_flag)
{
	if (in_flag & TTypeFlag::TMagnifyFilterNearest)
	{
		return TMagnifyFilter::TNearest;
	}
	return TMagnifyFilter::TLinear;
}

/*static*/ const GTextureType::TWrapMode::TEnum GTextureType::GetWrapModeFromFlag(const GU32 in_flag)
{
	if (in_flag & TTypeFlag::TWrapModeClamp)
	{
		return TWrapMode::TClamp;
	}
	return TWrapMode::TRepeat;
}

/*static*/ const GTextureType::TMipmapMagnifyFilter::TEnum GTextureType::GetMipmapMagnifyFilterFromFlag(const GU32 in_flag)
{
	if (in_flag & TTypeFlag::TMipmapMagnifyFilterNearest)
	{
		return TMipmapMagnifyFilter::TNearest;
	}
	return TMipmapMagnifyFilter::TLinear;
}

/*static*/ const GTextureType::TMipmapMinifyFilter::TEnum GTextureType::GetMipmapMinifyFilterFromFlag(const GU32 in_flag)
{
	if (in_flag & TTypeFlag::TMipmapMinifyFilterLinearMipmapNearest)
	{
		return TMipmapMinifyFilter::TLinearMipmapNearest;
	}
	else if (in_flag & TTypeFlag::TMipmapMinifyFilterLinearMipmapLinear)
	{
		return TMipmapMinifyFilter::TLinearMipmapLinear;
	}
	else if (in_flag & TTypeFlag::TMipmapMinifyFilterNearest)
	{
		return TMipmapMinifyFilter::TNearest;
	}
	else if (in_flag & TTypeFlag::TMipmapMinifyFilterNearestMipmapLinear)
	{
		return TMipmapMinifyFilter::TNearestMipmapLinear;
	}
	else if (in_flag & TTypeFlag::TMipmapMinifyFilterNearestMipmapNearest)
	{
		return TMipmapMinifyFilter::TNearestMipmapNearest;
	}
	return TMipmapMinifyFilter::TLinear;
}
