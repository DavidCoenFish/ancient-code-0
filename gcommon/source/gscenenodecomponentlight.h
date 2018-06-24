//
//  GSceneNodeComponentLight.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentLight_h_
#define _GSceneNodeComponentLight_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GSceneNodeComponentBase.h"
#include "GColour4Float.h"

class GScene;
class GSceneNode;
class GBuffer;
class GScenePostLoadCallback;
//class GRenderLight;
class GMatrix16Float;
class GMatrix9Float;
class GVector3Float;

/**/
class GSceneNodeComponentLight : public GSceneNodeComponentBase
{
	//typedef
private:
	struct TType
	{
		enum TEnum
		{
			TPoint = 0,
			TDirect,
			TSpot,

			TCount
		};
	};
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
	//typedef boost::shared_ptr<GRenderLight> TPointerRenderLight;

	//static public methods
public:
	//different code path adds components to node/ scene. may want to create without 'activation'
	//generic interface for generalised loading. return base rather than type pointer due to generic interface
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		GSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const unsigned char* const in_memory, //the start of the object in memory, eg. somewhere from loadBuffer
		const GBuffer& in_loadBuffer //the rest of the load buffer to extract other poiners
		);
	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentLight(
		const float in_radius,
		const TType::TEnum in_type,
		const GColour4Float& in_diffuseLight,
		const float in_attenuationConstant,
		const float in_attenuationLinear,
		const float in_attenuationQuadratic,
		const float in_spotExponent,
		const float in_spotCutoff
		);
	virtual ~GSceneNodeComponentLight();

	//public accessors
public:
	const float GetRadius()const{ return mRadius; }

	//TPointerRenderLight CreateRenderLight(
	//	const GMatrix16Float& in_parentNodeMatrix,
	//	const GMatrix9Float& in_recieverInvertRotation,
	//	const GVector3Float& in_recieverPosition
	//	)const;

	//private members;
private:
	float mRadius;
	TType::TEnum mType;
	GColour4Float mDiffuseLight;
	float mAttenuationConstant;
	float mAttenuationLinear;
	float mAttenuationQuadratic;
	float mSpotExponent;
	float mSpotCutoff;

};

#endif 