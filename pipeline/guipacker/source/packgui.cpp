//file: PackGui.cpp

#include "PackGui.h"

#include "main.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <TPackerMessage.h>

#include <GGuiType.h>

#include <math.h>

struct TGuiNodeFlag
{
	enum TEnum
	{
		TNone							= 0x00000000
		,TSquareMinimum					= 0x00000001
		,TSquareMaximum					= 0x00000002
	};
};

struct TGuiNodeComponentPanelLoadFlag
{
	enum TEnum
	{
		TNone		= 0x00,
		TColour1	= 0x01,
		TColour4	= 0x02,
		TUV			= 0x04,
		TVisible	= 0x08
	};
};

struct TGuiNodeComponentTextLoadFlag
{
	enum TEnum
	{
		TNone		= 0x0000,
		TLineWrap	= 0x0001,
		TVisible	= 0x0002,
	};
};

static unsigned char LocalColourValueFromFloat(const float in_value_0_to_1)
{
	const int temp = (int)((in_value_0_to_1 * 255.0F) + 0.5F);
	if (temp < 0)
	{
		return 0;
	}
	if (255 < temp)
	{
		return 255;
	}
	return temp;
}

static void LocalWriteComponentPanel(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const std::string name = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("name").Element());
	TXmlPack::PutString(&elementComponent, name);

	int flag = 0;
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagColour1").Element()))
	{
		flag |= TGuiNodeComponentPanelLoadFlag::TColour1;
	}
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagColour4").Element()))
	{
		flag |= TGuiNodeComponentPanelLoadFlag::TColour4;
	}
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagUV").Element()))
	{
		flag |= TGuiNodeComponentPanelLoadFlag::TUV;
	}
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagVisible").Element()))
	{
		flag |= TGuiNodeComponentPanelLoadFlag::TVisible;
	}

	TiXmlElement elementColour("pointer");
	if (flag & (TGuiNodeComponentPanelLoadFlag::TColour1 | TGuiNodeComponentPanelLoadFlag::TColour4))
	{
		const unsigned char blue0 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("blue0").Element()));
		TXmlPack::PutU8(&elementColour, blue0);
		const unsigned char green0 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("green0").Element()));
		TXmlPack::PutU8(&elementColour, green0);
		const unsigned char red0 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("red0").Element()));
		TXmlPack::PutU8(&elementColour, red0);
		const unsigned char alpha0 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("alpha0").Element()));
		TXmlPack::PutU8(&elementColour, alpha0);
	}
	if (flag & TGuiNodeComponentPanelLoadFlag::TColour4)
	{
		const unsigned char blue1 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("blue1").Element()));
		TXmlPack::PutU8(&elementColour, blue1);
		const unsigned char green1 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("green1").Element()));
		TXmlPack::PutU8(&elementColour, green1);
		const unsigned char red1 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("red1").Element()));
		TXmlPack::PutU8(&elementColour, red1);
		const unsigned char alpha1 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("alpha1").Element()));
		TXmlPack::PutU8(&elementColour, alpha1);

		const unsigned char blue2 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("blue2").Element()));
		TXmlPack::PutU8(&elementColour, blue2);
		const unsigned char green2 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("green2").Element()));
		TXmlPack::PutU8(&elementColour, green2);
		const unsigned char red2 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("red2").Element()));
		TXmlPack::PutU8(&elementColour, red2);
		const unsigned char alpha2 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("alpha2").Element()));
		TXmlPack::PutU8(&elementColour, alpha2);

		const unsigned char blue3 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("blue3").Element()));
		TXmlPack::PutU8(&elementColour, blue3);
		const unsigned char green3 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("green3").Element()));
		TXmlPack::PutU8(&elementColour, green3);
		const unsigned char red3 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("red3").Element()));
		TXmlPack::PutU8(&elementColour, red3);
		const unsigned char alpha3 = LocalColourValueFromFloat(TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("alpha3").Element()));
		TXmlPack::PutU8(&elementColour, alpha3);
	}
	elementComponent.InsertEndChild(elementColour);

	TiXmlElement elementUV("pointer");
	if (flag & TGuiNodeComponentPanelLoadFlag::TUV)
	{
		const float UVLeft = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("UVLeft").Element());
		TXmlPack::PutR32(&elementUV, UVLeft);
		const float UVTop = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("UVTop").Element());
		TXmlPack::PutR32(&elementUV, UVTop);
		const float UVRight = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("UVRight").Element());
		TXmlPack::PutR32(&elementUV, UVRight);
		const float UVBottom = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("UVBottom").Element());
		TXmlPack::PutR32(&elementUV, UVBottom);
	}
	elementComponent.InsertEndChild(elementUV);

	TXmlPack::PutU32(&elementComponent, flag);

	TXmlPack::PutU32(&out_elementComponentArray, GGuiType::TComponentFlag::TPanel);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

