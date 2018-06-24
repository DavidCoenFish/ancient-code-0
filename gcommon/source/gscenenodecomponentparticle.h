//
//  GSceneNodeComponentParticle.h
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentParticle_h_
#define _GSceneNodeComponentParticle_h_

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>
#include "GSceneNodeComponentBase.h"

class GAsset;
class GScene;
class GSceneNode;
class GScenePostLoadCallback;
class GSceneNodeComponentParticleLoad;
class GCamera;
class GVector3Float;
class GSceneNodeComponentVisual;
class GParticleSystemInstance;

/**/
class GSceneNodeComponentParticle : public GSceneNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
	typedef boost::shared_ptr<GSceneNodeComponentParticle> TPointerSceneNodeComponentParticle;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef std::vector<GScenePostLoadCallback> TArrayScenePostLoadCallback;

	typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
	typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;
	typedef boost::shared_ptr<GParticleSystemInstance> TPointerParticleSystemInstance;

	//static public methods
public:
	//different code path adds components to node/ scene. may want to create without 'activation'
	//generic interface for generalised loading. return base rather than type pointer due to generic interface
	static TPointerSceneNodeComponentBase Factory(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
		const void* const in_componentData
		);

	static TPointerSceneNodeComponentBase FactoryManual(
		GScene& inout_scene, //access to mesh/ material manager
		TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
		const std::string& in_materialName,
		const float in_radius,
		const int in_maxParticleCount,
		const int in_maxLineCount,
		const int in_maxEmittorCount,
		const int in_defaultEmittor = -1,
		const float in_defaultEmittorDuration = 0.0F,
		const std::string& in_data = ""
		);

	static const unsigned int GetComponentFlag();

	//constructor
public:
	GSceneNodeComponentParticle(
		const int in_defaultEmittor,
		const float in_defaultEmittorDuration,
		const std::string& in_data,
		TPointerSceneNodeComponentVisual& inout_componentVisual,
		TPointerParticleSystemInstance& inout_particleSystemInstance //WARN: transfer ownership
		);
	virtual ~GSceneNodeComponentParticle();

	//public methods
public:
	void Tick(TPointerSceneNode& in_parent, const float in_timeDelta);
	void VisitorAllCamera(GSceneNode& in_parentSceneNode, GCamera& in_camera);

	void SetVisible(const bool in_visible);
	const int GetActiveEmittorCount()const;

	void RequestParticle(
		const int in_particleIndex, 
		const GVector3Float& in_position,
		const GVector3Float& in_dataOne,
		const GVector3Float& in_dataTwo,
		const float in_lifespan,
		const float in_radiusStart,
		const float in_radiusEnd,
		const float in_alphaStart,
		const float in_alphaEnd
		);

	void RequestEmittor(
		const int in_emittorIndex, 
		const float in_duration //emittor needs 'HasDuration' flag to pay attention to duration
		);

	void StartDefaultEmittor();
	void ClearAllEmittor();
	void ClearAllParticles();

	//public accessors
public:
	const std::string& GetData()const{ return mData; }

	//private members;
private:
	const int mDefaultEmittor;
	const float mDefaultEmittorDuration;
	const std::string mData;

	//scene nodes own the components, but we keep a reference as it is the visual we created to display our manual mesh
	// we also want to query it's visiblity to know if we need to update the mesh
	TWeakSceneNodeComponentVisual mComponentVisual;

	//TPointerParticleSystemInstance mParticleSystemInstance;


};

#endif 