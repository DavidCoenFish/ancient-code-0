//
//  gmaterialinstance.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmaterialinstance.h"

#include "gmaterial.h"
#include "gmaterialtype.h"
#include "gresourcemanager.h"
#include "gstringutility.h"

////////////////////////////////////////////////////
//static public methods
/*static*/ GMaterialInstance::TPointerMaterialInstance GMaterialInstance::Factory(
	GResourceManager& inout_resourceManager, //load textures/ shaders
	const GMaterial& in_material,
	const GBOOL in_bindTexture,
	const GBOOL in_allowRuntimeBindTexture //can texture be bound/ unbound during it's scope (else bind/unbind on scope)
	)
{
	TPointerMaterialInstance pointerMaterialInstance;

	TPointerTextureInstance pointerTexture;
	if (in_material.GetTextureDataName())
	{
		pointerTexture = inout_resourceManager.GetTextureInstance(
			GStringUtility::SafeString(in_material.GetTextureDataName()),
			in_bindTexture,
			in_allowRuntimeBindTexture
			).lock();
	}

	TPointerShaderInstance pointerShader;
	if (in_material.GetVertexShaderFileName() && in_material.GetFragmentShaderFileName())
	{
		pointerShader = inout_resourceManager.GetShaderInstance(
			GStringUtility::SafeString(in_material.GetVertexShaderFileName()),
			GStringUtility::SafeString(in_material.GetFragmentShaderFileName()),
			in_material.GetArrayAttributeNameCount(),
			in_material.GetArrayAttributeName(),
			in_material.GetArrayUniformNameCount(),
			in_material.GetArrayUniformName()
			).lock();
	}

	pointerMaterialInstance.reset(new GMaterialInstance(
		in_material.GetColour(),
		in_material.GetAlphaBlendSource(),
		in_material.GetAlphaBlendDestination(),
		in_material.GetFlag(),
		pointerTexture,
		pointerShader
		));

	return pointerMaterialInstance;
}

/*static*/ GMaterialInstance::TPointerMaterialInstance GMaterialInstance::FactoryManualShader(
	GResourceManager& inout_resourceManager, //load textures/ shaders
	const std::string& in_name,
	const std::string& in_vertexShader,
	const std::string& in_fragmentShader,
	const GMaterial& in_material,
	const GBOOL in_bindTexture,
	const GBOOL in_allowRuntimeBindTexture //can texture be bound/ unbound during it's scope (else bind/unbind on scope)
	)
{
	TPointerMaterialInstance pointerMaterialInstance;

	TPointerTextureInstance pointerTexture;
	if (in_material.GetTextureDataName())
	{
		pointerTexture = inout_resourceManager.GetTextureInstance(
			GStringUtility::SafeString(in_material.GetTextureDataName()),
			in_bindTexture,
			in_allowRuntimeBindTexture
			).lock();
	}

	TPointerShaderInstance pointerShader;
	if (!in_vertexShader.empty() && !in_fragmentShader.empty())
	{
		pointerShader = inout_resourceManager.GetShaderInstanceSource(
			in_name,
			in_vertexShader,
			in_fragmentShader,
			in_material.GetArrayAttributeNameCount(),
			in_material.GetArrayAttributeName(),
			in_material.GetArrayUniformNameCount(),
			in_material.GetArrayUniformName()
			).lock();
	}

	pointerMaterialInstance.reset(new GMaterialInstance(
		in_material.GetColour(),
		in_material.GetAlphaBlendSource(),
		in_material.GetAlphaBlendDestination(),
		in_material.GetFlag(),
		pointerTexture,
		pointerShader
		));

	return pointerMaterialInstance;
}

////////////////////////////////////////////////////
//constructor
GMaterialInstance::GMaterialInstance(
	const GColour4Float& in_colour,
	const GMaterialType::TBlendMode::TEnum in_alphaBlendSource,
	const GMaterialType::TBlendMode::TEnum in_alphaBlendDestination,
	const GMaterialType::TState::TFlag in_flag,
	TPointerTextureInstance& inout_textureInstance,
	TPointerShaderInstance& inout_shaderInstance
	)
