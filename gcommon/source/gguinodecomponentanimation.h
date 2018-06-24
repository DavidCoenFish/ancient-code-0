//
//  GGuiNodeComponentAnimation.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentAnimation_h_
#define _GGuiNodeComponentAnimation_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "GGuiNodeComponentBase.h"

class GGuiManager;
class GGuiNodeComponentBase;
class GGuiNode;
class GGuiNodeComponentPostLoadData;

/**/
class GGuiNodeComponentAnimation : public GGuiNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNodeComponentBase> TPointerGuiNodeComponentBase;
	typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;

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
	GGuiNodeComponentAnimation();
	virtual ~GGuiNodeComponentAnimation();

	//implement GGuiNodeComponentBase
private:
	virtual void OnPostLoad(
		const int in_passCount,
		GGuiManager& inout_manager,
		GGuiNode& inout_parent,
		void* const in_data,
		TArrayGuiNodeComponentPostLoadData& inout_arrayComponentBase
		);

	//public methods
public:
	//public accessors
public:
	//private members;
private:
};

#endif 