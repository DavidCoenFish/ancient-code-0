//
//  GMeshModifier.h
//
//  Created by David Coen on 2010 12 16
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GMeshModifier_h_
#define _GMeshModifier_h_

class GMeshModifierSkin;
class GMeshModifierCloth;
class GMeshModifierMorph;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GMeshModifier
{
public:
	//include by pipeline
	struct TType
	{
		enum TEnum
		{
			TSkin = 0,
			TCloth,
			TMorph,
			TCount
		};
	};

	//constructor
public:
	GMeshModifier(
		const TType::TEnum in_type,
		const void* const in_data
		);
	~GMeshModifier();

	//public accessors
public:
	//const TType::TEnum GetType()const{ return (TType::TEnum)mType; }
	const bool GetIsSkin()const;
	const bool GetIsCloth()const;
	const bool GetIsMorph()const;

	const GMeshModifierSkin& GetSkin()const;
	const GMeshModifierCloth& GetCloth()const;
	const GMeshModifierMorph& GetMorph()const;

	//private members;
private:
	//const char* const mModifierName;
	const unsigned int mType;
	const void* const mData;

};

#endif 