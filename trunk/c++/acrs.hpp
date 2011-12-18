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
#include <list>

#include <inttypes.h>
#include <assert.h>

#include "addr.hpp"
#include "route4.hpp"

namespace Acrs
{
    template <class T> class Acrs : public std::list<T>
    {
    private:
        bool m_logging;
        std::ostream & m_os;
        int m_main_recurse_count;

        /* Compare two routes to determine which should come first.
         *
         * Sort by prefix length in descending order,
         * then by network address in ascending order,
         * then by metric in ascending order.
         */
        static bool acrsCmp(T & rt1, T & rt2)
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
            if (rt1.getNetworkN() < rt2.getNetworkN())
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
        static bool overlapCmp(T & rt1, T & rt2)
        {
            if (rt1 < rt2)
            {
                return true;
            }
            else if (rt1.getNetworkN() > rt2.getNetworkN())
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
        bool summarizeOverlap()
        {
            sort(overlapCmp);
            bool summarized = false;

            typename std::list<T>::iterator cur = this->begin();

            if (cur == this->end())
            {
                return false;
            }

            T * prev = &(*cur);

            /* prev starts loop at element 0, cur starts at element 1 */
            for (cur++; cur != this->end(); prev = &(*cur++))
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
                if (cur->getMetric() > prev->getMetric())
                {
                    continue;
                }

                /* Overlapping prefixes, remove the less specific one */
                log("*   Removing '" + cur->str() +
                    "', which falls within '" + prev->str() + "'\n");

                cur = this->erase(cur);
                summarized = true;

                /* Reduce cur by one so when it gets incremented it
                 * returns to the correct location
                 */
                 cur--;
            }

            return summarized;
        };

        /* Summarize a route list without caring about duplicates.
         * Return true if any summarization was done, return false otherwise.
         */
        bool summarizeMain()
        {
            bool summarized = false;

            m_main_recurse_count++;
            std::stringstream rc;
            rc << m_main_recurse_count;
            log("*   Pass " + rc.str() + "\n");

            typename std::list<T>::iterator cur = this->begin();

            if (cur == this->end())
            {
                return false;
            }

            T * prev = &(*cur);

            /* prev starts loop at element 0, cur starts at element 1 */
            for (cur++; cur != this->end(); prev = &(*cur++))
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

                /* 1. Network addresses must not match to begin with
                 *    XXX - Could speed things up in overlap removal if we
                 *          removed matching addresses here
                 * 2. Reduce prev's prefix length by one
                 * 3. Apply original prefix length to the new broadcast address
                 *    (cur's plen is the original, so it's safe to use that)
                 * 4. Network addresses must match to summarize
                 */
                if (cur->getNetworkN() == prev->getNetworkN())
                {
                    continue;
                }
                prev->setPlen(prev->getPlen() - 1);
                if (T(prev->getBroadcastN().getAddr(),
                      cur->getPlen()).getNetworkN() !=
                    cur->getNetworkN())
                {
                    /* Set prev's plen back to what it used to be */
                    prev->setPlen(prev->getPlen() + 1);
                    continue;
                }

                /* Set prev's plen back to what it used to be */
                prev->setPlen(prev->getPlen() + 1);

                /* Net address of the resulting network must be equal
                 * to net address of lowest network.
                 */
                T possible(prev->getNetworkP(), prev->getPlen() - 1,
                           IP::PLEN);

                assert(possible.isValid() == true);

                if (possible.getNetworkN() == prev->getNetworkN())
                {
                    /* Can summarize these */
                    log("*     Summarized '" + prev->str() + "' and '" +
                        cur->str() + "' into '");

                    cur = this->erase(cur);
                    prev->IP::Addr::setPlen(prev->getPlen() - 1);
                    summarized = true;
                    log(prev->str() + "'\n");

                    /* Reduce cur by one so when it gets incremented it
                     * returns to the correct location
                     */
                    cur--;
                }
            }

            /* If we summarized at all on this iteration, go over the
             * list again.
             */
            if (summarized == true)
            {
                summarizeMain();
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
        bool summarize()
        {
            if (getLogging() == true)
            {
                m_main_recurse_count = 0;
            }

            log("* Main summarization:\n");

            sort(acrsCmp);

            bool mainsum = summarizeMain();

            if (mainsum == false)
            {
                log("*   No routes affected by main summarization.\n");
            }

            log("* Overlap removal:\n");
            bool overlapsum = summarizeOverlap();

            if (overlapsum == false)
            {
                log("*   No overlapping routes.\n");
            }

            if ((mainsum | overlapsum) == true)
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

        void setLogging(bool logging) { m_logging = logging; };
        bool getLogging() { return m_logging; };

        /* Constructor */
        Acrs(std::ostream & os = std::cout, bool logging = false)
             :
             m_os(os), m_logging(logging) {};

        /* Destructor */
        virtual ~Acrs() {};
    };
}

#endif /* ACRS_H */
