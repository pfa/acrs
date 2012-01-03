/* acrs.hpp -- ACRS class template for IPv4/6 prefix summarization
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
 */

#ifndef ACRS_H
#define ACRS_H

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <inttypes.h>
#include <assert.h>

namespace Acrs
{
    class Acrs
    {
    private:
        bool m_logging;
        std::ostream & m_os;
        int m_main_recurse_count;

        /* Compare two routes to determine which should come first.
         *
         * Group by metric,
         * then sort by prefix length in descending order,
         * then sort by network address in ascending order.
         */
        template <class T> static bool acrsCmp(const T & rt1, const T & rt2)
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

            /* Compare network addresses */
            if (rt1.getNetworkN().nbo() < rt2.getNetworkN().nbo())
            {
                return true;
            }
            else
            {
                return false;
            }
        };

        /* Comparison function to sort for overlapping routes.
         * 
         * Sort by network address in ascending order,
         * then by prefix length in ascending order,
         * then by metric in ascending order.
         */
        template <class T> static bool overlapCmp(const T & rt1, const T & rt2)
        {
            if (rt1.getNetworkN().nbo() < rt2.getNetworkN().nbo())
            {
                return true;
            }
            else if (rt1.getNetworkN().nbo() > rt2.getNetworkN().nbo())
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
        };

        /* Summarize and remove overlapping address space.
         * Return true if any summarization was done, return false otherwise.
         */
        template <class T> bool summarizeOverlap(T & rt_container)
        {
            bool summarized = false;

            rt_container.sort(
                  static_cast<bool (*)(const typename T::value_type &,
                                       const typename T::value_type &)>
                                      (overlapCmp));

            typename T::iterator cur = rt_container.begin();
            if (cur == rt_container.end())
            {
                return false;
            }

            typename T::value_type * prev = &(*cur);

            /* prev starts loop at element 0, cur starts at element 1 */
            for (cur++; cur != rt_container.end(); prev = &(*cur++))
            {
                /* Lower network ANDed with higher mask must equal higher
                 * network.
                 */
                if ((cur->getNetworkN() & prev->getMaskN()) !=
                    prev->getNetworkN())
                {
                    continue;
                }

                /* Don't summarize if the summary route would have a
                 * higher metric than the more specific route.
                 */
                if (cur->getMetric() < prev->getMetric())
                {
                    continue;
                }

                /* Overlapping prefixes, remove the less specific one */
                log("*   Removing '" + cur->str() +
                    "', which falls within '" + prev->str() + "'\n");

                cur = rt_container.erase(cur);
                summarized = true;

                /* Reduce cur by one so when it gets incremented
                 * by the for loop it returns to the correct location
                 */
                 cur--;
            }

            return summarized;
        };

        /* Summarize a route list without caring about overlap.
         * Return true if any summarization was done, return false otherwise.
         */
        template <class T> bool summarizeMain(T & rt_container)
        {
            bool summarized = false;

            rt_container.sort(
                  static_cast<bool (*)(const typename T::value_type &,
                                       const typename T::value_type &)>
                                      (acrsCmp));

            m_main_recurse_count++;
            std::stringstream rc;
            rc << m_main_recurse_count;
            log("*   Pass " + rc.str() + "\n");

            typename T::iterator cur = rt_container.begin();

            if (cur == rt_container.end())
            {
                return false;
            }

            typename T::value_type * prev = &(*cur);
            cur++;

            /* prev starts loop at element 0, cur starts at element 1 */
            for (; cur != rt_container.end(); prev = &(*cur), cur++)
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

                /*
                 * If the prefix length is already 0, skip this route as
                 * we can't summarize further.
                 */
                if (prev->getPlen() == 0)
                {
                    continue;
                }

                /* Network addresses must not match. If they happen to,
                 * might as well remove them now so we save a little time
                 * during overlap removal.
                 */
                if (cur->getNetworkN() == prev->getNetworkN())
                {
                    log("*     Removed duplicate prefix: '" +
                        prev->str() + "'\n");
                    cur = rt_container.erase(cur);

                    /* Reduce cur by one so when it gets incremented by
                     * the for loop it returns to the correct location
                     */
                    cur--;
                    summarized = true;

                    continue;
                }

                /* Store prev in string form now, before its plen is
                 * decremented. This way it doesn't need to be incremented then
                 * decremented again if it can be summarized, in order to be
                 * logged.
                 */
                std::string old_prev_str = prev->str();

                /* 1. Temporarily reduce prev's prefix length by one
                 * 2. Apply original prefix length to the new broadcast address
                 *    (cur's plen is the original, so it's safe to use
                 *     that)
                 * 3. Network addresses must match to summarize
                 */
                prev->setPlen(prev->getPlen() - 1);

                typename T::value_type possible(prev->getBroadcastN().getAddr(),
                                                cur->getPlen());

                if (possible.getNetworkN() != cur->getNetworkN())
                {
                    /* Set prev's plen back to what it used to be */
                    prev->setPlen(prev->getPlen() + 1);
                    continue;
                }

                log("*     Summarized '" + old_prev_str + "' and '" +
                    cur->str() + "' into '" + prev->str() + "'\n");

                /* Summarize the routes by:
                 *   1) Decrementing prev's plen (already done above)
                 *   2) Deleting cur
                 */
                cur = rt_container.erase(cur);
                summarized = true;

                /* Reduce cur by one so when it gets incremented by
                 * the for loop it returns to the correct location
                 */
                cur--;
            }

            /* If we summarized at all on this iteration, go over the
             * list again.
             */
            if (summarized == true)
            {
                summarizeMain(rt_container);
                return true;
            }
            else
            {
                log("*     No routes to summarize on this pass.\n");
                return false;
            }
        };

        void log(const std::string & msg) const
        {
            if (m_logging == false)
            {
                return;
            }

            m_os << msg << std::flush;
        };

    public:
        template <class T> bool summarize(T & rt_container)
        {
            if (getLogging() == true)
            {
                m_main_recurse_count = 0;
            }

            log("* Main summarization:\n");
            bool mainsum = summarizeMain(rt_container);

            if (mainsum == false)
            {
                log("*   No routes affected by main summarization.\n");
            }

            log("* Overlap removal:\n");
            bool overlapsum = summarizeOverlap(rt_container);

            if (overlapsum == false)
            {
                log("*   No overlapping routes.\n");
            }

            if ((mainsum || overlapsum) == true)
            {
                log("* Finished. List was summarized.\n");
                return true;
            }
            else
            {
                log("* Finished. No summarization performed.\n");
                return false;
            }
        };

        void setLogging(bool logging)
        {
            m_logging = logging;
        };

        bool getLogging()
        {
            return m_logging;
        };

        /* Constructor */
        Acrs(std::ostream & os = std::cout, bool logging = false)
             :
             m_os(os), m_logging(logging) {};

        /* Destructor */
        virtual ~Acrs() {};
    };
}

#endif /* ACRS_H */
