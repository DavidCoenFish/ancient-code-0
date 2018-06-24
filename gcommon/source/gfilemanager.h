//
//  gfilemanager.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gfilemanager_h_
#define _gfilemanager_h_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>
#include <string>

class GBuffer;
class GFileSystem;

/*
	files that are used multiple times can be cached
*/
class GFileManager : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GFileSystem> TPointerFileSystem;
	typedef boost::weak_ptr<GFileSystem> TWeakFileSystem;

	typedef boost::shared_ptr<GBuffer> TPointerFile;
	typedef boost::weak_ptr<GBuffer> TWeakFile;

	typedef std::map<std::string, TPointerFile>TMapStringFile;

	///////////////////////////////////////////////////////
	//constructor
public:
	GFileManager(TPointerFileSystem& inout_fileSystem);
	~GFileManager();

	///////////////////////////////////////////////////////
	//public methods
public:
	//Doesn't return ownership
	TWeakFile LoadManagedFile(
		const std::string& in_fileName,
		const GBOOL in_bundelRelativePath,
		const GBOOL in_performPointerResolution
		);

	//returns ownership
	TPointerFile LoadUnmanagedFile(
		const std::string& in_fileName,
		const GBOOL in_bundelRelativePath,
		const GBOOL in_performPointerResolution
		);

	///////////////////////////////////////////////////////
	//private methods
private:
	TWeakFileSystem mFileSystem;
	TMapStringFile mMapStringFile;


};


#endif //_gfilemanager_h_