//
//  ZombieMissionClient.h
//  Zombie
//
//  Created by David Coen on 2011 02 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieMissionClient_h_
#define _ZombieMissionClient_h_

#include <GMissionClient.h>

class ZombieMission;

class ZombieMissionClient : public GMissionClient
{
	//constructor
public:
	ZombieMissionClient(ZombieMission& in_parent);
	~ZombieMissionClient();

	//implement GMissionClient
private:
	virtual void OnRequestClientTrigger(
		const std::string& in_clientTrigger,
		const std::string& in_data
		);
	virtual void OnRequestClientTriggerFloat(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const float in_value
		);
	virtual void OnRequestClientTriggerString(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const std::string& in_value
		);
	virtual void OnRequestClientTriggerVector(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const GVector3Float& in_value
		);
	virtual void OnRequestClientTriggerVectorVector(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const GVector3Float& in_valueOne,
		const GVector3Float& in_valueTwo
		);

	virtual void OnRequestAnimateClientTrigger(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const float in_duration,
		const GComponentLerp::TType::TEnum in_transitionType
		);
	virtual void OnRequestAnimateClientTriggerFloat(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const float in_value,
		const float in_duration,
		const GComponentLerp::TType::TEnum in_transitionType
		);
	virtual void OnRequestAnimateClientTriggerString(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const std::string& in_value,
		const float in_duration,
		const GComponentLerp::TType::TEnum in_transitionType
		);
	virtual void OnRequestAnimateClientTriggerVector(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const GVector3Float& in_value,
		const float in_duration,
		const GComponentLerp::TType::TEnum in_transitionType
		);
	virtual void OnRequestAnimateClientTriggerVectorVector(
		const std::string& in_clientTrigger,
		const std::string& in_data,
		const GVector3Float& in_valueOne,
		const GVector3Float& in_valueTwo,
		const float in_duration,
		const GComponentLerp::TType::TEnum in_transitionType
		);

	virtual void OnRequestPlayAnimation(
		const std::string& in_target,
		const std::string& in_animationName,
		const GAnimationPlayParam& in_playParam
		);

	//private members
private:
	ZombieMission& mParent;

};

#endif //_ZombieMissionClient_h_
