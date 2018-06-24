//file: PackAnimation.cpp

#include "PackAnimation.h"

#include "main.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <TCommon.h>
#include <tinyxml.h>

#include <GMatrix16Float.h>
#include <GVector3Float.h>
#include <GMath.h>
#include <GAnimationType.h>
#include <TPackerMessage.h>
/*
class GAnimationMasterLoad
{
	const unsigned int mArrayStreamInfoCount;
	const GAnimationStreamInfoLoad* const mArrayStreamInfo;
	const unsigned int mArrayAnimationCount;
	const GAnimationLoad* const mArrayAnimation;
};

class GAnimationStreamInfoLoad
{
	const char* const mTargetName; //bone name
	const int mTargetType; //transform_matrix, transform_pos, transform_rotation, transform_scale
};

class GAnimationLoad
{
	const char* const mName;
	const float mLength;
	const int mFlag;
	//const unsigned int mArrayStreamDataCount; use GAnimationMasterLoad::mArrayStreamInfoCount
	const GAnimationStreamDataLoad* const mArrayStreamDataLoad;
};

class GAnimationStreamDataLoad
{
	const unsigned int mArrayKeyCount;
	const GAnimationKeyLoad* const mArrayKeyLoad;
};

class GAnimationKeyLoad
{
	const float mTime;
	const void* const mData;
};

*/

typedef std::vector<std::string> TArrayString;

typedef std::pair<float, float> TPairFloatFloat;
typedef std::vector<TPairFloatFloat> TArrayPairFloatFloat;

typedef std::pair<GVector3Float, GVector3Float> TPairVectorVector;
typedef std::pair<float, TPairVectorVector> TPairFloatPairVectorVector;
typedef std::vector<TPairFloatPairVectorVector> TArrayPairFloatPairVectorVector;

typedef std::pair<float, GVector3Float> TPairFloatVector;
typedef std::vector<TPairFloatVector> TArrayPairFloatVector;

typedef std::pair<std::string, GAnimationType::TStreamType::TEnum> TPairStringType;
typedef std::vector<TPairStringType> TArrayPairStringType;

static const std::string LocalDoctorPath(
	const std::string& in_animationFile, 
	const std::string& in_relative
	)
{
	std::string localRelative(in_relative);
	//trim the relative path down to the last dir marker
	while (0 < localRelative.size())
	{
		const int index = localRelative.size() - 1;
		const char value = localRelative[index];
		if ((value == '\\') ||
			(value == '/'))
		{
			break;
		}
		localRelative.resize(index);
	}

	return localRelative + in_animationFile;
}

static const std::string LocalGetNameFromAnimationFilePath(
	const std::string& in_animationFile
	)
{
	std::string returnString = in_animationFile;

	//trim the path
	{
		const std::string::size_type pos = returnString.rfind("/");
		if (pos != std::string::npos)
		{
			returnString = returnString.substr(pos + 1, std::string::npos);
		}
	}
	//trim the path
	{
		const std::string::size_type pos = returnString.rfind("\\");
		if (pos != std::string::npos)
		{
			returnString = returnString.substr(pos + 1, std::string::npos);
		}
	}
	//trim the extention
	{
		const std::string::size_type pos = returnString.rfind(".");
		if (pos != std::string::npos)
		{
			returnString = returnString.substr(0, pos);
		}
	}


	return returnString;
}


static void LocalLoadBoneList(
	TArrayPairStringType& out_arrayStreamInfo,
	const std::string& in_boneListName, //"boneListPosition", "boneListRotation", "boneListScale", 
	const GAnimationType::TStreamType::TEnum in_type,
	TiXmlElement* const in_sourceElement
	)
{
	TiXmlElement* pTraceBone = TiXmlHandle(in_sourceElement).FirstChildElement(in_boneListName.c_str()).FirstChildElement("name").Element();
	while (pTraceBone)
	{
		const std::string boneName = TXml::GetTextString(pTraceBone);
		out_arrayStreamInfo.push_back(TPairStringType(boneName, in_type));
		pTraceBone = pTraceBone->NextSiblingElement("name");
	}

	return;
}

static void LocalGatherStreamInfo(
	TArrayPairStringType& out_arrayStreamInfo,
	TiXmlElement* const in_sourceElement
	)
{
	LocalLoadBoneList(out_arrayStreamInfo, "boneListPosition", GAnimationType::TStreamType::TPosition, in_sourceElement);
	LocalLoadBoneList(out_arrayStreamInfo, "boneListRotation", GAnimationType::TStreamType::TRotation, in_sourceElement);
	LocalLoadBoneList(out_arrayStreamInfo, "boneListScale", GAnimationType::TStreamType::TScale, in_sourceElement);
	LocalLoadBoneList(out_arrayStreamInfo, "boneListNoteTrack", GAnimationType::TStreamType::TNoteTrack, in_sourceElement);
	LocalLoadBoneList(out_arrayStreamInfo, "boneListMorphWeight", GAnimationType::TStreamType::TMorphWeight, in_sourceElement);
	return;
}

