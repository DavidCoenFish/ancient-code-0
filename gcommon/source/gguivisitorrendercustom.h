//
//  GGuiVisitorRenderCustom.h
//
//  Created by David Coen on 2011 04 14
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiVisitorRenderCustom_h_
#define _GGuiVisitorRenderCustom_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GGui;
class GGuiManager;
class GRender;
class GGuiNode;
class GMatrix16Float;

class GGuiVisitorRenderCustom : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNode> TPointerGuiNode;


	//public static methods
public:
	static void Run(
		GGui& inout_Gui,
		GRender& inout_renderer, 
		const GMatrix16Float& in_matrix
		);

	//constructor
private:
	GGuiVisitorRenderCustom(
		GRender& inout_renderer
		);
	~GGuiVisitorRenderCustom();

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