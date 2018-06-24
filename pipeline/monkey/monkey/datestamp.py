"""
/*******************************************************
* File:		datestamp.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/
"""


# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os

# ----------------------------------------------------------------------------------------------------
# public methods
# ----------------------------------------------------------------------------------------------------

def getDateStamp(in_fileName, in_fileMissingDate):
	dateStamp = 0.0
	
	#can we find the file
	if True == os.path.isfile(in_fileName):
		dateStamp = os.path.getmtime(in_fileName)

	#was file name a dir (then look at all files in the dir)
	elif True == os.path.isdir(in_fileName):
		for root, dirs, files in os.walk(in_fileName):
			for name in files:
				childFileName = os.path.normcase(os.path.normcase.join(root, name))
				testDateStamp = os.path.getmtime(childFileName)
				if dateStamp < testDateStamp:
					dateStamp = testDateStamp
	else:
		dateStamp = in_fileMissingDate
		
	return dateStamp	
	