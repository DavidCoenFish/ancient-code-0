//
//  GSceneNodeComponentLight.cpp
//
//  Created by David Coen on 30/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentLight.h"

#include "GSceneType.h"
#include "GBufferUtility.h"
#include "GColour4Byte.h"
#include "GColour4Float.h"
#include "GMatrix16Float.h"
#include "GMatrix9Float.h"
#include "GVector3Float.h"
//#include "GRenderLight.h"

struct TComponentLight
{
	const unsigned int mLightTypeStringOffset;
	const float mRadius;
	const unsigned char mDiffuseRed;
	const unsigned char mDiffuseGreen;
	const unsigned char mDiffuseBlue;
	const unsigned char mDiffuseAlpha;
	const float mAttentuationConstant;
	const float mAttentuationLinear;
	const float mAttentuationQuadric;
	const float mSpotExponent;
	const float mSpotCutoff;
};

/*static*/ GSceneNodeComponentLight::TPointerSceneNodeComponentBase GSceneNodeComponentLight::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	GSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const unsigned char* const in_memory, //the start of the object in memory, eg. somewhere from loadBuffer
	const GBuffer& in_loadBuffer //the rest of the load buffer to extract other poiners
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_memory)
	{
		return pointer;
	}

	//const TComponentLight& loadComponentLight = *(const TComponentLight*)in_memory;

	//TType::TEnum type = TType::TCount;
	//const std::string typeName = GBufferUtility::LoadBufferPointerStringGet(in_loadBuffer, loadComponentLight.mLightTypeStringOffset);
	//if (typeName == "point")
	//{
	//	type = TType::TPoint;
	//}
	//else if (typeName == "spotlight")
	//{
	//	type = TType::TSpot;
	//}
	//else if (typeName == "directional")
	//{
	//	type = TType::TDirect;
	//}

	//const GColour4Float diffuseLight = GColour4Float(GColour4Byte(
	//	loadComponentLight.mDiffuseRed,
	//	loadComponentLight.mDiffuseGreen,
	//	loadComponentLight.mDiffuseBlue,
	//	loadComponentLight.mDiffuseAlpha
	//	));

	//pointer.reset(new GSceneNodeComponentLight(
	//	loadComponentLight.mRadius,
	//	type,
	//	diffuseLight,
	//	loadComponentLight.mAttentuationConstant,
	//	loadComponentLight.mAttentuationLinear,
	//	loadComponentLight.mAttentuationQuadric,
	//	loadComponentLight.mSpotExponent,
	//	loadComponentLight.mSpotCutoff
	//	));

	return pointer;
}

/*static*/ const unsigned int GSceneNodeComponentLight::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TLight;
}

//constructor
GSceneNodeComponentLight::GSceneNodeComponentLight(
	const float in_radius,
	const TType::TEnum in_type,
	const GColour4Float& in_diffuseLight,
	const float in_attenuationConstant,
	const float in_attenuationLinear,
	const float in_attenuationQuadratic,
	const float in_spotExponent,
	const float in_spotCutoff
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mRadius(in_radius)
	, mType(in_type)
	, mDiffuseLight(in_diffuseLight)
	, mAttenuationConstant(in_attenuationConstant)
	, mAttenuationLinear(in_attenuationLinear)
	, mAttenuationQuadratic(in_attenuationQuadratic)
	, mSpotExponent(in_spotExponent)
	, mSpotCutoff(in_spotCutoff)
{
	return;
}

/*virtual*/ GSceneNodeComponentLight::~GSceneNodeComponentLight()
{
	return;
}

//public accessors
//GSceneNodeComponentLight::TPointerRenderLight GSceneNodeComponentLight::CreateRenderLight(
//	const GMatrix16Float& in_parentNodeMatrix,
//	const GMatrix9Float& in_recieverInvertRotation,
//	const GVector3Float& in_recieverPosition
//	)const
//{
//	const GVector3Float position = (in_parentNodeMatrix.GetPosition() - in_recieverPosition) * in_recieverInvertRotation;
//	const GVector3Float direction = in_parentNodeMatrix.GetAt() * in_recieverInvertRotation;
//
//	GRenderLight::TType::TEnum lightType = GRenderLight::TType::TCount;
//	switch (mType)
//	{
//	default:
//		break;
//	case TType::TDirect:
//		lightType = GRenderLight::TType::TDirect;
//		break;
//	case TType::TPoint:
//		lightType = GRenderLight::TType::TPoint;
//		break;
//	case TType::TSpot:
//		lightType = GRenderLight::TType::TSpot;
//		break;
//	}
//
//	TPointerRenderLight pointer;
//	pointer.reset(new GRenderLight(
//		lightType,
//		GColour4Float::sWhite,
//		mDiffuseLight,
//		position,
//		mAttenuationConstant,
//		mAttenuationLinear,
//		mAttenuationQuadratic,
//		direction,
//		mSpotExponent,
//		mSpotCutoff
//		));
//
//	return pointer;
//}
