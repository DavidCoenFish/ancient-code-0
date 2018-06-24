//
//  gwin32applicationdelegate.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gwin32applicationdelegate_H_
#define _gwin32applicationdelegate_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class GApplicationTask;

class GWin32ApplicationDelegate : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GApplicationTask> TPointerApplicationTask;
	typedef TPointerApplicationTask (*FApplicationTaskFactory)();

	///////////////////////////////////////////////////////
	//static methods
public:
	static void Run(const FApplicationTaskFactory in_applicationTaskFactory);

	//IPhone has a UIApplicationMain that owns and calls methods on the  delegate, on win32 allow singelton access?
	static GWin32ApplicationDelegate* const GetInstance(){ return sInstance; }

	///////////////////////////////////////////////////////
	//constructor	
private:
	GWin32ApplicationDelegate(const FApplicationTaskFactory in_applicationTaskFactory);
	~GWin32ApplicationDelegate();

	///////////////////////////////////////////////////////
	//public methods
public:
	//allow mimic of IPhone app delegate, though creation/ destruction handled differently?
	void ApplicationWillResignActive();
	void ApplicationDidBecomeActive();
	void ApplicationWillTerminate();

	///////////////////////////////////////////////////////
	//private methods
private:
	//allow mimic of IPhone app delegate, though creation/ destruction handled differently?
	void DidFinishLaunchingWithOptions(const FApplicationTaskFactory in_applicationTaskFactory);

	///////////////////////////////////////////////////////
	//private members
private:
	static GWin32ApplicationDelegate* sInstance;

	TPointerApplicationTask mApplicationTask;

};

#endif //#ifndef _gwin32applicationdelegate_H_
