//
//  gfilesystem.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gfilesystem.h"
#include "gstringutility.h"
#include "gbuffer.h"

#ifdef IPHONE
	#include "giphonefilesystem.h"
#elif WIN32
	#include "gwin32filesystem.h"
#endif


//constructor
GFileSystem::GFileSystem()
: mFileSystemImplementation()
{
	mFileSystemImplementation.reset(new GFileSystemImplementation());
	return;
}

GFileSystem::~GFileSystem()
{
	return;
}

//public methods
const GBOOL GFileSystem::LoadFile(
	GBuffer& out_buffer,
	const std::string& in_fileNamePath,
	const GBOOL in_bundelRelativePath
	)
{
	if (mFileSystemImplementation)
	{
		const GBOOL result = mFileSystemImplementation->LoadFile(out_buffer, in_fileNamePath, in_bundelRelativePath);
		return result;
	}
	return GFALSE;
}

const GBOOL GFileSystem::SaveFile(
	const GBuffer& in_buffer,
	const std::string& in_fileNamePath
	)
{
	if (mFileSystemImplementation)
	{
		return mFileSystemImplementation->SaveFile(in_buffer, in_fileNamePath);
	}
	return GFALSE;
}

const std::string& GFileSystem::GetWritableDir()const
{
	if (mFileSystemImplementation)
	{
		return mFileSystemImplementation->GetWritableDir();
	}
	return GStringUtility::sEmpty;
}

const std::string GFileSystem::AppendFileToPathName(const std::string& in_path, const std::string& in_fileName)
{
	if (mFileSystemImplementation)
	{
		return mFileSystemImplementation->AppendFileToPathName(in_path, in_fileName);
	}
	return GStringUtility::sEmpty;
}
