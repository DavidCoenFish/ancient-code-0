//
//  GGuiNodeComponentVisual.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentVisual_h_
#define _GGuiNodeComponentVisual_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "GGuiNodeComponentBase.h"

class GGuiManager;
class GMeshManual;
class GMaterialInstance;
class GMesh;

class GGuiNodeComponentVisual : public GGuiNodeComponentBase
{
	//typedef
private:
	typedef boost::shared_ptr<GMeshManual> TPointerMeshManual;
	typedef boost::weak_ptr<GMeshManual> TWeakMeshManual;

	typedef boost::shared_ptr<GMaterialInstance> TPointerMaterialInstance;
	typedef boost::weak_ptr<GMaterialInstance> TWeakMaterialInstance;

	//static public methods
public:
	static const unsigned int GetComponentFlag();

	//constructor
public:
	GGuiNodeComponentVisual(
		TPointerMeshManual& in_meshManual,
		TPointerMaterialInstance& in_materialInstance,
		const bool in_visible
		);
	virtual ~GGuiNodeComponentVisual();

	//public accessors
public:
	const bool GetVisible()const;
	void SetVisible(const bool in_visible);

	// else provide public render method? render
	const GMesh* const GetMesh()const; //can return null
	//const void* const GetMeshVertexData()const; //can return null

	GMaterialInstance* const GetMaterial();
	void SetMaterial(TPointerMaterialInstance& inout_materialInstance);

	//private members;
private:
	int mFlag;
	TWeakMeshManual mMeshManual;
	TWeakMaterialInstance mMaterialInstance;

};

#endif 