//
//  gfilemanager.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gfilemanager.h"

#include "gbuffer.h"
#include "gbufferutility.h"
#include "gfilesystem.h"

///////////////////////////////////////////////////////
//constructor
GFileManager::GFileManager(TPointerFileSystem& inout_fileSystem)
: mFileSystem(inout_fileSystem)
, mMapStringFile()
{
	return;
}

GFileManager::~GFileManager()
{
	return;
}

///////////////////////////////////////////////////////
//public methods
//Doesn't return ownership ownership
GFileManager::TWeakFile GFileManager::LoadManagedFile(
	const std::string& in_fileName,
	const GBOOL in_bundelRelativePath,
	const GBOOL in_performPointerResolution
	)
{
    if (in_fileName == "")
    {
		return TWeakFile();
    }

	TMapStringFile::iterator iterator = mMapStringFile.find(in_fileName);
	if (iterator != mMapStringFile.end())
	{
		return (*iterator).second;
	}

	TPointerFile pointerFile = LoadUnmanagedFile(
		in_fileName,
		in_bundelRelativePath,
		in_performPointerResolution
		);

	if (GNULL != pointerFile)
	{
		mMapStringFile[in_fileName] = pointerFile;
	}
	TWeakFile weakFile = pointerFile;
	return weakFile;
}

//returns ownership
GFileManager::TPointerFile GFileManager::LoadUnmanagedFile(
	const std::string& in_fileName,
	const GBOOL in_bundelRelativePath,
	const GBOOL in_performPointerResolution
	)
{
	TPointerFile pointerFile;
    if (in_fileName == "")
    {
		return pointerFile;
    }

	TPointerFileSystem fileSystem = mFileSystem.lock();
	if (GNULL == fileSystem)
	{
		return pointerFile;
	}

	pointerFile.reset(new GBuffer());
	GBuffer& buffer = *pointerFile;

	if (GTRUE == fileSystem->LoadFile(
		buffer,
		in_fileName,
		in_bundelRelativePath
		))
	{
		if (in_performPointerResolution)
		{
			GBufferUtility::LoadBufferApplyPointerTable(buffer);
		}

		return pointerFile;
	}

	pointerFile.reset();

	return pointerFile;
}