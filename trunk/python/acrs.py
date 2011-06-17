#!/usr/bin/env python

# XXX Summarization functions don't "back up" a route when a route is removed.
# May mean we skip routes. Have to see.

class Acrs:
    def summarize(rtlist):
        rtlist = _summarize_main(rtlist)
        return _remove_overlap(rtlist)

    # See here for sort info:
    # http://icfun.blogspot.com/2009/08/python-customize-sorting-with.html
    def _compare_main(rt1, rt2):
            if (rt1.getMetric() < rt2.getMetric()):
                return True
            elif (rt1.getMetric() > rt2.getMetric()):
                return False

            # If reached, metrics are the same

            if (rt1.getPlen() > rt2.getPlen()):
                return True
            elif (rt1.getPlen() < rt2.getPlen()):
                return False

            # If reached, prefix lengths are the same

            if (rt1.getNetwork() < rt2.getNetwork()):
                return True
            elif (rt1.getNetwork > rt2.getNetwork()):
                return False

    def _summarize_main(rtlist):
        summarized = False

        rtlist.sort(_compare_main)

        for high in rtlist:
            if (not low):
                continue

            # Prefix lengths must match
            if (low.getPlen() != high.getPlen()):
                continue

            # Metrics must match
            if (low.getMetric() != high.getMetric()):
                continue

            # Lower route's broadcast + 1 must equal higher's network address
            if (low.getBroadcast() + 1 != high.getNetwork()):
                continue

            # Net address of the resulting network must be equal to the net
            # address of the lower network
            if (low.getNetwork() & acrs.smtopl(low.getPlen()) !=
                low.getNetwork()):
                continue

            # Routes can be summarized
            rtlist.remove(high)
            low.setPlen(low.getPlen() - 1)

            summarized = true
            low = high

        # If we summarized at all on this iteration, go over the list again.
        if (summarized == True):
            _summarize_main(rtlist)
            return True
        else:
            return False

    def _compare_overlap(rtlist):
        if (rt1.getNetwork() < rt2.getNetwork()):
            return True
        elif (rt1.getNetwork() > rt2.getNetwork()):
            return False

        # If reached, network addresses are the same

        if (rt1.getPlen() < rt2.getPlen()):
            return True
        elif (rt1.getPlen() > rt2.getPlen()):
            return False

        # If reached, prefix lengths are the same

        if (rt1.getMetric() < rt2.getMetric()):
            return True
        else:
            return False

    def _remove_overlap(rtlist):
        summarized = False

        rtlist.sort(_compare_overlap)

        for high in rtlist:
            if (not low):
                continue

            if ((high.getNetwork() & low.getSnmask()) == low.getNetwork()):
                # Only summarize if the summary route would have an equal or
                # better metric compared to the current route's
                if (high.getMetric() < low.getMetric()):
                    continue

                rtlist.remove(high)
                summarized = True

        return summarized
