/* ipaddr.cpp - IP address abstract base class */

#include "ipaddr.hpp"

namespace IP
{
    bool IPAddr::setAddrSuccess(void)
    {
        return m_addr_valid = true;
    }

    bool IPAddr::setAddrFail(void)
    {
        return m_addr_valid = false;
    }

    bool IPAddr::setMaskSuccess(void)
    {
        return m_mask_valid = true;
    }

    bool IPAddr::setMaskFail(void)
    {
        return m_mask_valid = false;
    }

    uint32_t IPAddr::getPlen(void) const
    {
        if (! isValid())
        {
            return 0;
        }

        return m_plen;
    }

    bool IPAddr::isValid(void)
    {
        return (m_addr_valid && m_mask_valid);
    }
}