static const bool LocalFindBone(
	const TArrayString& in_arrayBoneName,
	const std::string& in_boneName
	)
{
	for (TArrayString::const_iterator iterator = in_arrayBoneName.begin(); iterator != in_arrayBoneName.end(); ++iterator)
	{
		if (in_boneName == (*iterator))
		{
			return true;
		}
	}

	return false;
}

/*
//need some way of knowing the default skeleton position for keys, so we can skip tracks that have no data
//is the easiest way of doing that by exporting a skeleton xml with mesh export for use by animation, bone names and default keys?
animation state is not restored, so if we want skeleton default pose, it need to be in the animation...
so don't skip default pose
*/
//collect keys
static void LocalCollectKeys(
	TArrayPairFloatVector& out_arrayKeyTranslation,
	TArrayPairFloatPairVectorVector& out_arrayKeyRotation,
	TArrayPairFloatFloat& out_arrayKeyScale,
	TiXmlElement& in_elementNode
	)
{
	TiXmlElement* pTraceNode = TiXmlHandle(&in_elementNode).FirstChildElement("animation").FirstChildElement("key").Element();
	while (pTraceNode)
	{
		const float localTime = TXml::GetTextFloat(TiXmlHandle(pTraceNode).FirstChildElement("time").Element());
		float data[16];
		TiXmlElement* pTraceMatrix = TiXmlHandle(pTraceNode).FirstChildElement("matrix").FirstChildElement("float").Element();
		for (int index = 0; index < 16; ++index)
		{
			data[index] = TXml::GetTextFloat(pTraceMatrix);
			if (pTraceMatrix)
			{
				pTraceMatrix = pTraceMatrix->NextSiblingElement("float");
			}
		}
		GMatrix16Float matrix(data);

		//position
		GVector3Float position = matrix.GetPosition();
		out_arrayKeyTranslation.push_back(TPairFloatVector(localTime, position));

		//rotation
		const float* const matrixData = matrix.GetData();
		GVector3Float at(matrixData[8], matrixData[9], matrixData[10]);
		GVector3Float up(matrixData[4], matrixData[5], matrixData[6]);
		const float scale = 0.5F * (Length(up) + Length(at));
		Orthogonalise(at, up);
		out_arrayKeyRotation.push_back(TPairFloatPairVectorVector(localTime, TPairVectorVector(at, up)));

		//scale
		out_arrayKeyScale.push_back(TPairFloatFloat(localTime, scale));

		pTraceNode = pTraceNode->NextSiblingElement("key");
	}

	//filter out homgonus keys
	bool keysSame = true;
	for (int index = 1; index < (int)out_arrayKeyTranslation.size(); ++index)
	{
		const GVector3Float offset = out_arrayKeyTranslation[0].second - out_arrayKeyTranslation[index].second;
		if ((!GMath::AlmostZero(offset.m_x, 0.001F)) ||
			(!GMath::AlmostZero(offset.m_y, 0.001F)) ||
			(!GMath::AlmostZero(offset.m_z, 0.001F)))
		{
			keysSame = false;
			break;
		}
	}
	if (keysSame)
	{
		out_arrayKeyTranslation.resize(1);
		//out_arrayKeyTranslation.clear();
	}

	keysSame = true;
	for (int index = 1; index < (int)out_arrayKeyRotation.size(); ++index)
	{
		const GVector3Float offset0 = out_arrayKeyRotation[0].second.first  - out_arrayKeyRotation[index].second.first;
		const GVector3Float offset1 = out_arrayKeyRotation[0].second.second - out_arrayKeyRotation[index].second.second;

		if ((!GMath::AlmostZero(offset0.m_x, 0.001F)) ||
			(!GMath::AlmostZero(offset0.m_y, 0.001F)) ||
			(!GMath::AlmostZero(offset0.m_z, 0.001F)) ||
			(!GMath::AlmostZero(offset1.m_x, 0.001F)) ||
			(!GMath::AlmostZero(offset1.m_y, 0.001F)) ||
			(!GMath::AlmostZero(offset1.m_z, 0.001F)))
		{
			keysSame = false;
			break;
		}
	}
	if (keysSame)
	{
		out_arrayKeyRotation.resize(1);
		//out_arrayKeyRotation.clear();
	}

	keysSame = true;
	for (int index = 1; index < (int)out_arrayKeyScale.size(); ++index)
	{
		if (!GMath::AlmostZero(out_arrayKeyScale[0].second - out_arrayKeyScale[index].second, 0.001F))
		{
			keysSame = false;
			break;
		}
	}
	if (keysSame)
	{
		out_arrayKeyScale.resize(1);
		//out_arrayKeyScale.clear();
	}

	return;
}

