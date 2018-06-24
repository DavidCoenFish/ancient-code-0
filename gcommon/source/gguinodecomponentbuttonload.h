//
//  GGuiNodeComponentButtonLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentButtonLoad_h_
#define _GGuiNodeComponentButtonLoad_h_

#include <boost/noncopyable.hpp>

class GGuiNodeComponentButtonLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiNodeComponentButtonLoad(
		const char* const in_name,
		const char* const in_actionScriptButtonDown,
		const char* const in_actionScriptButtonPress,
		const char* const in_actionScriptButtonCancel,		
		const char* const in_actionScriptButtonEnable,		
		const char* const in_actionScriptButtonDisable		
		);
	~GGuiNodeComponentButtonLoad();

	//public accessors
public:
	const char* const GetName()const{ return mName; } 
	const char* const GetActionScriptButtonDown()const{ return mActionScriptButtonDown; }
	const char* const GetActionScriptButtonPress()const{ return mActionScriptButtonPress; }
	const char* const GetActionScriptButtonCancel()const{ return mActionScriptButtonCancel; }
	const char* const GetActionScriptButtonEnable()const{ return mActionScriptButtonEnable; }
	const char* const GetActionScriptButtonDisable()const{ return mActionScriptButtonDisable; }

	//private members;
private:
	const char* const mName; 
	/*
		action script?
		"""
		SetVisible "text01" true
		SetColourAnim "panel04" 255 255 0 255 0.5
		"""
	*/
	const char* const mActionScriptButtonDown;
	const char* const mActionScriptButtonPress;
	const char* const mActionScriptButtonCancel;
	const char* const mActionScriptButtonEnable;
	const char* const mActionScriptButtonDisable;

};

#endif 