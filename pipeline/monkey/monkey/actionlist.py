"""
/*******************************************************
* File:		actionlist.py
* Author:	David Coen
* Date:		2010 12 04
*
******************************************************/
"""

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import xml.dom.minidom 
import os
import buildercommandline
import buildermulti
import nodefile
import nodebuild
import sys
import datestamp

"""
	action list
the things we need to do

"""

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------
class actionlist:

	# ----------------------------------------------------------------------------------------------------
	# public methods
	# ----------------------------------------------------------------------------------------------------
	def __init__(self, in_document, in_application):
		self.__mapFileNameNodeBuild = {}

		try:
			xmlActionListTrace = in_document.getElementsByTagName("actionList").item(0).firstChild
			while xmlActionListTrace:
				if xmlActionListTrace.ELEMENT_NODE == xmlActionListTrace.nodeType:
					if "action" == xmlActionListTrace.nodeName:
						self.__addActionXml(xmlActionListTrace, in_application)
					
				xmlActionListTrace = xmlActionListTrace.nextSibling
		except:
			in_application.logError("parse actionList failed " + str(sys.exc_value))
		in_application.logMessage("")
		
	def run(self, in_application, in_builderList, in_forceBuild = False):
		in_application.logMessage("actionlist::run")

		for keyFileName, valueNodeBuild in self.__mapFileNameNodeBuild.items():
			rebuild = in_forceBuild
			if False == rebuild:
				keyDateStamp = datestamp.getDateStamp(keyFileName, 0.0)
				valueDateStamp = valueNodeBuild.getDateStampDependancy(in_application, in_builderList, keyDateStamp)
				rebuild = keyDateStamp <= valueDateStamp
								
			valueNodeBuild.setRequiresBuild(rebuild)
			if rebuild:
				in_application.logMessage("rebuild:" + os.path.split(keyFileName)[1])
		in_application.logMessage("")
			
		#be sure that paths exsist
		for keyFileName, valueNodeBuild in self.__mapFileNameNodeBuild.items():
			in_application.createPath(keyFileName)
		
		#go through each in turn the be sure all are build, rely on recusion of build request to get order correct
		for keyFileName, valueNodeBuild in self.__mapFileNameNodeBuild.items():
			in_application.requestBuildFileName(keyFileName)

	def requestBuildFileName(self, in_fileName, in_builderList, in_application):
		if self.__mapFileNameNodeBuild.has_key(in_fileName):
			try:
				self.__mapFileNameNodeBuild[in_fileName].perform(in_builderList, in_application)
			except:
				in_application.logError("requestBuildFileName failed:" + os.path.split(in_fileName)[1] + " " + str(sys.exc_value));	
		
	def getDateStampDependancyFileName(self, in_fileName, in_builderList, in_fileMissingDateStamp, in_application):
		if self.__mapFileNameNodeBuild.has_key(in_fileName):
			return self.__mapFileNameNodeBuild[in_fileName].getDateStampDependancy(in_application, in_builderList, in_fileMissingDateStamp)
		return  datestamp.getDateStamp(in_fileName, in_fileMissingDateStamp)
			
				
	# ----------------------------------------------------------------------------------------------------
	# private methods
	# ----------------------------------------------------------------------------------------------------
	def __addActionXml(self, in_xmlElement, in_application):
		sourceNodeFile = nodefile.factoryNodeFileXml(in_xmlElement.getElementsByTagName("sourceFile").item(0), in_application)
		destinationNodeFile = nodefile.factoryNodeFileXml(in_xmlElement.getElementsByTagName("destinationFile").item(0), in_application)
		arrayDependancyNodeFile = []

		xmlDependancyTrace = in_xmlElement.getElementsByTagName("dependancy").item(0)
		while xmlDependancyTrace:
			if xmlDependancyTrace.ELEMENT_NODE == xmlDependancyTrace.nodeType:
				if "dependancy" == xmlDependancyTrace.nodeName:
					dependancyNodeFile = nodefile.factoryNodeFileXml(xmlDependancyTrace, in_application)
					arrayDependancyNodeFile.append(dependancyNodeFile.getFilePath())
				
			xmlDependancyTrace = xmlDependancyTrace.nextSibling

		if None == sourceNodeFile or None == destinationNodeFile:
			in_application.logError("parse actionList add action failed")
			return
		
		newNodeBuild = nodebuild.nodebuild(sourceNodeFile, destinationNodeFile, arrayDependancyNodeFile)

		self.__mapFileNameNodeBuild[destinationNodeFile.getFilePath()] = newNodeBuild
		
		in_application.logMessage("actionlist add " + sourceNodeFile.getFilePath() + " --> " + destinationNodeFile.getFilePath())
	