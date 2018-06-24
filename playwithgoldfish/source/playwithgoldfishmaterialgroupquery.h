//
//  PlayWithGoldfishMaterialGroupQuery.h
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 10
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishMaterialGroupQuery_h_
#define _PlayWithGoldfishMaterialGroupQuery_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GMaterialGroupManager;
class GScene;
class GSceneNode;
class GRenderMaterial;

class PlayWithGoldfishMaterialGroupQuery : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GRenderMaterial> TPointerRenderMaterial;
	typedef boost::weak_ptr<GRenderMaterial> TWeakRenderMaterial;
	typedef std::pair<std::string, TWeakRenderMaterial> TPairStringWeakRenderMaterial;
	typedef std::vector<TPairStringWeakRenderMaterial> TArrayPairStringWeakRenderMaterial;

	//public static methods
public:
	static void Run(
		GScene& inout_scene, 
		GMaterialGroupManager& inout_materialGroupManager, 
		TArrayPairStringWeakRenderMaterial& in_arrayMaterial
		);

	//constructor
private:
	PlayWithGoldfishMaterialGroupQuery(
		GMaterialGroupManager& inout_materialGroupManager, 
		TArrayPairStringWeakRenderMaterial& in_arrayMaterial
		);
	~PlayWithGoldfishMaterialGroupQuery();

	//public methods
public:
	//contract with GSceneNode query
	void QuerySubmitNode(
		GSceneNode& inout_node
		);
	void QueryFinish();

	//private methods
private:
	GMaterialGroupManager& mMaterialGroupManager;
	TArrayPairStringWeakRenderMaterial mArrayMaterial;

};

#endif //_PlayWithGoldfishMaterialGroupQuery_h_
