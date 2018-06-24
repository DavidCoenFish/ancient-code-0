//
//  ZombieMissionClient.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieMissionClient.h"

#include "ZombieMission.h"
#include "ZombieCameraMixer.h"
#include "ZombieGameVar.h"

#include <GCommon.h>
#include <map>
#include <GAssert.h>

struct TSoftBindClientTrigger
{
	enum TEnum
	{
		TLoadScene = 0,
		TRequestNextMission,

		TFactoryPosition,
		TFactoryRotation,
		TFactory,
		TSetPosition,
		TSetRotation,
		TSetCameraType,
		TSetAiActive,
		TActorDestroy,
		TCinematicText,
		//TPlayAnimation, -not needed, has own message type

		TOnOpponenetDeadSequence,
		TOn2OpponenetLeftSequence,
		TOnNamedOpponentDead,
		TOnActorDeadSequence,
		TOnNoOpponenetCinematicSequence,
		TOnNoOpponenetAiSequence,
		TVar0TrueSequence,
		TVar0FalseSequence,
		TSetVar0True,
		TSetVar0False,

		TTriggerBossWave0,
		TTriggerBossWave1,

		TCount
	};
};

struct TSoftBindSpawnActorData
{
	const char* const mData;
	const char* const mFactoryStubName;
	const char* const mFactoryData;
};

static const TSoftBindSpawnActorData sSoftBindSpawnActorData[] = 
{
	//{ "dummy", "", "" }
	  { "ZombieOneDefault", "FactoryZombieOne", "" }
	, { "ZombieOneRandom", "FactoryZombieOne", "random" }
	, { "ZombieOneStandard", "FactoryZombieOne", "standard" }
	, { "ZombieOneHalf", "FactoryZombieOne", "half" }
	, { "ZombieOneTorso", "FactoryZombieOne", "torso" }
};



typedef std::map<std::string, int> TMapStringInt;

static const TMapStringInt& LocalGetSoftBindClientTriggerMap()
{
	static TMapStringInt sSoftBindClientTriggerMap;
	if (0 == sSoftBindClientTriggerMap.size())
	{
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TLoadScene", TSoftBindClientTrigger::TLoadScene));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TRequestNextMission", TSoftBindClientTrigger::TRequestNextMission));

		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TFactoryPosition", TSoftBindClientTrigger::TFactoryPosition));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TFactoryRotation", TSoftBindClientTrigger::TFactoryRotation));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TFactory", TSoftBindClientTrigger::TFactory));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TSetPosition", TSoftBindClientTrigger::TSetPosition));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TSetRotation", TSoftBindClientTrigger::TSetRotation));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TSetCameraType", TSoftBindClientTrigger::TSetCameraType));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TSetAiActive", TSoftBindClientTrigger::TSetAiActive));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TActorDestroy", TSoftBindClientTrigger::TActorDestroy));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TCinematicText", TSoftBindClientTrigger::TCinematicText));

		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TOnOpponenetDeadSequence", TSoftBindClientTrigger::TOnOpponenetDeadSequence));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TOn2OpponenetLeftSequence", TSoftBindClientTrigger::TOn2OpponenetLeftSequence));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TOnNamedOpponentDead", TSoftBindClientTrigger::TOnNamedOpponentDead));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TOnActorDeadSequence", TSoftBindClientTrigger::TOnActorDeadSequence));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TOnNoOpponenetCinematicSequence", TSoftBindClientTrigger::TOnNoOpponenetCinematicSequence));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TOnNoOpponenetAiSequence", TSoftBindClientTrigger::TOnNoOpponenetAiSequence));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TVar0TrueSequence", TSoftBindClientTrigger::TVar0TrueSequence));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TVar0FalseSequence", TSoftBindClientTrigger::TVar0FalseSequence));

		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TSetVar0True", TSoftBindClientTrigger::TSetVar0True));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TSetVar0False", TSoftBindClientTrigger::TSetVar0False));

		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TTriggerBossWave0", TSoftBindClientTrigger::TTriggerBossWave0));
		sSoftBindClientTriggerMap.insert(TMapStringInt::value_type("TTriggerBossWave1", TSoftBindClientTrigger::TTriggerBossWave1));

	}
	return sSoftBindClientTriggerMap;
}

static const TSoftBindClientTrigger::TEnum LocalGetSoftBindClientTrigger(const std::string& in_clientTrigger)
{
	const TMapStringInt& softBindClientTriggerMap = LocalGetSoftBindClientTriggerMap();
	TMapStringInt::const_iterator iterator = softBindClientTriggerMap.find(in_clientTrigger);
	if (iterator != softBindClientTriggerMap.end())
	{
		return (TSoftBindClientTrigger::TEnum)(*iterator).second;
	}

	return TSoftBindClientTrigger::TCount;
}

