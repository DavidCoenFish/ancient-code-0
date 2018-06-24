//
//  gfilesystem.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GIPhoneFileSystem_h_
#define _GIPhoneFileSystem_h_

#include "gcommon.h"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

class GBuffer;

class GIPhoneFileSystem : public boost::noncopyable
{
	//////////////////////////////////////////////////////
	//constructor	
public:
	GIPhoneFileSystem();
	~GIPhoneFileSystem();

	//////////////////////////////////////////////////////
	//public methods
public:
	const GBOOL LoadFile(
		GBuffer& out_buffer,
		const std::string& in_fileNamePath,
		const GBOOL in_bundelRelativePath
		);

	const GBOOL SaveFile(
		const GBuffer& in_buffer,
		const std::string& in_fileNamePath
		);
	
	const std::string& GetWritableDir()const{ return mDocumentDir; }
	const std::string AppendFileToPathName(const std::string& in_path, const std::string& in_fileName);

	//////////////////////////////////////////////////////
	//private members
private:
	std::string mDocumentDir;

};

#endif 