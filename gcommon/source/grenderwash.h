//  GRenderWash.h

#ifndef _GRenderWash_h_
#define _GRenderWash_h_

#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "GColour4Float.h"

class GRender;
class GResourceManager;
class GMaterialInstance;
class GMeshManual;
class GCamera;

/*
	draw a wash of colour over screen
*/
class GRenderWash : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
	typedef boost::scoped_ptr<GCamera> TPointerCamera;

	typedef boost::shared_ptr<GRender> TPointerRender;

	//constructor
public:
	GRenderWash(
		GResourceManager& inout_resourceManager,
		const GColour4Float& in_colour,
		const GBOOL in_alpha,
		const GBOOL in_clearDepth
		);
	~GRenderWash();

	//public methods
public:
	void Draw(
		GRender& inout_render
		);

	const GColour4Float& GetColour()const { return m_colour; }
	GColour4Float& GetColour(){ return m_colour; }

	//private members
private:
	GColour4Float m_colour;
	TWeakMaterialInstance m_material;
	TPointerMeshManual m_mesh;
	TPointerCamera m_orthoCamera;

};

#endif //_GRenderWash_h_