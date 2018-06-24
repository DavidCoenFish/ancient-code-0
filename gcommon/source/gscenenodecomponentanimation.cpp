//
//  GSceneNodeComponentAnimation.cpp
//
//  Created by David Coen on 10/10/05.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentAnimation.h"

#include "GSceneNodeComponentAnimationLoad.h"
#include "GScene.h"
#include "GSceneType.h"
#include "GSceneNode.h"
#include "GFileManager.h"
#include "GResourceManager.h"
#include "GScenePostLoadCallback.h"
#include "GAnimationManagerInstance.h"
#include "GSceneNodeComponentAnimationAction.h"
#include "GAssert.h"
#include "GSceneNodeComponentVisual.h"
#include "GMeshInstance.h"
#include "GSkeletonInstance.h"
#include "GAnimationPlayParam.h"

typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;
typedef std::map<std::string, TWeakSkeletonInstance> TMapStringSkeletonInstance;

//static public methods
/*static*/ GSceneNodeComponentAnimation::TPointerSceneNodeComponentBase GSceneNodeComponentAnimation::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const void* const in_componentData
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_componentData)
	{
		return pointer;
	}
	const GSceneNodeComponentAnimationLoad& componentLoad = *((GSceneNodeComponentAnimationLoad*)in_componentData);

	TWeakFile animationManagerAsset = inout_scene.GetResourceManager()->GetFileManager().LoadManagedFile(
		componentLoad.GetAnimationAssetFileName(),
		true,
		true
		);

	TPointerFile pointerAsset = animationManagerAsset.lock();
	if (!pointerAsset)
	{
		return pointer;
	}

	TPointerAnimationManagerInstance pointerAnimationManagerInstance;
	pointerAnimationManagerInstance.reset(new GAnimationManagerInstance(pointerAsset));

	if (componentLoad.GetDefaultAnimationName())
	{
		GAnimationPlayParam playParam(1.0F, 0.0F);
		pointerAnimationManagerInstance->PlayAnimation(componentLoad.GetDefaultAnimationName(), playParam);
	}

	pointer.reset(new GSceneNodeComponentAnimation(
		pointerAnimationManagerInstance
		));


	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		PostLoadCallback	
		));

	return pointer;
}

/*static*/ void GSceneNodeComponentAnimation::PostLoadCallback(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentAnimation pointer = boost::dynamic_pointer_cast<GSceneNodeComponentAnimation>(inout_component);
	if (pointer)
	{
		pointer->AttachToNode(inout_scene, inout_owner);
	}
	return;
}

/*static*/ const unsigned int GSceneNodeComponentAnimation::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TAnimation;
}

