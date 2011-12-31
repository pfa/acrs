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

void Addr6Test::opAnd()
{
}

void Addr6Test::opOr()
{
}

void Addr6Test::op1Complement()
{
}

void Addr6Test::opLessThan()
{
}

void Addr6Test::opGreaterThan()
{
}

void Addr6Test::opEqual()
{
}

void Addr6Test::opNotEqual()
{
}

void Addr6Test::ordering()
{
}
