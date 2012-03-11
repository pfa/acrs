#!/usr/bin/env python
# Demo of ACRS. Usage:
#    ./acrs-demo.py <LIST OF ROUTES>
# List of routes should be a space separated list of IPv4 routes in CIDR
# notation. For example:
#
#    ./acrs-demo.py 192.168.0.0/24 192.168.1.0/24
#
# The output for the above input would be 192.168.0.0/23.
# See the wiki on acrs.googlecode.com for more information.

import sys
import ip4route
import acrs

def main(argv):
    rtlist = []

    for rtstr in argv[1:]:
        ret = rtstr.split("/")
        if (ret.__len__() != 2):
            print("Error, use CIDR notation (e.g. 1.1.1.1/24):", rtstr)
            return 255

        try:
            addr, mask = ret[0], int(ret[1])
        except:
            print("Error, use CIDR notation (e.g. 1.1.1.1/24):", rtstr)
            return 255

        rt = ip4route.IP4Route(addr, mask)
        if (rt.isValid() == False):
            print("Error, invalid route:", rtstr)
            return 255

        rtlist.append(rt)

    if (rtlist == []):
        print("One or more valid routes required.")
        return 255

    rtlist, summarized = acrs.summarize(rtlist)

    for rt in rtlist:
        assert(rt.isValid() == True)
        print(rt)

    if summarized == True:
        return 0
    elif summarized == False:
        return 1
    else:
        assert(False)

if (__name__ == "__main__"):
    sys.exit(main(sys.argv))