static void LocalAddAnimation(
	TiXmlElement& out_elementStreamArray,
	TiXmlElement& in_elementNode,
	const bool in_addPosition,
	const bool in_addRotation,
	const bool in_addScale
	)
{
	TArrayPairFloatVector arrayKeyTranslation;
	TArrayPairFloatPairVectorVector arrayKeyRotation;
	TArrayPairFloatFloat arrayKeyScale;

	//collect keys
	LocalCollectKeys(
		arrayKeyTranslation,
		arrayKeyRotation,
		arrayKeyScale,
		in_elementNode
		);

	//position stream
	const unsigned int keyTranslationCount = arrayKeyTranslation.size();
	if ((0 < keyTranslationCount) && in_addPosition)
	{
		//int key count
		TXmlPack::PutU32(&out_elementStreamArray, keyTranslationCount);

		//int keyOffset
		TiXmlElement elementKeyArray("pointer");
		elementKeyArray.SetAttribute("alignment", 4);

		for (unsigned int index = 0; index < keyTranslationCount; ++index)
		{
			//time
			TXmlPack::PutR32(&elementKeyArray, arrayKeyTranslation[index].first);

			TiXmlElement elementKeyData("pointer");
			elementKeyData.SetAttribute("alignment", 4);

			const GVector3Float& position = arrayKeyTranslation[index].second;
			TXmlPack::PutR32(&elementKeyData, position.m_x);
			TXmlPack::PutR32(&elementKeyData, position.m_y);
			TXmlPack::PutR32(&elementKeyData, position.m_z);
			elementKeyArray.InsertEndChild(elementKeyData);
		}
		out_elementStreamArray.InsertEndChild(elementKeyArray);
	}

	//rotation stream
	const unsigned int keyRotationCount = arrayKeyRotation.size();
	if ((0 < keyRotationCount) && in_addRotation)
	{
		//int key count
		TXmlPack::PutU32(&out_elementStreamArray, keyRotationCount);

		//int keyOffset
		TiXmlElement elementKeyArray("pointer");
		elementKeyArray.SetAttribute("alignment", 4);

		for (unsigned int index = 0; index < keyRotationCount; ++index)
		{
			//time
			TXmlPack::PutR32(&elementKeyArray, arrayKeyRotation[index].first);

			TiXmlElement elementKeyData("pointer");
			elementKeyData.SetAttribute("alignment", 4);

			const TPairVectorVector& rotation = arrayKeyRotation[index].second;
			TXmlPack::PutR32(&elementKeyData, rotation.first.m_x);
			TXmlPack::PutR32(&elementKeyData, rotation.first.m_y);
			TXmlPack::PutR32(&elementKeyData, rotation.first.m_z);

			TXmlPack::PutR32(&elementKeyData, rotation.second.m_x);
			TXmlPack::PutR32(&elementKeyData, rotation.second.m_y);
			TXmlPack::PutR32(&elementKeyData, rotation.second.m_z);
			elementKeyArray.InsertEndChild(elementKeyData);
		}
		out_elementStreamArray.InsertEndChild(elementKeyArray);
	}

	//scale stream
	const unsigned int keyScaleCount = arrayKeyScale.size();
	if ((0 < keyScaleCount) && in_addScale)
	{
		//int key count
		TXmlPack::PutU32(&out_elementStreamArray, keyScaleCount);

		//int keyOffset
		TiXmlElement elementKeyArray("pointer");
		elementKeyArray.SetAttribute("alignment", 4);

		for (unsigned int index = 0; index < keyScaleCount; ++index)
		{
			//time
			TXmlPack::PutR32(&elementKeyArray, arrayKeyScale[index].first);

			TiXmlElement elementKeyData("pointer");
			elementKeyData.SetAttribute("alignment", 4);

			TXmlPack::PutR32(&elementKeyData, arrayKeyScale[index].second);
			elementKeyArray.InsertEndChild(elementKeyData);
		}
		out_elementStreamArray.InsertEndChild(elementKeyArray);
	}

	return;
}

