//
//  GMeshInstance.h
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshInstance_h_
#define _GMeshInstance_h_

class GMatrix16Float;

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GBuffer;
class GMeshVertexData;
class GMeshInstanceModifier;
class GMeshInstanceModifierSkin;
class GMeshInstanceModifierMorph;
class GMeshModifierSkinBone;
class GSceneNode;
class GSkeletonInstance;
class GMeshManual;
class GMesh;

/**/
class GMeshInstance : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::weak_ptr<GBuffer> TWeakBuffer;

	typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
	typedef boost::weak_ptr<GMeshManual> TWeakMeshManual;

	typedef boost::shared_ptr<GMeshVertexData> TPointerRenderMeshVertexData;

	typedef boost::shared_ptr<GMeshInstanceModifier> TPointerRenderMeshInstanceModifier;
	typedef std::vector<TPointerRenderMeshInstanceModifier> TArrayPointerRenderMeshInstanceModifier;

	typedef boost::shared_ptr<GMeshInstanceModifierSkin> TPointerRenderMeshInstanceModifierSkin;
	typedef boost::weak_ptr<GMeshInstanceModifierSkin> TWeakRenderMeshInstanceModifierSkin;

	typedef boost::shared_ptr<GMeshInstanceModifierMorph> TPointerRenderMeshInstanceModifierMorph;
	typedef boost::weak_ptr<GMeshInstanceModifierMorph> TWeakRenderMeshInstanceModifierMorph;

	typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;

	//constructor
public:
	GMeshInstance(TPointerBuffer& inout_buffer);
	GMeshInstance(TPointerMeshManual& inout_meshManual);
	~GMeshInstance();

	//public methods
public:
	//morph
	const int GetMorphTargetCount()const;
	const char* const GetMorphTargetName(const int in_morphTargetIndex)const;
	void SetMorphTargetWeight(const int in_morphTargetIndex, const float in_weight);

	//skin
	const char* const GetSkeletonName()const;
	const bool GetHasBone(const std::string& in_boneName);
	//skins can keep a reference to this skeleton instance to find out what bone position are
	void SetSkeletonInstance(TPointerSkeletonInstance& inout_skeletonInstance);

	//tick cloth, jiggle... applies skinning... teleport flag is property of scene node
	void Update(const float in_timeDelta, GSceneNode& in_sceneNode);

	//public accessors
public:
	const void* const GetVertexData()const;

	//private methods
private:
	void Init();
	const GMesh* const GetMesh()const;

	const GMeshModifierSkinBone* const GetSkinBone(
		const int in_boneIndex, 
		int* const out_modifierIndex = NULL,
		int* const out_modifierBoneIndex = NULL
		)const;

	//private members;
private:
	//expecting either a vaild buffer or a renerMeshManual
	TWeakBuffer mBuffer;
	TWeakMeshManual mMeshManual;

	TPointerRenderMeshVertexData mVertexData;

	TArrayPointerRenderMeshInstanceModifier mArrayModifier;

	bool mUseAssetMesh;
	bool mUseRenderMeshManual;

	//shortcut to the skin modifier~ not worth it. want to allow more than one skin modifier under a bone
	//TWeakRenderMeshInstanceModifierSkin mModiferSkin;
};

#endif 