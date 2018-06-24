"""
/*******************************************************
* File:		languageproject.py
* Author:	David Coen
* Date:		2011 01 24
*
******************************************************/

split or merge a lproj XXX.string file
input is of xcode form
	"InfoTitle" = "Play With Goldfish";
	"InfoMessage" = "Touch:"

on split want a file of keys seperated by line feed
and a file of translation data, datablocks seperated by blank line

on merge, combine the key and data file into a xcode XXX.string file

example usage: D:\
development\project\tools\python\source\languageproject.py --split Localizable.strings key.txt data.txt
development\project\tools\python\source\languageproject.py --merge Localizable.strings key.txt data.txt

"D:\Program Files\Python26\python.exe"

"""

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os
import sys
import optparse
import string
import codecs

def __PartionOut(in_string, in_partitionStringBegin, in_partitionStringEnd):
	localString = in_string
	while True:
		(firstPart, partition, endPart) = localString.partition(in_partitionStringBegin)
		if "" == partition:
			return firstPart

		(endFirstPart, partition, endEndPart) = endPart.partition(in_partitionStringEnd)
		localString = firstPart + endEndPart

def __ParseFile(in_sourceFileString):
	returnValue = []
	localString = in_sourceFileString
	while True:
		indexKeyStart = localString.find("\"")
		indexKeyEnd = localString.find("\"", indexKeyStart + 1)
		indexDataStart = localString.find("\"", indexKeyEnd + 1)
		indexDataEnd = localString.find("\"", indexDataStart + 1)
		
		if -1 == indexKeyStart or -1 == indexKeyEnd or -1 == indexDataStart or -1 == indexDataEnd:
			break
		
		returnValue.append((localString[(indexKeyStart + 1):indexKeyEnd], localString[(indexDataStart + 1):indexDataEnd]))
		
		localString = localString[(indexDataEnd + 1):]
		
		
	return returnValue

def __ParseKeys(in_keyFileObject):
	returnArray = []
	for line in in_keyFileObject:	
		stripedString = line.lstrip().rstrip()
		returnArray.append(stripedString)
	return returnArray
	
def __ParseData(in_dataFileObject):
	returnArray = []
	sourceDataString = in_dataFileObject.read()	
	arrayLines = sourceDataString.splitlines()
	trace = ""
	for line in arrayLines:
		if "" == line:
			if "" != trace:
				returnArray.append(trace)
				trace = ""
		else:
			if "" != trace:
				trace += "\n"
			trace += line;
		
	if "" != trace:
		returnArray.append(trace)
	
	return returnArray

def __PairArray(in_arrayKey, in_arrayData):
	returnArray = []
	count = min(len(in_arrayKey), len(in_arrayData))
	for index in range(count):
		returnArray.append((in_arrayKey[index], in_arrayData[index]))
	return returnArray

# ----------------------------------------------------------------------------------------------------
# public methods
# ----------------------------------------------------------------------------------------------------
def SplitStringFile(in_stringFileName, in_keyFileName, in_dataFileName):
	print "SplitStringFile", in_stringFileName, in_keyFileName, in_dataFileName

	try:
		sourceFileObject = open(in_stringFileName, 'r')
	except:
		print str(sys.exc_value)
		return 

	try:
		destinationKeyFileObject = open(in_keyFileName, 'w')
	except:
		print str(sys.exc_value)
		return 
		
	try:
		destinationDataFileObject = open(in_dataFileName, 'w')
	except:
		print str(sys.exc_value)
		return 

	sourceFileString = sourceFileObject.read()	
	
	sourceFileString = __PartionOut(sourceFileString, "/*", "*/")
	sourceFileString = __PartionOut(sourceFileString, "//", "\n")
	
	arrayKeyData = __ParseFile(sourceFileString)
	for keyValue, dataValue in arrayKeyData:
		destinationKeyFileObject.write(keyValue + "\n")
		destinationDataFileObject.write(dataValue + "\n\n")

	sourceFileObject.close() 
	destinationKeyFileObject.close() 
	destinationDataFileObject.close() 
	
def MergeStringFile(in_stringFileName, in_keyFileName, in_dataFileName):
	print "MergeStringFile", in_stringFileName, in_keyFileName, in_dataFileName
				
	try:
		keyFileObject = open(in_keyFileName, 'r')
	except:
		print str(sys.exc_value)
		return 

	try:
		dataFileObject = open(in_dataFileName, 'r')
	except:
		print str(sys.exc_value)
		return 
		
	try:
		destinationFileObject = open(in_stringFileName, 'w')
	except:
		print str(sys.exc_value)
		return 
		
	arrayKey = __ParseKeys(keyFileObject)
	arrayData = __ParseData(dataFileObject)
				
	arrayKeyData = __PairArray(arrayKey, arrayData)
	
	for keyValue, dataValue in arrayKeyData:
		destinationFileObject.write("\"" + keyValue + "\" = \"" + dataValue + "\";\n")
		
	keyFileObject.close() 
	dataFileObject.close() 
	destinationFileObject.close() 
		
		
# ----------------------------------------------------------------------------------------------------
# application inpoint
# ----------------------------------------------------------------------------------------------------
if __name__ == '__main__':
	parser = optparse.OptionParser(
		usage = "usage: %prog [--split, --merge] filenameA filenameB filenameC",
		description = ""
		)

	parser.add_option(
		"-s", 
		"--split",
		action = "store_true", 
		dest = "split", 
		default = True,
		help = "split string file into key and data files"
		)

	parser.add_option(
		"-m", 
		"--merge",
		action = "store_false", 
		dest = "split", 
		help = "merge key and data files into string file"
		)
	
	(options, args) = parser.parse_args()
	if len(args) != 3:
		parser.error("incorrect number of arguments")
	else:
		if options.split == True:
			SplitStringFile(args[0], args[1], args[2])
		else:
			MergeStringFile(args[0], args[1], args[2])
