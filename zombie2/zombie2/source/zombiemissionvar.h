//
//  ZombieMissionVar.h
//  Zombie
//
//  Created by David Coen on 2011 03 15
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieMissionVar_h_
#define _ZombieMissionVar_h_

#include <boost/noncopyable.hpp>
#include <boost/intrusive/list.hpp>
#include <vector>
#include "SceneNodeComponentClientObjectBase.h"

class GMissionMaster;

class ZombieMissionVar : public boost::noncopyable
{
	//typedef
public:
	struct TCondition
	{
		enum TEnum
		{
			TOpponentDead = 0,
			TOpponent2Left,
			TNamedOpponentDead,
			TActorDead,
			TNoOpponentCinematic,
			TNoOpponentAi,
			TVar0True, // default is false?
			TVar0False,

			TCount
		};
	};

	struct TVariable
	{
		enum TEnum
		{
			TVar0,

			TCount
		};
	};
private:
	typedef std::pair<TCondition::TEnum, std::string> TPairConditionSubjectString;
	typedef std::pair<TPairConditionSubjectString, std::string> TPairPairConditionSubjectStringString;
	typedef std::vector<TPairPairConditionSubjectStringString> TArrayTPairPairConditionSubjectStringString;
	typedef std::vector<TCondition::TEnum> TArrayCondition;

	typedef boost::intrusive::list< 
		SceneNodeComponentClientObjectBase, 
		boost::intrusive::member_hook<SceneNodeComponentClientObjectBase, SceneNodeComponentClientObjectBase::TListHook, &SceneNodeComponentClientObjectBase::mListHook>,
		boost::intrusive::constant_time_size<false>
		> TListClientObjectBase;

	//constructor
public:
	ZombieMissionVar();
	~ZombieMissionVar();

	//public methods
public:
	//pass all the variables down from 
	void Tick(
		GMissionMaster& inout_missionMaster, //trigger sequences
		const TListClientObjectBase& in_listOpponents,
		const TListClientObjectBase& in_listActors
		);

	//public accessors
public:
	void SetConditionSequence(
		const TCondition::TEnum in_condition, 
		const std::string& in_sequence,
		const std::string& in_name
		);

	//default is false
	void SetVariable(
		const TVariable::TEnum in_var,
		const bool in_value
		);

	//private methods
private:
	const bool EvalueCondition(
		const TCondition::TEnum in_condition,
		const std::string& in_name,
		const TListClientObjectBase& in_listOpponents,
		const TListClientObjectBase& in_listActors
		);

	//private members
private:
	TArrayTPairPairConditionSubjectStringString mArrayCondition;
	bool mArrayVar[TVariable::TCount];

};

#endif //_ZombieMissionVar_h_
