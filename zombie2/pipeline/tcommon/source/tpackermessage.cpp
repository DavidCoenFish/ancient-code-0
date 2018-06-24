//file: TPackerMessage.cpp

#include "TPackerMessage.h"
#include "TXml.h"
#include <stdio.h>
#include <tinyxml.h>

// construction
TPackerMessage::TPackerMessage()
: mArrayMessageVebose()
, mArrayErrorCritical()
, mCritical(false)
{
	return;
}

TPackerMessage::~TPackerMessage()
{
	return;
}

// public methods
void TPackerMessage::AddMessage(const std::string& in_message, const bool in_verbose)
{
	mArrayMessageVebose.push_back(TPairStringBool(in_message, in_verbose));
	return;
}

void TPackerMessage::AddError(const std::string& in_error, const bool in_critical)
{
	mArrayErrorCritical.push_back(TPairStringBool(in_error, in_critical));
	mCritical |= in_critical;
	return;
}

void TPackerMessage::Print()const
{
	TiXmlDocument document;
	TXml::MakeRootElement(
		document, 
		"schema/packermessage.xsd"
		);

	TiXmlElement elementMessageList("messageList");
	for (TArrayStringBool::const_iterator iterator = mArrayMessageVebose.begin(); iterator != mArrayMessageVebose.end(); ++iterator)
	{
		TiXmlElement elementMessageItem("item");

		TiXmlElement elementMessage("message");
		TXml::SetTextString(&elementMessage, (*iterator).first);
		elementMessageItem.InsertEndChild(elementMessage);

		TiXmlElement elementVerbose("verbose");	
		TXml::SetTextInt(&elementVerbose, (int)(*iterator).second);
		elementMessageItem.InsertEndChild(elementVerbose);

		elementMessageList.InsertEndChild(elementMessageItem);
	}
	document.RootElement()->InsertEndChild(elementMessageList);

	TiXmlElement elementErrorList("errorList");
	for (TArrayStringBool::const_iterator iterator = mArrayErrorCritical.begin(); iterator != mArrayErrorCritical.end(); ++iterator)
	{
		TiXmlElement elementErrorItem("item");

		TiXmlElement elementError("message");
		TXml::SetTextString(&elementError, (*iterator).first);
		elementErrorItem.InsertEndChild(elementError);

		TiXmlElement elementCritical("critical");	
		TXml::SetTextInt(&elementCritical, (int)(*iterator).second);
		elementErrorItem.InsertEndChild(elementCritical);

		elementErrorList.InsertEndChild(elementErrorItem);
	}
	document.RootElement()->InsertEndChild(elementErrorList);

	document.Print();

	return;
}
