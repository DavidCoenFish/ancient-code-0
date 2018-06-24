//file: PackParticleManager.cpp

#include "PackParticleManager.h"

#include "main.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>

#include <GParticleType.h>
#include <TPackerMessage.h>
/*

*/


static void LocalPackArrayUvDataParticle(
	TiXmlElement* const in_destinationElement,
	TiXmlElement* const in_sourceArrayElement,
	TPackerMessage& in_packMessage
	)
{
	int trace = 0;
	TiXmlElement elementArray("pointer");

	TiXmlElement* pTrace = TiXmlHandle(in_sourceArrayElement).FirstChildElement("uvDataParticle").Element();
	while (pTrace)
	{
		const float originX = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("originX").Element());
		const float originY = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("originY").Element());
		const float sizeX = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sizeX").Element());
		const float sizeY = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sizeY").Element());

		TiXmlElement elementOrigin("pointer");
		TXmlPack::PutR32(&elementOrigin, originX);
		TXmlPack::PutR32(&elementOrigin, originY);
		elementArray.InsertEndChild(elementOrigin);

		TiXmlElement elementSize("pointer");
		TXmlPack::PutR32(&elementSize, sizeX);
		TXmlPack::PutR32(&elementSize, sizeY);
		elementArray.InsertEndChild(elementSize);

		pTrace = pTrace->NextSiblingElement("uvDataParticle");
		trace += 1;
	}

	TXmlPack::PutU32(in_destinationElement, trace);
	in_destinationElement->InsertEndChild(elementArray);

	return;
}

static void LocalPackArrayUvDataLine(
	TiXmlElement* const in_destinationElement,
	TiXmlElement* const in_sourceArrayElement,
	TPackerMessage& in_packMessage
	)
{
	int trace = 0;
	TiXmlElement elementArray("pointer");

	TiXmlElement* pTrace = TiXmlHandle(in_sourceArrayElement).FirstChildElement("uvDataLine").Element();
	while (pTrace)
	{
		const float originX = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("originX").Element());
		const float originY = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("originY").Element());
		const float width = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("width").Element());
		const float heightTop = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("heightTop").Element());
		const float heightBody = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("heightBody").Element());
		const float heightBottom = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("heightBottom").Element());

		TiXmlElement elementOrigin("pointer");
		TXmlPack::PutR32(&elementOrigin, originX);
		TXmlPack::PutR32(&elementOrigin, originY);
		elementArray.InsertEndChild(elementOrigin);

		TXmlPack::PutR32(&elementArray, width);
		TXmlPack::PutR32(&elementArray, heightTop);
		TXmlPack::PutR32(&elementArray, heightBody);
		TXmlPack::PutR32(&elementArray, heightBottom);

		pTrace = pTrace->NextSiblingElement("uvDataLine");
		trace += 1;
	}

	TXmlPack::PutU32(in_destinationElement, trace);
	in_destinationElement->InsertEndChild(elementArray);

	return;
}

