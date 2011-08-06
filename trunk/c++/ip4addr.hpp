#ifndef IP4ADDR_H
#define IP4ADDR_H

#include <inttypes.h>
#include <arpa/inet.h>
#include <string>

namespace IP4Addr
{
    class IP4Addr
    {
    private:
        std::pair<std::string, in_addr_t> m_addr;
        std::pair<std::string, in_addr_t> m_snmask;
        uint32_t m_plen;
        uint64_t m_hash;
        bool m_addr_valid;
        bool m_mask_valid;

        /* Helper functions */
        int smtopl(in_addr_t mask) const;
        in_addr_t pltosm(uint32_t plen) const;
        uint64_t makeHash(in_addr_t addr, uint32_t mask) const;
        bool isValidPlen(uint32_t plen) const;
        bool isValidSnmask(uint32_t mask) const;
        void updateNetInfo(void);
        bool setAddrSuccess(void);
        bool setAddrFail(void);
        bool setMaskSuccess(void);
        bool setMaskFail(void);

    public:
        /* Get functions */
        std::pair<std::string, in_addr_t> getAddr(void) const;
        std::pair<std::string, in_addr_t> getMask(void) const;
        std::pair<std::string, in_addr_t> getBroadcast(void) const;
        std::pair<std::string, in_addr_t> getHostmask(void) const;
        std::pair<std::string, in_addr_t> getNetwork(void) const;
        uint64_t getHash(void) const;
        uint32_t getPlen() const;

        /* Set functions */
        bool setAddr(const std::string & addr_s);
        bool setAddr(const in_addr_t addr_i);
        bool setMask(const std::string & snmask_s);
        bool setMask(const unsigned int snmask_i);
        bool setSnmask(const std::string & snmask_s);
        bool setSnmask(const in_addr_t snmask_i);
        bool setPlen(const uint32_t plen);

        /* Constructors */
        IP4Addr(void);
        IP4Addr(const std::string & addr_s, uint32_t mask);
        IP4Addr(const std::string & addr_s, const std::string & snmask_s);
        IP4Addr(in_addr_t addr_i, uint32_t mask);
        IP4Addr(in_addr_t addr_i, const std::string & snmask_s);

        virtual ~IP4Addr() {};

        /* Other */
        bool isValid(void) const;
        in_addr_t withMask(uint32_t mask) const;
        void printAll(std::ostream & os) const;
        std::string str(void);
        friend std::ostream & operator<<(std::ostream & os, IP4Addr & ip);
    };
}

#endif /* IP4ADDR_H */
