//file: PackScene.cpp

#include "PackScene.h"

#include "main.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>
#include <TPackerMessage.h>

#include <GSceneType.h> //alternative is to softbind on component name
#include <GSceneNodeComponentPhysicsLoad.h> //enum
#include <GSceneNodeComponentCollisionType.h> //enum
#include <GSceneNodeComponentSpringBoneDataLoad.h> //enum
#include <GSceneNodeComponentVisual.h>

#include <math.h>

static void LocalWriteNode(
	TiXmlElement& out_elementNodeArray,
	TiXmlElement& in_traceElementNode,
	TPackerMessage& inout_message
	);

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

static void LocalWriteComponentAnimation(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const std::string animationSetName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("animationSetName").Element());
	TXmlPack::PutString(&elementComponent, animationSetName);

	const std::string defaultAnimationName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("defaultAnimationName").Element());
	TXmlPack::PutString(&elementComponent, defaultAnimationName);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TAnimation);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentBoneCollisionData(
	TiXmlElement& out_elementArrayData, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	//const unsigned int mBoneNameOffset;
	const std::string boneName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("boneName").Element());
	TXmlPack::PutString(&out_elementArrayData, boneName);

	//const unsigned int mBoneNameOffset;
	const std::string data = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("data").Element());
	TXmlPack::PutString(&out_elementArrayData, data);

	//const float mCapsualOriginX;
	const float startX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("startX").Element());
	TXmlPack::PutR32(&out_elementArrayData, startX);

	//const float mCapsualOriginY;
	const float startY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("startY").Element());
	TXmlPack::PutR32(&out_elementArrayData, startY);

	//const float mCapsualOriginZ;
	const float startZ = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("startZ").Element());
	TXmlPack::PutR32(&out_elementArrayData, startZ);

	const float endX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("endX").Element());
	const float endY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("endY").Element());
	const float endZ = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("endZ").Element());

	float capsualDirectionX = endX - startX;
	float capsualDirectionY = endY - startY;
	float capsualDirectionZ = endZ - startZ;

	const float capsualLength = sqrt((capsualDirectionX * capsualDirectionX) + (capsualDirectionY * capsualDirectionY) + (capsualDirectionZ * capsualDirectionZ));
	if (capsualLength)
	{
		capsualDirectionX /= capsualLength;
		capsualDirectionY /= capsualLength;
		capsualDirectionZ /= capsualLength;
	}

	//const float mCapsualDirectionX;
	TXmlPack::PutR32(&out_elementArrayData, capsualDirectionX);

	//const float mCapsualDirectionY;
	TXmlPack::PutR32(&out_elementArrayData, capsualDirectionY);

	//const float mCapsualDirectionZ;
	TXmlPack::PutR32(&out_elementArrayData, capsualDirectionZ);

	//const float mCapsualLength; //length zero would be a sphere
	TXmlPack::PutR32(&out_elementArrayData, capsualLength);

	//const float mCapsualRadius;
	const float capsualRadius = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("radius").Element());
	TXmlPack::PutR32(&out_elementArrayData, capsualRadius);

	return;
}

