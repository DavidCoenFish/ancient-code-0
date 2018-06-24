//
//  gmaterialinstance.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmaterialinstance_h_
#define _gmaterialinstance_h_

#include "gcommon.h"
#include "gcolour4float.h"
#include "gmaterialtype.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GTextureInstance;
class GShaderInstance;
class GResourceManager;
class GMaterial;

class GMaterialInstance //shallow copyable
{
	//////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;
	typedef boost::weak_ptr<GTextureInstance> TWeakTextureInstance;

	typedef boost::shared_ptr<GShaderInstance> TPointerShaderInstance;
	typedef boost::weak_ptr<GShaderInstance> TWeakShaderInstance;

	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::shared_ptr<GResourceManager> TPointerResourceManager;

	////////////////////////////////////////////////////
	//static public methods
public:
	static TPointerMaterialInstance Factory(
		GResourceManager& inout_resourceManager, //load textures/ shaders
		const GMaterial& in_material,
		const GBOOL in_bindTexture,
		const GBOOL in_allowRuntimeBindTexture //can texture be bound/ unbound during it's scope (else bind/unbind on scope)
		);

	static TPointerMaterialInstance FactoryManualShader(
		GResourceManager& inout_resourceManager, //load textures/ shaders
		const std::string& in_name,
		const std::string& in_vertexShader,
		const std::string& in_fragmentShader,
		const GMaterial& in_material,
		const GBOOL in_bindTexture,
		const GBOOL in_allowRuntimeBindTexture //can texture be bound/ unbound during it's scope (else bind/unbind on scope)
		);

	////////////////////////////////////////////////////
	//constructor
public:
	GMaterialInstance(
		const GColour4Float& in_colour,
		const GMaterialType::TBlendMode::TEnum in_alphaBlendSource,
		const GMaterialType::TBlendMode::TEnum in_alphaBlendDestination,
		const GMaterialType::TState::TFlag in_flag,
		TPointerTextureInstance& inout_textureInstance,
		TPointerShaderInstance& inout_shaderInstance
		);
	GMaterialInstance(const GMaterialInstance& in_src);
	GMaterialInstance();
	~GMaterialInstance();

	const GMaterialInstance& operator=(const GMaterialInstance& in_rhs);

	////////////////////////////////////////////////////
	//public accessors
public:
	const GColour4Float& GetColour()const{ return mColour; }
	void SetColour(const GColour4Float& in_colour){ mColour = in_colour; return; }
	const GMaterialType::TBlendMode::TEnum GetAlphaBlendSource()const{ return (GMaterialType::TBlendMode::TEnum)mAlphaBlendSource; }
	const GMaterialType::TBlendMode::TEnum GetAlphaBlendDestination()const{ return (GMaterialType::TBlendMode::TEnum)mAlphaBlendDestination; }
	//const GMaterialType::TState::TFlag GetFlag()const{ return (GMaterialType::TState::TFlag)mFlag; }
	const bool GetFlagFaceCW()const;
	const bool GetFlagFaceCull()const;

	const bool GetFlagAlphaBlend()const;

	const bool GetFlagColourWrite()const;
	const bool GetFlagDepthRead()const; //infers z must be less than to write
	const bool GetFlagDepthWrite()const;

	const bool GetFlagStencilShadowFront()const;
	const bool GetFlagStencilShadowBack()const;
	const bool GetFlagStencilShadow()const;

	const bool GetFlagUseLight()const;
	const bool GetFlagUseTexture()const;
	const bool GetFlagDoBasePass()const;
	const bool GetFlagDoEnvironment()const;

	const TWeakTextureInstance& GetTextureInstance()const{ return mWeakTextureInstance; }
	TWeakTextureInstance& GetTextureInstance(){ return mWeakTextureInstance; }

	const TWeakShaderInstance& GetShaderInstance()const{ return mWeakShaderInstance; }
	TWeakShaderInstance& GetShaderInstance(){ return mWeakShaderInstance; }

	//////////////////////////////////////////////////////
	//private members
private:
	GColour4Float mColour;
	GMaterialType::TBlendMode::TEnum mAlphaBlendSource;
	GMaterialType::TBlendMode::TEnum mAlphaBlendDestination;
	GMaterialType::TState::TFlag mFlag;
	TWeakTextureInstance mWeakTextureInstance;
	TWeakShaderInstance mWeakShaderInstance;

};

#endif //_gmaterialinstance_h_