#include "Main.h"

#include "PackTexture.h"

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <tinyxml.h>
#include <TXml.h>
#include <TPackerMessage.h>
#include <FreeImage.h>

//public static methods
/*static*/ const int Main::Run(
	const std::string& in_sourceXml, 
	const std::string& in_destinationFileName,
	const std::string& in_dataName,
	const std::string& in_platform
	)
{
	Main object;
	const int result = object.Action(in_sourceXml, in_destinationFileName, in_dataName, in_platform);
	return result;
}

//construction
Main::Main()
	: mMessage()
{
	mMessage.reset(new TPackerMessage);
	FreeImage_Initialise();
	mMessage->AddMessage(std::string("initialised freeImage") + FreeImage_GetVersion());

	return;
}

Main::~Main()
{
	FreeImage_DeInitialise();
	mMessage->Print();
	return;
}

const int Main::Action(
	const std::string& in_sourceXml, 
	const std::string& in_destFile,
	const std::string& in_dataName,
	const std::string& in_platform
	)
{
	mMessage->AddMessage(std::string("TexturePacker::Run(") + in_sourceXml + ", " + in_destFile + ")");


	TiXmlDocument sourceDocument;
	if (sourceDocument.LoadFile(in_sourceXml))
	{
		mMessage->AddMessage(std::string("load src xml:") + in_sourceXml);
	}
	else
	{
		mMessage->AddError(std::string("load src xml:") + in_sourceXml, true);
	}

	TiXmlDocument destDocument;
	TXml::MakeRootElementFudgePacker(destDocument);

    try 
	{
		PackTexture::Pack(
			sourceDocument.RootElement(),
			destDocument.RootElement(),
			in_dataName,
			in_platform,
			*mMessage
			);
    }
    catch (...) 
	{
		mMessage->AddError(std::string("PackTexture threw error:"), true);
    }

	if (destDocument.SaveFile(in_destFile.c_str()))
	{
		mMessage->AddMessage(std::string("save dest xml:") + in_destFile);
	}
	else
	{
		mMessage->AddError(std::string("save dest xml:") + in_destFile);
	}

	const int result = (int)mMessage->GetCritical();
	return result;
}

//application inpoint
int main(int in_argc, char ** in_argv)
{
	int result = 0;
	if (5 != in_argc)
	{
		printf( "\nusage:\\TexturePacker.exe material.xml destFileName.xml dataName platform\nconvert texture dataName with settings from material\n" ); 
	}
	else
	{
		result = Main::Run(in_argv[1], in_argv[2], in_argv[3], in_argv[4]);
	}

#if DSC_DEBUG
	printf( "\n<press enter key to exit>" ); 
	char c; 
	scanf_s("%c", &c); 
#endif

	return result;
}
