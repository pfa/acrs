/* addr6netform.cpp
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

#include <string.h>
#include <netinet/in.h>

#include "addrnetform.hpp"
#include "addr6netform.hpp"

namespace IP
{
    Addr6NetForm::Addr6NetForm(const in6_addr & addr)
    {
        setAddr(addr);
    }

    bool Addr6NetForm::setAddr(const in6_addr & addr)
    {
        memcpy(&m_addr_net_form, &addr, sizeof(m_addr_net_form));
        return true;
    }

    Addr6NetForm Addr6NetForm::operator&(const Addr6NetForm & other) const
    {
        in6_addr newaddr;
        in6_addr thisaddr = this->getAddr();
        in6_addr otheraddr = other.getAddr();

        for (int i = 0; i < sizeof(m_addr_net_form); i++)
        {
            newaddr.s6_addr[i] = thisaddr.s6_addr[i] & otheraddr.s6_addr[i];
        }

        return Addr6NetForm(newaddr);
    };

    Addr6NetForm Addr6NetForm::operator|(const Addr6NetForm & other) const
    {
        in6_addr newaddr;
        in6_addr thisaddr = this->getAddr();
        in6_addr otheraddr = other.getAddr();

        for (int i = 0; i < sizeof(m_addr_net_form); i++)
        {
            newaddr.s6_addr[i] = thisaddr.s6_addr[i] | otheraddr.s6_addr[i];
        }

        return Addr6NetForm(newaddr);
    }

    Addr6NetForm Addr6NetForm::operator~() const
    {
        in6_addr newaddr;
        in6_addr thisaddr = this->getAddr();

        for (int i = 0; i < sizeof(m_addr_net_form); i++)
        {
            newaddr.s6_addr[i] = ~(thisaddr.s6_addr[i]);
        }

        return Addr6NetForm(newaddr);
    }

    bool Addr6NetForm::operator<(const Addr6NetForm & other) const
    {
        in6_addr thisaddr = this->getAddr();
        in6_addr otheraddr = other.getAddr();

        for (int i = 0; i < sizeof(m_addr_net_form); i++)
        {
            if (thisaddr.s6_addr[i] < otheraddr.s6_addr[i])
            {
                return true;
            }
            else if (thisaddr.s6_addr[i] > otheraddr.s6_addr[i])
            {
                return false;
            }
        }

        /* Full addresses were equal */
        return false;
    }

    bool Addr6NetForm::operator>(const Addr6NetForm & other) const
    {
        in6_addr thisaddr = this->getAddr();
        in6_addr otheraddr = other.getAddr();

        for (int i = 0; i < sizeof(m_addr_net_form); i++)
        {
            if (thisaddr.s6_addr[i] > otheraddr.s6_addr[i])
            {
                return true;
            }
            else if (thisaddr.s6_addr[i] < otheraddr.s6_addr[i])
            {
                return false;
            }
        }

        /* Full addresses were equal */
        return false;
    }

    bool Addr6NetForm::operator==(const Addr6NetForm & other) const
    {
        if ((*this < other) || (*this > other))
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool Addr6NetForm::operator!=(const Addr6NetForm & other) const
    {
        return ! operator==(other);
    }

    Addr6NetForm Addr6NetForm::nbo() const
    {
        return m_addr_net_form;
    }

    Addr6NetForm Addr6NetForm::hbo() const
    {
        return m_addr_net_form;
    }

    Addr6NetForm::Addr6NetForm()
    {
    }

    Addr6NetForm::~Addr6NetForm()
    {
    }
}
