//
//  GGuiManager.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiManager_h_
#define _GGuiManager_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>
#include <string>
#include "GVector2Float.h"
#include "GApplicationWindowType.h"

class GFileManager;
class GResourceManager;
class GGui;
class GGuiLoad;
class GGuiAnimationManager;
class GGuiFontManager;
class GMaterial;
class GMaterialInstance;
class GRender;
class GVector4Float;
class GColour4Float;

/*
what is this mess, should the gui (not the manager) take the screen resize directly?
*/
class GGuiManager : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GFileManager> TPointerFileManager;
	typedef boost::weak_ptr<GFileManager> TWeakFileManager;

	typedef boost::shared_ptr<GResourceManager> TPointerResourceManager;
	typedef boost::weak_ptr<GResourceManager> TWeakResourceManager;

	typedef boost::shared_ptr<GGui> TPointerGui;
	typedef boost::weak_ptr<GGui> TWeakGui;

	typedef std::map<std::string, TPointerGui> TMapStringGui;

	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef boost::shared_ptr<GRender> TPointerRenderComponent;
	typedef boost::weak_ptr<GRender> TWeakRenderComponent;

	typedef boost::scoped_ptr<GGuiAnimationManager> TPointerGuiAnimationManager;
	typedef boost::scoped_ptr<GGuiFontManager> TPointerGuiFontManager;

	//constructor
public:
	GGuiManager(
		TPointerResourceManager& inout_resourceManager,
		const std::string& in_textureName
		);
	~GGuiManager();

	//public methods
public:
	TWeakGui CreateGui(const std::string& in_name, const GGuiLoad& in_guiLoad);
	TWeakGui CreateBlankGui(const std::string& in_name);
	TWeakGui LoadGui(const std::string& in_name);
	TWeakGui FindGui(const std::string& in_name);
	void DestroyGui(const std::string& in_name);
	void DestroyGui(const TWeakGui& in_gui);

	TWeakMaterialInstance& GetMaterialColour(){ return mMaterialColour; }
	TWeakMaterialInstance& GetMaterialColourTexture(){ return mMaterialColourTexture; }

	TPointerGuiAnimationManager& GetAnimationManager(){ return mAnimationManager; }
	TPointerGuiFontManager& GetFontManager(){ return mFontManager; }

	void SetScreen(
		const GVector2Float& in_screenSize,
		const GApplicationWindowType::TOrientation::TEnum in_screenOrientation		
		);


	const int GetSizeChangeIndex()const{ return mSizeChangeIndex; }
	const GVector2Float& GetScreenSize()const{ return mScreenSize; }

	const GVector2Float GetDeviceCoordinate(const GVector2Float& in_location)const;
	const GVector2Float GetInputCoordinate(const GVector2Float& in_location)const;

	void SetGuiColour(const GColour4Float& in_colour); //set the colour (and alpha) of this gui system's material...
	const GApplicationWindowType::TOrientation::TEnum GetScreenOrientation()const{ return mScreenOrientation; }

	//private methods
private:
	//void SetScreenOrientation(const GApplicationWindowType::TOrientation::TEnum in_screenOrientation);

	//private members;
private:
	TWeakResourceManager mResourceManager;

	TMapStringGui mMapNameGui;
	TWeakMaterialInstance mMaterialColour;
	TWeakMaterialInstance mMaterialColourTexture;

	TPointerGuiAnimationManager mAnimationManager;
	TPointerGuiFontManager mFontManager;

	int mSizeChangeIndex;
	GVector2Float mScreenSize;
	GApplicationWindowType::TOrientation::TEnum mScreenOrientation;
	//GVector2Float mDeviceAxisX;
	//GVector2Float mDeviceAxisY;

};

#endif 