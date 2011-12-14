/* route.cpp - IP route abstract base class */

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
