/* ip4addr.cpp - IP address utility
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

/* Stores integers in NBO */

#include <arpa/inet.h>
#include <inttypes.h>
#include <iostream>
#include <string>
#include <cstring>

#include "ip4addr.hpp"

namespace IP4Addr
{
    hash_t IP4Addr::makeHash(in_addr_t addr, plen_t plen) const
    {
        return ((addr << 31) + plen);
    }

    hash_t IP4Addr::makeHash(in_addr_t addr, in_addr_t snmask) const
    {
        /* Since the hash is a network address + prefix length,
         * if we are given the subnet mask we must first convert it
         * to a prefix length. */
        plen_t plen = smtopl(snmask);

        /* Call the other hash function */
        return makeHash(addr, plen);
    }

    /* valid_snmask_i
     * Make sure all the bits flipped on are to the 'left' side of the
     * number.  Otherwise it's not a subnet mask.
     */
    bool IP4Addr::valid_snmask_i(in_addr_t snmask) const
    {
        char maskchk[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
        char * chkptr = maskchk;
        uint8_t * octet;
        bool hostbits = false;

        //snmask = htonl(snmask);
         octet = (uint8_t *) &snmask;

        /* For every octet, check if the mask is ok */
        for (int i = 0; i < 4; i++, octet++)
        {
            for (chkptr = maskchk; chkptr <= &maskchk[7]; chkptr++)
            {
                if (! hostbits)
                {
                    if (*chkptr & *octet)
                    {
                        continue;
                    }
                    else
                    {
                        hostbits++;
                    }
                }
                else
                {
                    if (*chkptr & *octet)
                    {
                        /* Bad mask */
                        return false;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }

        return true;
    }

    /* pltosn
     *
     * Prefix length to subnet mask.
     */
    in_addr_t IP4Addr::pltosm(plen_t plen) const
    {
        uint32_t highbit = 1 << 31;  /* Leftmost bit flipped on */

        /* The following won't work with a prefix length of 0,
         * so handle that now */
        if (plen == 0)
        {
            return 0;
        }

        /* If the prefix length was greater than the max,
         * reduce it to 32. */
        if (plen > 32)
        {
            plen = 32;
        }

        /*
          * 1. Zero out MASK, then set MASK's highest order bit equal to one
          * 2. Right shift MASK by one less than the PREFIX LENGTH
          * 3. Subtract 1 from MASK; now all the bits we want are flipped to 0
          * 4. Get the one's complement of MASK (flip all 0s to 1s, vice versa)
          * 5. MASK is now in network byte order; convert it to host order
          */
        return ntohl(~((highbit >> plen - 1) - 1));
    }

    /* smtopl
     * Subnet mask (as a host order integer) to prefix length conversion.
     */ 
    plen_t IP4Addr::smtopl(in_addr_t snmask) const
    {
        uint8_t * cur = (uint8_t *) &snmask;
        plen_t plen = 0;

        /* Check # of bits on in each octet */
        for (int i = 0; i < 4; i++, cur++) {
            if (*cur == 255) {
                /* All bits were on */
                plen += 8;
                continue;
            } else {
                /* Find consecutive leftmost bits on within this byte */
                char bits[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
                char * bptr = &bits[0];
                char * startbit = &bits[0];

                while (*cur & *bptr)
                {
                    bptr++;
                }
                plen += (bptr - startbit);
                break;
            }
        }

        return plen;
    }

    /* Get functions */

    std::pair<std::string, in_addr_t> IP4Addr::getAddr(void) const
    {
        if (! isValid())
        {
            return std::make_pair("", 0);
        }

        return std::make_pair(m_addr.first, m_addr.second);
    }

    std::pair<std::string, in_addr_t> IP4Addr::getMask(void) const
    {
        if (! isValid())
        {
            return std::make_pair("", 0);
        }

        return std::make_pair(m_snmask.first, m_snmask.second);
    }

    hash_t IP4Addr::getHash() const
    {
        if (! isValid())
        {
            return 0;
        }

        return m_hash;
    }

    plen_t IP4Addr::getPlen() const
    {
        if (! isValid())
        {
            return 0;
        }

        return m_plen;
    }

    /* Gives both the std::string and integer (NBO) representation of a
     * broadcast address.
     */
    std::pair<std::string, in_addr_t> IP4Addr::getBroadcast(void) const
    {
        in_addr_t bc;
        std::string bcstr;
        char buf[INET_ADDRSTRLEN];

        if (! isValid())
        {
            return std::make_pair("", 0);
        }

        bc = m_addr.second | getHostmask().second;
        int hboBc = ntohl(bc);
        inet_ntop(AF_INET, &hboBc, buf, INET_ADDRSTRLEN);
        bcstr = buf;

        return std::make_pair(bcstr, bc);
    }

    std::pair<std::string, in_addr_t> IP4Addr::getHostmask(void) const
    {
        in_addr_t hmask;
        std::string hmaskstr;
        char buf[INET_ADDRSTRLEN];

        if (! isValid())
        {
            return std::make_pair("", 0);
        }

        hmask = ~ getMask().second;

        int hboHmask = ntohl(hmask);
        inet_ntop(AF_INET, &hboHmask, buf, INET_ADDRSTRLEN);
        hmaskstr = buf;

        return std::make_pair(hmaskstr, hmask);
    }

    std::pair<std::string, in_addr_t> IP4Addr::getNetwork(void) const
    {
        in_addr_t net;
        char buf[INET_ADDRSTRLEN];
        std::string netstr;

        if (! isValid())
        {
            return std::make_pair("", 0);
        }

        net = m_snmask.second & m_addr.second;

        int hboNet = ntohl(net);
        inet_ntop(AF_INET, &hboNet, buf, INET_ADDRSTRLEN);
        netstr = buf;

        return std::make_pair(netstr, net);
    }

    bool IP4Addr::isValid() const
    {
        return (m_addr_valid && m_mask_valid);
    }

    /* Set functions */

    bool IP4Addr::setAddr(std::string & addr_s)
    {
        char buf[INET_ADDRSTRLEN];
        strncpy(buf, addr_s.c_str(), sizeof(buf));

        if (addr_s == "")
        {
            return setAddrFail();
        }

        if (! inet_pton(AF_INET, buf, &m_addr.second))
        {
            return setAddrFail();
        }

        /* Convert address to NBO */
        m_addr.second = htonl(m_addr.second);
        m_addr.first = buf;

        return setAddrSuccess();
    }

    bool IP4Addr::setAddr(in_addr_t addr_i)
    {
        char buf[INET_ADDRSTRLEN];

        if (! (inet_ntop(AF_INET, &addr_i, buf, INET_ADDRSTRLEN)))
        {
            return setAddrFail();
        }

        m_addr.first = buf;
        m_addr.second = htonl(addr_i);

        return setAddrSuccess();
    }

    bool IP4Addr::setSnmask(std::string & snmask_s)
    {
        char buf[INET_ADDRSTRLEN];
        strncpy(buf, snmask_s.c_str(), INET_ADDRSTRLEN);

        if (! inet_pton(AF_INET, buf, &m_snmask.second))
        {
            return setMaskFail();
        }

        m_plen = smtopl(m_snmask.second);
        m_snmask.second = htonl(m_snmask.second);
        m_snmask.first = buf;
        return setMaskSuccess();
    }

    bool IP4Addr::setSnmask(in_addr_t snmask_i)
    {
        char buf[INET_ADDRSTRLEN];

        if (! (inet_ntop(AF_INET, &snmask_i, buf, INET_ADDRSTRLEN)))
        {
            return setMaskFail();
        }

        m_snmask.first = buf;
        m_plen = smtopl(snmask_i);
        m_snmask.second = htonl(snmask_i);
        return setMaskSuccess();
    }

    /* Determines whether the argument is a prefix length or a subnet mask
     * and calls setPlen or setSnmask accordingly.
     */
    bool IP4Addr::setMask(uint32_t mask)
    {
        if (IP4Addr::isValidPlen(mask))
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

    bool IP4Addr::isValidSnmask(uint32_t mask)
    {
        if (mask == 0)
        {
            return true;
        }

        /* First bit must be on */
        if ((mask & 1) == 0)
        {
            return false;
        }

        uint8_t * cur;
        bool seenzero = false;
        void * last_byte = &mask + 3;

        /* For each bit in each octet, check for a 0 followed by a 1.
         * If found, this is not a valid subnet mask. */
        for (cur = (uint8_t *) &mask; &cur < last_byte ; cur++)
        {
            char bits[] = { 128, 64, 32, 16, 8, 4, 2, 1, 0 };
            char * bptr = bits;

            do 
            {
                if ((*cur & *bptr) == 0)
                {
                    /* Saw a 0 */
                    seenzero = true;
                }
                else if (seenzero == true)
                {
                    /* Saw a 1 after having seen a 0, bad mask */
                    return false;
                }
            }
            while (*(++bptr) != 0);

            /* Good mask */
            return true;
        }
    }

    bool IP4Addr::setMask(std::string & snmask_s)
    {
        return setSnmask(snmask_s);
    }

    bool IP4Addr::isValidPlen(uint32_t plen) const
    {
        if (plen > 32)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool IP4Addr::setPlen(int plen)
    {
        if (! isValidPlen(plen))
        {
            return setMaskFail();
        }

        setSnmask(pltosm(plen));

        return setMaskSuccess();
    }

    bool IP4Addr::setAddrSuccess()
    {
        if (m_mask_valid == true)
        {
            updateNetInfo();
        }

        return m_addr_valid = true;
    }

    bool IP4Addr::setAddrFail()
    {
        return m_addr_valid = false;
    }

    bool IP4Addr::setMaskSuccess()
    {
        if (m_addr_valid == true)
        {
            updateNetInfo();
        }

        return m_mask_valid = true;
    }

    bool IP4Addr::setMaskFail()
    {
        return m_mask_valid = false;
    }

    /* Compute any members that depend on the mask and address. Since the
     * hostmask, network address, etc., are computed only when their get
     * functions are called (not stored as member variables), this function
     * doesn't do much now. To increase speed, this can be modified if you
     * use particular derived network data more often than you change the
     * IP/mask on the IP4Addr object.
     */
    void IP4Addr::updateNetInfo()
    {
        m_hash = makeHash(m_addr.second, (in_addr_t) m_snmask.second);
        return;
    }

    /* CTORs -- as above */
    IP4Addr::IP4Addr(void)
    {
        setAddrFail();
        setMaskFail();
    }

    IP4Addr::IP4Addr(std::string addr_s, std::string snmask_s)
    {
        setAddr(addr_s);
        setMask(snmask_s);
    }

    IP4Addr::IP4Addr(std::string addr_s, uint32_t mask)
    {
        setAddr(addr_s);
        setMask(mask);
    }

    IP4Addr::IP4Addr(in_addr_t addr_i, std::string snmask_s)
    {
        setAddr(addr_i);
        setMask(snmask_s);
    }

    IP4Addr::IP4Addr(in_addr_t addr_i, uint32_t mask)
    {
        setAddr(addr_i);
        setMask(mask);
    }

    /* Other */

    std::ostream & operator<<(std::ostream & os, IP4Addr & ip)
    {
        if (ip.isValid() == false)
        {
            os << "Address is not valid." << std::endl;
            return os;
        }

        std::pair<std::string, in_addr_t> ip_pair = ip.getNetwork();

        os << ip_pair.first << "/"
           << ip_pair.second << std::endl;

        return os;
    }

    void IP4Addr::printAll(std::ostream & os)
    {
        if (isValid() == false)
        {
            os << "IP and/or mask is not valid." << std::endl;
            return;
        }

        os << "IP Address:    " << getAddr().first << " ("
           << getAddr().second << ")" << std::endl
           << "Prefix length: " << (int) getPlen() << std::endl
           << "Subnet Mask:   " << getMask().first << " ("
           << getMask().second << ")" << std::endl
           << "Broadcast:     " << getBroadcast().first << " ("
           << getBroadcast().second << ")" << std::endl
           << "Network:       " << getNetwork().first << " ("
           << getNetwork().second << ")" << std::endl
           << "Hostmask:      " << getHostmask().first
           << " (" << getHostmask().second << ")" << std::endl;

        return;
    }

    /* with_snmask
     * Return the network ANDed with a new network mask.
     */
    in_addr_t IP4Addr::withMask(in_addr_t snmask_i) const
    {
        if (! valid_snmask_i(snmask_i)) {
            return 0;
        }

        return m_addr.second & snmask_i;
    }

    /* with_snmask
     * Return the network ANDed with a new network mask.
     */
    in_addr_t IP4Addr::withMask(plen_t plen) const
    {
        if (plen > 32) {
            plen = 32;
        }

        in_addr_t snmask = pltosm(plen);

        return withMask(snmask);
    }
};