static void LocalWriteComponentBoneCollision(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	int boneDataCount = 0;
	TiXmlElement elementDataArray("pointer");

	TiXmlElement* pTraceElementData = TiXmlHandle(&in_traceElementSource).FirstChildElement("capsual").Element();
	while (pTraceElementData)
	{
		boneDataCount += 1;
		LocalWriteComponentBoneCollisionData(
			elementDataArray,
			*pTraceElementData, 
			inout_message
			);

		pTraceElementData = pTraceElementData->NextSiblingElement("capsual");
	}

	TXmlPack::PutU32(&elementComponent, boneDataCount);
	elementComponent.InsertEndChild(elementDataArray);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TBoneCollision);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentClient(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const std::string clientName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("clientName").Element());
	TXmlPack::PutString(&elementComponent, clientName);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TClient);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentCollision(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const int materialId = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("materialId").Element());
	TXmlPack::PutU32(&elementComponent, materialId);

	const std::string data = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("data").Element());
	TXmlPack::PutString(&elementComponent, data);

	int flag = 0;
	const bool flagActive = TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagActive").Element());
	if (flagActive)
	{
		flag = 1;
	}
	TXmlPack::PutU32(&elementComponent, flag);

	int dataCount = 0;
	TiXmlElement elementDataArray("pointer");

	TiXmlElement* pTraceElementData = TiXmlHandle(&in_traceElementSource).FirstChildElement("dataArray").FirstChildElement().Element();
	while (pTraceElementData)
	{
		std::string typeString;
		if (pTraceElementData->Value())
		{
			typeString = pTraceElementData->Value();
		}

		if (typeString == "box")
		{
			dataCount += 1;
			TiXmlElement elementData("pointer");

			const float lowX = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("lowX").Element());
			const float lowY = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("lowY").Element());
			const float lowZ = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("lowZ").Element());
			const float highX = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("highX").Element());
			const float highY = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("highY").Element());
			const float highZ = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("highZ").Element());

			TiXmlElement elementDataLow("pointer");
			TXmlPack::PutR32(&elementDataLow, lowX);
			TXmlPack::PutR32(&elementDataLow, lowY);
			TXmlPack::PutR32(&elementDataLow, lowZ);
			elementData.InsertEndChild(elementDataLow);

			TiXmlElement elementDataHigh("pointer");
			TXmlPack::PutR32(&elementDataHigh, highX);
			TXmlPack::PutR32(&elementDataHigh, highY);
			TXmlPack::PutR32(&elementDataHigh, highZ);
			elementData.InsertEndChild(elementDataHigh);

			TXmlPack::PutU32(&elementDataArray, GSceneNodeComponentCollisionType::TType::TBox);
			elementDataArray.InsertEndChild(elementData);
		}
		else if (typeString == "sphere")
		{
			dataCount += 1;
			TiXmlElement elementData("pointer");

			const float originX = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("originX").Element());
			const float originY = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("originY").Element());
			const float originZ = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("originZ").Element());
			const float radius = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("radius").Element());

			TXmlPack::PutR32(&elementData, radius);

			TiXmlElement elementDataOrigin("pointer");
			TXmlPack::PutR32(&elementDataOrigin, originX);
			TXmlPack::PutR32(&elementDataOrigin, originY);
			TXmlPack::PutR32(&elementDataOrigin, originZ);
			elementData.InsertEndChild(elementDataOrigin);

			TXmlPack::PutU32(&elementDataArray, GSceneNodeComponentCollisionType::TType::TSphere);
			elementDataArray.InsertEndChild(elementData);
		}
		else if (typeString == "capsual")
		{
			dataCount += 1;
			TiXmlElement elementData("pointer");

			const float startX = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("startX").Element());
			const float startY = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("startY").Element());
			const float startZ = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("startZ").Element());
			const float endX = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("endX").Element());
			const float endY = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("endY").Element());
			const float endZ = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("endZ").Element());
			const float radius = TXml::GetTextFloat(TiXmlHandle(pTraceElementData).FirstChildElement("radius").Element());

			TiXmlElement elementDataStart("pointer");
			TXmlPack::PutR32(&elementDataStart, startX);
			TXmlPack::PutR32(&elementDataStart, startY);
			TXmlPack::PutR32(&elementDataStart, startZ);
			elementData.InsertEndChild(elementDataStart);

			float capsualDirectionX = endX - startX;
			float capsualDirectionY = endY - startY;
			float capsualDirectionZ = endZ - startZ;

			const float capsualLength = sqrt((capsualDirectionX * capsualDirectionX) + (capsualDirectionY * capsualDirectionY) + (capsualDirectionZ * capsualDirectionZ));
			if (capsualLength)
			{
				capsualDirectionX /= capsualLength;
				capsualDirectionY /= capsualLength;
				capsualDirectionZ /= capsualLength;
			}

			TiXmlElement elementDataDirection("pointer");
			TXmlPack::PutR32(&elementDataDirection, capsualDirectionX);
			TXmlPack::PutR32(&elementDataDirection, capsualDirectionY);
			TXmlPack::PutR32(&elementDataDirection, capsualDirectionZ);
			elementData.InsertEndChild(elementDataDirection);

			TXmlPack::PutR32(&elementData, radius);
			TXmlPack::PutR32(&elementData, capsualLength);

			TXmlPack::PutU32(&elementDataArray, GSceneNodeComponentCollisionType::TType::TCapsual);
			elementDataArray.InsertEndChild(elementData);
		}

		pTraceElementData = pTraceElementData->NextSiblingElement();
	}

	TXmlPack::PutU32(&elementComponent, dataCount);
	elementComponent.InsertEndChild(elementDataArray);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TCollision);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentFactoryStub(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const std::string factoryStubName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("factoryStubName").Element());
	TXmlPack::PutString(&elementComponent, factoryStubName);

	const std::string data = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("data").Element());
	TXmlPack::PutString(&elementComponent, data);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TFactoryStub);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentFade(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const float duration = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("duration").Element());
	TXmlPack::PutR32(&elementComponent, duration);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TFade);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentLight(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	//	const int mLightTypeStringOffset;
	const std::string type = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("type").Element());
	TXmlPack::PutString(&elementComponent, type);

	//	const float mRadius;
	const float radius = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("radius").Element());
	TXmlPack::PutR32(&elementComponent, radius);

	//const unsigned char mDiffuseRed;
	const float diffuseRed = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("colourDiffuse").FirstChildElement("red").Element());
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(diffuseRed));

	//const unsigned char mDiffuseGreen;
	const float diffuseGreen = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("colourDiffuse").FirstChildElement("green").Element());
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(diffuseGreen));

	//const unsigned char mDiffuseBlue;
	const float diffuseBlue = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("colourDiffuse").FirstChildElement("blue").Element());
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(diffuseBlue));

	//const unsigned char mDiffuseAlpha;
	const float diffuseAlpha = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("colourDiffuse").FirstChildElement("blue").Element(), 1.0F);
	TXmlPack::PutU8(&elementComponent, LocalColourValueFromFloat(diffuseAlpha));

	//	const float mAttentuationConstant;
	const float constant = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("constant").Element());
	TXmlPack::PutR32(&elementComponent, constant);

	//	const float mAttentuationLinear;
	const float linear = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("linear").Element());
	TXmlPack::PutR32(&elementComponent, linear);

	//	const float mAttentuationQuadric;
	const float quadric = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("quadric").Element());
	TXmlPack::PutR32(&elementComponent, quadric);

	//	const float mSpotExponent;
	const float spotExponent = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("spotExponent").Element());
	TXmlPack::PutR32(&elementComponent, spotExponent);

	//	const float mSpotCutoff;
	const float spotCutoff = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("spotCutoff").Element());
	TXmlPack::PutR32(&elementComponent, spotCutoff);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TLight);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentLightCollector(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const int updateFrequency = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("updateFrequency").Element());
	TXmlPack::PutU32(&elementComponent, updateFrequency);

	const int updateOffset = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("updateOffset").Element());
	TXmlPack::PutU32(&elementComponent, updateOffset);

	TXmlPack::PutString(&out_elementComponentArray, "TLightCollector");
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentNodeAttach(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const std::string boneName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("boneName").Element());
	TXmlPack::PutString(&elementComponent, boneName);

	TiXmlElement* const pNodeElement = TiXmlHandle(&in_traceElementSource).FirstChildElement("node").Element();
	if (!pNodeElement)
	{
		inout_message.AddError("NodeAttach missing node in xml", true);
		return;
	}
	LocalWriteNode(
		elementComponent,
		*pNodeElement,
		inout_message
		);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TNodeAttach);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentParticle(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	//const char* const mData;			"data"		
	const std::string data = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("data").Element());
	TXmlPack::PutString(&elementComponent, data);

	//const char* const mMaterialName;	"material"
	const std::string material = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("material").Element());
	TXmlPack::PutString(&elementComponent, material);

	//const float mRadius;				"radius"
	const float radius = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("radius").Element());
	TXmlPack::PutR32(&elementComponent, radius);

	//const int mDefaultEmittor;			"defaultEmittor"
	const int defaultEmittor = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("defaultEmittor").Element());
	TXmlPack::PutU32(&elementComponent, defaultEmittor);

	//const float mDefaultEmittorDuration;"defaultEmittorDuration"
	const float defaultEmittorDuration = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("defaultEmittorDuration").Element());
	TXmlPack::PutR32(&elementComponent, defaultEmittorDuration);

	//const int mMaxParticleCount;		"maxParticleCount"
	const int maxParticleCount = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("maxParticleCount").Element());
	TXmlPack::PutU32(&elementComponent, maxParticleCount);

	//const int mMaxVertexCount;			"maxVertexCount"
	const int maxVertexCount = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("maxVertexCount").Element());
	TXmlPack::PutU32(&elementComponent, maxVertexCount);

	//const int mMaxEmittorCount;			"maxEmittorCount"
	const int maxEmittorCount = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("maxEmittorCount").Element());
	TXmlPack::PutU32(&elementComponent, maxEmittorCount);


	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TParticle);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentPhysics(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	//const float mMassInvert;
    //<xsd:element name="massInvert" type="xsd:float" />
	const float massInvert = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("massInvert").Element());
	TXmlPack::PutR32(&elementComponent, massInvert);

	//const GVector3Float* const mInitialVelocity;
	//<xsd:element name="initialVelocityX" type="xsd:float" />
	//<xsd:element name="initialVelocityY" type="xsd:float" />
	//<xsd:element name="initialVelocityZ" type="xsd:float" />
	const float initialVelocityX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("initialVelocityX").Element());
	const float initialVelocityY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("initialVelocityY").Element());
	const float initialVelocityZ = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("initialVelocityZ").Element());
	{
		TiXmlElement elementInitialVelocity("pointer");
		TXmlPack::PutR32(&elementInitialVelocity, initialVelocityX);
		TXmlPack::PutR32(&elementInitialVelocity, initialVelocityY);
		TXmlPack::PutR32(&elementInitialVelocity, initialVelocityZ);
		elementComponent.InsertEndChild(elementInitialVelocity);
	}

	//const int mFlag;
	//<xsd:element name="flagAllowMove" type="xsd:boolean" />
	//<xsd:element name="flagAllowRotate" type="xsd:boolean" />
	//<xsd:element name="flagFreeze" type="xsd:boolean" />
	const bool flagAllowMove = TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagAllowMove").Element());
	const bool flagAllowRotate = TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagAllowRotate").Element());
	const bool flagFreeze = TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagFreeze").Element());
	const bool flagBounceY0 = TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("flagBounceY0").Element());
	{
		int flag = 0;
		if (flagAllowMove)
		{
			flag |= GSceneNodeComponentPhysicsLoad::TFlag::TAllowMove;
		}
		if (flagAllowRotate)
		{
			flag |= GSceneNodeComponentPhysicsLoad::TFlag::TAllowRotate;
		}
		if (flagFreeze)
		{
			flag |= GSceneNodeComponentPhysicsLoad::TFlag::TFreeze;
		}
		if (flagBounceY0)
		{
			flag |= GSceneNodeComponentPhysicsLoad::TFlag::TBounceYZero;
		}

		TXmlPack::PutU32(&elementComponent, flag);
	}

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TPhysics);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}


