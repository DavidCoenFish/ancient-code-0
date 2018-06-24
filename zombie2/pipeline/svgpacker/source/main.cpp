#include "main.h"

#include "packsvg.h"

#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <tinyxml.h>
#include <TXml.h>
#include <TPackerMessage.h>
#include <GBuffer.h>
#include <GBufferUtility.h>
#include <GFileSystem.h>

//public static methods
/*static*/ const int Main::Run(
	const std::string& in_sourceXml, 
	const std::string& in_destinationFileName,
	const int in_accuracy
	)
{
	Main object;
	const int result = object.Action(in_sourceXml, in_destinationFileName, in_accuracy);
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
	const std::string& in_destFile,
	const int in_accuracy
	)
{
	mMessage->AddMessage(std::string("SvgPacker::Run(") + in_sourceXml + ", " + in_destFile + ")");


	TiXmlDocument sourceDocument;
	if (sourceDocument.LoadFile(in_sourceXml))
	{
		mMessage->AddMessage(std::string("load src xml:") + in_sourceXml);
	}
	else
	{
		mMessage->AddError(std::string("load src xml:") + in_sourceXml, true);
	}

	//TiXmlDocument destDocument;
	//TiXmlElement rootElement("root");
	//destDocument.InsertEndChild(rootElement);

	std::string output;

    try 
	{
		PackSvg::Pack(
			sourceDocument.RootElement(),
			output,
			//destDocument.RootElement(),
			*mMessage,
			in_accuracy
			);
    }
    catch (...) 
	{
		mMessage->AddError(std::string("SvgPacker threw error:"), true);
    }

	//if (destDocument.SaveFile(in_destFile.c_str()))
	//{
	//	mMessage->AddMessage(std::string("save dest xml:") + in_destFile);
	//}
	//else
	//{
	//	mMessage->AddError(std::string("save dest xml:") + in_destFile);
	//}

	GBuffer buffer;
	GBufferUtility::SetString(buffer, output);
	GFileSystem fileSystem;
	if (fileSystem.SaveFile(buffer, in_destFile.c_str()))
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
	if ((3 != in_argc) && (4 != in_argc))
	{
		printf( "\nusage:\\SvgPacker.exe src.xml destFileName.xml\n" ); 
	}
	else
	{
		int accuracy = -1;
		if (4 == in_argc)
		{
			sscanf_s(in_argv[3], "%d", &accuracy);
		}
		result = Main::Run(in_argv[1], in_argv[2], accuracy);
	}

#if DSC_DEBUG
	printf( "\n<press enter key to exit>" ); 
	char c; 
	scanf_s("%c", &c); 
#endif

	return result;
}
