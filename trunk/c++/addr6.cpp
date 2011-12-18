/* addr6.cpp -- IPv6 address class
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

#include <netinet/in.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>

#include "addr6.hpp"

namespace IP
{
    Addr6::Addr6()
    {
        setAddrFail();
        setMaskFail();
    }

    Addr6::Addr6(const std::string & addr, const uint32_t plen, MaskType type)
    {
        setAddr(addr);
        setMask(plen);
    }

    Addr6::Addr6(const std::string & addr, const in6_addr & mask, MaskType type)
    {
        setAddr(addr);
        setMask(mask);
    }

    Addr6::Addr6(const std::string & addr, const std::string & mask)
    {
        setAddr(addr);
        setMask(mask);
    }

    Addr6::Addr6(const in6_addr & addr, const uint32_t plen, MaskType type)
    {
        setAddr(addr);
        setPlen(plen);
    }

    Addr6::Addr6(const in6_addr & addr, const in6_addr & mask, MaskType type)
    {
        setAddr(addr);
        setMask(mask);
    }

    Addr6::Addr6(const in6_addr & addr, const std::string & mask)
    {
        setAddr(addr);
        setMask(mask);
    }

    bool Addr6::setAddr(const std::string & addr)
    {
        if (addr == "")
        {
            setAddrFail();
            return false;
        }

        in6_addr buf;

        if (! inet_pton(getAddrFamily(), addr.c_str(), &buf))
        {
            setAddrFail();
            return false;
        }

        m_addr_net_form.setAddr(buf);
        m_addr_pres_form = addr;
        setAddrSuccess();
        return true;
    }

    bool Addr6::setAddr(const in6_addr & addr)
    {
        char buf[getAddrStrLen()];

        if (! inet_ntop(getAddrFamily(), &addr, buf, getAddrStrLen()))
        {
            setAddrFail();
            return false;
        }

        m_addr_net_form.setAddr(addr);
        m_addr_pres_form = buf;
        setAddrSuccess();
        return true;
    }

    /* Requires that in6_addr.s6_addr be a uint8_t[16], which may not be true on
     * all systems.
     */
    bool Addr6::isValidSnmask(const in6_addr & mask) const
    {
        const void * p_end = &(mask.s6_addr[sizeof(mask.s6_addr) - 1]);
        bool sawzero = false;

        for (uint8_t * p_addr_byte = const_cast<uint8_t *>(mask.s6_addr);
             p_addr_byte != p_end;
             p_addr_byte++)
        {
            if (sawzero == true)
            {
                /* After seeing a 0, all remaining bytes should be 0 */
                if (*p_addr_byte != 0)
                {
                    return false;
                }
            }
            else if (*p_addr_byte != 255)
            {
                /* At least one bit was not 0 */
                sawzero = true;

                /* Check if the byte is valid */
                if (isValidSnmaskOnce(*p_addr_byte) == false)
                {
                    return false;
                }
            }
        }

        /* All bytes were valid */
        return true;
    }

    bool Addr6::setMask(const std::string & mask)
    {
        if (mask == "")
        {
            setMaskFail();
            return false;
        }

        in6_addr buf;

        if (! inet_pton(getAddrFamily(), mask.c_str(), &buf))
        {
            setMaskFail();
            return false;
        }

        if (! isValidSnmask(buf))
        {
            setMaskFail();
            return false;
        }

        m_mask_net_form.setAddr(buf);
        m_plen = smtopl(buf);
        setMaskSuccess();
        return true;
    }

    bool Addr6::setMask(const in6_addr & mask)
    {
        char buf[getAddrStrLen()];

        if (! isValidSnmask(mask))
        {
            setMaskFail();
            return false;
        }

        if (! inet_ntop(getAddrFamily(), &mask, buf, getAddrStrLen()))
        {
            setMaskFail();
            return false;
        }

        m_mask_net_form.setAddr(mask);
        m_plen = smtopl(mask);
        setMaskSuccess();
        return true;
    }

    bool Addr6::setMask(const uint32_t plen)
    {
        return setPlen(plen);
    }

    Addr6NetForm Addr6::getBroadcastN() const
    {
        return (Addr6NetForm(getNetworkN()) |
               Addr6NetForm(getHostmaskN())).getAddr();
    }

    Addr6NetForm Addr6::getNetworkN() const
    {
        return (getNetFormAddr() & getNetFormMask()).getAddr();
    }

    Addr6NetForm Addr6::getHostmaskN() const
    {
        return (~ getNetFormMask()).getAddr();
    }

    Addr6NetForm Addr6::getAddrN() const
    {
        return getNetFormAddr().getAddr();
    }

    Addr6NetForm Addr6::getMaskN() const
    {
        return getNetFormMask().getAddr();
    }

    std::string Addr6::getAddrP() const
    {
        std::tr1::function<Addr6NetForm ()> getFunc =
                std::tr1::bind(std::mem_fun(&IP::Addr6::getAddrN), this);

        return getPresData(getFunc);
    }

    std::string Addr6::getMaskP() const
    {
        std::tr1::function<Addr6NetForm ()> getFunc =
                std::tr1::bind(std::mem_fun(&IP::Addr6::getMaskN), this);

        return getPresData(getFunc);
    }

    std::string Addr6::getNetworkP() const
    {
        std::tr1::function<Addr6NetForm ()> getFunc =
                std::tr1::bind(std::mem_fun(&IP::Addr6::getNetworkN), this);

        return getPresData(getFunc);
    }

    std::string Addr6::getBroadcastP() const
    {
        std::tr1::function<Addr6NetForm ()> getFunc =
                std::tr1::bind(std::mem_fun(&IP::Addr6::getBroadcastN), this);

        return getPresData(getFunc);
    }

    std::string Addr6::getHostmaskP() const
    {
        std::tr1::function<Addr6NetForm ()> getFunc =
                std::tr1::bind(std::mem_fun(&IP::Addr6::getHostmaskN), this);

        return getPresData(getFunc);
    }

    Addr6NetForm Addr6::getNetFormAddr() const
    {
        return m_addr_net_form;
    }

    Addr6NetForm Addr6::getNetFormMask() const
    {
        return m_mask_net_form;
    }

    int Addr6::getAddrStrLen() const
    {
        return INET6_ADDRSTRLEN;
    }

    int Addr6::getAddrFamily() const
    {
        return AF_INET6;
    }

    void Addr6::updateNetInfo(void)
    {
        return;
    }

    void Addr6::printAll(std::ostream & os) const
    {
        if (isValid() == false)
        {
            os << "IP and/or mask is not valid." << std::endl;
            return;
        }

        os << "IP Address:    " << getAddrP() /* << " (" << getAddrN() << ")" */ << std::endl
           << "Prefix length: " << getPlen() << std::endl
           << "Subnet Mask:   " << getMaskP() /* << " (" << getMaskN() << ")" */ << std::endl
           << "Broadcast:     " << getBroadcastP() /* << " (" << getBroadcastN() << ")" */ << std::endl
           << "Network:       " << getNetworkP() /* << " (" << getNetworkN() << ")" */ << std::endl
           << "Hostmask:      " << getHostmaskP() /* << " (" << getHostmaskN() << ")" */ << std::endl;

        return;
    }

    /* Mask must be validated in the caller. This counts the number of
     * bits set to 1, assuming the mask is valid. (That is, it stops
     * counting after it counts a byte that is not all 1s.)
     */
    uint32_t Addr6::smtopl(const in6_addr & mask) const
    {
        const uint8_t * p_end = &(mask.s6_addr[sizeof(mask.s6_addr) - 1]);
        int plen = 0;

        for (uint8_t * p_mask_byte = const_cast<uint8_t *>(mask.s6_addr);
             p_mask_byte <= p_end;
             p_mask_byte++)
        {
            if (*p_mask_byte != 255)
            {
                return plen + onBits(*p_mask_byte);
            }
            else
            {
                plen += 8;
            }
        }

        /* All bits on */
        return plen;
    }

    bool Addr6::setSnmaskFromPlen(const uint32_t plen)
    {
        return setMask(pltosm(plen));
    }

    /* Prefix length should be validated by the caller */
    in6_addr Addr6::pltosm(const uint32_t plen) const
    {
        in6_addr mask;
        int count = 0;
        int fullbytes = plen / 8;

        /* Init mask to all 0s */
        memset(&mask.s6_addr, 0, sizeof(mask.s6_addr));

        for (count = 0; count != fullbytes; count++)
        {
            mask.s6_addr[count] = 255;
        }

        int extra_bytes = plen % 8;

        if (extra_bytes == 0)
        {
            return mask;
        }

        /* Magic number 8 = number of bits in a byte
         *
         * In the last non-zero mask byte, flip on a number of bits equal to
         * extra bytes from the prefix length.
         */
        mask.s6_addr[count] = ~((1 << ((8 - (extra_bytes)) + 1)) - 1);

        return mask;
    }

    bool Addr6::operator<(const Addr6 & other) const
    {
        return getNetFormAddr() < other.getNetFormAddr();
    }
}
