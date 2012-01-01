/* addr6-test.cpp */

#include "../addr6.hpp"
#include "addr6-test.hpp"

void Addr6Test::setAddrGood()
{
    IP::Addr6 addr1;
    addr1.setAddr("feee::1");

    /* Not valid until mask is set */
    TEST_ASSERT(addr1.isValid() == false);
    TEST_ASSERT(addr1.getAddrP() == "feee::1");

    addr1.setMask(64);

    TEST_ASSERT(addr1.isValid() == true);
    TEST_ASSERT(addr1.getAddrP() == "feee::1");
    TEST_ASSERT(addr1.getMaskP() == "ffff:ffff:ffff:ffff::");
}

void Addr6Test::opLessThan()
{
    IP::Addr6 addr1("feee::1", 128);
    IP::Addr6 addr2("feee::2", 128);

    TEST_ASSERT((addr1 < addr2) == true);
    TEST_ASSERT((addr2 < addr1) == false);
}

void Addr6Test::getFuncs()
{
    IP::Addr6 addr("feee::5", 120);

    TEST_ASSERT(addr.getAddrP() == "feee::5");
    TEST_ASSERT(addr.getMaskP() == "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff00");
    TEST_ASSERT(addr.getNetworkP() == "feee::");
    TEST_ASSERT(addr.getBroadcastP() == "feee::ff");
    TEST_ASSERT(addr.getHostmaskP() == "::ff");
    TEST_ASSERT(addr.getPlen() == 120);

    addr.setPlen(121);
    TEST_ASSERT(addr.getAddrP() == "feee::5");
    TEST_ASSERT(addr.getMaskP() == "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ff80");
    TEST_ASSERT(addr.getNetworkP() == "feee::");
    TEST_ASSERT(addr.getBroadcastP() == "feee::7f");
    TEST_ASSERT(addr.getHostmaskP() == "::7f");
    TEST_ASSERT(addr.getPlen() == 121);
}
