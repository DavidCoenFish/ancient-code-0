//file: main.h
#ifndef _main_H_
#define _main_H_

/**/
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <TCommon.h>

class TPackerMessage;

class Main : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<TPackerMessage> TPointerPackerMessage;

	//public static methods
public:
	static const int Run(
		const std::string& in_sourceXml, 
		const std::string& in_destinationXml,
		const int in_accuracy
		);

	//construction
private:
	Main();
	~Main();

	const int Action(
		const std::string& in_sourceXml, 
		const std::string& in_destinationFileName,
		const int in_accuracy
		);

	//members
private:
	TPointerPackerMessage mMessage;

};

#endif // _main_H_
