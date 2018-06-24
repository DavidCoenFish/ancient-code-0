//
//  ZombiePart.h
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombiePart_h_
#define _ZombiePart_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string>

class GSceneNodeComponentVisual;
class GSceneNode;
struct ZombiePartLoad;

class ZombiePart : public boost::noncopyable
{
	//typedef
private:
	typedef std::pair<float, std::string> TPairFloatString;
	typedef std::vector<TPairFloatString> TArrayPairFloatString;

	typedef boost::shared_ptr<GSceneNodeComponentVisual> TPointerSceneNodeComponentVisual;
	typedef boost::weak_ptr<GSceneNodeComponentVisual> TWeakSceneNodeComponentVisual;

	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef std::pair<TWeakSceneNodeComponentVisual, TWeakSceneNode> TPairWeakSceneNodeComponentVisualWeakSceneNode;
	typedef std::vector<TPairWeakSceneNodeComponentVisualWeakSceneNode> TArrayPairWeakSceneNodeComponentVisualWeakSceneNode;
	typedef std::vector<TArrayPairWeakSceneNodeComponentVisualWeakSceneNode> TArrayArrayPairWeakSceneNodeComponentVisualWeakSceneNode;

	//constructor
public:
	ZombiePart(
		const ZombiePartLoad& in_zombiePartLoad,
		const float in_health
		);
	~ZombiePart();

	//public methods
public:
	//return true if there is a state change
	void ModifiyHealth(float& inout_healthDelta);

	//contract with GSceneNode::QueryAll to find visual components
	void QuerySubmitNode(TPointerSceneNode& inout_sceneNode);

	const float GetHealth()const{ return mHealth; }
	void SetHealth(const float in_health);

	//to detach limb, we are going to take over the current visual nodes
	void StealCurrentVisual(TArrayPairWeakSceneNodeComponentVisualWeakSceneNode& out_arrayVisual);

	//private members
private:
	const ZombiePartLoad& mZombiePartLoad;
	float mHealth;
	int mVisualGroup; //current index of mArrayArrayVisual being displayed
	TArrayArrayPairWeakSceneNodeComponentVisualWeakSceneNode mArrayArrayVisual;

};

#endif //_ZombiePart_h_
