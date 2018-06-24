//
//  gapplicationtask.h
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//
#ifndef _gapplicationtask_H_
#define _gapplicationtask_H_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

class GApplicationWindow;

//interface class for application tasks
class GApplicationTask : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::scoped_ptr<GApplicationWindow> TPointerApplicationWindow;

	///////////////////////////////////////////////////////
	//constructor
public:
	GApplicationTask();
	virtual ~GApplicationTask();

	///////////////////////////////////////////////////////
	//public methods
public:
	const GBOOL Tick(const GR32 in_timeDelta);
	void SetActive(const GBOOL in_active);

	///////////////////////////////////////////////////////
	//interface
private:
	virtual const GBOOL OnTick(const GR32 in_timeDelta);
	virtual void OnSetActive(const GBOOL in_active);

	///////////////////////////////////////////////////////
	//private member
private:
	TPointerApplicationWindow mPointerApplicationWindow;

};

#endif //#ifndef _gapplicationtask_H_
