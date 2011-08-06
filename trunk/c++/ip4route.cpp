/* ip4route.cpp - IP route utility
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

#include "ip4addr.hpp"
#include "ip4route.hpp"

namespace IP4Route
{
    in_addr_t m_metric;
    bool m_metric_valid;

    bool IP4Route::setMetricFail()
    {
        return m_metric_valid = false;
    }

    bool IP4Route::setMetricSuccess()
    {
        return m_metric_valid = true;
    }

    bool IP4Route::isValidMetric(int metric) const
    {
        if (metric > MAX_METRIC || metric < MIN_METRIC)
        {
            return false;
        }

        return true;
    }

    int IP4Route::getMetric() const
    {
        if (isValid() == false)
        {
            return 0;
        }

        return m_metric;
    }

    bool IP4Route::setMetric(int metric)
    {
        if (isValidMetric(metric) == false)
        {
            return setMetricFail();
        }

        m_metric = metric;
        return setMetricSuccess();
    }

    bool IP4Route::isValid(void) const
    {
        return m_metric_valid && IP4Addr::isValid();
    }

    /* Constructors */

    IP4Route::IP4Route() : IP4Addr()
    {
        setMetricFail();
    }

    IP4Route::IP4Route(std::string addr_s, uint32_t mask, int metric) :
                       IP4Addr(addr_s, mask)
    {
        setMetric(metric);
    }

    IP4Route::IP4Route(std::string addr_s, std::string snmask_s,
                       int metric) : IP4Addr(addr_s, snmask_s)
    {
        setMetric(metric);
    }

    IP4Route::IP4Route(in_addr_t addr_i, uint32_t mask, int metric) :
                       IP4Addr(addr_i, mask)
    {
        setMetric(metric);
    }

    IP4Route::IP4Route(in_addr_t addr_i, std::string snmask_s, int metric) :
                       IP4Addr(addr_i, snmask_s)
    {
        setMetric(metric);
    }

    void IP4Route::printAll(std::ostream & os)
    {
        IP4Addr::printAll(os);
        os << "Metric: " << getMetric() << std::endl;
    }

    std::ostream & operator<<(std::ostream & os, IP4Route & rt)
    {
        if (rt.isValid() == false)
        {
            os << "Route is not valid." << std::endl;
            return os;
        }

        os << rt.getNetwork().first << "/" << rt.getPlen() << " in "
           << rt.getMetric();

        return os;
    }
}
