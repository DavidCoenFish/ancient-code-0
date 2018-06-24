//file: svgnode.cpp

#include "svgnode.h"

#include <TXml.h>
#include <TXmlPack.h>
#include <tinyxml.h>
#include <GMatrix9Float.h>
#include <GMath.h>
#include <GStringUtility.h>
#include <string>
#include <vector>

typedef std::vector<std::string> TArrayString;

static const std::string LocalGetStringSafe(const TArrayString& in_array, const int in_index)
{
	if ((0 <= in_index) && (in_index < (int)in_array.size()))
	{
		return in_array[in_index];
	}

	return std::string();
}

//static methods
/*static*/ SvgNode::TPointerSvgNode SvgNode::Factory(
	TPointerSvgNode& in_parentNode,
	TiXmlElement& in_sourceElement
	)
{
	GMatrix9Float transform = GMatrix9Float::sIdentity;
	const std::string transformString = TXml::GetAttributeString(&in_sourceElement, "transform");
	if (!transformString.empty())
	{
		//* translate(x, y)
		//* scale(sx, sy)
		//* rotate(angle, cx, cy)
		//* skewX(angle)
		//* skewY(angle)
		//* matrix(a, b, c, d, e, f)

		TArrayString arrayToken;
		GStringUtility::Split(transformString, " ", arrayToken);

		TArrayString::iterator iterator = arrayToken.begin();
		while (iterator != arrayToken.end())
		{
			TArrayString subArrayToken;
			GStringUtility::Split(*iterator, ",()", subArrayToken);
			++iterator;

			if (subArrayToken.size() < 2)
			{
				continue;
			}

			const std::string first = subArrayToken[0];

			if (first == "translate")
			{
				const float tx = TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 1));
				const float ty = TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 2));
				transform *= GMatrix9Float(
					1.0F, 0.0F, tx,
					0.0F, 1.0F, ty,
					0.0F, 0.0F, 1.0F
					);
			}
			else if (first == "scale")
			{
				const float sx = TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 1));
				const float sy = TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 2), sx);

				transform *= GMatrix9Float(
					sx, 0.0F, 0.0F,
					0.0F, sy, 0.0F,
					0.0F, 0.0F, 1.0F
					);
			}
			else if (first == "rotate")
			{
				const float angle = TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 1));
				const float cx = TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 2));
				const float cy = TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 3));

				transform *= GMatrix9Float(
					1.0F, 0.0F, cx,
					0.0F, 1.0F, cy,
					0.0F, 0.0F, 1.0F
					);

				transform *= GMatrix9Float(
					GMath::Cos(angle), -GMath::Sin(0.0174532925F * angle), 0.0F,
					GMath::Sin(angle), GMath::Cos(0.0174532925F * angle), 0.0F,
					0.0F, 0.0F, 1.0F
					);

				transform *= GMatrix9Float(
					1.0F, 0.0F, -cx,
					0.0F, 1.0F, -cy,
					0.0F, 0.0F, 1.0F
					);

			}
			else if (first == "skewX")
			{
				const float value = GMath::Tan(0.0174532925F * TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 1)));

				transform *= GMatrix9Float(
					1.0, value, 0.0F,
					0.0F, 1.0, 0.0F,
					0.0F, 0.0F, 1.0F
					);
			}
			else if (first == "skewY")
			{
				const float value = GMath::Tan(0.0174532925F * TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 1)));

				transform *= GMatrix9Float(
					1.0, 0.0F, 0.0F,
					value, 1.0, 0.0F,
					0.0F, 0.0F, 1.0F
					);
			}
			else if (first == "matrix")
			{
				const float value0 = GMath::Tan(TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 1)));
				const float value1 = GMath::Tan(TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 2)));
				const float value2 = GMath::Tan(TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 3)));
				const float value3 = GMath::Tan(TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 4)));
				const float value4 = GMath::Tan(TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 5)));
				const float value5 = GMath::Tan(TXml::GetTextFloat(LocalGetStringSafe(subArrayToken, 6)));

				transform *= GMatrix9Float(
					value0, value2, value4,
					value1, value3, value5,
					0.0F, 0.0F, 1.0F
					);
			}
			else
			{
				++iterator;
			}
		}
	}

	TPointerSvgNode pointer;
	pointer.reset(new SvgNode(in_parentNode, transform));
	return pointer;
}

//constructor
SvgNode::SvgNode(
	TPointerSvgNode& in_parentNode,
	const GMatrix9Float& in_transform
	)
	: m_parentNode(in_parentNode)
	, m_transform(in_transform)
{
	return;
}

SvgNode::~SvgNode()
{
	return;
}

//public methods
const GMatrix9Float SvgNode::GetAbsoluteTransform()const
{
	TPointerSvgNode parent = m_parentNode.lock();
	if (parent)
	{
		return (parent->GetAbsoluteTransform() * m_transform);
	}
	return m_transform;
}
