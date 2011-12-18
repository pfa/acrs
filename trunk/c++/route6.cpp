/* route6.cpp -- IPv6 route class
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
