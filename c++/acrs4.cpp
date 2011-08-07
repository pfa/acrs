/* acrs4.cpp - Automatic classless route summarization
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

#include <stdarg.h>
#include <stdio.h>

#include "acrs4.hpp"
#include "ip4addr.hpp"

namespace Acrs4
{
    void Acrs4::log(const std::string & msg) const
    {
        if (m_logging == false)
        {
            return;
        }

        m_os << msg << std::flush;
    }

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

    bool Acrs4::summarize(void)
    {
        log("* Main summarization:\n");
        bool summarized1 = summarizeMain();

        if (summarized1 == false)
        {
            log("*   No routes affected by main summarization.\n");
        }

        log("* Overlap removal:\n");
        bool summarized2 = summarizeOverlap();

        if (summarized2 == false)
        {
            log("*   No overlapping routes.\n");
        }

        if ((summarized1 | summarized2) == true)
        {
            log("* Finished. List was summarized.\n");
            return true;
        }
        else
        {
            log("* Finished. No summarization performed.\n");
            return false;
        }
    }

    /* summarize and remove overlapping address space.
     * return true if any summarization was done, return false otherwise.
     */
    bool Acrs4::summarizeOverlap(void)
    {
        sort(overlapCmp);
        bool summarized = false;

        /* prev starts at element 0, cur starts at element 1 */
        std::list<IP4Route::IP4Route>::iterator cur = begin();

        if (cur == end())
        {
            return false;
        }

        IP4Route::IP4Route * prev = &(*cur);
        for (cur++; cur != end(); prev = &(*cur), cur++)
        {
            if ((cur->getNetwork().second & prev->getMask().second) ==
                prev->getNetwork().second)
            {
                /* Only use if the summary route would have an equal or
                 * better metric compared to the specific one */
                if (cur->getMetric() < prev->getMetric())
                {
                    continue;
                }

                /* Overlapping prefixes */
                log("*   Removing '" + cur->str() + "', which falls within '" +
                    prev->str() + "'\n");

                erase(cur);
                cur--;
                summarized = true;
            }
        }

        return summarized;
    }

    /* summarize a route list without caring about duplicates.
     * return true if any summarization was done, return false otherwise.
     */
    bool Acrs4::summarizeMain(void)
    {
        bool summarized = false;

        sort(acrsCmp);

        /* prev starts at element 0, cur starts at element 1 */
        std::list<IP4Route::IP4Route>::iterator cur = begin();

        if (cur == end())
        {
            return false;
        }

        IP4Route::IP4Route * prev = &(*cur);
        for (cur++; cur != end(); prev = &(*cur), cur++)
        {
            /* Prefix lengths must match */
            if (prev->getPlen() != cur->getPlen())
            {
                continue;
            }

            /* Metrics must match */
            if (prev->getMetric() != cur->getMetric())
            {
                continue;
            }

            /* Detect integer overflow */
            uint32_t newbc = prev->getBroadcast().second + 1;
            if (newbc == 0)
            {
                continue;
            }

            /* Broadcast of first, plus one, must be equal to network
             * of second */
            if (newbc != cur->getNetwork().second)
            {
                continue;
            }

            if (prev->getPlen() == 0)
            {
                
            }

            /* Net address of the resulting network must be equal
             * to net address of lowest network. */
            IP4Route::IP4Route possible(prev->getNetwork().first,
                                        prev->getPlen() - 1, MaskType::PLEN);
            if (possible.getNetwork().second == prev->getNetwork().second)
            {
                /* Can summarize these */
                log("*   Summarized '" + prev->str() + "' and '" +
                    cur->str() + "' into '");

                erase(cur);
                cur--;
                cur->setPlen(cur->getPlen() - 1);
                summarized = true;

                log(cur->str() + "'\n");
            }
        }

        /* If we summarized at all on this iteration, go over the
         * list again.
         */
        if (summarized == true)
        {
            summarizeOverlap();
            return true;
        }
        else
        {
            return false;
        }
    }
}
