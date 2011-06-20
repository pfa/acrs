# ACRS - Automated classless route summarization
# Copyright (C) 2011  Patrick F. Allen
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.

# XXX Summarization functions don't "back up" a route when a route is removed.
# May mean we skip routes. Have to see.

from ip4route import IP4Route
from ip4addr import IP4Addr

class Acrs:
    @staticmethod
    def summarize(rtlist):
        if (rtlist.__class__.__name__ != "list"):
            raise TypeError

        i = 0
        for rt in rtlist:
            if (rt.__class__.__name__ != "IP4Route"):
                raise TypeError
            if (rt.isValid() == False):
                # Return index of invalid route as an argument
                raise invalidRoute(i)
            i += 1

        rtlist, summarized = Acrs._summarize_main(rtlist)
        return Acrs._remove_overlap(rtlist)

    @staticmethod
    def _compare_main(rt1, rt2):
            if (rt1.getMetric() < rt2.getMetric()):
                return -1
            elif (rt1.getMetric() > rt2.getMetric()):
                return 1

            # If reached, metrics are the same

            if (rt1.getPlen() > rt2.getPlen()):
                return -1
            elif (rt1.getPlen() < rt2.getPlen()):
                return 1

            # If reached, prefix lengths are the same

            if (rt1.getNetwork()[1] < rt2.getNetwork()[1]):
                return -1
            elif (rt1.getNetwork()[1] > rt2.getNetwork()[1]):
                return 1

            # Networks were the same.
            # XXX So, delete one? If that doesn't screw up the sort routine,
            # this would take care of overlap removal
            return 0
 
    @staticmethod
    def _cmp_main(rt1, rt2):
        # Deal with blank values
        if (rt1.__class__.__name__ == "NoneType"):
            if (rt2.__class__.__name__ == "NoneType"):
                return 0
            else:
                return -1
        else:
            if (rt2.__class__.__name__ == "NoneType"):
                return -1

        if (rt1.__class__.__name__ != "IP4Route" or
            rt2.__class__.__name__ != "IP4Route"):
            raise TypeError

        c1 = cmp(rt1.getMetric(), rt2.getMetric())

        # Reverse sort using * -1 to get prefix length in descending order
        c2 = cmp(rt1.getPlen(), rt2.getPlen()) * -1
        c3 = cmp(rt1.getNetwork()[1], rt2.getNetwork()[1])

        # Python's OR logic will return the first non-zero value,
        # or the value of c3
        return c1 or c2 or c3

    @staticmethod
    def _summarize_main(rtlist):
        summarized = False
        low = None

        rtlist = sorted(rtlist, cmp=Acrs._cmp_main)

        for high in rtlist:
            if (low == None):
                low = high
                continue

            # Prefix lengths must match
            if (low.getPlen() != high.getPlen()):
                low = high
                continue

            # Metrics must match
            if (low.getMetric() != high.getMetric()):
                low = high
                continue

            # Lower route's broadcast + 1 must equal higher's network address
            if (low.getBroadcast()[1] + 1 != high.getNetwork()[1]):
                low = high
                continue

            # Net address of the resulting network must be equal to the net
            # address of the lower network
            if (low.getNetwork()[1] & IP4Addr.pltosm(low.getPlen() - 1) !=
                                   low.getNetwork()[1]):
                low = high
                continue

            # Routes can be summarized
            rtlist.remove(high)
            low.setPlen(low.getPlen() - 1)

            low = high
            summarized = True

        # If we summarized at all on this iteration, go over the list again.
        if (summarized == True):
            rtlist = Acrs._summarize_main(rtlist)[0]
            return rtlist, True
        else:
            return rtlist, False

    @staticmethod
    def _cmp_overlap(rt1, rt2):
        # Deal with blank values
        if (rt1.__class__.__name__ == "NoneType"):
            if (rt2.__class__.__name__ == "NoneType"):
                return 0
            else:
                return -1
        else:
            if (rt2.__class__.__name__ == "NoneType"):
                return -1
    
        if (rt1.__class__.__name__ != "IP4Route" or
            rt2.__class__.__name__ != "IP4Route"):
            raise TypeError

        c1 = cmp(rt1.getNetwork()[1], rt2.getNetwork()[1])
        c2 = cmp(rt1.getPlen(), rt2.getPlen())
        c3 = cmp(rt1.getMetric(), rt2.getMetric())

        return -1

        # Python's OR logic will return the first non-zero value,
        # or the value of c3
        return c1 or c2 or c3

    @staticmethod
    def _remove_overlap(rtlist):
        summarized = False
        low = None

        rtlist = sorted(rtlist, cmp=Acrs._cmp_overlap)

        for high in rtlist:
            if (low == None):
                low = high
                continue

            if ((high.getNetwork()[1] & low.getMask()[1]) ==
                 low.getNetwork()[1]):
                # Only summarize if the summary route would have an equal or
                # better metric compared to the current route's
                if (high.getMetric() < low.getMetric()):
                    low = high
                    continue

                rtlist.remove(high)
                summarized = True

            low = high

        return rtlist, summarized