static void LocalWriteComponentPointAtBone(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const std::string boneName = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("boneName").Element());
	TXmlPack::PutString(&elementComponent, boneName);

	const float maxAngleDeg = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("maxAngleDeg").Element());
	TXmlPack::PutR32(&elementComponent, maxAngleDeg);

	//localAtX
	const float localAtX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("localAtX").Element());
	TXmlPack::PutR32(&elementComponent, localAtX);
	const float localAtY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("localAtY").Element());
	TXmlPack::PutR32(&elementComponent, localAtY);
	const float localAtZ = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("localAtZ").Element());
	TXmlPack::PutR32(&elementComponent, localAtZ);

	//worldTargetX
	const float worldTargetX = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("worldTargetX").Element());
	TXmlPack::PutR32(&elementComponent, worldTargetX);
	const float worldTargetY = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("worldTargetY").Element());
	TXmlPack::PutR32(&elementComponent, worldTargetY);
	const float worldTargetZ = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("worldTargetZ").Element());
	TXmlPack::PutR32(&elementComponent, worldTargetZ);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TPointAtBone);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentRope(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const int knotCount = TXml::GetTextInt(TiXmlHandle(&in_traceElementSource).FirstChildElement("knotCount").Element());
	TXmlPack::PutU32(&elementComponent, knotCount);

	const float knotRadius = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("knotRadius").Element());
	TXmlPack::PutR32(&elementComponent, knotRadius);

	const std::string material = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("material").Element());
	TXmlPack::PutString(&elementComponent, material);

	const float springConstant = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("springConstant").Element());
	TXmlPack::PutR32(&elementComponent, springConstant);

	const float springDampen = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("springDampen").Element());
	TXmlPack::PutR32(&elementComponent, springDampen);

	const int showDebug = TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("showDebug").Element());
	TXmlPack::PutU32(&elementComponent, showDebug);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TRope);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentSpringBone(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	int boneCount = 0;
	TiXmlElement elementBoneData("pointer");

	TiXmlElement* pElementBone = TiXmlHandle(&in_traceElementSource).FirstChildElement("data").Element();
	while (pElementBone)
	{
		boneCount += 1;

		const std::string boneName = TXml::GetTextString(TiXmlHandle(pElementBone).FirstChildElement("boneName").Element());
		TXmlPack::PutString(&elementBoneData, boneName);

		TiXmlElement* pVectorTrace = TiXmlHandle(pElementBone).FirstChildElement("centerMass").FirstChildElement("float").Element();
		const float centerMassX = TXml::GetTextFloat(pVectorTrace);
		TXmlPack::PutR32(&elementBoneData, centerMassX);

		pVectorTrace = pVectorTrace->NextSiblingElement("float");
		const float centerMassY = TXml::GetTextFloat(pVectorTrace);
		TXmlPack::PutR32(&elementBoneData, centerMassY);

		pVectorTrace = pVectorTrace->NextSiblingElement("float");
		const float centerMassZ = TXml::GetTextFloat(pVectorTrace);
		TXmlPack::PutR32(&elementBoneData, centerMassZ);

		const float constant = TXml::GetTextFloat(TiXmlHandle(pElementBone).FirstChildElement("constant").Element());
		TXmlPack::PutR32(&elementBoneData, constant);

		const float dampen = TXml::GetTextFloat(TiXmlHandle(pElementBone).FirstChildElement("dampen").Element());
		TXmlPack::PutR32(&elementBoneData, dampen);

		const float maxValue = TXml::GetTextFloat(TiXmlHandle(pElementBone).FirstChildElement("max").Element());
		TXmlPack::PutR32(&elementBoneData, maxValue);

		const bool targetDown = TXml::GetTextBool(TiXmlHandle(pElementBone).FirstChildElement("targetDown").Element());
		const bool debug = TXml::GetTextBool(TiXmlHandle(pElementBone).FirstChildElement("debug").Element());
		unsigned int flag = 0;
		if (targetDown)
		{
			flag |= GSceneNodeComponentSpringBoneDataLoad::TFlag::TTargetDown;
		}
		if (debug)
		{
			flag |= GSceneNodeComponentSpringBoneDataLoad::TFlag::TDebug;
		}
		TXmlPack::PutU32(&elementBoneData, flag);

		pElementBone = pElementBone->NextSiblingElement("data");
	}

	TXmlPack::PutU32(&elementComponent, boneCount);
	elementComponent.InsertEndChild(elementBoneData);

	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TSpringBone);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteComponentVisual(
	TiXmlElement& out_elementComponentArray, 
	TiXmlElement& in_traceElementSource,
	TPackerMessage& inout_message
	)
{
	TiXmlElement elementComponent("pointer");

	const std::string mesh = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("mesh").Element());
	TXmlPack::PutString(&elementComponent, mesh);

	const std::string material = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("material").Element());
	TXmlPack::PutString(&elementComponent, material);

	const float radius = TXml::GetTextFloat(TiXmlHandle(&in_traceElementSource).FirstChildElement("radius").Element());
	TXmlPack::PutR32(&elementComponent, radius);

	const std::string data = TXml::GetTextString(TiXmlHandle(&in_traceElementSource).FirstChildElement("data").Element());
	TXmlPack::PutString(&elementComponent, data);

	int flag = 0;
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("visible").Element()))
	{
		flag |= GSceneNodeComponentVisual::TVisualFlag::TVisable;
	}
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("useCameraTransform").Element()))
	{
		flag |= GSceneNodeComponentVisual::TVisualFlag::TUseCameraTransform;
	}
	if (TXml::GetTextBool(TiXmlHandle(&in_traceElementSource).FirstChildElement("onlyUseTransformPos").Element()))
	{
		flag |= GSceneNodeComponentVisual::TVisualFlag::TJustUsePosition;
	}
	TXmlPack::PutU32(&elementComponent, flag);


	TXmlPack::PutU32(&out_elementComponentArray, GSceneType::TComponentFlag::TVisual);
	out_elementComponentArray.InsertEndChild(elementComponent);

	return;
}

