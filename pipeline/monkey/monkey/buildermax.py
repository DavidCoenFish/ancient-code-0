"""
/*******************************************************
* File:		buildermax.py
* Author:	David Coen
* Date:		2010 12 20
*
******************************************************/
"""
import subprocess
import os
import random

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os


scriptText = """
try
(
	loadMaxFile "%s"
	%s "%s"
)
catch ()
quitMax #noPrompt 
"""

scriptTextDataId = """
try
(
	loadMaxFile "%s"
	%s "%s" "%s"
)
catch ()
quitMax #noPrompt 
"""

# ----------------------------------------------------------------------------------------------------
# public class
# ----------------------------------------------------------------------------------------------------
class buildermax:
	def __init__(self, in_executableFile, in_functionName):
		self.__executableFile = in_executableFile
		self.__functionName = in_functionName
		
	def perform(self, in_sourceFile, in_destinationFile, in_buildList, in_application):
		in_application.logMessage("buildermax::perform " + os.path.split(self.__executableFile)[1] + " " + os.path.split(in_sourceFile.getFilePath())[1] + " " + os.path.split(in_destinationFile.getFilePath())[1], True )
				
				
		#max script
		#if in_sourceFile.getDataId():
		#	scriptString = """%s \\\"%s\\\" \\\"%s\\\"""" % (self.__functionName, in_sourceFile.getDataId(), in_destinationFile.getFilePath().replace("\\", "\\\\"))
		#else:
		#	scriptString = """%s \\\"%s\\\"""" % (self.__functionName, in_destinationFile.getFilePath().replace("\\", "\\\\"))
		#commandLine = """\"%s\" -mxs \"%s\" \"%s\" -mip -silent""" % (self.__executableFile.replace("\\", "\\\\"), scriptString, in_sourceFile.getFilePath().replace("\\", "\\\\"))
				
		#make script file
		tempDir = in_application.getPath("Temp")
		in_application.createPath(tempDir)
		scriptPath = os.path.join(tempDir, "temp%s.ms" % random.random())
		fileObject = open(scriptPath, "wb")
		if in_sourceFile.getDataId():
			fileObject.write(scriptTextDataId % (in_sourceFile.getFilePath().replace("\\", "\\\\"), self.__functionName, in_sourceFile.getDataId(), in_destinationFile.getFilePath().replace("\\", "\\\\")))
		else:
			fileObject.write(scriptText % (in_sourceFile.getFilePath().replace("\\", "\\\\"), self.__functionName, in_destinationFile.getFilePath().replace("\\", "\\\\")))
		fileObject.close()
		
		#"d:/program files/autodesk/3ds max 9/3dsmax.exe" -u MAXScript test.ms -q -silent -mip		
		commandLine = """\"%s\" -u MAXScript \"%s\" -q -silent -mip""" % (self.__executableFile.replace("\\", "\\\\"), scriptPath.replace("\\", "\\\\"))
		#print commandLine
		
		proc = subprocess.Popen(
			commandLine,
			stdout = subprocess.PIPE,
			stderr = subprocess.STDOUT
			) 
		
		(stdoutdata, stderrdata) = proc.communicate()
		if 0 != proc.returncode:
			in_application.logError("max build:%s returned:%s" % (os.path.split(in_sourceFile.getFilePath())[1], str(proc.returncode)))
		
	def getOldestDataStamp(self):
		dateStamp = 0.0
		if True == os.path.isfile(self.__executableFile):
			dateStamp = os.path.getmtime(self.__executableFile)
		
		return dateStamp