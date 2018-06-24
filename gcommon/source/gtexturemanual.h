//
//  GTextureManual.h
//  GCommon
//
//  Created by David Coen on 2011 06 03
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GTextureManual_h_
#define _GTextureManual_h_

#include "gcommon.h"
#include "gtexturetype.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GBuffer;
class GTexture;

/*
TODO: do i really need to keep the texture data around after bind?
*/
class GTextureManual : public boost::noncopyable
{
	//////////////////////////////////////////////////////
	//typedef
private:
	typedef boost::shared_ptr<GBuffer> TPointerBuffer;
	typedef boost::shared_ptr<GTexture> TPointerTexture;

	////////////////////////////////////////////////////
	//constructor
public:
	GTextureManual(
		const GU32 in_width,
		const GU32 in_height
		);
	~GTextureManual();

	////////////////////////////////////////////////////
	//public accessors
public:
	const GTexture& GetTexture()const;
	GTexture& GetTexture();

	//////////////////////////////////////////////////////
	//private members
private:
	TPointerBuffer m_buffer;
	TPointerTexture m_texture;

};


#endif //_GTextureManual_h_