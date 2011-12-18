/* addr.cpp - IP address abstract base class
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

#include <string>
#include <sstream>
#include <iostream>
#include <tr1/functional>

#include <arpa/inet.h>
#include <inttypes.h>

#include "addr.hpp"

namespace IP
{
    bool Addr::setAddrSuccess()
    {
        if (m_mask_valid == true)
        {
            updateNetInfo();
        }

        return m_addr_valid = true;
    }

    bool Addr::setAddrFail()
    {
        return m_addr_valid = false;
    }

    bool Addr::setMaskSuccess()
    {
        if (m_addr_valid == true)
        {
            updateNetInfo();
        }

        return m_mask_valid = true;
    }

    bool Addr::setMaskFail()
    {
        return m_mask_valid = false;
    }

    uint32_t Addr::getPlen() const
    {
        if (! isValid())
        {
            return 0;
        }

        return m_plen;
    }

    bool Addr::isValid() const
    {
        return (m_addr_valid && m_mask_valid);
    }

    std::string Addr::str() const
    {
        if (isValid() == false)
        {
            return "";
        }

        std::stringstream ss;
        ss << getPlen();

        return getNetworkP() + "/" + ss.str();
    }

    std::string Addr::operator*() const
    {
        return str();
    }

    std::ostream & operator<<(std::ostream & os, Addr & ip)
    {
        return os << ip.str();
    }

    bool Addr::setPlen(const uint32_t plen)
    {
        return setSnmaskFromPlen(plen);
    }

    bool Addr::isValidPlen(const uint32_t plen) const
    {
        if (plen < 0 || plen > getMaxPlen())
        {
            return false;
        }

        return true;
    }

    Addr::~Addr()
    {
    }

    bool Addr::isValidSnmaskOnce(const uint32_t mask) const
    {
        /* Mask is valid if its inverse in NBO is one less than a power of 2 */
        if (((~htonl(mask)) & ((~htonl(mask)) + 1)) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /* Counts the number of bits set to 1 in a 4 byte integer */
    uint32_t Addr::onBits(uint32_t bytes) const
    {
        int count;

        /* Below is from:
         *    http://graphics.stanford.edu/~seander/bithacks.html
         */
        for (count = 0; bytes; count++)
        {
            bytes &= bytes - 1;
        }

        return count;
    }
}