//return true if found
static void LocalGetSoftBindSpawnActorData(
	std::string& out_factoryStubName, 
	std::string& out_factoryData, 
	const std::string& in_data,
	ZombieMission& in_parentMission
	)
{
	static TMapStringInt sSoftBindSpawnActorDataMap;
	if (0 == sSoftBindSpawnActorDataMap.size())
	{
		for (int index = 0; index < GCOMMON_ARRAY_SIZE(sSoftBindSpawnActorData); ++index)
		{
			sSoftBindSpawnActorDataMap.insert(TMapStringInt::value_type(sSoftBindSpawnActorData[index].mData, index));
		}
	}

	TMapStringInt::iterator iterator = sSoftBindSpawnActorDataMap.find(in_data);
	if (iterator != sSoftBindSpawnActorDataMap.end())
	{
		const int index = (*iterator).second;
		out_factoryStubName = sSoftBindSpawnActorData[index].mFactoryStubName;
		out_factoryData = sSoftBindSpawnActorData[index].mFactoryData;
	}
	else if ("PlayerSelf" == in_data)
	{
		out_factoryStubName = in_parentMission.GetGameVar()->GetPlayerFactoryStubName(ZombieGameVar::GetPlayerIdSelf());
	}
	else if ("PlayerRescue" == in_data)
	{
		out_factoryStubName = in_parentMission.GetGameVar()->GetPlayerFactoryStubName(ZombieGameVar::GetPlayerIdRescue());
	}
	else if ("PlayerOther" == in_data)
	{
		out_factoryStubName = in_parentMission.GetGameVar()->GetPlayerFactoryStubName(ZombieGameVar::GetPlayerIdOther());
	}
	else
	{
		out_factoryStubName = in_data;
	}

	return;
}

static const int LocalGetCameraType(const std::string& in_data)
{
	if (in_data == "Cinematic")
	{
		return ZombieCameraMixer::GetCameraTypeCinematic();
	}
	else if (in_data == "CinematicNoRoll")
	{
		return ZombieCameraMixer::GetCameraTypeCinematicNoRoll();
	}
	else if(in_data == "Game")
	{
		return ZombieCameraMixer::GetCameraTypeGame();
	}
	else if(in_data == "Debug")
	{
		return ZombieCameraMixer::GetCameraTypeDebug();
	}

	return GCOMMON_INVALID_INDEX;
}

//constructor
ZombieMissionClient::ZombieMissionClient(ZombieMission& in_parent)
: mParent(in_parent)
{
	return;
}

ZombieMissionClient::~ZombieMissionClient()
{
	return;
}

