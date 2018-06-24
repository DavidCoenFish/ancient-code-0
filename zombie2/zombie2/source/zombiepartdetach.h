//
//  ZombiePartDetach.h
//  Zombie
//
//  Created by David Coen on 2011 02 08
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombiePartDetach_h_
#define _ZombiePartDetach_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <GMatrix16Float.h>

class GSceneNodeComponentBase;
class GSceneNodeComponentVisual;
class GSceneNode;
class GScene;

class ZombiePartDetach : public boost::noncopyable
{
	//typedef
private:
	typedef std::vector<int> TArrayInt;

	typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
	typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;

	typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef std::pair<TWeakSceneNodeComponentVisual, TWeakSceneNode> TPairWeakSceneNodeComponentVisualWeakSceneNode;
	typedef std::vector<TPairWeakSceneNodeComponentVisualWeakSceneNode> TArrayPairWeakSceneNodeComponentVisualWeakSceneNode;

	//constructor
public:
	ZombiePartDetach(
		GScene& inout_scene,
		TPointerSceneNode& inout_sceenRoot, //child from this
		TPointerSceneNode& inout_zombieParentSceneNode //use this transform, and test if things are childed off this node
		);
	~ZombiePartDetach();

	//public methods
public:
	const bool SearchPartIndex(const int in_partIndex)const;

	void AddParts(
		const int in_partIndex,
		TArrayPairWeakSceneNodeComponentVisualWeakSceneNode& inout_arrayVisualComponent
		);

	//void TransferIntoSceneNode(
	//	GSceneNode& inout_node
	//	);

	//const TArrayInt& GetArrayPartIndex()const{ return mArrayPartIndex;}
	//const TArrayPairWeakSceneNodeComponentVisualWeakSceneNode& GetArrayVisualNode()const{ return mArrayVisualNode;}

	//private members
private:
	TWeakSceneNode mWeakSceneNode; //we don't own it, but we do create it, add visuals and children nodes to it
	TWeakSceneNode mWeakSceneNodeZombieParent; //for testing against added visuals
	GMatrix16Float mInvertWorldTransform;
	TArrayInt mArrayPartIndex;

};

#endif //_ZombiePartDetach_h_
