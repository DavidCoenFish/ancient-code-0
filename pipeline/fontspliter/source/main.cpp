#include "Main.h"

#include "fontspliter.h"

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
	const std::string& in_destinationXml
	)
{
	mMessage->AddMessage(std::string("FontSpliter::Run(") + in_sourceXml + ", " + in_destinationXml + ")");


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
	TXml::MakeRootElement(destDocument, "Schema/fontpacker.xsd");

    try 
	{
		FontSpliter::Pack(
			sourceDocument.RootElement(),
			destDocument.RootElement(),
			in_sourceXml,
			*mMessage
			);
    }
    catch (...) 
	{
		mMessage->AddError(std::string("FontSpliter threw error:"), true);
    }

	if (destDocument.SaveFile(in_destinationXml.c_str()))
	{
		mMessage->AddMessage(std::string("save dest xml:") + in_destinationXml);
	}
	else
	{
		mMessage->AddError(std::string("save dest xml:") + in_destinationXml);
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
		printf( "\nusage:\\FontSpliter.exe source.xml destination.xml\nconvert textures references by source.xml into collection of glyphs with description in destination.xml\n" ); 
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
