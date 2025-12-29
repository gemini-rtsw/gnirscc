""" Merge wfl documentation with C code.  Or split them apart.
Generates prototype documentation for each function when splitting and
no documentation preceeds the function.

split(fileName)  --- fileName.c is split into fileName.c and fileName.wfd
merge(fileName)  --- fileName.c and fileName.wfd are merged

With a plain C file, run split('filename.c').  This will create the
header file, 'filename.wfd'.  You can now merge this back with the program
using merge('filename.c'). You now have a file that can be edited to provide
the correct documentation.

Since the file is now likely to be much larger, the documenation gets in
the way, so you can edit the file more readily if you split off the
documentation (run split('filename.c').  Remerge whenever you wish.

The sequencing is a bit odd.  The first split, on a bare .c file, has no
'wfd' file to split off, so it creates one.  The subsequent merge puts the
two files together _and_ creates a table of contents at the head of
the documentation.  Do another split and you have a wfd file with the
table of contents (and the bare .c file again).  Do another merge and
you have a .c file with comments and the table of contents, as well as
the wfd file, also with the table of contents.

To make the of table of contents information match the PURPOSE
lines, after splitting the file, edit the wfd file and remove the
long comment whose second line is 'Copyright'.  A merge will now recreate
the table of contents with the correct information.  A subsequent split
followed by a merge will leave you with consistent .c and .wfd files.
The function 'fHead' does this sequence merge-split-merge for you.
"""

import string, re

class wflComment:
    def __init__(self):
        self.start = -1
        self.end = -1
        self.lines = None
        self.name = None
        self.purpose = "::DescribeMe::"
        self.retType = "void"
        self.arglist = []
    
wflDict = {}
haveCopyright = 0

# return index of last line of comment, or current line if not comment
def parseWfl(lines, index):
    global  wflDict, haveCopyright
    
    i = index
    line1 = lines[i]
    if (line1[0:2] != "/*"):
        return i
    line2 = lines[i+1]
    if (line1[2:3] != "+"  and line2[1:3] != "*+"):
        if line2[0:12] != " * Copyright":
            return index
        else:
            copyright = 1
    else:
        copyright = 0
    w = wflComment()
    w.lines = lines
    w.start = i
    # Don't have to scan for anything other than end of comment
    if copyright:
        w.name = '!HEADER!'
        while 1:
            if lines[i][0:3] == ' */':
                w.end = i
                haveCopyright = 1
                wflDict[w.name] = w
                return i
            else:
                i = i + 1
    while 1:
        line = lines[i]
        # End of the comment
        if line[1:3] == "*-":
            w.end = i+1
            wflDict[w.name] = w
            return i+1
        if string.find(line, "FUNCTION NAME") != -1:
            fline = lines[i+1]
            flist = string.split(string.split(fline, '(')[0])
            func = flist[-1]
            w.name = func
        elif string.find(line, "PURPOSE") != -1:
            w.purpose = lines[i+1][3:-1]  # drop newline at end
        i = i + 1
    
def isFunc(line):
    # functions start at col 0 with a lower case char
    if re.match("[a-zA-Z]", line) == None:
        return 0
    # skip function declarations
    if string.find(line, ";") != -1:
        return 0
    # there must be a '('
    if string.find(line, "(") == -1:
        return 0
    return 1

    
def writeWfl(w, file):
    for i in range(w.start, w.end+1):
        file.write(w.lines[i])

def makeProto(name, retval, args):
    global wflDict
    
    w = wflComment()
    w.start = 0
    w.name = name
    w.arglist = args
    w.retType = retval
    part1 = """/*
 *+
 * FUNCTION NAME:
 * %s
 *
 * INVOCATION:
 * %s""" % (name, name)
 
    argl = []
    if len(args) == 1 and (args[0] == '' or args[0] == 'void'):
        pars = " * None\n"
        part2 = "()"
    else:
        for i in range(0,len(args)):
            args[i] = string.join(string.split(args[i],'*'),'* ')
        for i in args:
            argl.append(string.split(i)[-1])
        a = string.join(argl, ', ')
        part2 = string.join(('(', a, ')'),'')
        pars = ""
        for i in args:
            a = string.split(i)
            pars = pars + " * (>) %s (%s)\n" % (a[-1], string.join(a[0:-1]))
    part3 = """\n *\n * PARAMETERS: (">" input, "!" modified, "<" output)\n""" + pars
    if retval == "void":
        retdes = "Returns 'void' and hence has no STATUS value"
    elif retval == "int":
        retdes = "Returns VME_OK (0) or VME_ERROR (1) as status"
    else:
        retdes = "Returns the status information structure XXX"
    part4 = """ *\n * FUNCTION VALUE:\n * (%s) %s\n""" % (retval, retdes)
    part5 = """ *\n * PURPOSE:
 * %s
 *
 * DESCRIPTION:
 * XXX
 *
 * EXTERNAL VARIABLES:
 *
 *
 * PRIOR REQUIREMENTS:
 *
 *
 * DEFICIENCIES:
 *
 *
 * HISTORY:
 * Jul 1, 2001      Initial Version                 (rwolff@noao.edu)
 *
 *-
 */\n""" % w.purpose
    w.lines = [part1, part2, part3, part4, part5]
    w.end = 4
    wflDict[name] = w

