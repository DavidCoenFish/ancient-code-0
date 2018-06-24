//
//  BeachVolleyBallGameVar.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallGameVar.h"

#include <GBuffer.h>
#include <GDictionary.h>
#include <GFileSystem.h>

static const char* const sDictionaryFileName = "gamevar.data";

//constructor
BeachVolleyBallGameVar::BeachVolleyBallGameVar(
	TPointerFileSystem& inout_fileSystem
	)
	: mFileSystem(inout_fileSystem)
	, mDictionary()
{
	mDictionary.reset(new GDictionary);
	if (mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TBikini, BeachVolleyBallType::TMatch::TOne), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TBikini, BeachVolleyBallType::TMatch::TTwo), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TBikini, BeachVolleyBallType::TMatch::TThree), false);

		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TReiko, BeachVolleyBallType::TMatch::TOne), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TReiko, BeachVolleyBallType::TMatch::TTwo), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TReiko, BeachVolleyBallType::TMatch::TThree), false);

		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TApril, BeachVolleyBallType::TMatch::TOne), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TApril, BeachVolleyBallType::TMatch::TTwo), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TApril, BeachVolleyBallType::TMatch::TThree), false);

		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TCuda, BeachVolleyBallType::TMatch::TOne), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TCuda, BeachVolleyBallType::TMatch::TTwo), false);
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TCuda, BeachVolleyBallType::TMatch::TThree), false);
	}

	LoadDictionary();

	return;
}

BeachVolleyBallGameVar::~BeachVolleyBallGameVar()
{
	return;
}

//public methods
void BeachVolleyBallGameVar::NotifyPlayerWin(
	const BeachVolleyBallType::TGirl::TEnum in_againstGirl,
	const BeachVolleyBallType::TMatch::TEnum in_match
	)
{
	if (mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		dictionary.SetValueBool(BeachVolleyBallType::GetUniqueName(in_againstGirl, in_match), true);
	}
	SaveDictionary();
	return;
}

void BeachVolleyBallGameVar::NotifyOpponentWin(
	const BeachVolleyBallType::TGirl::TEnum in_byGirl,
	const BeachVolleyBallType::TMatch::TEnum in_match
	)
{
	//NOP
	return;
}

//public accessors
const bool BeachVolleyBallGameVar::MenuHasPlay(const BeachVolleyBallType::TGirl::TEnum in_girl)
{
#ifdef DEBUG
	return true;
#endif
	if (mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		switch (in_girl)
		{
		default:
			break;
		case BeachVolleyBallType::TGirl::TBikini:
			return true;
#ifndef FREE
		case BeachVolleyBallType::TGirl::TReiko:
			return (dictionary.GetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TBikini, BeachVolleyBallType::TMatch::TThree)));
		case BeachVolleyBallType::TGirl::TApril:
			return (dictionary.GetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TReiko, BeachVolleyBallType::TMatch::TThree)));
		case BeachVolleyBallType::TGirl::TCuda:
			return (dictionary.GetValueBool(BeachVolleyBallType::GetUniqueName(BeachVolleyBallType::TGirl::TApril, BeachVolleyBallType::TMatch::TThree)));
#endif
		}
	}

	return false;
}

const bool BeachVolleyBallGameVar::MenuHasPlayMatch(
	const BeachVolleyBallType::TGirl::TEnum in_girl,
	const BeachVolleyBallType::TMatch::TEnum in_match		
	)
{
#ifdef DEBUG
	return true;
#endif
#ifdef FREE
	if (BeachVolleyBallType::TGirl::TBikini != in_girl)
	{
		return false;
	}
#endif

	if (mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		switch (in_match)
		{
		default:
			break;
		case BeachVolleyBallType::TMatch::TOne:
			return MenuHasPlay(in_girl);
		case BeachVolleyBallType::TMatch::TTwo:
			return (dictionary.GetValueBool(BeachVolleyBallType::GetUniqueName(in_girl, BeachVolleyBallType::TMatch::TOne)));
		case BeachVolleyBallType::TMatch::TThree:
			return (dictionary.GetValueBool(BeachVolleyBallType::GetUniqueName(in_girl, BeachVolleyBallType::TMatch::TTwo)));
		}
	}

	return false;
}

const bool BeachVolleyBallGameVar::MenuHasWatch(const BeachVolleyBallType::TGirl::TEnum in_girl)
{
#ifdef DEBUG
	return true;
#endif
#ifdef FREE
	if (BeachVolleyBallType::TGirl::TBikini != in_girl)
	{
		return false;
	}
#endif
	if (mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		return dictionary.GetValueBool(BeachVolleyBallType::GetUniqueName(in_girl, BeachVolleyBallType::TMatch::TOne));
	}
	return false;
}

const bool BeachVolleyBallGameVar::MenuHasWatchMatch(
	const BeachVolleyBallType::TGirl::TEnum in_girl,
	const BeachVolleyBallType::TMatch::TEnum in_match		
	)
{
#ifdef DEBUG
	return true;
#endif
#ifdef FREE
	if (BeachVolleyBallType::TGirl::TBikini != in_girl)
	{
		return false;
	}
#endif
	if (mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		return dictionary.GetValueBool(BeachVolleyBallType::GetUniqueName(in_girl, in_match));
	}
	return false;
}


//private methods
void BeachVolleyBallGameVar::LoadDictionary()
{
	TPointerFileSystem fileSystem = mFileSystem.lock();
	if (fileSystem && mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		GBuffer buffer;
		const std::string filePath = fileSystem->AppendFileToPathName(fileSystem->GetWritableDir(), sDictionaryFileName);
		if (fileSystem->LoadFile(buffer, filePath, false))
		{
			GDictionary tempDictionary;
			GDictionary::LoadFromBuffer(tempDictionary, buffer);

			//don't union, cull garbage data...
			GDictionary::SelfUpdateIntersection(dictionary, tempDictionary);
		}
	}

	return;
}
void BeachVolleyBallGameVar::SaveDictionary()
{
	TPointerFileSystem fileSystem = mFileSystem.lock();
	if (fileSystem && mDictionary)
	{
		GDictionary& dictionary = *mDictionary;
		GBuffer buffer;

		GDictionary::WriteToBuffer(buffer, dictionary);
		
		const std::string filePath = fileSystem->AppendFileToPathName(fileSystem->GetWritableDir(), sDictionaryFileName);
		fileSystem->SaveFile(buffer, filePath);
	}

	return;
}
