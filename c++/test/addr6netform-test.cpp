/* addr6netform-test.cpp */

#include <cpptest.h>
#include <netinet/in.h>
#include <string.h>

#include "addr6netform-test.hpp"
#include "../addr6netform.hpp"

void Addr6NetFormTest::opAnd()
{
    in6_addr base_addr;
    
    memset(base_addr.s6_addr, 0, sizeof(base_addr));
    base_addr.s6_addr[0] = 0xaa;
    base_addr.s6_addr[1] = 0xbb;
    base_addr.s6_addr[15] = 0xcc;

    IP::Addr6NetForm addr;
    addr.setAddr(base_addr);

    base_addr.s6_addr[0] = 0x00;
    base_addr.s6_addr[1] = 0x00;
    base_addr.s6_addr[15] = 0xcc;

    IP::Addr6NetForm mask;
    mask.setAddr(base_addr);

    TEST_ASSERT(addr_equals((addr & mask).getAddr(), base_addr) == true);
}

void Addr6NetFormTest::opOr()
{
    in6_addr base_addr;
    
    memset(base_addr.s6_addr, 0, sizeof(base_addr));
    base_addr.s6_addr[0] = 0xaa;
    base_addr.s6_addr[1] = 0xbb;
    base_addr.s6_addr[15] = 0xcc;

    IP::Addr6NetForm addr;
    addr.setAddr(base_addr);

    base_addr.s6_addr[14] = 0xdd;

    IP::Addr6NetForm mask;
    mask.setAddr(base_addr);

    TEST_ASSERT(addr_equals((addr | mask).getAddr(), base_addr) == true);
}

void Addr6NetFormTest::op1Complement()
{
    in6_addr base_addr;
    
    memset(base_addr.s6_addr, 0, sizeof(base_addr));
    base_addr.s6_addr[0] = 0xaa;
    base_addr.s6_addr[1] = 0xbb;
    base_addr.s6_addr[15] = 0xcc;

    IP::Addr6NetForm addr;
    addr.setAddr(base_addr);

    /* Reassign each byte to its one's complement */
    for (unsigned char * p_addr = base_addr.s6_addr;
         p_addr <= &(base_addr.s6_addr[15]);
         p_addr++)
    {
        *p_addr = ~(*p_addr);
    }

    TEST_ASSERT(addr_equals((~addr).getAddr(), base_addr) == true);
}

void Addr6NetFormTest::setAddrGood()
{
    in6_addr addr;
    memset(addr.s6_addr, 0, sizeof(addr));
    addr.s6_addr[0] = 0xaa;
    addr.s6_addr[1] = 0xbb;
    addr.s6_addr[15] = 0xcc;

    IP::Addr6NetForm nfaddr;
    nfaddr.setAddr(addr);

    TEST_ASSERT(addr_equals(nfaddr.getAddr(), addr) == true);
}

bool Addr6NetFormTest::addr_equals(const in6_addr & addr1,
                                   const in6_addr & addr2)
{
    if ((addr1.s6_addr[0] == addr2.s6_addr[0])   &&
        (addr1.s6_addr[1] == addr2.s6_addr[1])   &&
        (addr1.s6_addr[2] == addr2.s6_addr[2])   &&
        (addr1.s6_addr[3] == addr2.s6_addr[3])   &&
        (addr1.s6_addr[4] == addr2.s6_addr[4])   &&
        (addr1.s6_addr[5] == addr2.s6_addr[5])   &&
        (addr1.s6_addr[6] == addr2.s6_addr[6])   &&
        (addr1.s6_addr[7] == addr2.s6_addr[7])   &&
        (addr1.s6_addr[8] == addr2.s6_addr[8])   &&
        (addr1.s6_addr[9] == addr2.s6_addr[9])   &&
        (addr1.s6_addr[10] == addr2.s6_addr[10]) &&
        (addr1.s6_addr[11] == addr2.s6_addr[11]) &&
        (addr1.s6_addr[12] == addr2.s6_addr[12]) &&
        (addr1.s6_addr[13] == addr2.s6_addr[13]) &&
        (addr1.s6_addr[14] == addr2.s6_addr[14]) &&
        (addr1.s6_addr[15] == addr2.s6_addr[15]))
    {
        return true;
    }
    else
    {
        return false;
    }
}
