//
//  GSceneNodeFactory.h
//
//  Created by David Coen on 2010 12 29
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeFactory_h_
#define _GSceneNodeFactory_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>

class GSceneNodeLoad;
class GFileManager;
class GBuffer;

/*
param for manual creation of node, or memory image for load from buffer
*/
class GSceneNodeFactory : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSceneNodeFactory> TPointerSceneNodeFactory;

	typedef std::map<std::string, const GSceneNodeLoad*> TMapStringSceneNodeLoad;
	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::weak_ptr<GBuffer> TWeakBuffer;

	//static public methods
public:
	static TPointerSceneNodeFactory Factory(
		GFileManager& inout_fileManager,
		const std::string& in_factorySceneFileName
		);

	//constructor
public:
	GSceneNodeFactory(TPointerBuffer& inout_sceneLoadAsset);
	~GSceneNodeFactory();

	//public methods
public:
	//can return NULL if not found, or if scene asset not valid
	const GSceneNodeLoad* const GetSceneNodeLoad(const std::string& in_name)const;

	//private members;
private:
	TWeakBuffer mSceneLoadAsset;
	TMapStringSceneNodeLoad mMapNameSceneNodeLoad;

};

#endif 