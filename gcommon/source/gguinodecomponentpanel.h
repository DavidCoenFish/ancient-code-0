//
//  GGuiNodeComponentPanel.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentPanel_h_
#define _GGuiNodeComponentPanel_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include "GGuiNodeComponentBase.h"
#include "GVector2Float.h"

class GGui;
class GGuiManager;
class GGuiNodeComponentPostLoadData;
class GMeshManual;
class GGuiNodeComponentVisual;

/**/
class GGuiNodeComponentPanel : public GGuiNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNodeComponentBase> TPointerGuiNodeComponentBase;
	typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;
	typedef std::vector<float> TArrayFloat;

	typedef boost::shared_ptr<GMeshManual> TPointerRenderMeshManual;

	typedef boost::shared_ptr<GGuiNodeComponentVisual> TPointerGuiNodeComponentVisual;
	typedef boost::weak_ptr<GGuiNodeComponentVisual> TWeakGuiNodeComponentVisual;


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
	GGuiNodeComponentPanel(
		const std::string& in_name,
		TPointerRenderMeshManual& in_renderMeshManual, //WARN: transferal of ownership
		TWeakGuiNodeComponentVisual& in_guiNodeComponentVisual,
		const TArrayFloat& in_arrayAlpha,
		const bool in_visible
		);
	virtual ~GGuiNodeComponentPanel();

	//public methods
public:
	void Update(
		GGui& inout_gui, 
		GGuiNode& inout_node
		);

	//public accessors
public:
	const std::string& GetName()const{ return mName; }

	const bool GetVisible()const;
	void SetVisible(const bool in_visible);

	void SetAlpha(const float in_alpha);

	//private members;
private:
	const std::string mName; //panels have names so that buttons can find them
	int mFlag;
	TPointerRenderMeshManual mRenderMeshManual;
	float mAlpha;
	const TArrayFloat mArrayAlpha; //copy of the vertex colour values, if we have colour, to multiply with set alpha
	TWeakGuiNodeComponentVisual mVisual;

	GVector2Float mOldDeviceTopLeft;
	GVector2Float mOldDeviceBottomRight;
};

#endif 