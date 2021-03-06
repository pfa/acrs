/* addr4.hpp
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

#ifndef IP_ADDR4_H
#define IP_ADDR4_H

#include <string>

#include <arpa/inet.h>
#include <inttypes.h>

#include "addr.hpp"
#include "addr4.hpp"
#include "addr4netform.hpp"

namespace IP
{
    class Addr4 : public virtual Addr
    {
    private:
        enum
        {
            MAX_PLEN = 32
        };

        Addr4NetForm m_addr_net_form;
        Addr4NetForm m_mask_net_form;
        std::string m_mask_pres_form;

        void updateNetInfo();

        int getAddrFamily() const;
        int getAddrStrLen() const;

        bool setSnmaskFromPlen(const uint32_t plen);
        in_addr_t pltosm(const uint32_t plen) const;
        uint32_t smtopl(const in_addr_t mask) const;

    public:
        bool setAddr(const std::string & addr_s);
        bool setAddr(const in_addr_t addr_i);
        bool setMask(const std::string & mask_s);
        bool setMask(const in_addr_t snmask_i);
        bool setSnmask(const std::string & snmask_s);
        bool setSnmask(in_addr_t snmask_i);

        int getMaxPlen() const;

        Addr4NetForm getAddrN() const;
        Addr4NetForm getMaskN() const;
        Addr4NetForm getNetworkN() const;
        Addr4NetForm getBroadcastN() const;
        Addr4NetForm getHostmaskN() const;
        Addr4NetForm getNetFormAddr() const { return m_addr_net_form; };
        Addr4NetForm getNetFormMask() const { return m_mask_net_form; };

        std::string getMaskP() const;
        std::string getAddrP() const;
        std::string getNetworkP() const;
        std::string getBroadcastP() const;
        std::string getHostmaskP() const;

        bool isValidSnmask(uint32_t mask) const;
        in_addr_t withMask(uint32_t mask) const;
        void printAll(std::ostream & os) const;
        bool operator<(const Addr4 & other) const;

        /* Constructors
         *
         * Initialize with:
         * No arguments
         * Address and mask (either strings, ints, or mix). If using an
         * integer mask, masktype is an optional argument.
         */
        Addr4();
        Addr4(const std::string & addr_s, uint32_t mask,
              MaskType type = UNSPEC);
        Addr4(const std::string & addr_s, const std::string & snmask_s);
        Addr4(in_addr_t addr_i, uint32_t mask, MaskType type = UNSPEC);
        Addr4(in_addr_t addr_i, const std::string & snmask_s);
        //Addr4(const std::string & addr_s, uint32_t mask);
        //Addr4(in_addr_t addr_i, uint32_t mask);

        /* Destructor */
        virtual ~Addr4() {};

        static Addr4 getMaxAddr()
        {
            return Addr4("255.255.255.255", 32, PLEN);
        }
    };
}

#endif /* IP_ADDR4_H */
