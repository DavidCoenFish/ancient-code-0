//file: ModifierSkinBoneVertexData.h
#ifndef _ModifierSkinBoneVertexData_H_
#define _ModifierSkinBoneVertexData_H_

/**/
class ModifierSkinBoneVertexData
{
	//typedef
private:
	//typedef std::vector<int> TArrayInt;

	//constructor
public:
	ModifierSkinBoneVertexData(const ModifierSkinBoneVertexData& in_src);
	ModifierSkinBoneVertexData(
		const int in_vertexIndex = -1,
		const float in_vertexWeight = 0.0F
		);
	~ModifierSkinBoneVertexData();

	const ModifierSkinBoneVertexData& operator=(const ModifierSkinBoneVertexData& in_rhs);

	//public accessors
public:
	const int VertexIndex()const{ return mVertexIndex; }
	const float VertexWeight()const{ return mVertexWeight; }	

	//private members
private:
	int mVertexIndex;
	float mVertexWeight;


};

#endif // _ModifierSkinBoneVertexData_H_