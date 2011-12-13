#ifndef IP4ADDR_H
#define IP4ADDR_H

#include <string>

#include <inttypes.h>
#include <arpa/inet.h>

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
        uint64_t makeHash(in_addr_t addr, uint32_t mask) const;
        void updateNetInfo(void);
        bool setAddrSuccess(void);
        bool setAddrFail(void);
        bool setMaskSuccess(void);
        bool setMaskFail(void);

        int smtopl(in_addr_t mask) const;
        in_addr_t pltosm(uint32_t plen) const;
        bool isValidPlen(uint32_t plen) const;
        bool isValidSnmask(uint32_t mask) const;

    public:
        enum MaskType
        {
            PLEN,
            SNMASK,
            UNSPEC
        };

        /* Network or host order */
        enum Order
        {
            NBO,
            HBO
        };

        /* Get functions */
        std::pair<std::string, in_addr_t> getAddr(Order order = NBO) const;
        std::pair<std::string, in_addr_t> getMask(Order order = NBO) const;
        std::pair<std::string, in_addr_t> getBroadcast(Order order = NBO) const;
        std::pair<std::string, in_addr_t> getHostmask(Order order = NBO) const;
        std::pair<std::string, in_addr_t> getNetwork(Order order = NBO) const;
        uint64_t getHash(void) const;
        uint32_t getPlen() const;

        int getAddrStrLen() { return INET_ADDRSTRLEN; };
        int getAddrFamily() { return AF_INET; };

        /* Set functions */
        bool setAddr(const std::string & addr_s);
        bool setAddr(in_addr_t addr_i, Order order = HBO);
        bool setMask(const std::string & snmask_s);
        bool setMask(const in_addr_t snmask_i);
        bool setSnmask(const std::string & snmask_s);
        bool setSnmask(in_addr_t snmask_i, Order order = HBO);
        bool setPlen(const uint32_t plen);

        /* Constructors */
        IP4Addr(void);
        IP4Addr(const std::string & addr_s, uint32_t mask,
                MaskType type = UNSPEC, Order order = HBO);
        IP4Addr(const std::string & addr_s, const std::string & snmask_s);
        IP4Addr(in_addr_t addr_i, uint32_t mask,
                MaskType type = UNSPEC, Order order = HBO);
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
