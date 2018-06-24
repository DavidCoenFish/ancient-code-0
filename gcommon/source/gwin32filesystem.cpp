//
//  gwin32filesystem.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gwin32filesystem.h"
#include "gbuffer.h"
#include <windows.h>
#include <stdio.h>
#include <direct.h>

////////////////////////////////////////
// local var
static const GS32 sReadSize = 4096;
static const GS32 sWriteSize = 4096;
static const GS32 sMaxDirLength = 256;

static const GBOOL LocalBufferFromFile(
	GBuffer& out_buffer,
	FILE& in_file
	)
{
	fseek(&in_file, 0, SEEK_END);
	const GS32 size = ftell(&in_file);
	fseek(&in_file, 0, SEEK_SET);

	out_buffer.SetCount(size);

	GBOOL ok = GTRUE;

	GS32 readRemaining = size;
	GS32 trace = 0;
	while (sReadSize < readRemaining)
	{
		if (1 != (GS32)fread((void*)((out_buffer.GetData()) + trace), sReadSize, 1, &in_file))
		{
			ok = GFALSE;
			break;
		}
		readRemaining -= sReadSize;
		trace += sReadSize;
	}

	if (ok && (0 != readRemaining))
	{
		if (1 != (GS32)fread((void*)((out_buffer.GetData()) + trace), readRemaining, 1, &in_file))
		{
			ok = GFALSE;
		}
	}

	return ok;
}

static const GBOOL LocalFileFromBuffer( 
	FILE& inout_file,
	const GBuffer& in_buffer
	)
{
	GS32 writeRemaining = in_buffer.GetCount();
	GS32 trace = 0;
	GBOOL ok = GTRUE;
	while (sWriteSize < writeRemaining)
	{
		if (1 != (GS32)fwrite((void*)(in_buffer.GetData() + trace), sWriteSize, 1, &inout_file))
		{
			ok = GFALSE;
			break;
		}
		writeRemaining -= sWriteSize;
		trace += sWriteSize;
	}

	if (ok && (0 != writeRemaining))
	{
		if (1 != (GS32)fwrite((void*)(in_buffer.GetData() + trace), writeRemaining, 1, &inout_file))
		{
			ok = GFALSE;
		}
	}

	return ok;
}

////////////////////////////////////////
//constructor
GWin32FileSystem::GWin32FileSystem()
{
	GCHAR text[sMaxDirLength];
	mDocumentDir = _getcwd(text, sMaxDirLength);
	mDocumentDir += "/";
	return;
}

GWin32FileSystem::~GWin32FileSystem()
{
	return;
}

////////////////////////////////////////
//public static methods
const GBOOL GWin32FileSystem::LoadFile(
	GBuffer& out_buffer,
	const std::string& in_fileNamePath,
	const GBOOL
	)
{
	FILE* pFile = 0;
	fopen_s(&pFile, in_fileNamePath.c_str(), "rb");

	if(!pFile)
	{
		return GFALSE;
	}

	const GBOOL ok = LocalBufferFromFile(out_buffer, *pFile);

	fclose(pFile);

	return ok;
}

const GBOOL GWin32FileSystem::SaveFile(
	const GBuffer& in_buffer,
	const std::string& in_fileNamePath
	)
{
	FILE* pFile = 0;
	fopen_s(&pFile, in_fileNamePath.c_str(), "wb");

	if (!pFile)
	{
		return GFALSE;
	}

	const GBOOL ok = LocalFileFromBuffer(*pFile, in_buffer);

	fclose(pFile);

	return ok;
}

const std::string GWin32FileSystem::AppendFileToPathName(const std::string& in_path, const std::string& in_fileName)
{
	return (in_path + in_fileName);
}
