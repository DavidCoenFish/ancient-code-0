"""
/*******************************************************
* File:		concatjscript.py
* Author:	David Coen
* Date:		2011 09 14
*
******************************************************/

join multiple javascript files into one
example usage: D:\
development\project\tools\python\source\concatjscript.py D:\development\project\tools\python\source\test foo.txt

"""

# ----------------------------------------------------------------------------------------------------
# import
# ----------------------------------------------------------------------------------------------------
import os
import sys
import array

# ----------------------------------------------------------------------------------------------------
# private methods
# ----------------------------------------------------------------------------------------------------
def __concatFile(in_srcFile, in_destFile):
	for line in in_srcFile:	
		if -1 != line.find("""//-- END"""):
			return
		in_destFile.write(line)
		
	

def __concat(in_name, in_destFile):
	try:
		sourceFile = open(in_name, 'r')
	except:
		print str(sys.exc_value)
		return 

	__concatFile(sourceFile, in_destFile)

	sourceFile.close() 
# ----------------------------------------------------------------------------------------------------
# public methods
# ----------------------------------------------------------------------------------------------------
def concatjscript(in_sourceDir, in_destFile):
	try:
		destFile = open(in_destFile, 'w')
	except:
		print str(sys.exc_value)
		return 

	print "concatjscript:" + in_sourceDir
	for root, dirs, files in os.walk(in_sourceDir):
		for name in files:
			__concat(os.path.join(root, name), destFile)
			
	destFile.close() 
			
def concathack():
	arrayData = [\
"D:/development/depot/art/web/_mainline/project/grb/script/battleaction.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/battleactionmodifier.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/battleactionmodifierlock.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/battlecharacterdata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/battledistancestore.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/battledistancestoredata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/battlereport.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/common.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/commondraw.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/deploycharacterdata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/descriptiondata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/game.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamecharacter.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamedraw.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gameentity.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodebattle.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodedeploy.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodedrag.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodedroplist.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodeedit.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodeeditmember.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodehelp.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodelocalbattle.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodemain.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodeonlinebattle.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemoderanking.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemoderesult.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamemodeunlockskill.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/gamevar.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guibackground.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guibutton.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigamebattleminimap.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigamebattlenote.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigamebattletrace.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigamebattleview.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigamecharacter.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigamecharacterinfo.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigameslot.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigameslotinfo.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigameslotitem.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guigameslottable.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guilist.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guilistdata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guipanel.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guipanelfancy.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guiscrollbarvertical.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/guitext.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modaldialog.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modaldialogbuttondata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacter.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterai.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterbattle.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterclass.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharactergender.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterrace.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterslot.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterslotdata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterslotdataaction.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterslotdatadescription.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterslotnodeconnectdata.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharacterslotdragtarget.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiercharactervalid.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifiermap.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/modifierplusstat.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/node.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/nodegraph.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/nodeinput.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/vector.js",\
"D:/development/depot/art/web/_mainline/project/grb/script/vector3.js"\
];
		    
	try:
		destFile = open("out.txt", 'w')
	except:
		print str(sys.exc_value)
		return 

	for name in arrayData:
		__concat(name, destFile)

	destFile.close() 
				
# ----------------------------------------------------------------------------------------------------
# application inpoint
# ----------------------------------------------------------------------------------------------------
if __name__ == '__main__':

	#if 3 != len(sys.argv):
	#	print "usage concatjscript.py <target dir> <dest file>"
	#else:
	#	concatjscript(sys.argv[1], sys.argv[2])
	
	concathack();    
    
    