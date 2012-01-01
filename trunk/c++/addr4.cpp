/* addr4.cpp -- IPv4 address utility class
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
#include <cstdio>
#include <string>
#include <cstring>
#include <sstream>
#include <tr1/functional>

#include <arpa/inet.h>
#include <inttypes.h>

#include "addr4.hpp"
#include "addr4netform.hpp"

namespace IP
{
    /* Get functions */

    Addr4NetForm Addr4::getAddrN() const
    {
        return getNetFormAddr().getAddr();
    }

    Addr4NetForm Addr4::getMaskN() const
    {
        return getNetFormMask().getAddr();
    }

    Addr4NetForm Addr4::getBroadcastN() const
    {
        return getNetworkN() | getHostmaskN();
    }

    Addr4NetForm Addr4::getHostmaskN() const
    {
        return ~ getMaskN();
    }

    Addr4NetForm Addr4::getNetworkN() const
    {
        return getMaskN() & getAddrN();
    }

    std::string Addr4::getAddrP() const
    {
        return m_addr_pres_form;
    }

    std::string Addr4::getMaskP() const
    {
        return m_snmask_pres_form;
    }

    std::string Addr4::getNetworkP() const
    {
        /* Bind the getNetworkN member function to a non-member function */
        std::tr1::function<Addr4NetForm ()> getFunc =
                std::tr1::bind(std::mem_fun(&IP::Addr4::getNetworkN), this);

        return getPresData(getFunc);
    }

    std::string Addr4::getHostmaskP() const
    {
        /* Bind the getHostmaskN member function to a non-member function */
        std::tr1::function<Addr4NetForm ()> getFunc =
                std::tr1::bind(std::mem_fun(&IP::Addr4::getHostmaskN), this);

        return getPresData(getFunc);
    }

    std::string Addr4::getBroadcastP() const
    {
        /* Bind the getBroadcastN member function to a non-member function */
        std::tr1::function<Addr4NetForm ()> getFunc =
               std::tr1::bind(std::mem_fun(&IP::Addr4::getBroadcastN), this);

        return getPresData(getFunc);
    }
 
    /* Set functions */

    bool Addr4::setAddr(const std::string & addr)
    {
        if (addr == "")
        {
            setAddrFail();
            return false;
        }

        in_addr_t buf;
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

    bool Addr4::setAddr(const in_addr_t addr)
    {
        return m_addr_net_form.setAddr(addr);
    }

    bool Addr4::setSnmask(const std::string & snmask)
    {
        char buf[getAddrStrLen()];
        strncpy(buf, snmask.c_str(), getAddrStrLen());

        if (! inet_pton(getAddrFamily(), buf, &m_mask_net_form))
        {
            setMaskFail();
            return false;
        }

        /* XXX Fix me */
        m_plen = smtopl(m_mask_net_form.getAddr());
        //m_mask_net_form =
        m_snmask_pres_form = buf;

        setMaskSuccess();
        return true;
    }

    bool Addr4::setSnmask(in_addr_t snmask_i)
    {
        char buf[getAddrStrLen()];

        if (! (inet_ntop(getAddrFamily(), &snmask_i, buf, getAddrStrLen())))
        {
            setMaskFail();
            return false;
        }

        if (! isValidSnmask(snmask_i))
        {
            setMaskFail();
            return false;
        }

        m_snmask_pres_form = buf;
        m_plen = smtopl(snmask_i);
        m_mask_net_form = snmask_i;

        setMaskSuccess();
        return true;
    }

    /* Determines whether the argument is a prefix length or a subnet mask
     * and calls setPlen or setSnmask accordingly.
     */
    bool Addr4::setMask(const uint32_t mask)
    {
        if (isValidPlen(mask))
        {
            return setPlen(mask);
        }
        else if (isValidSnmask(mask))
        {
            return setSnmask(mask);
        }
        else
        {
            return setMaskFail();
        }
    }

    bool Addr4::isValidSnmask(const uint32_t mask) const
    {
        return isValidSnmaskOnce(mask);
    }

    bool Addr4::setMask(const std::string & snmask)
    {
        return setSnmask(snmask);
    }

    /* Compute any members that depend on the mask and address. Since the
     * hostmask, network address, etc., are computed only when their 'get'
     * functions are called (not stored as member variables), this function
     * doesn't do much now. At the price of increased memory usage, to increase
     * speed this can be modified if you use particular derived network data
     * more often than you change the IP or mask on the Addr4 object.
     */
    void Addr4::updateNetInfo()
    {
        return;
    }

    bool Addr4::setSnmaskFromPlen(const uint32_t plen)
    {
        return setSnmask(pltosm(plen));
    }

    /* Constructors */
    Addr4::Addr4()
    {
        setAddrFail();
        setMaskFail();
    }

    Addr4::Addr4(const std::string & addr, const std::string & snmask)
    {
        setAddr(addr);
        setMask(snmask);
    }

    Addr4::Addr4(const std::string & addr, uint32_t mask, MaskType type)
    {
        setAddr(addr);

        switch (type)
        {
        case UNSPEC:
            setMask(mask);
            break;
        case PLEN:
            setPlen(mask);
            break;
        case SNMASK:
            setSnmask(mask);
            break;
        default:
            setMaskFail();
            break;
        }
    }

    Addr4::Addr4(in_addr_t addr, const std::string & snmask)
    {
        setAddr(addr);
        setMask(snmask);
    }

    Addr4::Addr4(in_addr_t addr, uint32_t mask, MaskType type)
    {
        setAddr(addr);

        switch (type)
        {
        case UNSPEC:
            setMask(mask);
            break;
        case PLEN:
            setPlen(mask);
            break;
        case SNMASK:
            setSnmask(mask);
            break;
        default:
            setMaskFail();
            break;
        }
    }

    /* Other */

    std::ostream & operator<<(std::ostream & os, Addr4 & ip)
    {
        return os << ip.str();
    }

    void Addr4::printAll(std::ostream & os) const
    {
        if (isValid() == false)
        {
            os << "IP and/or mask is not valid." << std::endl;
            return;
        }

        os << "IP Address:    " << getAddrP() << " (" << getAddrN().getAddr() << ")" << std::endl
           << "Prefix length: " << getPlen() << std::endl
           << "Subnet Mask:   " << getMaskP() << " (" << getMaskN().getAddr() << ")" << std::endl
           << "Broadcast:     " << getBroadcastP() << " (" << getBroadcastN().getAddr() << ")" << std::endl
           << "Network:       " << getNetworkP() << " (" << getNetworkN().getAddr() << ")" << std::endl
           << "Hostmask:      " << getHostmaskP() << " (" << getHostmaskN().getAddr() << ")" << std::endl;

        return;
    }

    /* withMask
     * Return the network ANDed with a new network mask.
     */
    in_addr_t Addr4::withMask(uint32_t mask) const
    {
        /* If given a prefix length, convert to a subnet mask */
        if (isValidPlen(mask))
        {
            mask = pltosm(mask);
        }
 
        if (! isValidSnmask(mask))
        {
            return 0;
        }

        return getAddrN().getAddr() & mask;
    }

    int Addr4::getAddrStrLen() const
    {
        return INET_ADDRSTRLEN;
    }

    int Addr4::getAddrFamily() const
    {
        return AF_INET;
    }

    int Addr4::getMaxPlen() const
    {
        return MAX_PLEN;
    }


    /* smtopl
     * Subnet mask to prefix length conversion.
     * If this is a valid subnet mask, then the prefix length is equal
     * to the Hamming weight. The burden is on the caller to
     * validate the subnet mask (call isValidSnmask() with the mask
     * in host order).
     *
     */ 
    uint32_t Addr4::smtopl(const in_addr_t mask) const
    {
        return onBits(mask);
    }

    /* pltosm
     * Prefix length to subnet mask.
     *
     * Steps:
     * Zero out mask, then set mask's highest order bit equal to one.
     * Right shift mask by one less than the prefix length.
     * Subtract 1 from mask. Now all the bits we want are flipped to 0.
     * Get the one's complement of mask.
     * Mask is now in network byte order. Convert it to host order.
     */
    in_addr_t Addr4::pltosm(const uint32_t plen) const
    {
        /* If prefix length is invalid or zero, return 0 */
        if ((isValidPlen(plen) == false) || plen == 0)
        {
            return 0;
        }

        uint32_t highbit = 1 << 31;  /* Highest order bit flipped on */

        return ntohl(~((highbit >> plen - 1) - 1));
    }

    /* Convert to NBO then compare network addresses */
    bool Addr4::operator<(const Addr4 & other) const
    {
        return getNetworkN().nbo() < other.getNetworkN().nbo();
    }
};
