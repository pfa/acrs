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
#include <string>

#include "acrs4.hpp"
#include "ip4addr.hpp"

namespace Acrs4 {
    /* Compare two IP4Route::IP4Route to determine which should come first.
     *
     * Sort by prefix length in descending order,
     * then by network address in ascending order,
     * then by metric in ascending order.
     */
    bool acrsCmp(IP4Route::IP4Route & rt1, IP4Route::IP4Route & rt2)
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

        if (rt1.getNetwork().second < rt2.getNetwork().second)
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
    bool overlapCmp(IP4Route::IP4Route & rt1, IP4Route::IP4Route & rt2)
    {
        if (rt1.getNetwork().second < rt2.getNetwork().second)
        {
            return true;
        }
        else if (rt1.getNetwork().second > rt2.getNetwork().second)
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
    bool Summarize(std::list<IP4Route::IP4Route> & rtlist, bool logging,
                   std::ostream & os)
    {
        if (logging)
        {
            os << "* Main summarization:" << std::endl;
        }

        bool summarized = SummarizeOverlap(rtlist, logging, os);
        std::list<IP4Route::IP4Route>::iterator oiter;

        if (logging)
        {
            os << "* Overlap removal:" << std::endl;
        }

        rtlist.sort(overlapCmp);

        IP4Route::IP4Route * prev = NULL;

        for (std::list<IP4Route::IP4Route>::iterator cur = rtlist.begin();
             cur != rtlist.end();
             prev = &(*cur), cur++)
        {
            if (! prev)
            {
                continue;
            }

            if ((cur->getNetwork().second & prev->getMask().second) ==
                prev->getNetwork().second)
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
    bool SummarizeOverlap(std::list<IP4Route::IP4Route> & rtlist,
                          bool logging, std::ostream & os)
    {
        IP4Route::IP4Route * prev = NULL;
        bool summarized = false;

        rtlist.sort(acrsCmp);

        for (std::list<IP4Route::IP4Route>::iterator cur = rtlist.begin();
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

            if (prev->getBroadcast().second + 1 != cur->getNetwork().second)
            {
                continue;
            }

            /* Net address of the resulting network must be equal
             * to net address of lowest network. */
            IP4Route::IP4Route possible(prev->getNetwork().first,
                                prev->getPlen() - 1, 0);
            if (possible.getNetwork().second == prev->getNetwork().second)
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
}