static void LocalAddNoteTrackData(
	TiXmlElement& out_elementStreamArray,
	TiXmlElement& in_elementNode
	)
{
	TiXmlElement elementKeyData("pointer");
	elementKeyData.SetAttribute("alignment", 4);

	int noteTrackCount = 0;
	TiXmlElement* pTraceNoteTrackData = TiXmlHandle(&in_elementNode).FirstChildElement("noteTrack").FirstChildElement("key").Element();
	while (pTraceNoteTrackData)
	{
		noteTrackCount += 1;
		const float keyTime = TXml::GetTextFloat(TiXmlHandle(pTraceNoteTrackData).FirstChildElement("time").Element());
		const std::string value = TXml::GetTextString(TiXmlHandle(pTraceNoteTrackData).FirstChildElement("value").Element());

		//float mTime;
		TXmlPack::PutR32(&elementKeyData, keyTime);
		//int mNoteOffset;
		TXmlPack::PutString(&elementKeyData, value);		

		pTraceNoteTrackData = pTraceNoteTrackData->NextSiblingElement("key");
	}

	TXmlPack::PutU32(&out_elementStreamArray, noteTrackCount);
	out_elementStreamArray.InsertEndChild(elementKeyData);

	return;
}

static const bool LocalAddMorphWeightData(
	TiXmlElement& out_elementStreamArray,
	TiXmlElement& in_elementNode,
	const std::string& in_targetName
	)
{
	TiXmlElement* pTraceTarget = TiXmlHandle(&in_elementNode).FirstChildElement("morphTrack").FirstChildElement("target").Element();
	while (pTraceTarget)
	{
		const std::string name = TXml::GetTextString(TiXmlHandle(pTraceTarget).FirstChildElement("name").Element());
		if (in_targetName == name)
		{
			TiXmlElement elementKeyArray("pointer");
			elementKeyArray.SetAttribute("alignment", 4);
			int trace = 0;

			TiXmlElement* pTraceKey = TiXmlHandle(pTraceTarget).FirstChildElement("key").Element();
			while (pTraceKey)
			{
				const float keyTime = TXml::GetTextFloat(TiXmlHandle(pTraceKey).FirstChildElement("time").Element());
				const float weight = TXml::GetTextFloat(TiXmlHandle(pTraceKey).FirstChildElement("weight").Element());

				//float mTime;
				TXmlPack::PutR32(&elementKeyArray, keyTime);
				//float mWeight;
				TiXmlElement elementKeyData("pointer");
				elementKeyData.SetAttribute("alignment", 4);

				TXmlPack::PutR32(&elementKeyData, weight);
				elementKeyArray.InsertEndChild(elementKeyData);

				trace += 1;
				pTraceKey = pTraceKey->NextSiblingElement("key");
			}

			TXmlPack::PutU32(&out_elementStreamArray, trace);
			out_elementStreamArray.InsertEndChild(elementKeyArray);

			return true;
		}

		pTraceTarget = pTraceTarget->NextSiblingElement("target");
	}

	return false;
}


static const bool LocalAddStreams(
	TiXmlElement& out_elementStreamArray,
	TiXmlElement* in_pElementParent,
	const std::string& in_targetName,
	const GAnimationType::TStreamType::TEnum in_type
	)
{
	TiXmlElement* pTraceNode = TiXmlHandle(in_pElementParent).FirstChildElement("node").Element();
	while (pTraceNode)
	{
		//position/ rotation/ scale streams
		const std::string name = TXml::GetTextString(TiXmlHandle(pTraceNode).FirstChildElement("name").Element());
		if ((name == in_targetName) &&
			((GAnimationType::TStreamType::TPosition == in_type) || (GAnimationType::TStreamType::TRotation == in_type) || (GAnimationType::TStreamType::TScale == in_type)))
		{
			LocalAddAnimation(
				out_elementStreamArray,
				*pTraceNode,
				(GAnimationType::TStreamType::TPosition == in_type),
				(GAnimationType::TStreamType::TRotation == in_type),
				(GAnimationType::TStreamType::TScale == in_type)
				);
			return true;
		}

		//note track
		if ((name == in_targetName) &&
			(GAnimationType::TStreamType::TNoteTrack == in_type))
		{
			LocalAddNoteTrackData(
				out_elementStreamArray,
				*pTraceNode
				);

			return true;
		}

		//morph weight track
		if (GAnimationType::TStreamType::TMorphWeight == in_type)
		{
			if (LocalAddMorphWeightData(
				out_elementStreamArray,
				*pTraceNode,
				in_targetName
				))
			{
				return true;
			}
		}

		//recusion
		if (LocalAddStreams(
			out_elementStreamArray,
			pTraceNode,
			in_targetName,
			in_type
			))
		{
			return true;
		}

		pTraceNode = pTraceNode->NextSiblingElement("node");
	}

	return false;
}


