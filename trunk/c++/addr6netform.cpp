/* addr6netform.cpp  */

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
