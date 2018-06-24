//
//  gapplicationtask.cpp
//  GCommon
//
//  Created by David Coen on 2011 05 30
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gapplicationtask.h"
#include "gapplicationwindow.h"

///////////////////////////////////////////////////////
//constructor
GApplicationTask::GApplicationTask()
{
	return;
}

/*virtual*/ GApplicationTask::~GApplicationTask()
{
	return;
}

///////////////////////////////////////////////////////
//public methods
const GBOOL GApplicationTask::Tick(const GR32 in_timeDelta)
{
	return OnTick(in_timeDelta);
}

void GApplicationTask::SetActive(const GBOOL in_active)
{
	OnSetActive(in_active);
	return;
}


///////////////////////////////////////////////////////
//interface
/*virtual*/ const GBOOL GApplicationTask::OnTick(const GR32)
{
	return false;
}

/*virtual*/ void GApplicationTask::OnSetActive(const GBOOL)
{
	return;
}


