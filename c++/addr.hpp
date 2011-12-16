/* addr.hpp -- IP address abstract base class */

#ifndef IP_ADDR_H
#define IP_ADDR_H

#include <string>
#include <tr1/functional>

#include <inttypes.h>
#include <arpa/inet.h>

namespace IP
{
    enum MaskType
    {
        PLEN,      /* Mask is a prefix length                */
        SNMASK,    /* Mask is a subnet mask                  */
        UNSPEC     /* Let the library figure it out          */
    };

    class Addr
    {
    private:
    protected:
        bool m_addr_valid;
        bool m_mask_valid;
        uint32_t m_plen;
        std::string m_addr_pres_form;
        std::string m_snmask_pres_form;

        bool setAddrSuccess();
        bool setAddrFail();
        bool setMaskSuccess();
        bool setMaskFail();
        virtual void updateNetInfo() {};

        /* Checks if a number has consecutive bits set from the highest bit.
         * This will validate an entire IPv4 subnet mask, and can validate
         * 1-4 bytes of an IPv6 address (depending on how v6 address are
         * implemented on the system). For IPv6, the idea is to call this
         * on the byte where the first 0 bit is seen.
         */
        bool isValidSnmaskOnce(const uint32_t num) const;
        uint32_t onBits(uint32_t mask) const;

        virtual int getAddrFamily() const {};
        virtual int getAddrStrLen() const {};
        virtual bool setSnmaskFromPlen(const uint32_t plen) {};

        /* Converts network form data (in_addr_t, in6_addr) into a presentation
         * form string using the provided function to get the network form
         * data.
         */
        template <typename T> std::string
                       getPresData(std::tr1::function<T ()> getFunc) const
        {
            char buf[getAddrStrLen()];
            T addr_net_form = getFunc();
            decltype(addr_net_form.getAddr()) addr = addr_net_form.getAddr();
    
            inet_ntop(getAddrFamily(), &addr, buf, sizeof(buf));
    
            return std::string(buf);
        }

    public:
        virtual int getMaxPlen() const {};
        virtual void printAll(std::ostream & os) const {};
        bool isValidPlen(const uint32_t plen) const;

        virtual std::string getAddrP() const {};
        virtual std::string getNetworkP() const {};
        virtual std::string getHostmaskP() const {};
        virtual std::string getBroadcastP() const {};
        uint32_t getPlen() const;
        /* Subnet mask in presentation form is for v4 only */

        /* Set functions */
        virtual bool setAddr(const std::string & addr) {};
        bool setPlen(const uint32_t plen);

        /* Helper functions */
        bool isValid() const;
        std::string str() const;
        std::string operator*() const;
        friend std::ostream & operator<<(std::ostream & os, Addr & ip);

        /* Constructors */
        Addr() {};

        /* Destructor */
        virtual ~Addr() = 0;

        //template <typename T> bool setSnmask(const T snmask)
//        {
//            char buf[getAddrStrLen()];
//
//            if (! (inet_ntop(getAddrFamily(), &snmask, buf, getAddrStrLen())))
//            {
//                setMaskFail();
//                return false;
//            }
//
//            m_snmask_pres_form = buf;
//            m_plen = smtopl<T>(snmask);
//            m_snmask_net_form = snmask;
//
//            setMaskSuccess();
//
//            return true;
        //}
    };
}

#endif /* IP_ADDR_H */
