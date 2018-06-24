//file: ModifierClothVertexData.h
#ifndef _ModifierClothVertexData_H_
#define _ModifierClothVertexData_H_

/**/
class ModifierClothVertexData
{
	//constructor
public:
	ModifierClothVertexData(const ModifierClothVertexData& in_src);
	ModifierClothVertexData(
		const float in_vertexWeight = 0.0F,
		const int in_duplicateTableOffset = -1
		);
	~ModifierClothVertexData();

	const ModifierClothVertexData& operator=(const ModifierClothVertexData& in_rhs);

	//public accessors
public:
	const float GetVertexWeight()const{ return mVertexWeight; }	
	const int GetDuplicateTableOffset()const{ return mDuplicateTableOffset; }	

	//private members
private:
	float mVertexWeight;
	int mDuplicateTableOffset;

};

#endif // _ModifierClothVertexData_H_