/* route6.cpp */

#include <iostream>
#include <string>

#include "addr6.hpp"
#include "route6.hpp"

namespace IP
{
    void Route6::printAll(std::ostream & os) const
    {
        Addr6::printAll(os);
        os << "Metric: " << getMetric() << std::endl;
    }

    Route6::Route6(const std::string & addr, const uint32_t plen,
                   const MaskType type, const int metric)
                   : Addr6(addr, plen)
    {
        setMetric(metric);
    }

    Route6::Route6(const std::string & addr, const in6_addr & mask,
                   const MaskType type, const int metric)
                   : Addr6(addr, mask)
    {
        setMetric(metric);
    }

    Route6::Route6(const std::string & addr, const std::string & mask,
                   const int metric)
                   : Addr6(addr, mask)
    {
        setMetric(metric);
    }

    Route6::Route6(const in6_addr & addr, const uint32_t plen,
                   const MaskType type, const int metric)
                   : Addr6(addr, plen)
    {
        setMetric(metric);
    }

    Route6::Route6(const in6_addr & addr, const in6_addr & mask,
                   const MaskType type, const int metric)
                   : Addr6(addr, mask)
    {
        setMetric(metric);
    }

    Route6::Route6(const in6_addr & addr, const std::string & mask,
                   const int metric)
                   : Addr6(addr, mask)
    {
        setMetric(metric);
    }
}
