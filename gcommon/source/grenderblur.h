//
//  GRenderBlur.h
//

#ifndef _GRenderBlur_h_
#define _GRenderBlur_h_

#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "GVector3Float.h"
#include "GColour4Float.h"

class GRender;
class GResourceManager;
class GMaterialInstance;
class GMeshManual;
class GScene;
class GCamera;
class GTextureManual;

/**/
class GRenderBlur : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
	typedef boost::scoped_ptr<GCamera> TPointerCamera;

	typedef boost::shared_ptr<GTextureManual> TPointerTextureManual;

	//constructor
public:
	GRenderBlur(
		GRender& inout_render,
		GResourceManager& inout_resourceManager,
		const GS32 in_pixelWidth,
		const GS32 in_pixelHeight,
		const GBOOL in_depth,
		const GBOOL in_stencil
		);
	~GRenderBlur();

	//public methods
public:
	//setup render
	GVOID SetupRender(GRender& inout_render);

	//between setup and finish, we expect everything else that will be blured to render

	//finish blur
	GVOID FinishBlur(GRender& inout_render);

	//private members
private:
	TPointerMeshManual m_mesh;
	
	TPointerCamera m_orthoCamera;

	TWeakMaterialInstance m_material;
	TWeakMaterialInstance m_materialBlur0;
	TWeakMaterialInstance m_materialBlur1;
	TWeakMaterialInstance m_materialBlur2;

	TPointerTextureManual m_textureFullScreen;
	TPointerTextureManual m_texture0;
	TPointerTextureManual m_texture1;
	TPointerTextureManual m_texture2;

	GU32 m_renderTextureId;
	GU32 m_renderTexture0Id;
	GU32 m_renderTexture1Id;
	GU32 m_renderTexture2Id;

	const GU32 m_width;
	const GU32 m_height;
    
    const GU32 m_renderBufferWidth;
    const GU32 m_renderBufferHeight;
    
    const GU32 m_viewwportWidth;
    const GU32 m_viewwportHeight;
};

#endif //_GRenderBlur_h_