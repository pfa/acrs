/* addr4netform.cpp
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

#include <string.h>
#include <arpa/inet.h>

#include "addrnetform.hpp"
#include "addr4netform.hpp"

namespace IP
{
    bool Addr4NetForm::setAddr(const in_addr_t & addr)
    {
        m_addr_net_form = addr;
        return true;
    }

    Addr4NetForm Addr4NetForm::operator&(const Addr4NetForm & other) const
    {
        return Addr4NetForm(getAddr() & other.getAddr());
    }

    Addr4NetForm Addr4NetForm::operator|(const Addr4NetForm & other) const
    {
        return getAddr() | other.getAddr();
    }

    Addr4NetForm Addr4NetForm::operator~() const
    {
        return ~getAddr();
    }

    bool Addr4NetForm::operator<(const Addr4NetForm & other) const
    {
        return getAddr() < other.getAddr();
    }

    bool Addr4NetForm::operator>(const Addr4NetForm & other) const
    {
        return getAddr() > other.getAddr();
    }

    bool Addr4NetForm::operator==(const Addr4NetForm & other) const
    {
        return getAddr() == other.getAddr();
    }

    bool Addr4NetForm::operator!=(const Addr4NetForm & other) const
    {
        return ! operator==(other);
    }

    Addr4NetForm Addr4NetForm::nbo() const
    {
        /* Stored in HBO, convert to NBO */
        return ntohl(getAddr());
    }

    Addr4NetForm Addr4NetForm::hbo() const
    {
        /* Stored in HBO, don't convert */
        return getAddr();
    }

    Addr4NetForm::Addr4NetForm()
    {
    }

    Addr4NetForm::~Addr4NetForm()
    {
    }

    Addr4NetForm::Addr4NetForm(const in_addr_t addr)
    {
        setAddr(addr);
    }
}
