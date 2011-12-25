/* addr6netform-test.cpp */

#include <cpptest.h>
#include <netinet/in.h>
#include <string.h>

#include "addr6netform-test.hpp"
#include "../addr6netform.hpp"

void Addr6NetFormTest::setAddrGood()
{
    in6_addr addr;
    memset(addr.s6_addr, 0, sizeof(addr));
    addr.s6_addr[0] = 0xaa;
    addr.s6_addr[1] = 0xbb;
    addr.s6_addr[15] = 0xcc;

    IP::Addr6NetForm nfaddr;
    nfaddr.setAddr(addr);

    const in6_addr & retaddr = nfaddr.getAddr();

    TEST_ASSERT(retaddr.s6_addr[0] == addr.s6_addr[0]);
    TEST_ASSERT(retaddr.s6_addr[1] == addr.s6_addr[1]);
    TEST_ASSERT(retaddr.s6_addr[2] == addr.s6_addr[2]);
    TEST_ASSERT(retaddr.s6_addr[3] == addr.s6_addr[3]);
    TEST_ASSERT(retaddr.s6_addr[4] == addr.s6_addr[4]);
    TEST_ASSERT(retaddr.s6_addr[5] == addr.s6_addr[5]);
    TEST_ASSERT(retaddr.s6_addr[6] == addr.s6_addr[6]);
    TEST_ASSERT(retaddr.s6_addr[7] == addr.s6_addr[7]);
    TEST_ASSERT(retaddr.s6_addr[8] == addr.s6_addr[8]);
    TEST_ASSERT(retaddr.s6_addr[9] == addr.s6_addr[9]);
    TEST_ASSERT(retaddr.s6_addr[10] == addr.s6_addr[10]);
    TEST_ASSERT(retaddr.s6_addr[11] == addr.s6_addr[11]);
    TEST_ASSERT(retaddr.s6_addr[12] == addr.s6_addr[12]);
    TEST_ASSERT(retaddr.s6_addr[13] == addr.s6_addr[13]);
    TEST_ASSERT(retaddr.s6_addr[14] == addr.s6_addr[14]);
    TEST_ASSERT(retaddr.s6_addr[15] == addr.s6_addr[15]);
}
