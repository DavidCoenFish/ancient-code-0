//
//  GSceneNodeComponentLightCollector.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentLightCollector_h_
#define _GSceneNodeComponentLightCollector_h_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GSceneNodeComponentBase.h"

class GScene;
class GSceneNode;
class GBuffer;
class GScenePostLoadCallback;
//class GRenderLight;

/**/
class GSceneNodeComponentLightCollector : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentLightCollector> TPointerSceneNodeComponentVisual;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;
	//typedef boost::shared_ptr<GRenderLight> TPointerRenderLight;
	//typedef std::vector<TPointerRenderLight> TArrayPointerRenderLight;

	struct TVar
	{
		enum TEnum
		{
			TLightCount = 8
		};
	};

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
	GSceneNodeComponentLightCollector(
		const unsigned int in_updateFrequency,
		const unsigned int in_updateOffset,
		const float in_radius = 0.0F //todo: allow component to increace radius if required
		);
	virtual ~GSceneNodeComponentLightCollector();

	//public methods
public:
	void Update(
		GScene& inout_scene,
		GSceneNode& inout_parentNode, //may need the scene node world matrix
		const unsigned int in_frameId
		);

	//const TArrayPointerRenderLight& GetLightArray()const{ return mArrayLight; }

	//private methods
private:
	const bool NeedsUpdate(const int in_frameId)const;

	//private members;
private:
	const unsigned int mUpdateFrequency;
	const unsigned int mUpdateOffset;
	const float mRadius;

	//TArrayPointerRenderLight mArrayLight;
};

#endif 