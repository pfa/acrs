/* addr4netform.cpp */

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
