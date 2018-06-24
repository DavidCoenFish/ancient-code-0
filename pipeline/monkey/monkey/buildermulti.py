"""
/*******************************************************
* File:		buildermulti.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/
"""


# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import random
import os
import nodefile

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------

"""
3 step build chain, source type, middle type, destination type
when we are performed, we create a new node with file in temp dir
"""

class buildermulti:
	def __init__(self, in_sourceType, in_middleType, in_destinationType, in_builderList):
		self.__sourceType = in_sourceType
		self.__middleType = in_middleType
		self.__destinationType = in_destinationType
		self.__builderList = in_builderList

	def perform(self, in_sourceNodeFile, in_destinationNodeFile, in_buildList, in_application):
		tempDir = in_application.getPath("Temp")
		in_application.createPath(tempDir)
		tempFilePath = os.path.join(tempDir, "temp%s.xml" % random.random())
		tempNodeFile = nodefile.nodefile(tempFilePath, self.__middleType, "")

		in_buildList.performBuild(in_sourceNodeFile, tempNodeFile, in_application)
		in_buildList.performBuild(tempNodeFile, in_destinationNodeFile, in_application)
		
	def getOldestDataStamp(self):
		dateStamp = 0.0
		tempDateStamp = self.__builderList.getBuilderDataStamp(self.__sourceType, self.__middleType)
		if dateStamp < tempDateStamp:
			dateStamp = tempDateStamp

		tempDateStamp = self.__builderList.getBuilderDataStamp(self.__middleType, self.__destinationType)
		if dateStamp < tempDateStamp:
			dateStamp = tempDateStamp
	
		return dateStamp