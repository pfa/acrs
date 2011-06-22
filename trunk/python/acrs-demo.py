#!/usr/bin/env python
# Demo of ACRS

from sys import argv
from ip4route import IP4Route
from acrs import summarize

def main(argv):
    rtlist = []

    for rtstr in argv:
        ret = rtstr.split("/")
        if (ret.__len__() != 2):
            print "Error, use CIDR notation (e.g. 1.1.1.1/24):", rtstr
            return 1

        try:
            addr, mask = ret[0], int(ret[1])
        except:
            print "Error, use CIDR notation (e.g. 1.1.1.1/24):", rtstr
            return 1

        rt = IP4Route(addr, mask)
        if (rt.isValid() == False):
            print "Error, invalid route:", rtstr
            return 1

        rtlist.append(rt)

    if (rtlist == []):
        print "One or more valid routes required."
        return 1

    rtlist, summarized = summarize(rtlist)

    for rt in rtlist:
        assert(rt.isValid() == True)
        print rt

    return 0

if (__name__ == "__main__"):
    main(argv[1::])