/**/
static void LocalWriteComponentText(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	//const float mScale;
	const float scale = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("scale").Element());
	TXmlPack::PutR32(&elementComponent, scale);

	//const float horizontalCharPad;
	const float horizontalCharPad = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("horizontalCharPad").Element());
	TXmlPack::PutR32(&elementComponent, horizontalCharPad);

	//const GColour4Byte mColour;
	const float blue = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("blue").Element());
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(blue));
	const float green = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("green").Element());
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(green));
	const float red = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("red").Element());
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(red));
	const float alpha = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("alpha").Element());
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(alpha));

	//const int mFlag;
	int flag = 0;
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagVisible").Element()))
	{
		flag |= TGuiNodeComponentTextLoadFlag::TVisible;
	}
	TXmlPack::PutU32(&elementComponent, flag);

	//const int mMaximumCharCount;
	const int maximumCharCount = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("maximumCharCount").Element());
	TXmlPack::PutU32(&elementComponent, maximumCharCount);

	//const char* const mName;
	const std::string name = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("name").Element());
	TXmlPack::PutString(&elementComponent, name);

	//const char* const mText;
	const std::string text = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("text").Element());
	TXmlPack::PutString(&elementComponent, text);

	//const char* const mFontName;
	const std::string fontName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("fontName").Element());
	TXmlPack::PutString(&elementComponent, fontName);

	TXmlPack::PutU32(&out_elementComponentArray, GGuiType::TComponentFlag::TText);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

