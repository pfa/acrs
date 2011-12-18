/* addr6netform.hpp
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

#ifndef IP_ADDR6NETFORM
#define IP_ADDR6NETFORM

#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "addrnetform.hpp"

namespace IP
{
    class Addr6NetForm : public AddrNetForm
    {
        private:
        protected:
            in6_addr m_addr_net_form;

        public:
            in6_addr getAddr() const { return m_addr_net_form; };
            bool setAddr(const in6_addr & addr);

            Addr6NetForm operator&(const Addr6NetForm & other) const;
            Addr6NetForm operator|(const Addr6NetForm & other) const;
            Addr6NetForm operator~() const;
            bool operator<(const Addr6NetForm & other) const;
            bool operator>(const Addr6NetForm & other) const;
            bool operator==(const Addr6NetForm & other) const;
            bool operator!=(const Addr6NetForm & other) const;
            Addr6NetForm nbo() const;
            Addr6NetForm hbo() const;

            Addr6NetForm();
            Addr6NetForm(const in6_addr & addr);
            virtual ~Addr6NetForm();
    };
}

#endif /* IP_ADDR6NETFORM */