def makeHeader(filename):
    part1 = """/*
 * Copyright 2001 Association of Universities for Research in Astronomy, Inc.
 * See the file COPYRIGHT for more details.
 *
 * FILENAME:
 * %s
 *
 * FUNCTION NAME(S):\n""" % filename
    part2 = ""
    funcs = wflDict.keys()
    if '!HEADER!' in funcs:
        funcs.remove('!HEADER!')
    funcs.sort()
    for i in funcs:
        part2 = string.join((part2, "* ", i, "  -  ", wflDict[i].purpose, "\n"))
    part3 = """ *
 *INDENT-OFF*
 * $Log: wfd.py,v $
 * Revision 1.2  2009/05/27 19:32:06  fkraemer
 * fkraemer - copied my complete working dir over trunk
 *
 *INDENT-ON*
 *-
 */\n"""
    w = wflComment()
    w.name = '!HEADER!'
    w.start = 0
    w.end = 2
    w.lines = [part1, part2, part3]
    wflDict['!HEADER!'] = w


def doFile(filename, merge):
    
    global wflDict, haveCopyright

    # create cfile and wfile
    f = string.split(filename, '.')[0]
    cfile = open(f + ".c", "r")
    if cfile == None:
        print "Cannot open %s" % (f + ".c",)
        return
    if merge:
        wflfile = open(f+".wfd", "r")
        if wflfile == None:
            print "Cannot open file %s" % (f + ".wfd")
            cfile.close()
            return
    else:
        wflfile = open(f+".wfd", "w")
        if wflfile == None:
            print "Cannot open file %s for writing" % (f + ".wfd")
            cfile.close()
            return
    
    clines = cfile.readlines()
    wflDict = {}
    
    if merge:
        wlines = wflfile.readlines()
        index = 0
        while index < len(wlines):
            ret = parseWfl(wlines, index)
            index = ret + 1
        wflfile.close()
        cfile.close()

    cfile = open(f + ".c", "w")
    if cfile == None:
        print "Cannot open %s for writing" % (f + ".c")

    index = 0
    while index < len(clines):
        if not merge:
            ret = parseWfl(clines, index)
            # if not found a comment, look for function (returns current line)
            # if is a comment, returns last line of comment which isn't index
            if ret != index:
                if haveCopyright:
                    writeWfl(wflDict['!HEADER!'], wflfile)
                    haveCopyright = 0
                index = ret + 1
                continue
        if haveCopyright and merge and index >= 10:
            # emit header
            writeWfl(wflDict['!HEADER!'], cfile)
            haveCopyright = 0
        if isFunc(clines[index]):
            fline = clines[index]
            if string.find(fline, ')') == -1:
                fline = fline + clines[index + 1]
            fl = string.split(fline, ')')[0]
            fl = string.split(fl, '(')
            args = string.split(fl[1], ',')
            fl = string.split(fl[0])
            name = fl[-1]
            retval = string.join(fl[0:-1])
            if name[0] == '*':
                name = name[1:]
                retval = retval + '*'
            # if no comment, write out a prototype
            # if splitting, write out comment to wfl
            # if merging, write it out to c file.
            if not wflDict.has_key(name):
                makeProto(name, retval, args)
            if merge:
                writeWfl(wflDict[name], cfile)
            else:
                writeWfl(wflDict[name], wflfile)
        # function or not, write out the line
        cfile.write(clines[index])
        index = index + 1

    # tack on header 
    if not wflDict.has_key('!HEADER!') and not merge:
        makeHeader(f + ".c")
        writeWfl(wflDict['!HEADER!'], wflfile)

def split(file):
    doFile(file, 0)

def merge(file):
    doFile(file, 1)

def fHead(file):
    """If input wfd file has no copyright header, this sequence will
    create one with all the one-line summaries filled in from the
    PURPOSE line."""
    merge(file)
    split(file)
    merge(file)
