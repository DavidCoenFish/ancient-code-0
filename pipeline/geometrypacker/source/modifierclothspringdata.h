//file: ModifierClothSpringData.h
#ifndef _ModifierClothSpringData_H_
#define _ModifierClothSpringData_H_

/**/
class ModifierClothSpringData
{
	//constructor
public:
	ModifierClothSpringData(const ModifierClothSpringData& in_src);
	ModifierClothSpringData(
		const int in_vertexIndexOne = -1,
		const int in_vertexIndexTwo = -1,
		const float in_length = 0.0F
		);
	~ModifierClothSpringData();

	const ModifierClothSpringData& operator=(const ModifierClothSpringData& in_rhs);

	//public accessors
public:
	const int GetVertexIndexOne()const{ return mVertexIndexOne; }
	const int GetVertexIndexTwo()const{ return mVertexIndexTwo; }
	const float GetLength()const{ return mLength; }	

	//private members
private:
	int mVertexIndexOne;
	int mVertexIndexTwo;
	float mLength;

};

#endif // _ModifierClothSpringData_H_