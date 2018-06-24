//
//  GGuiNodeComponentBase.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentBase.h"

//constructor
GGuiNodeComponentBase::GGuiNodeComponentBase(const unsigned int in_componentFlag)
: mComponentFlag(in_componentFlag)
{
	return;
}

/*virtual*/ GGuiNodeComponentBase::~GGuiNodeComponentBase()
{
	return;
}

//public methods
void GGuiNodeComponentBase::PostLoad(
	const int in_passCount,
	GGuiManager& inout_manager,
	GGuiNode& inout_parent,
	void* const in_data,
	TArrayGuiNodeComponentPostLoadData& inout_arrayComponentBase
	)
{
	OnPostLoad(in_passCount, inout_manager, inout_parent, in_data, inout_arrayComponentBase);
	return;
}

//private interface
/*virtual*/ void GGuiNodeComponentBase::OnPostLoad(
	const int,
	GGuiManager&,
	GGuiNode&,
	void* const,
	TArrayGuiNodeComponentPostLoadData&
	)
{
	return;
}
