//
//  giphonerenderopengles2info.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "giphonerenderopengles2info.h"

#include "gstringutility.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

// creation
GIPhoneRenderOpenGLES2Info::GIPhoneRenderOpenGLES2Info()
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
, m_sampleBuffers( 0 )
, m_samples( 0 )
, m_maxVertexAttribs( 0 )
, m_maxVertexUniformVectors( 0 )
, m_maxVaryingVectors( 0 )
, m_maxCombinedTextureImageUnits( 0 )
, m_maxVertexTextureImageUnits( 0 )
, m_maxTextureImageUnits( 0 )
, m_maxFragmentUniformVectors( 0 )
, m_numCompressedTextureFormats( 0 )
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

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_maxTextureSize );

    glGetIntegerv(GL_SAMPLE_BUFFERS, &m_sampleBuffers );
    glGetIntegerv(GL_SAMPLES, &m_samples );

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_maxVertexAttribs );
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &m_maxVertexUniformVectors );
    glGetIntegerv(GL_MAX_VARYING_VECTORS, &m_maxVaryingVectors );
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_maxCombinedTextureImageUnits );
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_maxVertexTextureImageUnits );
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureImageUnits );
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &m_maxFragmentUniformVectors );

	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &m_numCompressedTextureFormats );

	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, &m_numCompressedTextureFormats );
	

	return;
}

/**/
GIPhoneRenderOpenGLES2Info::~GIPhoneRenderOpenGLES2Info()
{
	return;
}

///////////////////////////////////////////////////////
// public methods
/**/
const bool  GIPhoneRenderOpenGLES2Info::ExtentionTest( const std::string& in_extention )
{
	TTreeStringInt::iterator iterator = m_treeExtensions.find( in_extention );
	if( iterator == m_treeExtensions.end() )
	{
		return false;
	}

	return true;
}

/**/