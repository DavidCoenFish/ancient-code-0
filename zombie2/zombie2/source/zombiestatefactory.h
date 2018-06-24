//
//  ZombieStateFactory.h
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieStateFactory_h_
#define _ZombieStateFactory_h_

#include <GStateFactory.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GDictionary;
class GState;

class ZombieStateFactory : public GStateFactory
{
	//typedef
private:
	typedef boost::shared_ptr<GState> TPointerState;

	typedef boost::weak_ptr<GDictionary> TWeakDictionary;
	typedef boost::shared_ptr<GDictionary> TPointerDictionary;

	//constructor
public:
	ZombieStateFactory(TPointerDictionary& inout_paramObject);
	virtual ~ZombieStateFactory();

	//implement GStateFactory
private:
	virtual TPointerState OnCreateState(); 

	//private members
private:
	TWeakDictionary mParamObject; //no ownership? weak pointer would be better?

};

#endif //_ZombieStateFactory_h_
