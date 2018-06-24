//
//  GGuiNode.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNode.h"

#include "GAssert.h"
#include "GGui.h"
#include "GGuiType.h"
#include "GGuiNodeLoad.h"
#include "GGuiNode.h"
#include "GGuiNodeComponentBase.h"
#include "GGuiNodeComponentBaseLoad.h"
#include "GGuiNodeComponentAnimation.h"
#include "GGuiNodeComponentButton.h"
#include "GGuiNodeComponentPanel.h"
#include "GGuiNodeComponentText.h"
#include "GGuiNodeComponentVisual.h"
#include "GMath.h"

typedef boost::shared_ptr<GGuiNodeComponentBase> TPointerGuiNodeComponentBase;
typedef std::vector<GGuiNodeComponentPostLoadData> TArrayGuiNodeComponentPostLoadData;

struct TGuiNodeFlag
{
	enum TEnum
	{
		TNone							= 0x00000000
		,TSquareMinimum					= 0x00000001
		,TSquareMaximum					= 0x00000002
		,TTransformDirty				= 0x00000004

		,TComponentFlagDirty			= 0x00000008 //component added or removed to this node
		,TComponentFlagHierarchyDirty	= 0x00000010 //child node added or removed, could be grandchild, great grand child ...

	};
};

static void LocalWriteFlag(unsigned int& inout_flag, const unsigned int in_bit, const bool in_active)
{
	if (in_active == (in_bit == (inout_flag & in_bit)))
	{
		return;
	}
	if (in_active)
	{
		inout_flag |= in_bit;
	}
	else
	{
		inout_flag &= ~in_bit;
	}
	return;
}

TPointerGuiNodeComponentBase LocalCreateComponent(
	const GGuiNodeComponentBaseLoad& in_componentBaseLoad,
	GGuiManager& inout_manager,
	GGuiNode& in_guiNode,
	TArrayGuiNodeComponentPostLoadData& inout_arrayPostLoad
	)
{
	TPointerGuiNodeComponentBase pointer;
	switch (in_componentBaseLoad.GetType())
	{
	default:
		break;
	case GGuiType::TComponentFlag::TAnimation:
		pointer = GGuiNodeComponentAnimation::Factory(
			in_componentBaseLoad.GetData(),
			inout_manager,
			in_guiNode,
			inout_arrayPostLoad
			);
		break;
	case GGuiType::TComponentFlag::TButton:
		pointer = GGuiNodeComponentButton::Factory(
			in_componentBaseLoad.GetData(),
			inout_manager,
			in_guiNode,
			inout_arrayPostLoad
			);
		break;
	case GGuiType::TComponentFlag::TPanel:
		pointer = GGuiNodeComponentPanel::Factory(
			in_componentBaseLoad.GetData(),
			inout_manager,
			in_guiNode,
			inout_arrayPostLoad
			);
		break;
	case GGuiType::TComponentFlag::TText:
		pointer = GGuiNodeComponentText::Factory(
			in_componentBaseLoad.GetData(),
			inout_manager,
			in_guiNode,
			inout_arrayPostLoad
			);
		break;
	//case GGuiType::TComponentFlag::TVisual:
	//	pointer = GGuiNodeComponentVisual::Factory(
	//		in_componentBaseLoad.GetData(),
	//		inout_manager,
	//		in_guiNode,
	//		inout_arrayPostLoad
	//		);
	//	break;
	}

	return pointer;
}

//static public methods
/*static*/ GGuiNode::TPointerGuiNode GGuiNode::Factory(
	const GGuiNodeLoad& in_guiNodeLoad,
	GGuiManager& inout_manager,
	TArrayGuiNodeComponentPostLoadData& inout_arrayPostLoad
	)
{
	TPointerGuiNode pointer;

	pointer.reset(new GGuiNode(
		in_guiNodeLoad.GetName(),
		in_guiNodeLoad.GetFlag(),
		in_guiNodeLoad.GetTopLeft(),
		in_guiNodeLoad.GetSize()
		));

	for (int index = 0; index < (int)in_guiNodeLoad.GetArrayComponentCount(); ++index)
	{
		TPointerGuiNodeComponentBase pointerComponent = LocalCreateComponent(
			in_guiNodeLoad.GetArrayComponent()[index],
			inout_manager,
			*pointer,
			inout_arrayPostLoad
			);

		pointer->AddComponent(pointerComponent);
	}

	for (int index = 0; index < (int)in_guiNodeLoad.GetArrayGuiNodeCount(); ++index)
	{
		TPointerGuiNode pointerChildNode = Factory(
			in_guiNodeLoad.GetArrayGuiNode()[index],
			inout_manager,
			inout_arrayPostLoad
			);

		pointer->AddChild(pointerChildNode);
	}

	return pointer;
}