static void LocalWriteMatrix(
	TiXmlElement& out_element,
	TiXmlElement* in_elementSource
	)
{
	TiXmlElement* pElementMatrixFloat = TiXmlHandle(in_elementSource).FirstChildElement("float").Element();
	float value;
	for (int index = 0; index < 16; ++index)
	{
		if (pElementMatrixFloat)
		{
			value = TXml::GetTextFloat(pElementMatrixFloat);
		}
		else
		{
			value = 0.0F;
			if ((0 == index) ||
				(5 == index) ||
				(10 == index) ||
				(15 == index))
			{
				value = 1.0F;
			}
		}

		TXmlPack::PutR32(&out_element, value);
		if (pElementMatrixFloat)
		{
			pElementMatrixFloat = pElementMatrixFloat->NextSiblingElement("float");
		}
	}

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

	//matrix
	TiXmlElement elementMatrix("pointer");
	LocalWriteMatrix(elementMatrix, TiXmlHandle(&in_traceElementNode).FirstChildElement("matrix").Element());
	out_elementNodeArray.InsertEndChild(elementMatrix);

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

		if (type == "componentAnimation")
		{
			componentNodeCount += 1;
			LocalWriteComponentAnimation(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentBoneCollision")
		{
			componentNodeCount += 1;
			LocalWriteComponentBoneCollision(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentClient")
		{
			componentNodeCount += 1;
			LocalWriteComponentClient(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentCollision")
		{
			componentNodeCount += 1;
			LocalWriteComponentCollision(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentFactoryStub")
		{
			componentNodeCount += 1;
			LocalWriteComponentFactoryStub(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentFade")
		{
			componentNodeCount += 1;
			LocalWriteComponentFade(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentLight")
		{
			componentNodeCount += 1;
			LocalWriteComponentLight(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentLightCollector")
		{
			componentNodeCount += 1;
			LocalWriteComponentLightCollector(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentNodeAttach")
		{
			componentNodeCount += 1;
			LocalWriteComponentNodeAttach(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentParticle")
		{
			componentNodeCount += 1;
			LocalWriteComponentParticle(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentPhysics")
		{
			componentNodeCount += 1;
			LocalWriteComponentPhysics(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentPointAtBone")
		{
			componentNodeCount += 1;
			LocalWriteComponentPointAtBone(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentRope")
		{
			componentNodeCount += 1;
			LocalWriteComponentRope(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentSpringBone")
		{
			componentNodeCount += 1;
			LocalWriteComponentSpringBone(elementComponentArray, *pTraceElementComponent, inout_message);
		}
		else if (type == "componentVisual")
		{
			componentNodeCount += 1;
			LocalWriteComponentVisual(elementComponentArray, *pTraceElementComponent, inout_message);
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



/*static*/ const bool PackScene::Pack(
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

	TiXmlElement* pTraceElementNode = TiXmlHandle(in_sourceElement).FirstChildElement("node").Element();
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