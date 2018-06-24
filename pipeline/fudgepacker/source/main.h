//file: main.h
#ifndef _main_H_
#define _main_H_

/**/
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <TCommon.h>
#include <vector>
#include <map>

class PointerData;
class TPackerMessage;
class TiXmlElement;

/*
todo: allow pooling of pointers to same data (not just string table)
remove mMapStringPointerData
add mapPoolData <pointerDataKey, pointerpointerData>
to pool data pointer table must be empty? that seems silly
pointerDataKey (alignment, arrayData, arrayChildData(flat array of data in child offset table including offset)

aim is to get memory reuse for animation, but also gives more generalised string reuese
*/
//class Main : public boost::noncopyable 
//{
//	//typedef
//private:
//	typedef boost::shared_ptr<TPackerMessage> TPointerPackerMessage;
//	typedef boost::shared_ptr<PointerData> TPointerPointerData;
//
//	typedef std::map<std::string, TPointerPointerData> TMapStringPointer;
//	typedef std::vector<TPointerPointerData> TArrayPointerData;
//	typedef std::vector<U8> TArrayByte;
//	typedef std::vector<int> TArrayInt;
//
//	//public static methods
//public:
//	static const int Run(
//		const std::string& in_sourceXml, 
//		const std::string& in_destinationFileName
//		);
//
//	//construction
//private:
//	Main();
//	~Main();
//
//	const int Action(
//		const std::string& in_sourceXml, 
//		const std::string& in_destinationFileName
//		);
//
//	void LoadXml(const std::string& in_sourceXml);
//
//	void ParseElement(
//		TiXmlElement* const in_pParentElement,
//		TPointerPointerData& in_pointerData
//		);
//
//	void FlattenData(
//		TArrayByte& out_data//,
//		//TArrayByte& out_arrayPointerTable
//		);
//
//	void SaveFile(
//		const std::string& in_destinationFileName,
//		const TArrayByte& in_arrayByte
//		);
//
//	void AddString(
//		TPointerPointerData& inout_pointerData,
//		const std::string& in_string
//		);
//
//	TPointerPointerData AddPointer(
//		TPointerPointerData& inout_pointerData,
//		const int in_alignment,
//		const std::string& in_label
//		);
//
//	//members
//private:
//	TPointerPackerMessage mMessage;
//	TPointerPointerData mPointerData;
//
//	//string table
//	TMapStringPointer mMapStringPointerData;
//	TMapStringPointer mMaplabelPointerData;
//	TArrayPointerData mArrayPointerData;
//
//	//TArrayByte mOutput; on stack when needed
//	//TArrayPairIntString mStringTable;
//	//TMapStringInt mWrittenStringTable; string, offset of 'pointer'(offset) in mOutput
//	//TArrayInt mPointerTable; //indexs to 'pointers' in mOutput
//};

#endif // _main_H_
