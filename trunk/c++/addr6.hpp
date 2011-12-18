/* addr6.hpp
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

#ifndef ADDR6_H
#define ADDR6_H

#include <arpa/inet.h>
#include <inttypes.h>

#include "addr.hpp"
#include "addr6netform.hpp"

namespace IP
{
    class Addr6 : public virtual Addr
    {
    private:
        enum
        {
            MAX_PLEN = 128
        };

        Addr6NetForm m_addr_net_form;
        Addr6NetForm m_mask_net_form;

        void updateNetInfo();
        bool isValidSnmask(const in6_addr & mask) const;
        bool setSnmaskFromPlen(const uint32_t plen);

        uint32_t smtopl(const in6_addr & mask) const;
        in6_addr pltosm(const uint32_t plen) const;

    public:
        int getMaxPlen(void) const { return MAX_PLEN; };

        bool setAddr(const in6_addr & addr);
        bool setAddr(const std::string & addr);
        bool setMask(const in6_addr & mask);
        bool setMask(const std::string & mask);
        bool setMask(const uint32_t plen);

        Addr6NetForm getAddrN() const;
        Addr6NetForm getMaskN() const;
        Addr6NetForm getBroadcastN() const;
        Addr6NetForm getNetworkN() const;
        Addr6NetForm getHostmaskN() const;

        std::string getMaskP() const;
        std::string getAddrP() const;
        std::string getNetworkP() const;
        std::string getBroadcastP() const;
        std::string getHostmaskP() const;

        int getAddrFamily() const;
        int getAddrStrLen() const;

        Addr6NetForm getNetFormAddr() const;
        Addr6NetForm getNetFormMask() const;

        void printAll(std::ostream & os) const;
        bool operator<(const Addr6 & other) const;

        /* Constructors */
        Addr6();
        Addr6(const std::string & addr, const uint32_t plen,
              MaskType type = PLEN);
        Addr6(const std::string & addr, const in6_addr & mask,
              MaskType type = SNMASK);
        Addr6(const std::string & addr, const std::string & mask);
        Addr6(const in6_addr & addr, const uint32_t plen,
              MaskType type = PLEN);
        Addr6(const in6_addr & addr, const in6_addr & mask,
              MaskType type = SNMASK);
        Addr6(const in6_addr & addr, const std::string & mask);

        /* Destructor */
        ~Addr6() {};
    };
}

#endif /* ADDR6_H */
