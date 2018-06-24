//file: PackMaterial.cpp

#include "PackMaterial.h"

#include "main.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>

#include <GMaterialType.h>

//WARNING: matches enum in engine (GRenderMaterialBasic.cpp)
struct TState //basic
{
	enum TFlag
	{
			TNone				= 0x0000,

			TFaceCW				= 0x0001, //(lightwave is clockwise) meshload pipeline is CW
			TFaceCull			= 0x0002,

			TAlphaBlend			= 0x0004,

			TColourWrite		= 0x0008,

			TDepthRead			= 0x0010,
			//TDepthReadAlway
			TDepthWrite			= 0x0040,

			TStencilShadowFront	= 0x0100,
			TStencilShadowBack	= 0x0200,
			TStencilShadow		= 0x0400,
			////TStencilReadAlway
			//TStencilWriteIncDepth	= 0x0400,
			//TStencilWriteZero	= 0x0800,

			TUseLight			= 0x1000,
			TUseTexture			= 0x2000,
			TDoBasePass		    = 0x4000, //hack: help out opengles1 to let it know what shader ops to mimic
			TDoEnvironment      = 0x8000, //hack: help out opengles1 to let it know what shader ops to mimic

			TPadFlagSize		= 0xFFFFFFFF
	};
};

typedef std::pair<std::string, int> TPairStringInt;
typedef std::vector<TPairStringInt> TArrayPairStringInt;
typedef std::vector<std::string> TArrayString;

static const TArrayPairStringInt& LocalGetBlendModeArrayFlag()
{
	static TArrayPairStringInt sArray;
	if (0 == sArray.size())
	{
		sArray.push_back(TPairStringInt("TZero", GMaterialType::TBlendMode::TZero));
		sArray.push_back(TPairStringInt("TOne", GMaterialType::TBlendMode::TOne));
		sArray.push_back(TPairStringInt("TSrcColor", GMaterialType::TBlendMode::TSrcColor));
		sArray.push_back(TPairStringInt("TOneMinusSrcColor", GMaterialType::TBlendMode::TOneMinusSrcColor));
		sArray.push_back(TPairStringInt("TDstColor", GMaterialType::TBlendMode::TDstColor));
		sArray.push_back(TPairStringInt("TOneMinusDstColor", GMaterialType::TBlendMode::TOneMinusDstColor));
		sArray.push_back(TPairStringInt("TSrcAlpha", GMaterialType::TBlendMode::TSrcAlpha));
		sArray.push_back(TPairStringInt("TOneMinusSrcAlpha", GMaterialType::TBlendMode::TOneMinusSrcAlpha));
		sArray.push_back(TPairStringInt("TDstAlpha", GMaterialType::TBlendMode::TDstAlpha));
		sArray.push_back(TPairStringInt("TOneMinusDstAlpha", GMaterialType::TBlendMode::TOneMinusDstAlpha));
		sArray.push_back(TPairStringInt("TSrcAlphaSaturate", GMaterialType::TBlendMode::TSrcAlphaSaturate));
	}
	return sArray;
}

