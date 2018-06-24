#include "Main.h"

#include "PackSprite.h"

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
	const std::string& in_destinationFileName
	)
{
	Main object;
	const int result = object.Action(in_sourceXml, in_destinationFileName);
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
	const std::string& in_destFile
	)
{
	mMessage->AddMessage(std::string("PackFont::Run(") + in_sourceXml + ", " + in_destFile + ")");


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
		PackSprite::Pack(
			sourceDocument.RootElement(),
			destDocument.RootElement(),
			*mMessage
			);
    }
    catch (...) 
	{
		mMessage->AddError(std::string("PackFont threw error:"), true);
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
	if (3 != in_argc)
	{
		printf( "\nusage:\\SpritePacker.exe source.xml destination.xml\nconvert sprite description xml to font xml and write image data to a texture\n" ); 
	}
	else
	{
		result = Main::Run(in_argv[1], in_argv[2]);
	}

#if DSC_DEBUG
	printf( "\n<press enter key to exit>" ); 
	char c; 
	scanf_s("%c", &c); 
#endif

	return result;
}


/*
static const bool LocalPackSprite(
	const std::string& in_srcFile,
	const std::string& in_destFile
	)
{
	//load the xml
	TiXmlDocument sourceDocument;
	if (sourceDocument.LoadFile(in_srcFile))
	{
		printf( "\nload src xml: %s\n", in_srcFile.c_str()); 
	}
	else
	{
		printf( "\nFAILED: load src xml: %s\n", in_srcFile.c_str()); 
		return false;
	}

	TiXmlDocument destDocument;
	TXml::MakeRootElementFudgePacker(destDocument);

	PackSprite::Pack(
		sourceDocument.RootElement(),
		destDocument.RootElement()
		);

	if (destDocument.SaveFile(in_destFile.c_str()))
	{
		printf( "\nsave dest xml: %s\n", in_destFile.c_str()); 
	}
	else
	{
		printf( "\nFAILED: save dest xml: %s\n", in_destFile.c_str()); 
	}

	return true;
}

//command line input is (2 xml file paths), or (2 xml and a image path)
int main(int in_argc, char ** in_argv)
{
	bool ok = true;
	if (3 != in_argc)
	{
		printf( "\nusage:\\SpritePacker.exe src.xml destFileName.xml\n\
				change Sprite files into fudge packer xml source\n\
				" ); 
		ok = false;
	}

	if (ok)
	{
		ok &= LocalPackSprite(
			in_argv[1],
			in_argv[2]
		);
	}

	if (ok)
	{
		printf( "\nSpritePacker finished\n" ); 
	}
	else
	{
		printf( "\nSpritePacker finished: ERROR\n" ); 
	}

#if DSC_DEBUG
	printf( "\n<press enter key to exit>" ); 
	char c; 
	scanf_s("%c", &c); 
#endif

	return 0;
}
/**/