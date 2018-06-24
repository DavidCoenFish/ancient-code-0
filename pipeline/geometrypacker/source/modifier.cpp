//file: Modifier.cpp

#include "Modifier.h"

Modifier::Modifier()
{
	return;
}

/*virtual*/ Modifier::~Modifier()
{
	return;
}

void Modifier::WriteOutData(
	TiXmlElement& inout_parentElement
	)const
{
	OnWriteOutData(inout_parentElement);
	return;
}

/*virtual*/ void Modifier::OnWriteOutData(
	TiXmlElement& inout_parentElement
	)const
{
	return;
}

