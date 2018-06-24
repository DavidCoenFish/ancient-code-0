//
//  gtextureinstance.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gtextureinstance_h_
#define _gtextureinstance_h_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GBuffer;
class GRender;
class GTextureInstance;
class GTexture;
class GTextureManual;

class GTextureInstance : public boost::noncopyable
{
	//////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::weak_ptr<GBuffer> TWeakBuffer;

	typedef boost::shared_ptr<GRender> TPointerRender;
	typedef boost::weak_ptr<GRender> TWeakRender;

	typedef boost::shared_ptr<GTextureInstance> TPointerTextureInstance;

	////////////////////////////////////////////////////
	//static public methods
public:
	static TPointerTextureInstance Factory(
		TPointerBuffer& inout_buffer, //can be null, WARN: transfer of ownership
		TPointerRender& inout_render,
		const bool in_bindTexture,
		const bool in_allowRuntimeBindTexture
		);

	static TPointerTextureInstance Factory(
		TPointerRender& inout_render,
		const GTextureManual& in_textureManual
		);

	////////////////////////////////////////////////////
	//constructor
public:
	GTextureInstance(
		TPointerRender& inout_render,
		const GU32 in_textureHandle
		);
	GTextureInstance(
		TPointerRender& inout_render
		);
	~GTextureInstance();

	////////////////////////////////////////////////////
	//public methods
public:
	void Bind(); //requires construction with reference to buffer
	void Unbind();
	const GU32 GetTextureHandle()const{ return mTextureHandle; }

	void Replace(
		TPointerBuffer& inout_buffer, //can be null, WARN: transfer of ownership
		const bool in_bindTexture,
		const bool in_allowRuntimeBindTexture
		);

	//////////////////////////////////////////////////////
	//private members
private:
	TPointerBuffer mBuffer; //optional, for runtime bind/ unbind
	TWeakRender mRender; //for unbind
	GU32 mTextureHandle;

	//GBOOL mBound; use 0 as unbound flag in mTextureHandle?

};


#endif //_gtextureinstance_h_