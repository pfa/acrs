/* route4.cpp */

#include <iostream>
#include <string>

#include "addr4.hpp"
#include "route4.hpp"

namespace IP
{
    void Route4::printAll(std::ostream & os) const
    {
        Addr4::printAll(os);
        os << "Metric: " << getMetric() << std::endl;
    }

    Route4::Route4(const std::string & addr, const uint32_t mask,
                   const MaskType type, const int metric)
                   : Addr4(addr, mask, type)
    {
        setMetric(metric);
    }

    Route4::Route4(const std::string & addr, const std::string & mask,
                   const int metric)
                   : Addr4(addr, mask)
    {
        setMetric(metric);
    }

    Route4::Route4(const in_addr_t addr, const uint32_t mask,
                   const MaskType type, const int metric)
                   : Addr4(addr, mask, type)
    {
        setMetric(metric);
    }

    Route4::Route4(const in_addr_t addr, const std::string & mask,
                   const int metric)
                   : Addr4(addr, mask)
    {
        setMetric(metric);
    }
}
