//
//  BeachVolleyBallGameVar.h
//  BeachVolleyBall
//
//  Created by David Coen on 2011 05 02
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _BeachVolleyBallGameVar_h_
#define _BeachVolleyBallGameVar_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "BeachVolleyBallType.h"

class GDictionary;
class GFileSystem;

class BeachVolleyBallGameVar : public boost::noncopyable
{
	//typedef
private:
	typedef boost::scoped_ptr<GDictionary> TPointerDictionary;
	typedef boost::shared_ptr<GFileSystem> TPointerFileSystem;
	typedef boost::weak_ptr<GFileSystem> TWeakFileSystem;

	//constructor
public:
	BeachVolleyBallGameVar(
		TPointerFileSystem& inout_fileSystem
		);
	~BeachVolleyBallGameVar();

	//public methods
public:
	void NotifyPlayerWin(
		const BeachVolleyBallType::TGirl::TEnum in_againstGirl,
		const BeachVolleyBallType::TMatch::TEnum in_match
		);
	void NotifyOpponentWin(
		const BeachVolleyBallType::TGirl::TEnum in_byGirl,
		const BeachVolleyBallType::TMatch::TEnum in_match
		);

	//public accessors
public:
	const bool MenuHasPlay(const BeachVolleyBallType::TGirl::TEnum in_girl);
	const bool MenuHasPlayMatch(
		const BeachVolleyBallType::TGirl::TEnum in_girl,
		const BeachVolleyBallType::TMatch::TEnum in_match		
		);

	const bool MenuHasWatch(const BeachVolleyBallType::TGirl::TEnum in_girl);
	const bool MenuHasWatchMatch(
		const BeachVolleyBallType::TGirl::TEnum in_girl,
		const BeachVolleyBallType::TMatch::TEnum in_match		
		);

	//private methods
private:
	void LoadDictionary();
	void SaveDictionary();

	//private members
private:
	TWeakFileSystem mFileSystem;
	TPointerDictionary mDictionary;

};

#endif //_BeachVolleyBallGameVar_h_