static void LocalPackArrayParticle(
	TiXmlElement* const in_destinationElement,
	TiXmlElement* const in_sourceArrayElement,
	TPackerMessage& in_packMessage
	)
{
	int trace = 0;
	TiXmlElement elementArray("pointer");

	TiXmlElement* pTrace = TiXmlHandle(in_sourceArrayElement).FirstChildElement("particle").Element();
	while (pTrace)
	{
		const int uvDataIndex = TXml::GetTextInt(TiXmlHandle(pTrace).FirstChildElement("uvDataIndex").Element());
		const float duration = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("duration").Element());
		const int nextParticleIndex = TXml::GetTextInt(TiXmlHandle(pTrace).FirstChildElement("nextParticleIndex").Element(), -1);
		int flag = 0;
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagWorldSpace").Element()))
		{
			flag |= GParticleType::TParticle::TWorldSpace;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagStationary").Element()))
		{
			flag |= GParticleType::TParticle::TStationary;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagLine").Element()))
		{
			flag |= GParticleType::TParticle::TLine;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagUseGravity").Element()))
		{
			flag |= GParticleType::TParticle::TUseGravity;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagUseGravityWeak").Element()))
		{
			flag |= GParticleType::TParticle::TUseGravityWeak;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagUseAntiGravity").Element()))
		{
			flag |= GParticleType::TParticle::TUseAntiGravity;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagUseAntiGravityWeak").Element()))
		{
			flag |= GParticleType::TParticle::TUseAntiGravityWeak;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagBounceZeroY").Element()))
		{
			flag |= GParticleType::TParticle::TBounceZeroY;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagSplatZeroY").Element()))
		{
			flag |= GParticleType::TParticle::TSplatZeroY;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagHasDuration").Element()))
		{
			flag |= GParticleType::TParticle::THasDuration;
		}

		TXmlPack::PutU32(&elementArray, flag);
		TXmlPack::PutU32(&elementArray, uvDataIndex);
		TXmlPack::PutR32(&elementArray, duration);
		TXmlPack::PutU32(&elementArray, nextParticleIndex);

		pTrace = pTrace->NextSiblingElement("particle");
		trace += 1;
	}

	TXmlPack::PutU32(in_destinationElement, trace);
	in_destinationElement->InsertEndChild(elementArray);

	return;
}
static void LocalPackArrayEmittorData(
	TiXmlElement* const in_destinationElement,
	TiXmlElement* const in_sourceArrayElement,
	TPackerMessage& in_packMessage
	)
{
	int trace = 0;
	TiXmlElement elementArray("pointer");
	TiXmlElement* pTrace = TiXmlHandle(in_sourceArrayElement).FirstChildElement("emittorData").Element();
	while (pTrace)
	{
		const float rate = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("rate").Element());
		const float rateDelta = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("rateDelta").Element());
		const int particleIndex = TXml::GetTextInt(TiXmlHandle(pTrace).FirstChildElement("particleIndex").Element());
		const float lifeSpan = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("lifeSpan").Element());
		const float lifeSpanDelta = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("lifeSpanDelta").Element());
		const float radiusStart = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("radiusStart").Element());
		const float radiusStartDelta = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("radiusStartDelta").Element());
		const float radiusEnd = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("radiusEnd").Element());
		const float radiusEndDelta = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("radiusEndDelta").Element());
		const float alphaStart = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("alphaStart").Element());
		const float alphaStartDelta = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("alphaStartDelta").Element());
		const float alphaEnd = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("alphaEnd").Element());
		const float alphaEndDelta = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("alphaEndDelta").Element());

		const float initialVelocityX = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("initialVelocityX").Element());
		const float initialVelocityY = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("initialVelocityY").Element());
		const float initialVelocityZ = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("initialVelocityZ").Element());
		const float initialVelocityDeltaX = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("initialVelocityDeltaX").Element());
		const float initialVelocityDeltaY = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("initialVelocityDeltaY").Element());
		const float initialVelocityDeltaZ = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("initialVelocityDeltaZ").Element());

		TXmlPack::PutR32(&elementArray, rate);
		TXmlPack::PutR32(&elementArray, rateDelta);
		TXmlPack::PutU32(&elementArray, particleIndex);
		TXmlPack::PutR32(&elementArray, lifeSpan);
		TXmlPack::PutR32(&elementArray, lifeSpanDelta);
		TXmlPack::PutR32(&elementArray, radiusStart);
		TXmlPack::PutR32(&elementArray, radiusStartDelta);
		TXmlPack::PutR32(&elementArray, radiusEnd);
		TXmlPack::PutR32(&elementArray, radiusEndDelta);
		TXmlPack::PutR32(&elementArray, alphaStart);
		TXmlPack::PutR32(&elementArray, alphaStartDelta);
		TXmlPack::PutR32(&elementArray, alphaEnd);
		TXmlPack::PutR32(&elementArray, alphaEndDelta);

		TiXmlElement elementInitialVelocity("pointer");
		TXmlPack::PutR32(&elementInitialVelocity, initialVelocityX);
		TXmlPack::PutR32(&elementInitialVelocity, initialVelocityY);
		TXmlPack::PutR32(&elementInitialVelocity, initialVelocityZ);
		elementArray.InsertEndChild(elementInitialVelocity);

		TiXmlElement elementInitialVelocityDelta("pointer");
		TXmlPack::PutR32(&elementInitialVelocityDelta, initialVelocityDeltaX);
		TXmlPack::PutR32(&elementInitialVelocityDelta, initialVelocityDeltaY);
		TXmlPack::PutR32(&elementInitialVelocityDelta, initialVelocityDeltaZ);
		elementArray.InsertEndChild(elementInitialVelocityDelta);


		pTrace = pTrace->NextSiblingElement("emittorData");
		trace += 1;
	}
	TXmlPack::PutU32(in_destinationElement, trace);
	in_destinationElement->InsertEndChild(elementArray);
	return;
}

