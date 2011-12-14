/* ipaddr.hpp */

#ifndef IP_H
#define IP_H

#include <string>

#include <inttypes.h>

namespace IP
{
    class Addr
    {
    private:
    protected:
        bool m_addr_valid;
        bool m_mask_valid;
        uint32_t m_plen;

        /* Helper functions */
        virtual bool isValidPlen(uint32_t plen) const;
        virtual bool updateNetInfo(void);
        bool setAddrSuccess(void);
        bool setAddrFail(void);
        bool setMaskSuccess(void);
        bool setMaskFail(void);

    public:
        enum MaskType
        {
            PLEN,
            SNMASK,
            UNSPEC
        };

        enum Order
        {
            NBO,
            HBO
        };

        virtual bool setAddr(const std::string & addr_s);
        virtual bool setPlen(const uint32_t plen);
        virtual std::string str(void) const;
        virtual void printAll(std::ostream & os) const;

        uint32_t getPlen(void) const;
        bool isValid(void) const;
        friend std::ostream & operator<<(std::ostream & os, IPAddr & ip);
    };

    class Route : public Addr
    {
    private:
    public:
    };

    class Route4 : public Route, Addr4
    {
    private:
    public:
    };

    class Route6 : public Route, Addr6
    {
    private:
    public:
    };
}

#endif /* IP_H */
