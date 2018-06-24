//file: FontSpliterData.h
#ifndef _FontSpliterData_H_
#define _FontSpliterData_H_

#include <string>

#include <boost/noncopyable.hpp>

class TiXmlElement;
class TPackerMessage;

/**/
class FontSpliterData : public boost::noncopyable
{
	// construction
public:
	FontSpliterData(
		const std::string& in_fileName,
		const int in_baseLine		
		);
	~FontSpliterData();

	// accessors
public:
	const std::string& GetFileName()const{ return mFileName; }
	const int GetBaseLine()const{ return mBaseLine; }

	// private members
private:
	const std::string mFileName;
	const int mBaseLine;

};

#endif // _FontSpliterData_H_