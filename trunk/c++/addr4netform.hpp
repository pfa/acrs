/* addr4netform.hpp
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

#ifndef IP_ADDR4NETFORM_H
#define IP_ADDR4NETFORM_H

#include <string>

#include <inttypes.h>
#include <arpa/inet.h>

#include "addrnetform.hpp"

namespace IP
{
    class Addr4NetForm : public AddrNetForm
    {
        private:
        protected:
            in_addr_t m_addr_net_form;

        public:
            in_addr_t getAddr() const { return m_addr_net_form; };
            bool setAddr(const in_addr_t & addr);

            Addr4NetForm operator&(const Addr4NetForm & other) const;
            Addr4NetForm operator|(const Addr4NetForm & other) const;
            Addr4NetForm operator~() const;
            bool operator<(const Addr4NetForm & other) const;
            bool operator>(const Addr4NetForm & other) const;
            bool operator==(const Addr4NetForm & other) const;
            bool operator!=(const Addr4NetForm & other) const;
            Addr4NetForm nbo() const;
            Addr4NetForm hbo() const;

            Addr4NetForm();
            Addr4NetForm(const in_addr_t addr);
            virtual ~Addr4NetForm();
    };
}

#endif /* IP_ADDR4NETFORM_H */
