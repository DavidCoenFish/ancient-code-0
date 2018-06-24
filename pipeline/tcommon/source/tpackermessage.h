//file: TPackerMessage.h
#ifndef _TPackerMessage_H_
#define _TPackerMessage_H_

#include <vector>
#include <string>

class TiXmlElement;

/**/
class TPackerMessage
{
	// typedefs
private:
	typedef std::pair<std::string, bool> TPairStringBool;
	typedef std::vector<TPairStringBool> TArrayStringBool;

	// construction
public:
	TPackerMessage();
	~TPackerMessage();

	// public methods
public:
	//if message not verbose, it can be hidden
	void AddMessage(const std::string& in_message, const bool in_verbose = false);
	//if error not critical, it can be ignored
	void AddError(const std::string& in_error, const bool in_critical = false);

	void Print()const;

	const bool GetCritical()const{ return mCritical; }

private:
	TArrayStringBool mArrayMessageVebose;
	TArrayStringBool mArrayErrorCritical;
	bool mCritical;

};

#endif // _TPackerMessage_H_