//
//  BeachVolleyBallStateFactory.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallStateFactory_h_
#define _BeachVolleyBallStateFactory_h_

#include <GStateFactory.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GDictionary;
class GState;

class BeachVolleyBallStateFactory : public GStateFactory
{
	//typedef
private:
	typedef boost::shared_ptr<GState> TPointerState;

	typedef boost::weak_ptr<GDictionary> TWeakDictionary;
	typedef boost::shared_ptr<GDictionary> TPointerDictionary;

	//constructor
public:
	BeachVolleyBallStateFactory(TPointerDictionary& inout_paramObject);
	virtual ~BeachVolleyBallStateFactory();

	//implement GStateFactory
private:
	virtual TPointerState OnCreateState(); 

	//private members
private:
	TWeakDictionary mParamObject; //no ownership? weak pointer would be better?

};

#endif //_BeachVolleyBallStateFactory_h_
