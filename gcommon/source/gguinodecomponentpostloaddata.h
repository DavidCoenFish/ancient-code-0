//
//  GGuiNodeComponentPostLoadData.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentPostLoadData_h_
#define _GGuiNodeComponentPostLoadData_h_

class GGuiNode;
class GGuiNodeComponentBase;

class GGuiNodeComponentPostLoadData
{
	//constructor
public:
	GGuiNodeComponentPostLoadData(
		GGuiNode* const in_guiNode,
		GGuiNodeComponentBase* const in_guiNodeComponentBase,
		void* const in_data	
		);
	~GGuiNodeComponentPostLoadData();

	//public accessors
public:
	GGuiNode* const GetGuiNode()const{ return mGuiNode; }
	GGuiNodeComponentBase* const GetGuiNodeComponentBase()const{ return mGuiNodeComponentBase; }
	void* const GetData()const{ return mData; }

	//private members;
private:
	GGuiNode* mGuiNode;
	GGuiNodeComponentBase* mGuiNodeComponentBase;
	void* mData;

};

#endif 