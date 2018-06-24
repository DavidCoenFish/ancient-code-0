//
//  ZombieGuiAmmo.h
//  Zombie
//
//  Created by David Coen on 2011 01 31
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieGuiSplash_h_
#define _ZombieGuiSplash_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <GMaterialInstance.h>

class GGui;
class GGuiManager;
class GRender;

class ZombieGuiSplash : public boost::noncopyable
{
	//constructor
public:
	ZombieGuiSplash(GGuiManager& inout_guiManager);
	~ZombieGuiSplash();

	//public methods
public:
	void Draw(GRender& inout_renderComponent);

	//private members
private:
	boost::weak_ptr<GGui> mSplash;
	boost::weak_ptr<GMaterialInstance> mMaterial;

};

#endif //_ZombieGuiAmmo_h_
