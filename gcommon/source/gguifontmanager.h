//
//  GGuiFontManager.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiFontManager_h_
#define _GGuiFontManager_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

class GResourceManager;
class GGuiFont;

/**/
class GGuiFontManager : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GResourceManager> TPointerResourceManager;
	typedef boost::weak_ptr<GResourceManager> TWeakResourceManager;

	typedef boost::shared_ptr<GGuiFont> TPointerGuiFont;
	typedef boost::weak_ptr<GGuiFont> TWeakGuiFont;

	typedef std::map<std::string, TPointerGuiFont> TMapStringGuiFont;

	//constructor
public:
	GGuiFontManager(TPointerResourceManager& inout_resourceManager);
	~GGuiFontManager();

	//public accessors
public:
	//we keep ownership, everyone else can get a weak ref
	TWeakGuiFont GetFont(const std::string& in_fontFilePath);

	//private members;
private:
	TWeakResourceManager mResourceManager;
	TMapStringGuiFont mMapNameFont;

};

#endif 