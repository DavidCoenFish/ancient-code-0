//
//  GGuiNodeComponentAnimation.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentAnimation.h"
#include "GGuiType.h"

//static public methods
/*static*/ GGuiNodeComponentAnimation::TPointerGuiNodeComponentBase GGuiNodeComponentAnimation::Factory(
	const void* const in_data,
	GGuiManager& inout_manager,
	GGuiNode& inout_parent,
	TArrayGuiNodeComponentPostLoadData& inout_arrayPostLoad
	)
{
	return TPointerGuiNodeComponentBase();
}

/*static*/ const unsigned int GGuiNodeComponentAnimation::GetComponentFlag()
{
	return GGuiType::TComponentFlag::TAnimation;
}

//constructor
GGuiNodeComponentAnimation::GGuiNodeComponentAnimation()
: GGuiNodeComponentBase(GetComponentFlag())
{
	return;
}

/*virtual*/ GGuiNodeComponentAnimation::~GGuiNodeComponentAnimation()
{
	return;
}

//implement GGuiNodeComponentBase
/*virtual*/ void GGuiNodeComponentAnimation::OnPostLoad(
	const int in_passCount,
	GGuiManager& inout_manager,
	GGuiNode& inout_parent,
	void* const in_data,
	TArrayGuiNodeComponentPostLoadData& inout_arrayComponentBase
	)
{
	return;
}