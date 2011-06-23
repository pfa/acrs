#!/usr/bin/env python
# CGI demo of ACRS
# Needs acrs.py, ip4route.py, and ip4addr.py in the python search path
# (such as /usr/lib/python2.6/ or the current directory).

import cgi
import cgitb

import ip4route
import acrs

# ARGS_LIMIT limits the number of routes that may be submitted, to prevent
# misuse. Feel free to crank it up if you like.
#
# On my desktop, summarizing 100 consecutive host routes (not very
# strenuous) completes in ~50-60ms. (The C++ version is faster by orders of
# magnitude.)
ARGS_LIMIT = 50

def main():
    rtlist = []

    print "Content-type: text/html\n\n"
    print "<h1>Results:</h1>"
    print "(Use your browser's back button to return to the previous page.)"
    print "</br></br>"

    # If an exception is thrown, cgitb will allow errors to be printed to
    # the web page, in addition to the python version and a timestamp.
    # This is useful for debugging purposes. If you don't want this to happen,
    # either comment out the next line, or see the python documentation
    # on how to make it write to a log file.
    cgitb.enable()
    form = cgi.FieldStorage()

    if (not "summary-input" in form):
        print "Error: One or more prefixes required."
        return 1

    args = form["summary-input"].value.split()

    if (args.__len__() > ARGS_LIMIT):
        print "The web version of this demo is artificially limited to", \
              ARGS_LIMIT, "prefixes so the web server doesn't get too bogged " \
              "down. Consider downloading a command line version from " \
              "acrs.googlecode.com (it's open source and no cost) " \
              "or bribing the server operator to increase ARGS_LIMIT in ", \
              "acrs-web-demo.py."
        return 1

    for rtstr in args:
        ret = rtstr.split("/")
        if (ret.__len__() != 2):
            print "Error, use CIDR notation (e.g. 1.1.1.0/24):", rtstr
            return 1

        try:
            addr, mask = ret[0], int(ret[1])
        except:
            print "Error, use CIDR notation (e.g. 1.1.1.0/24):", rtstr
            return 1

        rt = ip4route.IP4Route(addr, mask)
        if (rt.isValid() == False):
            print "Error, invalid route:", rtstr
            return 1

        rtlist.append(rt)

    if (rtlist == []):
        print "One or more valid routes required."
        return 1

    rtlist, summarized = acrs.summarize(rtlist)

    for rt in rtlist:
        assert(rt.isValid() == True)
        print rt, "</br>"

    return 0

if (__name__ == "__main__"):
    try:
        main()
    except:
        print "<h2>Something broke, but it wasn't your fault. Please report " \
              "the following error.<h2/>"
        raise
