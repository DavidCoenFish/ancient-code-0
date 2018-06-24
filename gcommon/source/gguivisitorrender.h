//
//  GGuiVisitorRender.h
//
//  Created by David Coen on 2011 04 14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiVisitorRender_h_
#define _GGuiVisitorRender_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GGui;
class GGuiManager;
class GRender;
class GGuiNode;

class GGuiVisitorRender : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNode> TPointerGuiNode;

	//public static methods
public:
	static void Run(
		GGui& inout_Gui,
		GRender& inout_renderer
		);

	//constructor
private:
	GGuiVisitorRender(
		GRender& inout_renderer
		);
	~GGuiVisitorRender();

	//public methods
public:
	//contract with GGuiNode query
	void QuerySubmitNode(
		TPointerGuiNode& inout_node
		);

	//private members
private:
	GRender& mRender;

};

#endif 