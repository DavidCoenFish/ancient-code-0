//
//  GGuiNodeComponentText.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentText_h_
#define _GGuiNodeComponentText_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include "GGuiNodeComponentBase.h"
#include "GColour4Float.h"

class GGui;
class GGuiManager;
class GGuiNodeComponentPostLoadData;
class GGuiNodeComponentVisual;
class GGuiFont;
class GMeshManual;
class GColour3Float;

class GGuiNodeComponentText : public GGuiNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNodeComponentBase> TPointerGuiNodeComponentBase;
	typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;

	typedef boost::shared_ptr<GGuiNodeComponentVisual> TPointerGuiNodeComponentVisual;
	typedef boost::weak_ptr<GGuiNodeComponentVisual> TWeakGuiNodeComponentVisual;

	typedef boost::shared_ptr<GGuiFont> TPointerGuiFont;
	typedef boost::weak_ptr<GGuiFont> TWeakGuiFont;

	typedef std::vector<unsigned int> TArrayInt;

	typedef boost::shared_ptr<GMeshManual> TPointerRenderMeshManual;

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
	GGuiNodeComponentText(
		const std::string& in_name,
		const GS32 in_maxCharCount,
		TPointerRenderMeshManual& in_renderMeshManual, //WARN: transferal of ownership
		TWeakGuiNodeComponentVisual& in_guiNodeComponentVisual,
		TWeakGuiFont& in_guiFont,
		const GColour4Float& in_colour,
		const float in_scale,
		const float in_horizontalCharPad,
		const bool in_visible	
		);
	virtual ~GGuiNodeComponentText();

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

	const GColour4Float& GetColour()const{ return mColour; }
	void SetColour(const GColour4Float& in_colour);

	void SetAlpha(const float in_alpha);
	void SetScale(const GR32 in_scale); 

	void SetText(const std::string& in_text); //look for each char in the font
	void SetText(const TArrayInt& in_text); //look for each int in the font

	//private methods
private:
	void SetArrayFontDataIndex(const TArrayInt& in_arrayFontDataIndex);
	void UpdateMesh(GGui& inout_gui, GGuiNode& inout_node);

	//private members
private:
	const std::string mName; //text have names so that buttons can find them
	const GS32 mMaxCharCount;
	int mFlag;
	TPointerRenderMeshManual mRenderMeshManual;
	GColour4Float mColour;
	float mScale;
	float mHorizontalCharPad;
	TWeakGuiNodeComponentVisual mVisual;
	TWeakGuiFont mFont;
	float m_alpha;

	//std::string mText;
	TArrayInt mArrayFontDataIndex;
	int mParentTransformChangeId;

};

#endif 