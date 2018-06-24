"""
/*******************************************************
* File:		nodefile.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/
"""


# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os
import nodebuild
import sys
import nodefile
import datestamp

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------

"""
identify a file, or part of a file
the filepath, type, and a dataId 
dataId is for things like a material name for data inside a max file
"""

class nodefile:
	def __init__(self, in_filePath, in_typeString, in_dataId):
		self.__filePath = in_filePath
		self.__typeString = in_typeString
		self.__dataId = in_dataId

	def getFilePath(self):
		return self.__filePath
		
	def getTypeString(self):
		return self.__typeString

	def getDataId(self):
		return self.__dataId

	def getDataStamp(self, in_fileMissingDate):
		dateStamp = datestamp.getDateStamp(self.__filePath, in_fileMissingDate)
		return dateStamp
	
	def __str__( self ):
		return "%s %s %s" % ( self.__filePath, self.__typeString, self.__dataId)
			
def factoryNodeFileXml(in_xmlElement, in_application):
	try:
		sourceFilePath = in_application.getPathXml(in_xmlElement)
		type = None
		dataId = None
		
		try:
			type = in_xmlElement.getElementsByTagName("type").item(0).firstChild.nodeValue
		except:
			pass
			
		try:
			dataId = in_xmlElement.getElementsByTagName("dataId").item(0).firstChild.nodeValue
		except:
			pass
			
		return nodefile(sourceFilePath, type, dataId)
	except:
		in_application.logError("bad xml file " + str(sys.exc_value))
		
	return None
		



