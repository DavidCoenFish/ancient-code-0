//
//  GGuiNodeComponentBase.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentBase_h_
#define _GGuiNodeComponentBase_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GGuiManager;
class GGuiNode;
class GGuiNodeComponentPostLoadData;

class GGuiNodeComponentBase : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GGuiNodeComponentBase> TPointerGuiNodeComponentBase;
	typedef boost::weak_ptr<GGuiNodeComponentBase> TWeakGuiNodeComponentBase;

	typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;

	//constructor
public:
	GGuiNodeComponentBase(const unsigned int in_componentFlag);
	virtual ~GGuiNodeComponentBase();

	//public methods
public:
	void PostLoad(
		const int in_passCount,
		GGuiManager& inout_manager,
		GGuiNode& inout_parent,
		void* const in_data,
		TArrayGuiNodeComponentPostLoadData& inout_arrayComponentBase
		);

	//public accessors
public:
	//ideally this would be virtual, but virtual function can be expencive
	const unsigned int GetComponentFlag()const{ return mComponentFlag; }

	//private interface
private:
	virtual void OnPostLoad(
		const int in_passCount,
		GGuiManager& inout_manager,
		GGuiNode& inout_parent,
		void* const in_data,
		TArrayGuiNodeComponentPostLoadData& inout_arrayComponentBase
		);

	//private members
private:
	const unsigned int mComponentFlag;

};

#endif 