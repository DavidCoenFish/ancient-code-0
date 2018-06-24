//
//  gfilesystem.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GFileSystem_h_
#define _GFileSystem_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
#include "gcommon.h"

class GBuffer;

#ifdef IPHONE
	class GIPhoneFileSystem;
	typedef GIPhoneFileSystem GFileSystemImplementation;

#elif WIN32
	class GWin32FileSystem;
	typedef GWin32FileSystem GFileSystemImplementation;
#endif

/*
	to avoid a data copy on the iphone, could make a GFile - GIphoneFile class to wrap NSData, or change GBuffer to be a NSData
	but for now going with the simpler interface of inout GBuffer
*/
class GFileSystem : public boost::noncopyable
{
	///////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::scoped_ptr<GFileSystemImplementation> TPointerFileSystemImplementation;

	///////////////////////////////////////////////////////
	//constructor
public:
	GFileSystem();
	~GFileSystem();

	///////////////////////////////////////////////////////
	//public methods
public:
	const GBOOL LoadFile(
		GBuffer& out_buffer,
		const std::string& in_fileNamePath,
		const GBOOL in_bundelRelativePath = GTRUE
		);

	const GBOOL SaveFile(
		const GBuffer& in_buffer,
		const std::string& in_fileNamePath
		);

	const std::string& GetWritableDir()const;

	const std::string AppendFileToPathName(const std::string& in_path, const std::string& in_fileName);

	///////////////////////////////////////////////////////
	//private methods
private:
	TPointerFileSystemImplementation mFileSystemImplementation;

};

#endif 