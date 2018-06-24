//file: Modifier.h
#ifndef _Modifier_H_
#define _Modifier_H_

#include <boost/noncopyable.hpp>

class TiXmlElement;

/**/
class Modifier : public boost::noncopyable 
{
public:
	Modifier();
	virtual ~Modifier();

public:
	void WriteOutData(
		TiXmlElement& inout_parentElement
		)const;

private:
	virtual void OnWriteOutData(
		TiXmlElement& inout_parentElement
		)const;

};

#endif // _Modifier_H_