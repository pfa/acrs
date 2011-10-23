/* acrs4.hpp */

#ifndef ACRS4_H
#define ACRS4_H

#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "ip4route.hpp"

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

            if (rt1.getNetwork().second < rt2.getNetwork().second)
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
        };

        /* Summarize and remove overlapping address space.
         *  Return true if any summarization was done, return false otherwise.
         */
        bool summarizeOverlap(void)
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
            for (cur++; cur != this->end(); prev = &(*cur), cur++)
            {
                if ((cur->getNetwork().second & prev->getMask().second) ==
                    prev->getNetwork().second)
                {
                    /* Only use if the summary route would have an equal or
                     * better metric compared to the more specific route */
                    if (cur->getMetric() < prev->getMetric())
                    {
                        continue;
                    }

                    /* Overlapping prefixes */
                    log("*   Removing '" + cur->str() +
                        "', which falls within '" + prev->str() + "'\n");

                    this->erase(cur);
                    cur--;
                    summarized = true;
                }
            }

            return summarized;
        };

        /* Summarize a route list without caring about duplicates.
         * Return true if any summarization was done, return false otherwise.
         */
        bool summarizeMain(void)
        {
            bool summarized = false;

            sort(acrsCmp);

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
            for (cur++; cur != this->end(); prev = &(*cur), cur++)
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

                /* Broadcast of first, plus one, must be equal to
                 * network of second */
                if (newbc != cur->getNetwork().second)
                {
                    continue;
                }

                /* Net address of the resulting network must be equal
                 * to net address of lowest network.
                 */
                T possible(prev->getNetwork().first, prev->getPlen() - 1,
                           IP4Addr::IP4Addr::PLEN);
                if (possible.getNetwork().second == prev->getNetwork().second)
                {
                    /* Can summarize these */
                    log("*     Summarized '" + prev->str() + "' and '" +
                        cur->str() + "' into '");

                    this->erase(cur);
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
                summarizeMain();
                return true;
            }
            else
            {
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
        bool summarize(void)
        {
            if (getLogging() == true)
            {
                m_main_recurse_count = 0;
            }

            log("* Main summarization:\n");
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
        bool getLogging(void) { return m_logging; };

        /* Constructor */
        Acrs(std::ostream & os = std::cout, bool logging = false)
             :
             m_os(os), m_logging(logging) {};

        /* Destructor */
        virtual ~Acrs(void) {};
    };
}

#endif /* ACRS4_H */
