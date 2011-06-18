/* acrs.cpp - Automatic classless route summarization
 *
 * Copyright 2011 Patrick F. Allen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

#include "acrs.hpp"
#include "ip4addr.hpp"

namespace IP4Addr { namespace Acrs {
    /* Compare two AcrsRoute4 to determine which should come first.
     *
     * Sort by prefix length in descending order,
     * then by network address in ascending order,
     * then by metric in ascending order.
     */
    bool acrsCmp(AcrsRoute4 & rt1, AcrsRoute4 & rt2)
    {
        if (rt1.getMetric() < rt2.getMetric())
        {
            return true;
        }
        else if (rt1.getMetric() > rt2.getMetric())
        {
            return false;
        }

        /* If reached, metrics are the same */

        if (rt1.getPlen() > rt2.getPlen())
        {
            return true;
        }
        else if (rt1.getPlen() < rt2.getPlen())
        {
            return false;
        }

        /* If reached, prefix lengths are the same */

        if (rt1.getNetwork(0) < rt2.getNetwork(0))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /* Comparison function to sort for overlapping routes.
     * 
     * Sort by network address in ascending order,
     * then by prefix length in ascending order,
     * then by metric in ascending order.
     */
    bool overlapCmp(AcrsRoute4 & rt1, AcrsRoute4 & rt2)
    {
        if (rt1.getNetwork(0) < rt2.getNetwork(0))
        {
            return true;
        }
        else if (rt1.getNetwork(0) > rt2.getNetwork(0))
        {
            return false;
        }

        /* If reached, network addresses are the same */

        if (rt1.getPlen() < rt2.getPlen())
        {
            return true;
        }
        else if (rt1.getPlen() > rt2.getPlen())
        {
            return false;
        }

        /* If reached, prefix lengths the same */

        if (rt1.getMetric() < rt2.getMetric())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /* Summarize and remove overlapping address space.
     * return true if any summarization was done, return false otherwise.
     */
    bool Summarize(std::list<AcrsRoute4> & rtlist, bool logging,
                   std::ostream & os)
    {
        if (logging)
        {
            os << "* Main summarization:" << std::endl;
        }

        bool summarized = SummarizeOverlap(rtlist, logging, os);
        std::list<AcrsRoute4>::iterator oiter;

        if (logging)
        {
            os << "* Overlap removal:" << std::endl;
        }

        rtlist.sort(overlapCmp);

        AcrsRoute4 * prev = NULL;

        for (std::list<AcrsRoute4>::iterator cur = rtlist.begin();
             cur != rtlist.end();
             prev = &(*cur), cur++)
        {
            if (! prev)
            {
                continue;
            }

            if ((cur->getNetwork(0) & prev->getSnmask(0)) ==
                prev->getNetwork(0))
            {
                /* Only use if the summary route would
                 * have an equal or better metric compared to
                 * the specific one */
                if (cur->getMetric() < prev->getMetric())
                {
                    continue;
                }

                /* Overlapping prefixes */
                if (logging)
                {
                    os << "*   Removing '" << *cur
                       << "', which falls within '"
                       << *prev << "'" << std::endl;
                }

                rtlist.erase(cur);
                cur--;
                summarized = true;

            }
        }

        if (logging)
        {
            os << "* Finished. ";

            if (summarized)
            {
                os << "List was summarized."
                       << std::endl;
            }
            else
            {
                os << "No summarization performed."
                       << std::endl;
            }
        }
        return summarized;
    }

    /* Summarize a route list without caring about duplicates.
     * return true if any summarization was done, return false otherwise.
     */
    bool SummarizeOverlap(std::list<AcrsRoute4> & rtlist,
                          bool logging, std::ostream & os)
    {
        AcrsRoute4 * prev = NULL;
        bool summarized = false;

        rtlist.sort(acrsCmp);

        for (std::list<AcrsRoute4>::iterator cur = rtlist.begin();
             cur != rtlist.end();
             prev = &(*cur), cur++)
        {
            if (! prev)
            {
                continue;
            }

            // Prefix lengths must match
            if (prev->getPlen() != cur->getPlen())
            {
                continue;
            }

            /* Metrics must match */
            if (prev->getMetric() != cur->getMetric())
            {
                continue;
            }

            if (prev->getBroadcast(0) + 1 != cur->getNetwork(0))
            {
                continue;
            }

            /* Net address of the resulting network must be equal
             * to net address of lowest network. */
            char buf[INET_ADDRSTRLEN];
            prev->getNetwork(buf);
            AcrsRoute4 possible(buf, prev->getPlen() - 1, 0);
            if (possible.getNetwork(0) == prev->getNetwork(0))
            {
                /* Can summarize these */

                if (logging)
                {
                    os << "*   Summarized '" << *prev
                       << "' and '" << *cur << "' into '";
                }

                rtlist.erase(cur);
                cur--;
                cur->setPlen(cur->getPlen() - 1);

                summarized = true;

                if (logging)
                {
                    os << *cur << "'" << std::endl;
                }
            }
        }

        /* If we summarized at all on this iteration, go over the
         * list again.
         */
        if (summarized == true)
        {
            SummarizeOverlap(rtlist, logging, os);
            return true;
        }
        else
        {
            return false;
        }
    }

    void AcrsRoute4::init(int metric)
    {
        m_metric = metric;
    }

    AcrsRoute4::AcrsRoute4(int metric) : IP4Addr::IP4Addr()
    {
        AcrsRoute4::init(metric);
    }

    AcrsRoute4::AcrsRoute4(in_addr_t addr, in_addr_t mask, int metric)
                           : IP4Addr::IP4Addr(addr, mask)
    {
        AcrsRoute4::init(metric);
    }

    AcrsRoute4::AcrsRoute4(char * addr, char * mask, int metric)
                           : IP4Addr::IP4Addr(addr, mask)
    {
        AcrsRoute4::init(metric);
    }

    AcrsRoute4::AcrsRoute4(char * addr, plen_t plen, int metric)
                           : IP4Addr::IP4Addr(addr, plen)
    {
        AcrsRoute4::init(metric);
    }

    std::ostream & operator<<(std::ostream & os, AcrsRoute4 & rt)
    {
        char net[INET_ADDRSTRLEN];

        rt.getNetwork(net);

        os << net << "/" << (int) rt.getPlen() << " in "
           << rt.getMetric();

        return os;
    }
} }
