/* addr.cpp - IP address abstract base class */

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
