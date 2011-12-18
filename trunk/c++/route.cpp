/* route.cpp - IP route abstract base class
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

#include <sstream>

#include "addr.hpp"
#include "route.hpp"

namespace IP
{
    void Route::setMetricFail()
    {
        m_metric_valid = false;
        return;
    }
    
    void Route::setMetricSuccess()
    {
        m_metric_valid = true;
        return;
    }
    
    int Route::getMetric(void) const
    {
        return m_metric;
    }
    
    bool Route::setMetric(int metric)
    {
        if (isValidMetric(metric))
        {
            m_metric = metric;
            setMetricSuccess();
            return true;
        }
        else
        {
            setMetricFail();
            return false;
        }
    }
    
    bool Route::isValidMetric(int metric) const
    {
        if (metric <= MAX_METRIC && metric >= MIN_METRIC)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int Route::getMaxMetric()
    {
        return MAX_METRIC;
    }

    int Route::getMinMetric()
    {
        return MIN_METRIC;
    }
    
    bool Route::isValid() const
    {
        return Addr::isValid() && m_metric_valid;
    }

    std::ostream & operator<<(std::ostream & os, const Route & rt)
    {
        return os << rt.str();
    }

    std::string Route::str() const
    {
        if (isValid() == false)
        {
            return "Route is not valid.";
        }

        std::stringstream metric;
        metric << getMetric();

        return Addr::str() + " in " + metric.str();
    }

    std::string Route::operator*() const
    {
        return this->str();
    }

    Route::Route() : Addr()
    {
        setMetricFail();
    }

    Route::~Route()
    {
    }
}
