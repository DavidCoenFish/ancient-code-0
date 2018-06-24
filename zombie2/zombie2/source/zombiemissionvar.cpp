//
//  ZombieMissionVar.cpp
//  Zombie
//
//  Created by David Coen on 2011 03 15
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieMissionVar.h"
#include <GMissionMaster.h>

//constructor
ZombieMissionVar::ZombieMissionVar()
: mArrayCondition()
//, mArrayVar[TVariable::TCount]
{
	for (int index = 0; index < TVariable::TCount; ++index)
	{
		mArrayVar[index] = false;
	}

	return;
}

ZombieMissionVar::~ZombieMissionVar()
{
	return;
}

//public methods
//pass all the variables down from 
void ZombieMissionVar::Tick(
	GMissionMaster& inout_missionMaster, //trigger sequences
	const TListClientObjectBase& in_listOpponents,
	const TListClientObjectBase& in_listActors
	)
{
	TArrayTPairPairConditionSubjectStringString localArray(mArrayCondition);
	mArrayCondition.clear();
	TArrayTPairPairConditionSubjectStringString::iterator iterator = localArray.begin();
	while (iterator != localArray.end())
	{
		//evalue condition
		if (EvalueCondition(
			(*iterator).first.first, 
			(*iterator).first.second, 
			in_listOpponents,
			in_listActors
			))
		{
			inout_missionMaster.TriggerSequence((*iterator).second);
			iterator = localArray.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}
	
	mArrayCondition.insert(mArrayCondition.end(), localArray.begin(), localArray.end());

	return;
}

//public accessors
void ZombieMissionVar::SetConditionSequence(
	const TCondition::TEnum in_condition, 
	const std::string& in_sequence,
	const std::string& in_name
	)
{
	mArrayCondition.push_back(TPairPairConditionSubjectStringString(
		TPairConditionSubjectString(in_condition, in_name),
		in_sequence
		));
	return;
}

//default is false
void ZombieMissionVar::SetVariable(
	const TVariable::TEnum in_var,
	const bool in_value
	)
{
	mArrayVar[in_var] = in_value;
	return;
}

//private methods
const bool ZombieMissionVar::EvalueCondition(
	const TCondition::TEnum in_condition,
	const std::string& in_name,
	const TListClientObjectBase& in_listOpponents,
	const TListClientObjectBase& in_listActors
	)
{
	switch (in_condition)
	{
	default:
		break;
	case TCondition::TOpponentDead:
		if (in_listOpponents.size() <= 0)
		{
			return true;
		}
		break;
	case TCondition::TOpponent2Left:
		if (in_listOpponents.size() <= 2)
		{
			return true;
		}
		break;
	case TCondition::TNamedOpponentDead:
		{
			bool found = false;
			for (TListClientObjectBase::const_iterator iterator = in_listOpponents.begin(); iterator != in_listOpponents.end(); ++iterator)
			{
				const SceneNodeComponentClientObjectBase& objectBase = (*iterator);
				if (in_name == objectBase.GetName())
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				return true;
			}
		}
		break;
	case TCondition::TActorDead:
		if (in_listActors.size() <= 0)
		{
			return true;
		}
		break;
	case TCondition::TNoOpponentCinematic:
		for (TListClientObjectBase::const_iterator iterator = in_listOpponents.begin(); iterator != in_listOpponents.end(); ++iterator)
		{
			const SceneNodeComponentClientObjectBase& objectBase = (*iterator);
			if (!objectBase.GetAiActive())
			{
				return false;
			}
		}
		return true;
	case TCondition::TNoOpponentAi:
		for (TListClientObjectBase::const_iterator iterator = in_listOpponents.begin(); iterator != in_listOpponents.end(); ++iterator)
		{
			const SceneNodeComponentClientObjectBase& objectBase = (*iterator);
			if (objectBase.GetAiActive())
			{
				return false;
			}
		}
		return true;
	case TCondition::TVar0True:
		return (mArrayVar[TVariable::TVar0] == true);
	case TCondition::TVar0False:
		return (mArrayVar[TVariable::TVar0] != true);
	}

	return false;
}