static void LocalPackArrayEmittor(
	TiXmlElement* const in_destinationElement,
	TiXmlElement* const in_sourceArrayElement,
	TPackerMessage& in_packMessage
	)
{
	int trace = 0;
	TiXmlElement elementArray("pointer");

	TiXmlElement* pTrace = TiXmlHandle(in_sourceArrayElement).FirstChildElement("emittor").Element();
	while (pTrace)
	{
		const float sourcePointX = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sourcePointX").Element());
		const float sourcePointY = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sourcePointY").Element());
		const float sourcePointZ = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sourcePointZ").Element());
		const float sourcePointDeltaX = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sourcePointDeltaX").Element());
		const float sourcePointDeltaY = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sourcePointDeltaY").Element());
		const float sourcePointDeltaZ = TXml::GetTextFloat(TiXmlHandle(pTrace).FirstChildElement("sourcePointDeltaZ").Element());

		int flag = 0;
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagWorldSpace").Element()))
		{
			flag |= GParticleType::TEmittor::TWorldSpace;
		}
		if (TXml::GetTextBool(TiXmlHandle(pTrace).FirstChildElement("flagHasDuration").Element()))
		{
			flag |= GParticleType::TEmittor::THasDuration;
		}

		TXmlPack::PutU32(&elementArray, flag);
		TiXmlElement elementSourcePoint("pointer");
		TXmlPack::PutR32(&elementSourcePoint, sourcePointX);
		TXmlPack::PutR32(&elementSourcePoint, sourcePointY);
		TXmlPack::PutR32(&elementSourcePoint, sourcePointZ);
		elementArray.InsertEndChild(elementSourcePoint);

		TiXmlElement elementSourcePointDelta("pointer");
		TXmlPack::PutR32(&elementSourcePointDelta, sourcePointDeltaX);
		TXmlPack::PutR32(&elementSourcePointDelta, sourcePointDeltaY);
		TXmlPack::PutR32(&elementSourcePointDelta, sourcePointDeltaZ);
		elementArray.InsertEndChild(elementSourcePointDelta);

		LocalPackArrayEmittorData(
			&elementArray,
			TiXmlHandle(pTrace).FirstChildElement("arrayData").Element(),
			in_packMessage
			);

		pTrace = pTrace->NextSiblingElement("emittor");
		trace += 1;
	}

	TXmlPack::PutU32(in_destinationElement, trace);
	in_destinationElement->InsertEndChild(elementArray);

	return;
}

/*static*/ const bool PackParticleManager::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	TPackerMessage& in_packMessage
	)
{
	if (!in_sourceElement || !in_destinationElement)
	{
		return false;
	}
	//FirstChildElement("uvDataParticle")
	LocalPackArrayUvDataParticle(in_destinationElement, TiXmlHandle(in_sourceElement).FirstChildElement("arrayUvDataParticle").Element(), in_packMessage);
	LocalPackArrayUvDataLine(in_destinationElement, TiXmlHandle(in_sourceElement).FirstChildElement("arrayUvDataLine").Element(), in_packMessage);
	LocalPackArrayParticle(in_destinationElement, TiXmlHandle(in_sourceElement).FirstChildElement("arrayParticle").Element(), in_packMessage);
	LocalPackArrayEmittor(in_destinationElement, TiXmlHandle(in_sourceElement).FirstChildElement("arrayEmittor").Element(), in_packMessage);

	return true;
}