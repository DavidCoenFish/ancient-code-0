"""
/*******************************************************
* File:		runmaxscript.py
* Author:	David Coen
* Date:		2011 02 18
*
******************************************************/

run a max script on all max files found in path
example usage:
D:\development\project\tools\python\source\runmaxscript.py D:\development\project\zombie\zombie\art

"""

__maxscript = """
try
(
	loadMaxFile "%s"
	fDscUtilityChangeAllSceneNodeComponentVisualMaterialFilePath "..\\\\..\\\\..\\\\resources\\\\material_00.data"
	savemaxfile "%s"
)
catch ()
quitMax #noPrompt 
"""


# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os
import sys
import tempfile
import subprocess

# ----------------------------------------------------------------------------------------------------
# private methods
# ----------------------------------------------------------------------------------------------------
def __runmaxscript(in_fileName):
	print "__runmaxscript:" + in_fileName
	localScript = __maxscript % (in_fileName.replace("\\", "\\\\"), in_fileName.replace("\\", "\\\\"))
	
	fileObject = tempfile.NamedTemporaryFile(suffix=".ms", delete=False)
	scriptPath = fileObject.name
	
	fileObject.write(localScript)
	fileObject.close()
	
	commandLine = """\"D:\\Program Files\\Autodesk\\3ds Max 9\\3dsmax.exe" -u MAXScript \"%s\" -q -silent -mip""" % (scriptPath.replace("\\", "\\\\"))
	proc = subprocess.Popen(
		commandLine,
		stdout = subprocess.PIPE,
		stderr = subprocess.STDOUT
		) 
	
	(stdoutdata, stderrdata) = proc.communicate()
	if 0 != proc.returncode:
		print("returned:%s" % (str(proc.returncode)))

	os.remove(scriptPath)

# ----------------------------------------------------------------------------------------------------
# public methods
# ----------------------------------------------------------------------------------------------------
def runmaxscript(in_filePath):
	print "runmaxscript:" + in_filePath
	for root, arrayDir, arrayFile in os.walk(in_filePath):
		for file in arrayFile:
			normalCaseFileName = os.path.normcase(file) 
			if ".max" == os.path.splitext(normalCaseFileName)[1]:
				fullPath = os.path.join(root, file)
				__runmaxscript(fullPath)

				
# ----------------------------------------------------------------------------------------------------
# application inpoint
# ----------------------------------------------------------------------------------------------------
if __name__ == '__main__':

	if 2 != len(sys.argv):
		print "usage runmaxscript.py <target dir>"
	else:
		runmaxscript(sys.argv[1])
	