static void LocalPackMaterial(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement								   
	)
{
	const std::string textureDataName = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("textureDataName").Element());
	const float red = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("red").Element(), 1.0F);
	const float green = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("green").Element(), 1.0F);
	const float blue = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("blue").Element(), 1.0F);
	const float alpha = TXml::GetTextFloat(TiXmlHandle(in_sourceElement).FirstChildElement("alpha").Element(), 1.0F);
	const int blendModeSrc = TCommon::StringToFlag(LocalGetBlendModeArrayFlag(), TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("blendModeSrc").Element()));
	const int blendModeDst = TCommon::StringToFlag(LocalGetBlendModeArrayFlag(), TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("blendModeDst").Element()));

	int state = 0;

	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("faceCW").Element()))
	{
		state |= TState::TFaceCW;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("faceCull").Element()))
	{
		state |= TState::TFaceCull;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("colourWrite").Element()))
	{
		state |= TState::TColourWrite;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("useTexture").Element()))
	{
		state |= TState::TUseTexture;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("alphaBlend").Element()))
	{
		state |= TState::TAlphaBlend;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("depthRead").Element()))
	{
		state |= TState::TDepthRead;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("depthWrite").Element()))
	{
		state |= TState::TDepthWrite;
	}

	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("stencilShadowFront").Element()))
	{
		state |= TState::TStencilShadowFront;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("stencilShadowBack").Element()))
	{
		state |= TState::TStencilShadowBack;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("stencilShadow").Element()))
	{
		state |= TState::TStencilShadow;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("useLighting").Element()))
	{
		state |= TState::TUseLight;
	}
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("doBasePass").Element()))
	{
		state |= TState::TDoBasePass;
	}	
	if (true == TXml::GetTextBool(TiXmlHandle(in_sourceElement).FirstChildElement("doEnvironmentPass").Element()))
	{
		state |= TState::TDoEnvironment;
	}

	//const int environmentUVTile = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("environmentUVTile").Element(), 1);
	//const int environmentUVIndex = TXml::GetTextInt(TiXmlHandle(in_sourceElement).FirstChildElement("environmentUVIndex").Element(), 0);

	const std::string vertexFileName = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("vertexFileName").Element());
	const std::string fragmentFileName = TXml::GetTextString(TiXmlHandle(in_sourceElement).FirstChildElement("fragmentFileName").Element());

	//attributeName
	TArrayString arrayAttributeName;
	TiXmlElement* attributeNameElement = TiXmlHandle(in_sourceElement).FirstChildElement("attributeNameArray").FirstChildElement("item").Element();
	while (attributeNameElement)
	{
		const std::string attributeName = TXml::GetTextString(attributeNameElement);
		arrayAttributeName.push_back(attributeName);

		attributeNameElement = attributeNameElement->NextSiblingElement("item");
	}

	//uniformName
	TArrayString arrayUniformName;
	TiXmlElement* uniformNameElement = TiXmlHandle(in_sourceElement).FirstChildElement("uniformNameArray").FirstChildElement("item").Element();
	while (uniformNameElement)
	{
		const std::string uniformName = TXml::GetTextString(uniformNameElement);
		arrayUniformName.push_back(uniformName);

		uniformNameElement = uniformNameElement->NextSiblingElement("item");
	}

	//write out material-

	//const GColour4Float mColour;
	TXmlPack::PutR32(in_destinationElement, red);
	TXmlPack::PutR32(in_destinationElement, green);
	TXmlPack::PutR32(in_destinationElement, blue);
	TXmlPack::PutR32(in_destinationElement, alpha);

	//const GU32 mAlphaBlendSource;
	TXmlPack::PutU32(in_destinationElement, blendModeSrc);
	//const GU32 mAlphaBlendDestination;
	TXmlPack::PutU32(in_destinationElement, blendModeDst);
	//const GU32 mFlag;
	TXmlPack::PutU32(in_destinationElement, state);

	//const GCHAR* const mTextureDataName;
	TXmlPack::PutString(in_destinationElement, textureDataName);

	//const GCHAR* const mVertexShaderFileName;
	TXmlPack::PutString(in_destinationElement, vertexFileName);
	//const GCHAR* const mFragmentShaderFileName;
	TXmlPack::PutString(in_destinationElement, fragmentFileName);

	//const GU32 mArrayAttributeNameCount;
	TXmlPack::PutU32(in_destinationElement, arrayAttributeName.size());
	//const GCHAR** mArrayAttributeName;
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);
		for (TArrayString::iterator iterator = arrayAttributeName.begin(); iterator != arrayAttributeName.end(); ++iterator)
		{
			TXmlPack::PutString(&arrayElement, (*iterator));
		}
		in_destinationElement->InsertEndChild(arrayElement);
	}

	//const GU32 mArrayUniformNameCount;
	TXmlPack::PutU32(in_destinationElement, arrayUniformName.size());
	//const GCHAR** mArrayUniformName;
	{
		TiXmlElement arrayElement("pointer");
		arrayElement.SetAttribute("alignment", 4);
		for (TArrayString::iterator iterator = arrayUniformName.begin(); iterator != arrayUniformName.end(); ++iterator)
		{
			TXmlPack::PutString(&arrayElement, (*iterator));
		}
		in_destinationElement->InsertEndChild(arrayElement);
	}

	return;
}

/*static*/ const bool PackMaterial::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement
	)
{
	LocalPackMaterial(in_sourceElement, in_destinationElement);

	return true;
}