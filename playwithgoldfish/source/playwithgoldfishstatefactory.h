//
//  PlayWithGoldfishStateFactory.h
//  playwithgoldfish
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishStateFactory_h_
#define _PlayWithGoldfishStateFactory_h_

#include <GStateFactory.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GDictionary;
class GState;

class PlayWithGoldfishStateFactory : public GStateFactory
{
	//typedef
private:
	typedef boost::shared_ptr<GState> TPointerState;

	typedef boost::weak_ptr<GDictionary> TWeakDictionary;
	typedef boost::shared_ptr<GDictionary> TPointerDictionary;

	//constructor
public:
	PlayWithGoldfishStateFactory(TPointerDictionary& inout_paramObject);
	virtual ~PlayWithGoldfishStateFactory();

	//implement GStateFactory
private:
	virtual TPointerState OnCreateState(); 

	//private members
private:
	TWeakDictionary mParamObject; //no ownership? weak pointer would be better?

};

#endif //_PlayWithGoldfishStateFactory_h_
