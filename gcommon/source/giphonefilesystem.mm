//
//  gfilesystem.mm
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphonefilesystem.h"

#include "gstringutility.h"
#include "gbuffer.h"
#import <Foundation/Foundation.h>
#import <Foundation/NSData.h>

//////////////////////////////////////////////////////
//constructor
GIPhoneFileSystem::GIPhoneFileSystem()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	mDocumentDir = [[paths objectAtIndex:0] UTF8String];
	mDocumentDir += "/";
	//[paths release]; asserts latter?

	return;
}

GIPhoneFileSystem::~GIPhoneFileSystem()
{
	return;
}

//////////////////////////////////////////////////////
//public methods
/*
/ a regular file
NSString *plistPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"File.plist"];
 
// this works also for localized files, gets the best matching localized variant
NSBundle *thisBundle = [NSBundle bundleForClass:[self class]]; // alternate method to get bundle
NSString *path = [thisBundle pathForResource:@"File" ofType:@"plist"];



NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
NSString *documentsDirectory = [paths objectAtIndex:0];
NSString *plistPath = [documentsDirectory stringByAppendingPathComponent:@"File.plist"];
*/
const GBOOL GIPhoneFileSystem::LoadFile(
	GBuffer& out_buffer,
	const std::string& in_fileNamePath,
	const GBOOL in_bundelRelativePath
	)
{
	out_buffer.Clear();
	NSString *filePath = NULL;
	
	if (in_bundelRelativePath)
	{
		std::string fileNamePath;
		std::string extention;
		std::string fileName;
		std::string subDir;
		GStringUtility::PathSplitExtention(fileNamePath, extention, in_fileNamePath);
		GStringUtility::PathSplitLeaf(subDir, fileName, fileNamePath);

		filePath = [[NSBundle mainBundle] 
						  pathForResource:[[[NSString alloc] initWithUTF8String:fileName.c_str()] autorelease]
						  ofType:[[[NSString alloc] initWithUTF8String:extention.c_str()] autorelease]
						  inDirectory:[[[NSString alloc] initWithUTF8String:subDir.c_str()] autorelease]
						  ];
	}
	else 
	{
		filePath = [[NSString alloc] initWithUTF8String:in_fileNamePath.c_str()];
	}

	NSData* data = [[NSData alloc] initWithContentsOfFile:filePath];
	if (!data)
	{
		return GFALSE;
	}

	out_buffer.AppendData(
		[data length],
		(const GU8* const)[data bytes]
		);
	[data release];

	return GTRUE;
}

const GBOOL GIPhoneFileSystem::SaveFile(
	const GBuffer& in_buffer,
	const std::string& in_fileNamePath
	)
{
	//NSData* data = [[NSData alloc] initWithBytesNoCopy:((void*)in_buffer.GetData()) length:in_buffer.GetCount()]; takes ownership then asserts?
	NSData* data = [[NSData alloc] initWithBytes:in_buffer.GetData() length:in_buffer.GetCount()];
	[data writeToFile:[[[NSString alloc] initWithUTF8String:in_fileNamePath.c_str()] autorelease]
		   atomically:YES
	 ];
	[data release];
	
	return GTRUE;
}

const std::string GIPhoneFileSystem::AppendFileToPathName(const std::string& in_path, const std::string& in_fileName)
{
    std::string result = [
			[
				[[[NSString alloc] initWithUTF8String:in_path.c_str()] autorelease]
				stringByAppendingPathComponent:
				[[[NSString alloc] initWithUTF8String:in_fileName.c_str()] autorelease]
			] 
		UTF8String
		];
	
	return result;
}