//constructor
GGuiNode::GGuiNode(
	const std::string& in_name,
	const GU32 in_flag,
	const GGuiCoordinate& in_topLeft,
	const GGuiCoordinate& in_size
	)
	: mFlag(TGuiNodeFlag::TTransformDirty | in_flag)
	, mName(in_name)
	, mTopLeft(in_topLeft)
	, mSize(in_size)
	, mTransformChangeId(0)
	, mParent(NULL)
	, mArrayChildren()
	, mArrayComponentBase()
	, mComponentComposititeFlag(0)
	, mChildComponentComposititeFlag(0)
	, mTransformScreenTopLeft()
	, mTransformScreenBottomRight()
{
	return;
}

GGuiNode::~GGuiNode()
{
	return;
}

//public methods
const unsigned int GGuiNode::GetComponentComposititeFlag()
{
	if (0 != (mFlag & TGuiNodeFlag::TComponentFlagDirty))
	{
		mComponentComposititeFlag = 0;
		for (TArrayPointerGuiNodeComponentBase::iterator iterator = mArrayComponentBase.begin(); iterator != mArrayComponentBase.end(); ++iterator)
		{
			mComponentComposititeFlag |= (*iterator)->GetComponentFlag();
		}
	}

	mFlag &= ~TGuiNodeFlag::TComponentFlagDirty;

	return mComponentComposititeFlag;
}

const unsigned int GGuiNode::GetChildComponentComposititeFlag()
{
	if (0 != (mFlag & TGuiNodeFlag::TComponentFlagHierarchyDirty))
	{
		mChildComponentComposititeFlag = 0;
		for (TArrayPointerGuiNode::iterator iterator = mArrayChildren.begin(); iterator != mArrayChildren.end(); ++iterator)
		{
			mChildComponentComposititeFlag |= (*iterator)->GetComponentComposititeFlag();
			mChildComponentComposititeFlag |= (*iterator)->GetChildComponentComposititeFlag();
		}

		mFlag &= ~TGuiNodeFlag::TComponentFlagHierarchyDirty;
	}

	return mChildComponentComposititeFlag;

}

void GGuiNode::AddChild(
	TPointerGuiNode& inout_childNode
	)
{
	if (!inout_childNode)
	{
		return;
	}
	mArrayChildren.push_back(inout_childNode);

	GASSERT(NULL == inout_childNode->mParent, "invalid state");
	inout_childNode->mParent = this;

	WriteFlagUpTree(TGuiNodeFlag::TComponentFlagHierarchyDirty, true);

	return;
}

void GGuiNode::RemoveChild(
	TPointerGuiNode& inout_childNode
	)
{
	if (!inout_childNode)
	{
		return;
	}

	for (TArrayPointerGuiNode::iterator iterator = mArrayChildren.begin(); iterator != mArrayChildren.end(); ++iterator)
	{
		if (inout_childNode == (*iterator))
		{
			GASSERT(this == inout_childNode->mParent, "invalid state");
			inout_childNode->mParent = this;
			mArrayChildren.erase(iterator);
			return;
		}
	}

	WriteFlagUpTree(TGuiNodeFlag::TComponentFlagHierarchyDirty, true);

	return;
}

void GGuiNode::AddComponent(
	TPointerGuiNodeComponentBase& inout_component
	)
{
	if (!inout_component)
	{
		return;
	}

	mArrayComponentBase.push_back(inout_component);

	mFlag |= TGuiNodeFlag::TComponentFlagDirty;
	return;
}

void GGuiNode::RemoveComponent(
	TPointerGuiNodeComponentBase& inout_component
	)
{
	if (!inout_component)
	{
		return;
	}

	for (TArrayPointerGuiNodeComponentBase::iterator iterator = mArrayComponentBase.begin(); iterator != mArrayComponentBase.end(); ++iterator)
	{
		if (inout_component == (*iterator))
		{
			mArrayComponentBase.erase(iterator);
			return;
		}
	}

	mFlag |= TGuiNodeFlag::TComponentFlagDirty;
	return;
}

