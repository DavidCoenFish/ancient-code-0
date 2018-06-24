"""
/*******************************************************
* File:		pathlist.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/
"""

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import xml.dom.minidom 
import os
import sys
import utilityxml
# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------
class pathlist:

	# ----------------------------------------------------------------------------------------------------
	# public methods
	# ----------------------------------------------------------------------------------------------------
	def __init__(self, in_document, in_logger):
		self.__mapNamePath = {}

		try:
			xmlpathlistTrace = in_document.getElementsByTagName("pathList").item(0).getElementsByTagName("base").item(0)
			while xmlpathlistTrace:
				if xmlpathlistTrace.ELEMENT_NODE == xmlpathlistTrace.nodeType:
					if "base" == xmlpathlistTrace.nodeName:
						self.__addPath(xmlpathlistTrace, in_logger)
					
				xmlpathlistTrace = xmlpathlistTrace.nextSibling
		except:
			in_logger.logError("parse pathlist failed " + str(sys.exc_value))
		in_logger.logMessage("")
			
	def getPath(self, in_name):
		try:
			return self.__mapNamePath[in_name]
		except:
			return ""			

	#return a tuple of a file path and a folder path, either can be None
	def pathListFactory(self, in_parentElement, in_application):
		absoluteFilePath = None
		absolutePath = None

		try:
			filePathAbsoluteElement = in_parentElement.getElementsByTagName("filePathAbsolute").item(0)
			absoluteFilePath = os.path.normcase(pathAbsoluteElement.firstChild.nodeValue)
		except:
			pass

		try:
			filePathRelativeElement = in_parentElement.getElementsByTagName("filePathRelative").item(0)
			baseName = filePathRelativeElement.getAttribute("baseName")
			subPath = utilityxml.getText(filePathRelativeElement)
			absoluteFilePath = os.path.normcase(os.path.join(self.getPath(baseName), subPath))
		except:
			pass

		try:
			pathAbsoluteElement = in_parentElement.getElementsByTagName("pathAbsolute").item(0)
			absolutePath = os.path.normcase(pathAbsoluteElement.firstChild.nodeValue)
		except:
			pass
			
		try:
			pathRelativeElement = in_parentElement.getElementsByTagName("pathRelative").item(0)
			baseName = pathRelativeElement.getAttribute("baseName")
			subPath = utilityxml.getText(pathRelativeElement)
			absolutePath = os.path.normcase(os.path.join(self.getPath(baseName), subPath))
		except:
			pass

		if None == absoluteFilePath and None == absolutePath:
			in_application.logError("GetPathXml failed")
			
		return (absolutePath, absoluteFilePath)
		
	# ----------------------------------------------------------------------------------------------------
	# private methods
	# ----------------------------------------------------------------------------------------------------
	def __addPath(self, in_xmlElement, in_logger):
		name = utilityxml.getText(in_xmlElement.getElementsByTagName("name").item(0))
		tupleFilePathFolderPath = self.pathListFactory(in_xmlElement, in_logger)
		if tupleFilePathFolderPath[0]:
			self.__addPathAbsolute(name, tupleFilePathFolderPath[0], in_logger)
		else:
			in_logger.logError("bad xml pathlist")
								
	def __addPathAbsolute(self, in_name, in_pathAbsolute, in_logger):
		pathAbsolute = os.path.normcase(in_pathAbsolute)
		self.__mapNamePath[in_name] = pathAbsolute
		in_logger.logMessage("pathlist add {'" + in_name + "' : '" + pathAbsolute + "'}")
