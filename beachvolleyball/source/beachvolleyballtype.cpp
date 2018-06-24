//
//  BeachVolleyBallType.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 22
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallType.h"

//static public methods
/*static*/ const std::string BeachVolleyBallType::GetFactoryName(
	const TGirl::TEnum in_girl,
	const TMatch::TEnum in_match
	)
{
	return std::string("factory") + GetUniqueName(in_girl, in_match);
}

/*static*/ const std::string BeachVolleyBallType::GetUniqueName(
	const TGirl::TEnum in_girl,
	const TMatch::TEnum in_match
	)
{
	switch (in_girl)
	{
	default:
		break;
	case TGirl::TBikini:
		switch (in_match)
		{
		default:
			break;
		case TMatch::TOne:
			return "bikini0";
		case TMatch::TTwo:
			return "bikini1";
		case TMatch::TThree:
			return "bikini2";
		}
		break;
	case TGirl::TReiko:
		switch (in_match)
		{
		default:
			break;
		case TMatch::TOne:
			return "reiko0";
		case TMatch::TTwo:
			return "reiko1";
		case TMatch::TThree:
			return "reiko2";
		}
		break;
	case TGirl::TApril:
		switch (in_match)
		{
		default:
			break;
		case TMatch::TOne:
			return "april0";
		case TMatch::TTwo:
			return "april1";
		case TMatch::TThree:
			return "april2";
		}
		break;
	case TGirl::TCuda:
		switch (in_match)
		{
		default:
			break;
		case TMatch::TOne:
			return "cuda0";
		case TMatch::TTwo:
			return "cuda1";
		case TMatch::TThree:
			return "cuda2";
		}
		break;
	}

	return "";
}
