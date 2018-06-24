//
//  GGuiVisitorInput.h
//
//  Created by David Coen on 2011 04 22
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiVisitorInput_h_
#define _GGuiVisitorInput_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "GVector2Float.h"

class GGui;
class GGuiNode;
class GInput;
class GGuiNodeComponentButton;

class GGuiVisitorInput : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNodeComponentButton> TPointerGuiNodeComponentButton;
	typedef boost::weak_ptr<GGuiNodeComponentButton> TWeakGuiNodeComponentButton;
	typedef boost::shared_ptr<GGuiNode> TPointerGuiNode;

	//public static methods
public:
	static void Run(
		GGui& inout_gui, 
		const GInput& in_input
		);

	//constructor
private:
	GGuiVisitorInput(
		GGui& inout_gui, 
		const GVector2Float& in_touchLocation,
		GVector2Float& out_foundClickRatio
		);
	~GGuiVisitorInput();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		TPointerGuiNode& inout_node
		);

	TWeakGuiNodeComponentButton& GetWeakFoundButton(){ return mWeakFoundButton; }

private:
	GGui& mGui;
	GVector2Float& m_foundClickRatio;
	const GVector2Float mTouchLocation;
	TWeakGuiNodeComponentButton mWeakFoundButton;
	bool mFound;

};

#endif 