/**/
static void LocalWriteComponentButton(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	//const char* const mName;
	const std::string name = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("name").Element());
	TXmlPack::PutString(&elementComponent, name);

	//<xsd:element name="actionScriptButtonDown" type="xsd:string"/>
	const std::string actionScriptButtonDown = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("actionScriptButtonDown").Element());
	TXmlPack::PutString(&elementComponent, actionScriptButtonDown);

	//<xsd:element name="actionScriptButtonPress" type="xsd:string"/>
	const std::string actionScriptButtonPress = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("actionScriptButtonPress").Element());
	TXmlPack::PutString(&elementComponent, actionScriptButtonPress);

	//<xsd:element name="actionScriptButtonCancel" type="xsd:string"/>
	const std::string actionScriptButtonCancel = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("actionScriptButtonCancel").Element());
	TXmlPack::PutString(&elementComponent, actionScriptButtonCancel);

	//<xsd:element name="actionScriptButtonEnable" type="xsd:string"/>
	const std::string actionScriptButtonEnable = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("actionScriptButtonEnable").Element());
	TXmlPack::PutString(&elementComponent, actionScriptButtonEnable);

	//<xsd:element name="actionScriptButtonDisable" type="xsd:string"/>
	const std::string actionScriptButtonDisable = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("actionScriptButtonDisable").Element());
	TXmlPack::PutString(&elementComponent, actionScriptButtonDisable);

	TXmlPack::PutU32(&out_elementComponentArray, GGuiType::TComponentFlag::TButton);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteNode(
	TiXmlElement& out_elementNodeArray,
	TiXmlElement& in_traceElementNode,
	TPackerMessage& inout_message
	)
{
	//name
	const std::string nodeName = TXml::GetTextString(TiXmlHandle(&in_traceElementNode).FirstChildElement("name").Element());
	TXmlPack::PutString(&out_elementNodeArray, nodeName);

	//flag
	int flag = 0;
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementNode).FirstChildElement("flagSquareMin").Element()))
	{
		flag |= TGuiNodeFlag::TSquareMinimum;
	}
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementNode).FirstChildElement("flagSquareMax").Element()))
	{
		flag |= TGuiNodeFlag::TSquareMaximum;
	}
	TXmlPack::PutU32(&out_elementNodeArray, flag);

	//const GGuiCoordinate mTopLeft;
	const float originRatioX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("originRatioX").Element());
	TXmlPack::PutR32(&out_elementNodeArray, originRatioX);
	const float originRatioY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("originRatioY").Element());
	TXmlPack::PutR32(&out_elementNodeArray, originRatioY);
	const float originOffsetX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("originOffsetX").Element());
	TXmlPack::PutR32(&out_elementNodeArray, originOffsetX);
	const float originOffsetY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("originOffsetY").Element());
	TXmlPack::PutR32(&out_elementNodeArray, originOffsetY);

	//const GGuiCoordinate mSize;
	const float sizeRatioX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("sizeRatioX").Element());
	TXmlPack::PutR32(&out_elementNodeArray, sizeRatioX);
	const float sizeRatioY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("sizeRatioY").Element());
	TXmlPack::PutR32(&out_elementNodeArray, sizeRatioY);
	const float sizeOffsetX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("sizeOffsetX").Element());
	TXmlPack::PutR32(&out_elementNodeArray, sizeOffsetX);
	const float sizeOffsetY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementNode).FirstChildElement("sizeOffsetY").Element());
	TXmlPack::PutR32(&out_elementNodeArray, sizeOffsetY);

	//components
	unsigned int componentNodeCount = 0;
	TiXmlElement elementComponentArray("pointer");
	TiXmlElement* pTraceElementComponent = TiXmlHandle(&in_traceElementNode).FirstChildElement().Element();
	while (pTraceElementComponent)
	{
		std::string type;
		if (pTraceElementComponent->Value())
		{
			type = pTraceElementComponent->Value();
		}

		if (type == "componentPanel")
		{
			componentNodeCount += 1;
			LocalWriteComponentPanel(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentText")
		{
			componentNodeCount += 1;
			LocalWriteComponentText(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentButton")
		{
			componentNodeCount += 1;
			LocalWriteComponentButton(elementComponentArray, *pTraceElementComponent, inout_message);
		}

		pTraceElementComponent = pTraceElementComponent->NextSiblingElement();
	}
	TXmlPack::PutU32(&out_elementNodeArray, componentNodeCount);
	out_elementNodeArray.InsertEndChild(elementComponentArray);

	//child nodes
	unsigned int countNode = 0;
	TiXmlElement elementNodeArray("pointer");
	TiXmlElement* pTraceElementNode = TiXmlHandle(&in_traceElementNode).FirstChildElement("node").Element();
	while (pTraceElementNode)
	{
		countNode += 1;
		LocalWriteNode(
			elementNodeArray,
			*pTraceElementNode,
			inout_message
			);

		pTraceElementNode = pTraceElementNode->NextSiblingElement("node");
	}
	TXmlPack::PutU32(&out_elementNodeArray, countNode);
	out_elementNodeArray.InsertEndChild(elementNodeArray);

	return;
}

/*static*/ const bool PackGui::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	TPackerMessage& inout_message
	)
{
	if ((!in_sourceElement) || (!in_destinationElement))
	{
		return false;
	}

	unsigned int countNode = 0;
	TiXmlElement elementNodeArray("pointer");

	TiXmlElement* pTraceElementNode = TiXmlHandle(in_sourceElement).FirstChildElement("gui").FirstChildElement("node").Element();
	while (pTraceElementNode)
	{
		countNode += 1;
		LocalWriteNode(
			elementNodeArray,
			*pTraceElementNode,
			inout_message
			);

		pTraceElementNode = pTraceElementNode->NextSiblingElement("node");
	}

	//mSceneNodeCount
	TXmlPack::PutU32(in_destinationElement, countNode);
	//mArraySceneNode
	in_destinationElement->InsertEndChild(elementNodeArray);

	return true;
}