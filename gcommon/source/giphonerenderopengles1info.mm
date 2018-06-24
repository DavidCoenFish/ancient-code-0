//
//  giphoneapplicationwindow.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphonerenderopengles1info.h"

#include "gstringutility.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

// creation
GIPhoneRenderOpenGLES1Info::GIPhoneRenderOpenGLES1Info()
: m_vendor()
, m_renderer()
, m_version()
, m_treeExtensions()
, m_arrayExtentions()
, m_redBits( 0 )
, m_greenBits( 0 )
, m_blueBits( 0 )
, m_alphaBits( 0 )
, m_depthBits( 0 )
, m_stencilBits( 0 )
, m_maxTextureSize( 0 )
, m_maxLights( 0 )
, m_maxAttribStacks( 0 )
, m_maxModelViewStacks( 0 )
, m_maxProjectionStacks( 0 )
, m_maxClipPlanes( 0 )
, m_maxTextureStacks( 0 )
, m_maxMultipassTexureUnit( 0 )
{
	m_vendor = ( const char* const )glGetString( GL_VENDOR );
	m_renderer = ( const char* const )glGetString( GL_RENDERER );
	m_version = ( const char* const )glGetString( GL_VERSION );

	const std::string stringExtentions( ( const char* const )glGetString( GL_EXTENSIONS ) );
	GStringUtility::Split( stringExtentions, ' ', m_arrayExtentions );
	for (TArrayString::iterator iterator = m_arrayExtentions.begin(); iterator != m_arrayExtentions.end(); ++iterator)
	{
		m_treeExtensions.insert(TTreeStringInt::value_type((*iterator), 0));
	}

    // get number of color bits
    glGetIntegerv( GL_RED_BITS, &m_redBits );
    glGetIntegerv( GL_GREEN_BITS, &m_greenBits );
    glGetIntegerv( GL_BLUE_BITS, &m_blueBits );
    glGetIntegerv( GL_ALPHA_BITS, &m_alphaBits );

    // get depth bits
    glGetIntegerv(GL_DEPTH_BITS, &m_depthBits );

    // get stecil bits
    glGetIntegerv(GL_STENCIL_BITS, &m_stencilBits );

    // get max number of lights allowed
    glGetIntegerv(GL_MAX_LIGHTS, &m_maxLights );

    // get max texture resolution
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize );

    // get max number of clipping planes
    glGetIntegerv(GL_MAX_CLIP_PLANES, &m_maxClipPlanes );

    // get max modelview and projection matrix stacks
    glGetIntegerv( GL_MAX_MODELVIEW_STACK_DEPTH, &m_maxModelViewStacks );
    glGetIntegerv( GL_MAX_PROJECTION_STACK_DEPTH, &m_maxProjectionStacks );
    //glGetIntegerv( GL_MAX_ATTRIB_STACK_DEPTH, &m_maxAttribStacks );

    // get max texture stacks
    glGetIntegerv( GL_MAX_TEXTURE_STACK_DEPTH, &m_maxTextureStacks );

	//glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &m_maxMultipassTexureUnit );

	return;
}

/**/
GIPhoneRenderOpenGLES1Info::~GIPhoneRenderOpenGLES1Info()
{
	return;
}

///////////////////////////////////////////////////////
// public methods
/**/
const bool  GIPhoneRenderOpenGLES1Info::ExtentionTest( const std::string& in_extention )
{
	TTreeStringInt::iterator iterator = m_treeExtensions.find( in_extention );
	if( iterator == m_treeExtensions.end() )
	{
		return false;
	}

	return true;
}

/**/