void GGuiNode::SetFlagTransformDirty()
{
	//WriteFlagDownTree(TGuiNodeFlag::TTransformDirty, true);
	mTransformChangeId += 1;
	LocalWriteFlag(mFlag, TGuiNodeFlag::TTransformDirty, true);

	for (TArrayPointerGuiNode::iterator iterator = mArrayChildren.begin(); iterator != mArrayChildren.end(); ++iterator)
	{
		(*iterator)->SetFlagTransformDirty();
	}

	return;
}

//public accessors
void GGuiNode::SetTopLeft(const GGuiCoordinate& in_topLeft)
{
	mTopLeft = in_topLeft;
	SetFlagTransformDirty();
	return;
}

void GGuiNode::SetSize(const GGuiCoordinate& in_size)
{
	mSize = in_size;
	SetFlagTransformDirty();
	return;
}

const int GGuiNode::GetComponentCount()const
{
	return (int)mArrayComponentBase.size();
}

const GVector2Float& GGuiNode::GetTransformScreenTopLeft(const GGui& in_gui)
{
	if (0 != (mFlag & TGuiNodeFlag::TTransformDirty))
	{
		UpdateTransform(in_gui);
		mFlag &= ~TGuiNodeFlag::TTransformDirty;
	}

	return mTransformScreenTopLeft;
}

const GVector2Float& GGuiNode::GetTransformScreenBottomRight(const GGui& in_gui)
{
	if (0 != (mFlag & TGuiNodeFlag::TTransformDirty))
	{
		UpdateTransform(in_gui);
		mFlag &= ~TGuiNodeFlag::TTransformDirty;
	}
	return mTransformScreenBottomRight;
}

//private methods
void GGuiNode::WriteFlagDownTree(const unsigned int in_flag, const bool in_active)
{
	LocalWriteFlag(mFlag, in_flag, in_active);
	for (TArrayPointerGuiNode::iterator iterator = mArrayChildren.begin(); iterator != mArrayChildren.end(); ++iterator)
	{
		(*iterator)->WriteFlagDownTree(in_flag, in_active);
	}
	return;
}

void GGuiNode::WriteFlagUpTree(const unsigned int in_flag, const bool in_active)
{
	LocalWriteFlag(mFlag, in_flag, in_active);
	if (mParent)
	{
		mParent->WriteFlagUpTree(in_flag, in_active);
	}
	return;
}

void GGuiNode::UpdateTransform(const GGui& in_gui)
{
	GVector2Float parentTopLeft;
	GVector2Float parentBottomRight;
	if (mParent)
	{
		parentTopLeft = mParent->GetTransformScreenTopLeft(in_gui);
		parentBottomRight = mParent->GetTransformScreenBottomRight(in_gui);
	}
	else
	{
		parentBottomRight = in_gui.GetScreenSize();
	}

	if (mFlag & TGuiNodeFlag::TSquareMinimum)
	{
		const float valueMinimum = GMath::Minimum(parentBottomRight.m_x - parentTopLeft.m_x, parentBottomRight.m_y - parentTopLeft.m_y); 
		mTransformScreenTopLeft = parentTopLeft + ((parentBottomRight - parentTopLeft - GVector2Float(valueMinimum, valueMinimum)) * 0.5F);
		mTransformScreenBottomRight = mTransformScreenTopLeft + GVector2Float(valueMinimum, valueMinimum);
	}
	else if (mFlag & TGuiNodeFlag::TSquareMaximum)
	{
		const float valueMaximum = GMath::Maximum(parentBottomRight.m_x - parentTopLeft.m_x, parentBottomRight.m_y - parentTopLeft.m_y); 
		mTransformScreenTopLeft = parentTopLeft + ((parentBottomRight - parentTopLeft - GVector2Float(valueMaximum, valueMaximum)) * 0.5F);
		mTransformScreenBottomRight = mTransformScreenTopLeft + GVector2Float(valueMaximum, valueMaximum);
	}
	else
	{
		mTransformScreenTopLeft = parentTopLeft + mTopLeft.GetPixelOffset() + ((parentBottomRight - parentTopLeft) * mTopLeft.GetOriginRatio());
		mTransformScreenBottomRight = mTransformScreenTopLeft + mSize.GetPixelOffset() + ((parentBottomRight - parentTopLeft) * mSize.GetOriginRatio());
	}

	mTransformChangeId += 1;

	return;
}
