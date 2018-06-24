"""
/*******************************************************
* File:		utilityxml.py
* Author:	David Coen
* Date:		2010 12 05
*
******************************************************/
"""


# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import xml.dom.minidom 

# ----------------------------------------------------------------------------------------------------
# public methods
# ----------------------------------------------------------------------------------------------------

def getText(in_xmlParentElement):
	traceNode = in_xmlParentElement.firstChild
	while traceNode:
		if traceNode.TEXT_NODE == traceNode.nodeType:
			return traceNode.wholeText
		traceNode = traceNode.nextSibling
	return ""
	
def getBool(in_xmlParentElement):
	text = getText(in_xmlParentElement)
	if "0" == text or "true" == text.lower():
		return True
	return False
