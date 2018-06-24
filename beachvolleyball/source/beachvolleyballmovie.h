//
//  BeachVolleyBallMovie.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 08
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallMovie_h_
#define _BeachVolleyBallMovie_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GCamera.h>
#include <vector>

class BeachVolleyBallState;
class GScene;
class GSceneNode;
class GRenderComponent;

class BeachVolleyBallMovie : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNode> TPointerSceneNode;
	typedef boost::weak_ptr<GSceneNode> TWeakSceneNode;

	typedef std::vector<TPointerSceneNode> TArraySceneNode;

	//constructor
public:
	BeachVolleyBallMovie(
		BeachVolleyBallState& inout_parentState, 
		const float in_cameraAspect
		);
	~BeachVolleyBallMovie();

	//public methods
public:
	void Tick(const float in_timeDelta);
	void Resize(const float in_cameraAspect);

	//public accessors
public:
	GCamera& GetCamera(){ return mCamera; }

	//private methods
private:
	void UpdateCamera();

	//private members
private:
	BeachVolleyBallState& mParentState;
	TWeakSceneNode mSceneNodeRoot;
	TWeakSceneNode mSceneNodeGirl;
	GCamera mCamera;
	float mTime;
	float mGirlCountdown;
	float mStartCountDown;
	int mGirl;
	
	TArraySceneNode mArraySceneNode;

};

#endif //_BeachVolleyBallMovie_h_
