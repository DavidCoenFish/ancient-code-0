"""
/*******************************************************
* File:		buildercommandline.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/
"""
import subprocess
import os

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------
class buildercommandline:
	def __init__(self, in_executableFile, in_useDataId, in_passPlatform):
		self.__executableFile = in_executableFile
		self.__useDataId = in_useDataId
		self.__passPlatform = in_passPlatform
		
	def perform(self, in_sourceFile, in_destinationFile, in_buildList, in_application):
		in_application.logMessage("buildercommandline::perform " + os.path.split(self.__executableFile)[1] + " " + os.path.split(in_sourceFile.getFilePath())[1] + " " + os.path.split(in_destinationFile.getFilePath())[1], True )
	
		commandList = [self.__executableFile, in_sourceFile.getFilePath(), in_destinationFile.getFilePath()]
		if True == self.__useDataId:
			commandList.append(in_sourceFile.getDataId())
			if None == in_sourceFile.getDataId():
				in_application.logError("buildercommandline useDataId with none " + os.path.split(self.__executableFile)[1], True )

		if True == self.__passPlatform:
			commandList.append(in_application.getPlatform())
	
		proc = subprocess.Popen(
			commandList,
			stdout = subprocess.PIPE,
			stderr = subprocess.STDOUT
			) 

		#proc.wait() 		
		(stdoutdata, stderrdata) = proc.communicate()

		in_application.logXml(stdoutdata)
		if 0 != proc.returncode:
			in_application.logError("command line build:%s returned:%s" % (os.path.split(self.__executableFile)[1], str(proc.returncode)))
		
	def getOldestDataStamp(self):
		dateStamp = 0.0
		if True == os.path.isfile(self.__executableFile):
			dateStamp = os.path.getmtime(self.__executableFile)
		
		return dateStamp