"""
/*******************************************************
* File:		application.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/
"""

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import xml.dom.minidom 
import pathlist
import builderlist
import actionlist
import sys
import os

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------
class application:

	# ----------------------------------------------------------------------------------------------------
	# public methods
	# ----------------------------------------------------------------------------------------------------
	def __init__(self, in_buildXmlFilePath, in_verbose, in_rebuild, in_keepTemp, in_platform):
		self.__verbose = in_verbose
		self.__rebuild = in_rebuild
		self.__keepTemp = in_keepTemp
		self.__platform = in_platform
		self.__errorCount = 0
		self.__errorArray = []
		self.__errorCritial = False
		self.__pathList = None
		self.__builderList = None
		self.__actionList = None
		
		self.logMessage("Build Monkey: " + in_buildXmlFilePath, True)

		self.__loadXml(in_buildXmlFilePath)
		self.__run()
		self.__printFinialMessage()

	def logXml(self, in_xmlString):
		try:
			document = xml.dom.minidom.parseString(in_xmlString)
		except:
			print in_xmlString
			self.logError("logXml2 failed " + str(sys.exc_value));
			return

		try:
			xmlMessageListTrace = document.getElementsByTagName("messageList").item(0).firstChild
			while xmlMessageListTrace:
				if xmlMessageListTrace.ELEMENT_NODE == xmlMessageListTrace.nodeType:
					if "item" == xmlMessageListTrace.nodeName:
						self.logMessage(
							" " + xmlMessageListTrace.getElementsByTagName("message").item(0).firstChild.nodeValue, 
							0 != eval(xmlMessageListTrace.getElementsByTagName("verbose").item(0).firstChild.nodeValue)
							)
					
				xmlMessageListTrace = xmlMessageListTrace.nextSibling
		except:
			self.logError("parse logXml failed " + str(sys.exc_value))
		
		try:
			xmlErrorListTrace = document.getElementsByTagName("errorList").item(0).firstChild
			while xmlErrorListTrace:
				if xmlErrorListTrace.ELEMENT_NODE == xmlErrorListTrace.nodeType:
					if "item" == xmlErrorListTrace.nodeName:
						self.logError(
							" " + xmlErrorListTrace.getElementsByTagName("message").item(0).firstChild.nodeValue, 
							0 != eval(xmlErrorListTrace.getElementsByTagName("critical").item(0).firstChild.nodeValue)
							)
					
				xmlErrorListTrace = xmlErrorListTrace.nextSibling
		except:
			self.logError("parse logXml failed " + str(sys.exc_value))


	def logMessage(self, in_message, in_verbose = False):
		if (in_verbose or self.__verbose):
			self.__printMessage(in_message)
			
	def logError(self, in_message, in_critical = False):
		if in_critical:
			self.__errorCritial = True
			if self.__verbose:
				self.logMessage("Critical Error:" + in_message, True)
		else:
			self.__errorCount += 1
			if self.__verbose:
				self.logMessage("Error:" + in_message, True)
		self.__errorArray.append(in_message)
		
		
	def getPath(self, in_pathName):
		if self.__pathList:
			return self.__pathList.getPath(in_pathName)
		return ""
		
	def getPathXml(self, in_filePathXml):
		if self.__pathList:
			result = self.__pathList.pathListFactory(in_filePathXml, self)
			if None != result[0]:
				return result[0]
			if None != result[1]:
				return result[1]
		return ""
			
	#ignores the last leaf on given file path, just creates the path, not the file		
	def createPath(self, in_filePath):
		stem, leaf = os.path.split(in_filePath)
		if leaf:
			self.createPath(stem)
		if False == os.path.exists(stem):
			try:
				os.mkdir(stem)
			except:
				pass
	
	def requestBuildFileName(self, in_fileName):
		self.__actionList.requestBuildFileName(in_fileName, self.__builderList, self)
		
	def getDateStampDependancyFileName(self, in_fileName, in_fileMissingDateStamp):
		return self.__actionList.getDateStampDependancyFileName(in_fileName, self.__builderList, in_fileMissingDateStamp, self)

	def getPlatform(self):
		return self.__platform
		
	# ----------------------------------------------------------------------------------------------------
	# private methods
	# ----------------------------------------------------------------------------------------------------
	def __loadXml(self, in_buildXmlFilePath):
		try:
			document = xml.dom.minidom.parse(in_buildXmlFilePath)
		except:
			self.logError("load document failed " + in_buildXmlFilePath + " " + str(sys.exc_value), True);
			return

		self.logMessage("")

		self.__pathList = pathlist.pathlist(document, self)
		self.__builderList = builderlist.builderlist(document, self)
		self.__actionList = actionlist.actionlist(document, self)

	def __run(self):
		self.logMessage("Run Monkey", True)
		self.__actionList.run(self, self.__builderList, self.__rebuild)
		
		if False == self.__keepTemp:
			tempDir = self.getPath("Temp")
			if os.path.isdir(tempDir):
				self.logMessage("Delete temp dir:" + tempDir)
				for root, dirs, files in os.walk(tempDir):
					for name in files:
						os.remove(os.path.join(root, name))
				os.rmdir(tempDir)
			
			
			

	def __printFinialMessage(self):
		self.__printMessage("")
		self.__printMessage("==================================================")
		self.__printMessage("==================================================")

		if self.__errorCritial or 0 < self.__errorCount:

			if self.__errorCritial:
				self.__printMessage("==           build finished CRITICAL ERROR      ==")
			else:
				self.__printMessage("==           build finished with errors         ==")

			for item in self.__errorArray:
				self.__printMessage("== Error:" + item)
			
		else:
			self.__printMessage("==           build finished                     ==")

		self.__printMessage("==================================================")
		self.__printMessage("==================================================")
		
	def __printMessage(self, in_message):
		print in_message
		
		
		