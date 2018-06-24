"""
/*******************************************************
* File:		builderlist.py
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
import buildercommandline
import buildermax
import buildermulti
import sys
import utilityxml

"""
hold the builder to get from source type to destiation type
"""

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------
class builderlist:

	# ----------------------------------------------------------------------------------------------------
	# public methods
	# ----------------------------------------------------------------------------------------------------
	def __init__(self, in_document, in_logger):
		self.__mapTupleSourceTypeDestTypeBuilder = {}

		try:
			xmlbuilderlistTrace = in_document.getElementsByTagName("builderList").item(0).firstChild
			while xmlbuilderlistTrace:
				if xmlbuilderlistTrace.ELEMENT_NODE == xmlbuilderlistTrace.nodeType:
					if "builderCommandLine" == xmlbuilderlistTrace.nodeName:
						self.__addbuilderCommandLineXml(xmlbuilderlistTrace, in_logger)
					if "builderMax" == xmlbuilderlistTrace.nodeName:
						self.__addbuilderMaxXml(xmlbuilderlistTrace, in_logger)
					
				xmlbuilderlistTrace = xmlbuilderlistTrace.nextSibling
		except:
			in_logger.logError("parse builderlist failed " + str(sys.exc_value))
		in_logger.logMessage("")
			
	def getPath(self, in_name):
		if self.__mapNamePath.has_key(in_name):
			return self.__mapNamePath[in_name]
		return ""	
				
	def getBuilderDataStamp(self, in_sourceType, in_destinationType):
		key = in_sourceType, in_destinationType
		if self.__mapTupleSourceTypeDestTypeBuilder.has_key(key):
			builder = self.__mapTupleSourceTypeDestTypeBuilder[key]
			return builder.getOldestDataStamp()
		return 0.0
			
	def performBuild(self, in_sourceFileNode, in_destinationFileNode, in_application):
		key = in_sourceFileNode.getTypeString(), in_destinationFileNode.getTypeString()
		if self.__mapTupleSourceTypeDestTypeBuilder.has_key(key):
			builder = self.__mapTupleSourceTypeDestTypeBuilder[key]
			builder.perform(in_sourceFileNode, in_destinationFileNode, self, in_application)
		else:
			in_application.logError("no key perform build:" + str(key))
		
			
			
	# ----------------------------------------------------------------------------------------------------
	# private methods
	# ----------------------------------------------------------------------------------------------------
	def __addbuilderCommandLineXml(self, in_xmlElement, in_logger):
		try:
			sourceType = in_xmlElement.getElementsByTagName("sourceType").item(0).firstChild.nodeValue
			destinationType = in_xmlElement.getElementsByTagName("destinationType").item(0).firstChild.nodeValue
			executablePath = in_logger.getPathXml(in_xmlElement.getElementsByTagName("executablePath").item(0))
			useDataId = False
			usePlatform = False

			try:
				useDataId = utilityxml.getBool(in_xmlElement.getElementsByTagName("useDataId").item(0))
			except:
				pass

			try:
				usePlatform = utilityxml.getBool(in_xmlElement.getElementsByTagName("usePlatform").item(0))
			except:
				pass

			builder = buildercommandline.buildercommandline(executablePath, useDataId, usePlatform)
			self.__addBuilder(sourceType, destinationType, builder, in_logger)
		except:
			in_logger.logError("bad xml builderCommandLine " + str(sys.exc_value))
			
	def __addbuilderMaxXml(self, in_xmlElement, in_logger):
		try:
			sourceType = in_xmlElement.getElementsByTagName("sourceType").item(0).firstChild.nodeValue
			destinationType = in_xmlElement.getElementsByTagName("destinationType").item(0).firstChild.nodeValue
			executablePath = in_logger.getPathXml(in_xmlElement.getElementsByTagName("executablePath").item(0))
			functionName = utilityxml.getText(in_xmlElement.getElementsByTagName("functionName").item(0))
			builder = buildermax.buildermax(executablePath, functionName)
			self.__addBuilder(sourceType, destinationType, builder, in_logger)
		except:
			in_logger.logError("bad xml builderCommandLine " + str(sys.exc_value))			
								
	def __addBuilder(self, in_sourceType, in_destinationType, in_builder, in_logger):
		in_logger.logMessage("builderlist add {'" + in_sourceType + "', '" + in_destinationType + "'}")
		self.__mapTupleSourceTypeDestTypeBuilder[in_sourceType, in_destinationType] = in_builder
		self.__addExtraBuilders(in_sourceType, in_destinationType, in_logger)

	def __addExtraBuilders(self, in_sourceType, in_destinationType, in_logger):
		#we have a new source and dest, add multi builder for any matching links in chain
		#ie, if the output of any builders match our source, 
		#	or the input of any builder match our destination

		buildersToAdd = []
		for key, value in self.__mapTupleSourceTypeDestTypeBuilder.items():
			if key[0] == in_destinationType:
				buildersToAdd.append([in_sourceType, in_destinationType, key[1]])
			if key[1] == in_sourceType:
				buildersToAdd.append([key[0], in_sourceType, in_destinationType])

		for value in buildersToAdd:
			if False == self.__mapTupleSourceTypeDestTypeBuilder.has_key((value[0], value[2])):
				builder = buildermulti.buildermulti(value[0], value[1], value[2], self)
				self.__addBuilder(value[0], value[2], builder, in_logger)
								
				