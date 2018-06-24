//file: Face.h
#ifndef _Face_H_
#define _Face_H_

class TiXmlElement;

/**/
struct Face
{
	Face(const int in_index0, const int in_index1, const int in_index2);
	int mIndex0;
	int mIndex1;
	int mIndex2;
};

#endif // _Face_H_