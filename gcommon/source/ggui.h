//
//  GGui.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGui_h_
#define _GGui_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GVector2Float.h"
#include "GGuiNode.h"
#include "GApplicationWindowType.h"

class GAsset;
class GAssetManager;
class GGuiLoad;
class GGuiManager;

/**/
class GGui : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGui> TPointerGui;

	typedef boost::shared_ptr<GGuiNode> TPointerGuiNode;
	typedef boost::weak_ptr<GGuiNode> TWeakGuiNode;

	typedef std::vector<TPointerGuiNode> TArrayPointerGuiNode;

	typedef boost::shared_ptr<GAssetManager> TPointerAssetManager;
	typedef boost::weak_ptr<GAssetManager> TWeakAssetManager;

	//static public methods
public:
	static TPointerGui Factory(
		const GGuiLoad& in_guiLoad,
		GGuiManager& inout_manager
		);

	//constructor
public:
	GGui(GGuiManager& in_parent);
	~GGui();

	//public methods
public:
	//query all, visit all suitable components and all children
	template <typename IN_VISITOR>
	void QueryAll(IN_VISITOR& inout_visitor, const int in_compositeComponentFlag)
	{
		//visit all children
		for (TArrayPointerGuiNode::iterator iterator = mArrayGuiNode.begin(); iterator != mArrayGuiNode.end(); ++iterator)
		{
			GGuiNode::QueryAll<IN_VISITOR>((*iterator), inout_visitor, in_compositeComponentFlag);
		}

		return;
	}

	void Update();

	//public accessors
public:
	const GVector2Float& GetScreenSize()const;
	const GVector2Float CalculateDeviceFromScreenCoordinates(const GVector2Float& in_screenCoordinates);
	const GVector2Float CalculateInputCoordsFromScreenCoordinate(const GVector2Float& in_screenCoordinates);

	void AddGuiNode(TPointerGuiNode& in_guiNode);
	void RemoveGuiNode(TPointerGuiNode& in_guiNode);

	const GApplicationWindowType::TOrientation::TEnum GetScreenOrientation()const;

	//private members;
private:
	TArrayPointerGuiNode mArrayGuiNode;

	GGuiManager& mParent;
	int mSizeChangeIndex;

	////when these change, we mark all gui nodes as dirty
	//GVector2Float mScreenSize;
	//GVector2Float mDeviceCoordinatesTopLeft;
	//GVector2Float mDeviceCoordinatesBottomRight;


};

#endif 