//implement GMissionClient
/*virtual*/ void ZombieMissionClient::OnRequestClientTrigger(
	const std::string& in_clientTrigger,
	const std::string& in_data
	)
{
	switch (LocalGetSoftBindClientTrigger(in_clientTrigger))
	{
	default:
		break;
	case TSoftBindClientTrigger::TLoadScene:
		mParent.LoadScene(in_data);
		break;
	case TSoftBindClientTrigger::TRequestNextMission:
		mParent.RequestNextMission();
		break;
	case TSoftBindClientTrigger::TOnOpponenetDeadSequence:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TOpponentDead, in_data);
		break;	
	case TSoftBindClientTrigger::TOn2OpponenetLeftSequence:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TOpponent2Left, in_data);
		break;
	case TSoftBindClientTrigger::TOnActorDeadSequence:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TActorDead, in_data);
		break;
	case TSoftBindClientTrigger::TOnNoOpponenetCinematicSequence:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TNoOpponentCinematic, in_data);
		break;
	case TSoftBindClientTrigger::TOnNoOpponenetAiSequence:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TNoOpponentAi, in_data);
		break;
	case TSoftBindClientTrigger::TVar0TrueSequence:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TVar0True, in_data);
		break;
	case TSoftBindClientTrigger::TVar0FalseSequence:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TVar0False, in_data);
		break;
	case TSoftBindClientTrigger::TSetVar0True:
		mParent.SetMissionVariable(ZombieMissionVar::TVariable::TVar0, true);
		break;
	case TSoftBindClientTrigger::TSetVar0False:
		mParent.SetMissionVariable(ZombieMissionVar::TVariable::TVar0, false);
		break;
	case TSoftBindClientTrigger::TSetAiActive:
		mParent.SetOpponentAiActive(in_data, true);
		break;
	case TSoftBindClientTrigger::TActorDestroy:
		mParent.ActorDestroy(in_data);
		break;
	case TSoftBindClientTrigger::TTriggerBossWave0:
		mParent.TriggerBossWave(0);
		break;
	case TSoftBindClientTrigger::TTriggerBossWave1:
		mParent.TriggerBossWave(1);
		break;
	}
	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestClientTriggerFloat(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const float in_value
	)
{
	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestClientTriggerString(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const std::string& in_value
	)
{
	switch (LocalGetSoftBindClientTrigger(in_clientTrigger))
	{
	default:
		break;
	case TSoftBindClientTrigger::TFactory:
		{
			std::string factoryStubName;
			std::string factoryData;
			LocalGetSoftBindSpawnActorData(factoryStubName, factoryData, in_data, mParent);
			mParent.InvokeFactory(
				in_value,
				factoryStubName,
				factoryData
				);
		}
		break;
	case TSoftBindClientTrigger::TCinematicText:
		{
			std::string factoryStubName;
			if ("PlayerSelf" == in_data)
			{
				factoryStubName = mParent.GetGameVar()->GetPlayerFactoryStubName(ZombieGameVar::GetPlayerIdSelf());
			}
			else if ("PlayerRescue" == in_data)
			{
				factoryStubName = mParent.GetGameVar()->GetPlayerFactoryStubName(ZombieGameVar::GetPlayerIdRescue());
			}
			else if ("PlayerOther" == in_data)
			{
				factoryStubName = mParent.GetGameVar()->GetPlayerFactoryStubName(ZombieGameVar::GetPlayerIdOther());
			}
			mParent.ActorCinematicDialog(in_data, factoryStubName + in_value);
		}
		break;
	case TSoftBindClientTrigger::TOnNamedOpponentDead:
		mParent.SetMissionVarConditionSequence(ZombieMissionVar::TCondition::TNamedOpponentDead, in_data, in_value);
		break;
	}

	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestClientTriggerVector(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const GVector3Float& in_value
	)
{
	switch (LocalGetSoftBindClientTrigger(in_clientTrigger))
	{
	default:
		break;
	case TSoftBindClientTrigger::TFactoryPosition:
		{
			mParent.SetFactoryPosition(in_value);
		}
		break;
	}
	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestClientTriggerVectorVector(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const GVector3Float& in_valueOne,
	const GVector3Float& in_valueTwo
	)
{
	switch (LocalGetSoftBindClientTrigger(in_clientTrigger))
	{
	default:
		break;
	case TSoftBindClientTrigger::TFactoryRotation:
		{
			mParent.SetFactoryAt(in_valueOne);
			mParent.SetFactoryUp(in_valueTwo);
		}
		break;
	}
	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestAnimateClientTrigger(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const float in_duration,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	switch (LocalGetSoftBindClientTrigger(in_clientTrigger))
	{
	default:
		break;
	case TSoftBindClientTrigger::TSetCameraType:
		{
			int type = LocalGetCameraType(in_data);
			mParent.SetCameraType(
				type,
				in_duration,
				in_transitionType
				);
		}
		break;
	}

	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestAnimateClientTriggerFloat(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const float in_value,
	const float in_duration,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestAnimateClientTriggerString(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const std::string& in_value,
	const float in_duration,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
}

/*virtual*/ void ZombieMissionClient::OnRequestAnimateClientTriggerVector(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const GVector3Float& in_value,
	const float in_duration,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	switch (LocalGetSoftBindClientTrigger(in_clientTrigger))
	{
	default:
		break;
	case TSoftBindClientTrigger::TSetPosition:
		{
			const int cameraType = LocalGetCameraType(in_data);
			if (GCOMMON_INVALID_INDEX != cameraType)
			{
				mParent.SetCameraPosition(cameraType, in_value, in_duration, in_transitionType);
			}
			else
			{
				mParent.SetActorPosition(in_data, in_value, in_duration, in_transitionType);
			}
		}
		break;
	}

	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestAnimateClientTriggerVectorVector(
	const std::string& in_clientTrigger,
	const std::string& in_data,
	const GVector3Float& in_valueOne,
	const GVector3Float& in_valueTwo,
	const float in_duration,
	const GComponentLerp::TType::TEnum in_transitionType
	)
{
	switch (LocalGetSoftBindClientTrigger(in_clientTrigger))
	{
	default:
		break;
	case TSoftBindClientTrigger::TSetRotation:
		{
			const int cameraType = LocalGetCameraType(in_data);
			if (GCOMMON_INVALID_INDEX != cameraType)
			{
				mParent.SetCameraRotation(cameraType, in_valueOne, in_valueTwo, in_duration, in_transitionType);
			}
			else
			{
				mParent.SetActorRotation(in_data, in_valueOne, in_valueTwo, in_duration, in_transitionType);
			}
		}
		break;
	}

	return;
}

/*virtual*/ void ZombieMissionClient::OnRequestPlayAnimation(
	const std::string& in_target,
	const std::string& in_animationName,
	const GAnimationPlayParam& in_playParam
	)
{
	mParent.AnimateActor(in_target, in_animationName, in_playParam);
	return;
}