//constructor
GSceneNodeComponentAnimation::GSceneNodeComponentAnimation(
	TPointerAnimationManagerInstance in_animationManagerInstance
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mAnimationManagerInstance(in_animationManagerInstance)
	, mArrayAction()
	, mArraySkeletonInstance()
{
	return;
}

/*virtual*/ GSceneNodeComponentAnimation::~GSceneNodeComponentAnimation()
{
	return;
}

//public methods
void GSceneNodeComponentAnimation::Tick(TPointerSceneNode& inout_sceneNode, const float in_timeDelta)
{
	if (mAnimationManagerInstance)
	{
		mAnimationManagerInstance->Tick(in_timeDelta);
	}
	for (TArrayPointerSceneNodeComponentAnimationAction::const_iterator iterator = mArrayAction.begin(); iterator != mArrayAction.end(); ++iterator)
	{
		(*iterator)->Apply(
			*inout_sceneNode,
			mAnimationManagerInstance,
			in_timeDelta
			);
	}

	return;
}

const int GSceneNodeComponentAnimation::GetSkeletonCount()const
{
	return mArraySkeletonInstance.size();
}

GSceneNodeComponentAnimation::TPointerSkeletonInstance& GSceneNodeComponentAnimation::GetSkeleton(const int in_skeletonIndex)
{
	return mArraySkeletonInstance[in_skeletonIndex];
}


const bool GSceneNodeComponentAnimation::AddBoneCallback(
	const int in_boneIndex,
	TPointerSkeletonInstance& inout_skeletonInstance,
	const TBoneUpdateCallback in_boneUpdateCallback,
	void* const in_callbackData
	)
{
	for (TArrayPointerSceneNodeComponentAnimationAction::iterator iterator = mArrayAction.begin(); iterator != mArrayAction.end(); ++iterator)
	{
		GSceneNodeComponentAnimationAction& action = *(*iterator);
		if (!action.TargetIsBone(in_boneIndex, inout_skeletonInstance))
		{
			continue;
		}

		action.AddBoneUpdateCallback(in_boneUpdateCallback, in_callbackData);
		return true;
	}

	return false;
}

//private methods
class GSceneNodeComponentAnimationVisitor : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
	typedef std::vector<TPointerSkeletonInstance> TArrayPointerSkeletonInstance;
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::shared_ptr<GMeshInstance> TPointerMeshInstance;
	typedef boost::weak_ptr<GMeshInstance> TWeakMeshInstance;
	typedef boost::shared_ptr<GBuffer> TPointerFile;
	typedef boost::weak_ptr<GBuffer> TWeakFile;

	//constructor
public:
	GSceneNodeComponentAnimationVisitor(	
		GScene& inout_scene,
		TMapStringSkeletonInstance& inout_mapNameSkeletonInstance,
		TArrayPointerSkeletonInstance& inout_arraySkeletonInstance
		)
		: m_scene(inout_scene)
		, m_mapNameSkeletonInstance(inout_mapNameSkeletonInstance)
		, m_arraySkeletonInstance(inout_arraySkeletonInstance)
	{
		return;
	}
	~GSceneNodeComponentAnimationVisitor()
	{
		return;
	}

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(TPointerSceneNode& inout_node)
	{
		const int componentCount = inout_node->GetComponentCount();
		for (int index = 0; index < componentCount; ++index)
		{
			GSceneNodeComponentVisual* const pVisual = inout_node->GetComponent<GSceneNodeComponentVisual>(index);
			if ((!pVisual) || (!pVisual->GetMeshHasModifier()))
			{
				continue;
			}

			TPointerMeshInstance renderMeshInstance = pVisual->GetMeshInstance();
			if (!renderMeshInstance)
			{
				continue;
			}

			const char* const skeletonNameChar = renderMeshInstance->GetSkeletonName();
			if (!skeletonNameChar)
			{
				continue;
			}

			const std::string skeletonName = skeletonNameChar;
			TMapStringSkeletonInstance::iterator found = m_mapNameSkeletonInstance.find(skeletonName);
			if (found != m_mapNameSkeletonInstance.end())
			{
				TPointerSkeletonInstance skeletonInstance = (*found).second.lock();
				renderMeshInstance->SetSkeletonInstance(skeletonInstance);
			}
			else
			{
				TWeakFile skeletonAsset = m_scene.GetResourceManager()->GetFileManager().LoadManagedFile(skeletonName, true, true);
				TPointerFile pointerSkeletonAsset = skeletonAsset.lock();
				if (!pointerSkeletonAsset)
				{
					continue;
				}

				TPointerSkeletonInstance pointerSkeletonInstance;
				pointerSkeletonInstance.reset(new GSkeletonInstance(pointerSkeletonAsset));

				m_arraySkeletonInstance.push_back(pointerSkeletonInstance);
				m_mapNameSkeletonInstance[skeletonName] = pointerSkeletonInstance;

				renderMeshInstance->SetSkeletonInstance(pointerSkeletonInstance);
			}
		}
	}
	void QueryFinish()
	{
		return;
	}

private:
	GScene& m_scene;
	TMapStringSkeletonInstance& m_mapNameSkeletonInstance;
	TArrayPointerSkeletonInstance& m_arraySkeletonInstance;

};


void GSceneNodeComponentAnimation::AttachToNode(GScene& inout_scene, TPointerSceneNode& inout_owner)
{
	//make a skeleton for each mesh

	//TODO recurse down tree from owner node till we hit another component animation. all visuals we hit till then are ours to animate
	TMapStringSkeletonInstance mapNameSkeletonInstance;
	GSceneNodeComponentAnimationVisitor visitor(inout_scene, mapNameSkeletonInstance, mArraySkeletonInstance);
	GSceneNode::QueryAll<GSceneNodeComponentAnimationVisitor>(inout_owner, visitor, GSceneNodeComponentVisual::GetComponentFlag());

	//create an action for each skeleton bone, if action found
	for (TArrayPointerSkeletonInstance::iterator iterator = mArraySkeletonInstance.begin(); iterator != mArraySkeletonInstance.end(); ++iterator)
	{
		GSceneNodeComponentAnimationAction::Factory(
			mArrayAction,
			mAnimationManagerInstance,
			(*iterator)
			);
	}

	return;
}
