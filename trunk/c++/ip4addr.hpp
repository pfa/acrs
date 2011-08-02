#ifndef IP4ADDR_H
#define IP4ADDR_H

#include <inttypes.h>
#include <arpa/inet.h>
#include <string>

namespace IP4Addr
{
    typedef uint64_t hash_t;
    typedef uint8_t plen_t;

    class IP4Addr
    {
    private:
        std::pair<std::string, in_addr_t> m_addr;
        std::pair<std::string, in_addr_t> m_snmask;
        plen_t m_plen;
        hash_t m_hash;
        bool m_addr_valid;
        bool m_mask_valid;

        /* Helper functions */
        plen_t smtopl(in_addr_t mask) const;
        in_addr_t pltosm(plen_t plen) const;
        hash_t makeHash(in_addr_t addr, in_addr_t snmask) const;
        hash_t makeHash(in_addr_t addr, plen_t plen) const;
        bool valid_snmask_i(in_addr_t snmask) const;
        bool isValidPlen(unsigned int plen) const;
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
        hash_t getHash(void) const;
        plen_t getPlen(void) const;

        /* Set functions */
        bool setAddr(std::string & addr_s);
        bool setAddr(in_addr_t addr_i);
        bool setMask(std::string & snmask_s);
        bool setMask(unsigned int snmask_i);
        bool setSnmask(std::string & snmask_s);
        bool setSnmask(in_addr_t snmask_i);
        bool setPlen(int plen);

        /* Constructors */
        IP4Addr(void);
        IP4Addr(std::string addr_s, uint32_t mask);
        IP4Addr(std::string addr_s, std::string snmask_s);
        IP4Addr(in_addr_t addr_i, uint32_t mask);
        IP4Addr(in_addr_t addr_i, std::string snmask_s);

        virtual ~IP4Addr() {};

        /* Other */
        bool isValid(void) const;
        in_addr_t withMask(in_addr_t snmask_i) const;
        in_addr_t withMask(plen_t plen) const;
        void printAll(std::ostream & os);
        friend std::ostream & operator<<(std::ostream & os, IP4Addr & ip);
    };
}

#endif /* IP4ADDR_H */
