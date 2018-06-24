#include "Main.h"

#include "PackScene.h"

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <tinyxml.h>
#include <TXml.h>
#include <TPackerMessage.h>

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

	return;
}

Main::~Main()
{
	mMessage->Print();
	return;
}

const int Main::Action(
	const std::string& in_sourceXml, 
	const std::string& in_destFile
	)
{
	mMessage->AddMessage(std::string("ScenePacker::Run(") + in_sourceXml + ", " + in_destFile + ")");


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
		PackScene::Pack(
			sourceDocument.RootElement(),
			destDocument.RootElement(),
			*mMessage
			);
    }
    catch (...) 
	{
		mMessage->AddError(std::string("PackScene threw error:"), true);
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
		printf( "\nusage:\\ScenePacker.exe src.xml destFileName.xml\nchange Scene files into fudge packer xml source\n" ); 
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
