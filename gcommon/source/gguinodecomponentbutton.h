//
//  GGuiNodeComponentButton.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentButton_h_
#define _GGuiNodeComponentButton_h_

#include "GCommon.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include "GGuiNodeComponentBase.h"

class GGuiManager;
class GGuiNodeComponentPostLoadData;
class GGuiNodeComponentAnimation;
class GGuiNodeComponentButtonCallback;
class GVector2Float;

/**/
class GGuiNodeComponentButton : public GGuiNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNodeComponentBase> TPointerGuiNodeComponentBase;
	typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;

	typedef boost::shared_ptr<GGuiNodeComponentAnimation> TPointerGuiNodeComponentAnimation;
	typedef boost::weak_ptr<GGuiNodeComponentAnimation> TWeakGuiNodeComponentAnimation;
	typedef boost::shared_ptr<GGuiNodeComponentButtonCallback> TPointerGuiNodeComponentButtonCallback;

	typedef std::vector<unsigned int> TArrayInt;

	//static public methods
public:
	static TPointerGuiNodeComponentBase Factory(
		const void* const in_data,
		GGuiManager& inout_manager,
		GGuiNode& inout_parent,
		TArrayGuiNodeComponentPostLoadData& inout_arrayPostLoad
		);
	static const unsigned int GetComponentFlag();

	//constructor
public:
	GGuiNodeComponentButton(
		GGuiNode& in_parentNode,
		const std::string& in_name,
		TPointerGuiNodeComponentAnimation& in_componentAnimation
		//const TArrayAnimationAction& in_arrayAnimationActionButtonDown,
		//const TArrayAnimationAction& in_arrayAnimationActionButtonPress,
		//const TArrayAnimationAction& in_arrayAnimationActionButtonCancel
		);
	virtual ~GGuiNodeComponentButton();

	//public methods
public:
	//need GGui if we want to check click drag is still inside node boundary
	void ClickDown(const GVector2Float& in_screenPos, const unsigned int in_clickId, const GVector2Float& in_buttionRelativeRatio);
	void ClickDrag(const GVector2Float& in_screenPos, const unsigned int in_clickId, const GVector2Float& in_buttionRelativeRatio);
	void ClickRelease(const GVector2Float& in_screenPos, const unsigned int in_clickId, const GVector2Float& in_buttionRelativeRatio);

	//public accessors
public:
	void SetCallback(
		TPointerGuiNodeComponentButtonCallback& in_callback //WARNING: transferal ownership
		);
	const std::string& GetName()const{ return mName; }
	const bool GetActive()const{ return m_active; }
	GVOID SetActive(const GBOOL in_active){ m_active = in_active; return; }

	//private methods
private:
	void SetButtonState(const int in_buttonState);

	//private members;
private:
	GGuiNode& mParentNode;
	const std::string mName;
	//const TArrayAnimationAction mArrayAnimationActionButtonDown;
	//const TArrayAnimationAction mArrayAnimationActionButtonPress;
	//const TArrayAnimationAction mArrayAnimationActionButtonCancel;

	TPointerGuiNodeComponentButtonCallback mCallback;
	TWeakGuiNodeComponentAnimation mComponentAnimation;
	int mClickState;

	//keep track of the clicks that are active and inside the button
	TArrayInt mArrayInsideClickId;

	GBOOL m_active;

};

#endif 