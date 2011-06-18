#ifndef IP4ADDR_H
#define IP4ADDR_H

#include <inttypes.h>
#include <arpa/inet.h>

/* XXX Integers must be set in Little Endian, and are stored in Big Endian.
 *     Update to handle any endianness and store in NBO.
 */

namespace IP4Addr
{
    typedef uint64_t hash_t;
    typedef uint8_t plen_t;

    class IP4Addr
    {
    private:
        in_addr_t    m_addr_i;
        char         m_addr_s[INET_ADDRSTRLEN];
        in_addr_t    m_snmask_i;
        char         m_snmask_s[INET_ADDRSTRLEN];
        plen_t       m_plen;
        hash_t       m_hash;

        bool addrValid;
        bool maskValid;

        /* Helper functions */
        plen_t smtopl(in_addr_t mask);
        in_addr_t pltosm(plen_t plen) const;
        hash_t makeHash(in_addr_t addr, in_addr_t snmask);
            hash_t makeHash(in_addr_t addr, plen_t plen);
        bool valid_snmask_i(in_addr_t snmask) const;
        void updateNetInfo();

        bool setAddrSuccess();
        bool setAddrFail();
        bool setMaskSuccess();
        bool setMaskFail();

    public:
        /* Get functions */
        in_addr_t      getAddr(char * buf);
        in_addr_t      getSnmask(char * buf);
        in_addr_t      getBroadcast(char * buf);
        in_addr_t      getHostmask(char * buf);
        in_addr_t      getNetwork(char * buf);
        hash_t         getHash();
        plen_t         getPlen();

        /* Set functions (non-overloaded) */
        bool           setAddr_s(char * addr_s);
        bool           setAddr_i(in_addr_t addr_i);
        bool           setSnmask_s(char * snmask_s);
        bool           setSnmask_i(in_addr_t snmask_i);
        bool           setPlen(int plen);

        /* CTORs -- arguments are the same as setter functions */
        IP4Addr();
        IP4Addr(char * addr_s, plen_t plen);
        IP4Addr(char * addr_s, char * snmask_s);
        IP4Addr(in_addr_t addr_i, in_addr_t snmask_i);
        IP4Addr(in_addr_t addr_i, plen_t plen);

        /* Other */
        bool isValid(void);
        in_addr_t withSnmask(in_addr_t snmask_i) const;
        in_addr_t withSnmask(plen_t plen) const;
        friend std::ostream & operator<<(std::ostream & os,
                                         IP4Addr & ip);
    };
}

#endif /* IP4ADDR_H */
