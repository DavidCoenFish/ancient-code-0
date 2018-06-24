"""
/*******************************************************
* File:		lowercasefiles.py
* Author:	David Coen
* Date:		2010 12 03
*
******************************************************/

change folders and files to lower case

example usage:
D:\>
development\depot\tool\python\_mainline\python\source\lowercasefiles.py "D:\development\depot\tool\python\_mainline\python\Copy of test_lowercase"

"D:\Program Files\Python26\python.exe"

"""

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os
import sys

# ----------------------------------------------------------------------------------------------------
# private methods
# ----------------------------------------------------------------------------------------------------
def __rename(in_root, in_name):
	lowerName = in_name.lower()
	if in_name != lowerName:
		fullName = os.path.join(in_root, in_name)
		lowerName = os.path.join(in_root, lowerName)
		try:
			os.rename(fullName, lowerName)
		except:
			print "rename failed:" + fullName + " --> " + lowerName

# ----------------------------------------------------------------------------------------------------
# public methods
# ----------------------------------------------------------------------------------------------------
def lowercasefiles(in_filePath):
	print "lowercasefiles:" + in_filePath
	for root, dirs, files in os.walk(in_filePath):
		#print " root:" + root
		for name in dirs:
			__rename(root, name)
		for name in files:
			__rename(root, name)
				
# ----------------------------------------------------------------------------------------------------
# application inpoint
# ----------------------------------------------------------------------------------------------------
if __name__ == '__main__':

	if 2 != len(sys.argv):
		print "usage lowercasefiles.py <target dir>"
	else:
		lowercasefiles(sys.argv[1])
	