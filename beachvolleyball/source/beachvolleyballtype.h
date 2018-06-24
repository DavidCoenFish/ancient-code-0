//
//  BeachVolleyBallType.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallType_h_
#define _BeachVolleyBallType_h_

#include <string>

class BeachVolleyBallType
{
	//typedef
public:
	struct TGirl
	{
		enum TEnum
		{
			TBikini = 0,
			TReiko,
			TApril,
			TCuda,
			TCount
		};
	};
	struct TMatch
	{
		enum TEnum
		{
			TOne = 0,
			TTwo,
			TThree,

			TCount
		};
	};

	//static public methods
public:
	static const std::string GetFactoryName(
		const TGirl::TEnum in_girl,
		const TMatch::TEnum in_match
		);

	static const std::string GetUniqueName(
		const TGirl::TEnum in_girl,
		const TMatch::TEnum in_match
		);

	//disabled
private:
	~BeachVolleyBallType();

};

#endif //_BeachVolleyBallType_h_