: mColour(in_colour)
, mAlphaBlendSource(in_alphaBlendSource)
, mAlphaBlendDestination(in_alphaBlendDestination)
, mFlag(in_flag)
, mWeakTextureInstance(inout_textureInstance)
, mWeakShaderInstance(inout_shaderInstance)
{
	return;
}

GMaterialInstance::GMaterialInstance(const GMaterialInstance& in_src)
: mColour()
, mAlphaBlendSource(GMaterialType::TBlendMode::TZero)
, mAlphaBlendDestination(GMaterialType::TBlendMode::TZero)
, mFlag(GMaterialType::TState::TNone)
, mWeakTextureInstance()
, mWeakShaderInstance()
{
	(*this) = in_src;
	return;
}

GMaterialInstance::GMaterialInstance()
: mColour()
, mAlphaBlendSource(GMaterialType::TBlendMode::TZero)
, mAlphaBlendDestination(GMaterialType::TBlendMode::TZero)
, mFlag(GMaterialType::TState::TNone)
, mWeakTextureInstance()
, mWeakShaderInstance()
{
	return;
}

GMaterialInstance::~GMaterialInstance()
{
	return;
}

const GMaterialInstance& GMaterialInstance::operator=(const GMaterialInstance& in_rhs)
{
	if (this != &in_rhs)
	{
		mColour = in_rhs.mColour;
		mAlphaBlendSource = in_rhs.mAlphaBlendSource;
		mAlphaBlendDestination = in_rhs.mAlphaBlendDestination;
		mFlag = in_rhs.mFlag;
		mWeakTextureInstance = in_rhs.mWeakTextureInstance;
		mWeakShaderInstance = in_rhs.mWeakShaderInstance;
	}

	return (*this);
}

////////////////////////////////////////////////////
//public accessors
const bool GMaterialInstance::GetFlagFaceCW()const
{
	return (0 != (mFlag & GMaterialType::TState::TFaceCW));
}

const bool GMaterialInstance::GetFlagFaceCull()const
{
	return (0 != (mFlag & GMaterialType::TState::TFaceCull));
}

const bool GMaterialInstance::GetFlagAlphaBlend()const
{
	return (0 != (mFlag & GMaterialType::TState::TAlphaBlend));
}

const bool GMaterialInstance::GetFlagColourWrite()const
{
	return (0 != (mFlag & GMaterialType::TState::TColourWrite));
}

const bool GMaterialInstance::GetFlagDepthRead()const
{
	return (0 != (mFlag & GMaterialType::TState::TDepthRead));
}

const bool GMaterialInstance::GetFlagDepthWrite()const
{
	return (0 != (mFlag & GMaterialType::TState::TDepthWrite));
}

const bool GMaterialInstance::GetFlagStencilShadowFront()const
{
	return (0 != (mFlag & GMaterialType::TState::TStencilShadowFront));
}
const bool GMaterialInstance::GetFlagStencilShadowBack()const
{
	return (0 != (mFlag & GMaterialType::TState::TStencilShadowBack));
}
const bool GMaterialInstance::GetFlagStencilShadow()const
{
	return (0 != (mFlag & GMaterialType::TState::TStencilShadow));
}

const bool GMaterialInstance::GetFlagUseLight()const
{
	return (0 != (mFlag & GMaterialType::TState::TUseLight));
}

const bool GMaterialInstance::GetFlagUseTexture()const
{
	return (0 != (mFlag & GMaterialType::TState::TUseTexture));
}

const bool GMaterialInstance::GetFlagDoBasePass()const
{
	return (0 != (mFlag & GMaterialType::TState::TDoBasePass));
}

const bool GMaterialInstance::GetFlagDoEnvironment()const
{
	return (0 != (mFlag & GMaterialType::TState::TDoEnvironment));
}
