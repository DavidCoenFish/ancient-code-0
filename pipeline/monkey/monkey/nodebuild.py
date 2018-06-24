"""
/*******************************************************
* File:		nodebuild.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/
"""


# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os
import datestamp
import sys

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------

"""
"""

class nodebuild:
	def __init__(self, in_sourceNodeFile, in_destinationNodeFile, in_arrayDependancyFileName):
		self.__sourceNodeFile = in_sourceNodeFile
		self.__destinationNodeFile = in_destinationNodeFile
		self.__arrayDependancyFileName = in_arrayDependancyFileName

		self.__requiresBuild = False
		self.__performedBuild = False
		
	def getDateStampDependancy(self, in_application, in_builderList, in_fileMissingDate):
		#get the source dependancy date stamp
		dataStamp = in_application.getDateStampDependancyFileName(self.__sourceNodeFile.getFilePath(), in_fileMissingDate)

		#get the source date stamp
		testDateStamp = datestamp.getDateStamp(self.__sourceNodeFile.getFilePath(), in_fileMissingDate)
		if dataStamp < testDateStamp:
			dataStamp = testDateStamp

		#get the builder date stamp
		testDateStamp = in_builderList.getBuilderDataStamp(self.__sourceNodeFile.getTypeString(), self.__destinationNodeFile.getTypeString())
		if dataStamp < testDateStamp:
			dataStamp = testDateStamp

		#get the dependancie date stamp
		for dependancyFileName in self.__arrayDependancyFileName:
			testDateStamp = datestamp.getDateStamp(dependancyFileName, in_fileMissingDate)
			if dataStamp < testDateStamp:
				dataStamp = testDateStamp

			testDateStamp = in_application.getDateStampDependancyFileName(dependancyFileName, in_fileMissingDate)
			if dataStamp < testDateStamp:
				dataStamp = testDateStamp

		return dataStamp
		
	def setRequiresBuild(self, in_requiresBuild):
		self.__requiresBuild = in_requiresBuild
		
	def perform(self, in_builderList, in_application):
		if False == self.__performedBuild and True == self.__requiresBuild:
			self.__performedBuild = True
			
			#request build on source
			try:
				in_application.requestBuildFileName(self.__sourceNodeFile.getFilePath())
			except:
				in_application.logError("perform0 " + str(sys.exc_value));	
									
			#request build on all dependancies
			try:
				for dependancyFileName in self.__arrayDependancyFileName:
					in_application.requestBuildFileName(dependancyFileName)
			except:
				in_application.logError("perform1 " + str(sys.exc_value));	
									
			#request build for this node
			try:
				in_builderList.performBuild(self.__sourceNodeFile, self.__destinationNodeFile, in_application)
			except:
				in_application.logError("perform2 " + str(sys.exc_value));	