static const bool LocalPackAnimation(
	TiXmlElement& out_elementAnimationArray,
	const std::string& in_animationFile,
	const TArrayPairStringType& in_arrayStreamInfo,
	TPackerMessage& in_packMessage
	)
{
	//try to load file
	TiXmlDocument sourceDocument;
	if (sourceDocument.LoadFile(in_animationFile))
	{
		in_packMessage.AddMessage(std::string("load src xml:") + in_animationFile);
	}
	else
	{
		in_packMessage.AddError(std::string("failed load src xml:") + in_animationFile);
		return false;
	}

	//const std::string name = TXml::GetTextString(TiXmlHandle(sourceDocument.RootElement()).FirstChildElement("name").Element());
	const std::string name = LocalGetNameFromAnimationFilePath(in_animationFile);
	const int flag = TXml::GetTextBool(TiXmlHandle(sourceDocument.RootElement()).FirstChildElement("loop").Element());
	const float length = TXml::GetTextFloat(TiXmlHandle(sourceDocument.RootElement()).FirstChildElement("length").Element());

//const char* const mName;
	TXmlPack::PutString(&out_elementAnimationArray, name);

//const float mLength;
	TXmlPack::PutR32(&out_elementAnimationArray, length);

//const int mFlag;
	TXmlPack::PutU32(&out_elementAnimationArray, flag);

	TiXmlElement elementStreamArray("pointer");
	elementStreamArray.SetAttribute("alignment", 4);

	for (TArrayPairStringType::const_iterator iterator = in_arrayStreamInfo.begin(); iterator != in_arrayStreamInfo.end(); ++iterator)
	{
		if (!LocalAddStreams(
			elementStreamArray,
			sourceDocument.RootElement(),
			iterator->first,
			iterator->second
			))
		{
			//add dummy stream key count
			TXmlPack::PutU32(&elementStreamArray, 0);
			//add dummy stream key NULL pointer
			TiXmlElement elementDummy("pointer");
			elementDummy.SetAttribute("alignment", 4);
			elementStreamArray.InsertEndChild(elementDummy);
		}
	}

//const GAnimationStreamLoad* const mArrayAnimationStreamLoad;
	out_elementAnimationArray.InsertEndChild(elementStreamArray);

	return true;
}

/*static*/ const bool PackAnimation::Pack(
	TiXmlElement* const in_sourceElement,
	TiXmlElement* const in_destinationElement,
	const std::string& in_masterAnimationFilePath,
	TPackerMessage& in_packMessage
	)
{
	if (!in_sourceElement || !in_destinationElement)
	{
		return false;
	}

	TArrayPairStringType arrayStreamInfo;
	LocalGatherStreamInfo(
		arrayStreamInfo,
		in_sourceElement
		);

	//streamInfoCount
	TXmlPack::PutU32(in_destinationElement, arrayStreamInfo.size());
	{
		TiXmlElement elementStreamInfoArray("pointer");
		elementStreamInfoArray.SetAttribute("alignment", 4);
		for (TArrayPairStringType::const_iterator iterator = arrayStreamInfo.begin(); iterator != arrayStreamInfo.end(); ++iterator)
		{	
			TXmlPack::PutString(&elementStreamInfoArray, (*iterator).first);
			TXmlPack::PutU32(&elementStreamInfoArray, (*iterator).second);
		}
		//arraystreamInfo
		in_destinationElement->InsertEndChild(elementStreamInfoArray);
	}

	int animationCount = 0;
	TiXmlElement elementAnimationArray("pointer");
	elementAnimationArray.SetAttribute("alignment", 4);
	TiXmlElement* pTraceAnimation = TiXmlHandle(in_sourceElement).FirstChildElement("animationList").FirstChildElement("file").Element();
	while (pTraceAnimation)
	{
		const std::string animationFile = TXml::GetTextString(pTraceAnimation);
		const std::string doctoredAnimationFile = LocalDoctorPath(animationFile, in_masterAnimationFilePath);
		if (LocalPackAnimation(
			elementAnimationArray,
			doctoredAnimationFile,
			arrayStreamInfo,
			in_packMessage
			))
		{
			animationCount += 1;
		}

		pTraceAnimation = pTraceAnimation->NextSiblingElement("file");
	}

	//animationCount
	TXmlPack::PutU32(in_destinationElement, animationCount);
	//animationOffset
	in_destinationElement->InsertEndChild(elementAnimationArray);

	return true;
}