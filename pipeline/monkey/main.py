"""
/*******************************************************
* File:		main.py
* Author:	David Coen
* Date:		2010 12 02
*
******************************************************/

todo:
set source and destination dir 
	(define named relative path bases, and attribute of xml <file> could be <file basepath="sourceDir">foo.xml</file>
clean up xml loading code

example usage:
D:\>development\project\tools\pipeline\monkey\Main.py D:\development\project\tools\pipeline\data\build.xml -v
D:\>"D:\Program Files\Python26\python.exe"

"""

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import sys
import optparse
import monkey.application

# ----------------------------------------------------------------------------------------------------
# private methods
# ----------------------------------------------------------------------------------------------------
def __main():
	parser = optparse.OptionParser(
		usage = "usage: %prog buildRules.xml [options]",
		description = "simpler builder script. The build rules are specified by an xml file. A list of builders, and a list of actions. Wanted explicit definition of dependancies."
		)

	parser.add_option(
		"-v", 
		"--verbose",
		action = "store_true", 
		dest = "verbose", 
		default = False,
		help = "print to stdout all messages"
		)
	parser.add_option(
		"-q", 
		"--quiet",
		action = "store_false", 
		dest = "verbose",
		help = "do not print trivial messages to stdout"
		)
	parser.add_option(
		"-r", 
		"--rebuild",
		action = "store_true", 
		dest = "rebuild", 
		default = False,
		help = "force rebuild"
		)
	parser.add_option(
		"-t", 
		"--keeptemp",
		action = "store_true", 
		dest = "keepTemp", 
		default = False,
		help = "keep the files created in the temp dir"
		)
	parser.add_option(
		"-p", 
		"--platform",
		dest = "platform", 
		default = "win32",
		help = "passes platform flag builders which take it"
		)        
	(options, args) = parser.parse_args()
	if len(args) != 1:
		parser.error("incorrect number of arguments")

	application = monkey.application.application(
		args[0], 
		options.verbose,
		options.rebuild,
		options.keepTemp,
		options.platform
		)
	
# ----------------------------------------------------------------------------------------------------
# application inpoint
# ----------------------------------------------------------------------------------------------------
if __name__ == '__main__':
	__main()
