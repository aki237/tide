#!/usr/bin/python2
import sys

def PrintBlockComments(filename):
    # Try reading the file
    commentStarted = False
    try:
        f = open(filename)
        for line in f:
            l = line.strip()
            if l == "*/" :
                commentStarted = False
                print "\n"
            if commentStarted :
                print l
            if l == "/*":
                commentStarted = True
            if (not commentStarted) and (l.startswith("// ")):
                print "#" + l[3:]
    except IOError as e:
        print "I/O error({0}): {1}".format(e.errno, e.strerror)
    except:
        print "Unexpected error:", sys.exc_info()[0]
        raise

if __name__ == '__main__' :
    if len(sys.argv) < 2:
        print "Usage :\n\t" + sys.argv[0] + " <filename>"
        exit
    for filename in sys.argv[1:]:
        PrintBlockComments(filename)
