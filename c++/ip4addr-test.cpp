#include <iostream>

#include <cpptest.h>
#include <arpa/inet.h>

#include "ip4addr.hpp"
#include "ip4addr-test.hpp"

void IP4AddrTest::invalid_addr(IP4Addr::IP4Addr addr)
{
    TEST_ASSERT(addr.isValid() == false);
    TEST_ASSERT(addr.getAddr().first == "");
    TEST_ASSERT(addr.getAddr().second == 0);
    TEST_ASSERT(addr.getMask().first == "");
    TEST_ASSERT(addr.getMask().second == 0);
    TEST_ASSERT(addr.getHostmask().first == "");
    TEST_ASSERT(addr.getHostmask().second == 0);
    TEST_ASSERT(addr.getBroadcast().first == "");
    TEST_ASSERT(addr.getBroadcast().second == 0);
    TEST_ASSERT(addr.getNetwork().first == "");
    TEST_ASSERT(addr.getNetwork().second == 0);
    TEST_ASSERT(addr.getPlen() == 0);
}

void IP4AddrTest::addr_equals(std::pair<std::string, in_addr_t> ippair,
                              std::string s,
                              in_addr_t i)
{
//    std::cerr << " *** " << std::endl;
//
//    std::cerr << ippair.first + " vs " + s << std::endl;
    TEST_ASSERT(ippair.first == s);

//    std::cerr << ippair.second << " vs " << i << std::endl;
    TEST_ASSERT(ippair.second == i);

//    std::cerr << " *** " << std::endl;
}

void IP4AddrTest::goodstr_goodint24(void)
{
    std::string s = "192.168.1.1";
    IP4Addr::IP4Addr addr(s, 16777215);

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getMask(), "255.255.255.0", 4294967040U);
    addr_equals(addr.getHostmask(), "0.0.0.255", 255);
    addr_equals(addr.getBroadcast(), "192.168.1.255", 3232236031U);
    addr_equals(addr.getNetwork(), "192.168.1.0", 3232235776U);
    TEST_ASSERT(addr.getPlen() == 24);
}

void IP4AddrTest::goodstr_goodstr24(void)
{
    IP4Addr::IP4Addr addr("192.168.1.1", "255.255.255.0");

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getMask(), "255.255.255.0", 4294967040U);
    addr_equals(addr.getHostmask(), "0.0.0.255", 255);
    addr_equals(addr.getBroadcast(), "192.168.1.255", 3232236031U);
    addr_equals(addr.getNetwork(), "192.168.1.0", 3232235776U);
    TEST_ASSERT(addr.getPlen() == 24);
}

void IP4AddrTest::goodstr_goodplen24(void)
{
    IP4Addr::IP4Addr addr("192.168.1.1", 24);

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getMask(), "255.255.255.0", 4294967040U);
    addr_equals(addr.getHostmask(), "0.0.0.255", 255);
    addr_equals(addr.getBroadcast(), "192.168.1.255", 3232236031U);
    addr_equals(addr.getNetwork(), "192.168.1.0", 3232235776U);
    TEST_ASSERT(addr.getPlen() == 24);
}

void IP4AddrTest::goodint_goodplen24()
{
    IP4Addr::IP4Addr addr(16885952, 24);

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getMask(), "255.255.255.0", 4294967040U);
    addr_equals(addr.getHostmask(), "0.0.0.255", 255);
    addr_equals(addr.getBroadcast(), "192.168.1.255", 3232236031U);
    addr_equals(addr.getNetwork(), "192.168.1.0", 3232235776U);
    TEST_ASSERT(addr.getPlen() == 24);
}

void IP4AddrTest::goodint_goodstr24()
{
    IP4Addr::IP4Addr addr(16885952, "255.255.255.0");

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getMask(), "255.255.255.0", 4294967040U);
    addr_equals(addr.getHostmask(), "0.0.0.255", 255);
    addr_equals(addr.getBroadcast(), "192.168.1.255", 3232236031U);
    addr_equals(addr.getNetwork(), "192.168.1.0", 3232235776U);
    TEST_ASSERT(addr.getPlen() == 24);
}

void IP4AddrTest::goodint_goodint24()
{
    IP4Addr::IP4Addr addr(16885952U, 16777215U);

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getMask(), "255.255.255.0", 4294967040U);
    addr_equals(addr.getHostmask(), "0.0.0.255", 255);
    addr_equals(addr.getBroadcast(), "192.168.1.255", 3232236031U);
    addr_equals(addr.getNetwork(), "192.168.1.0", 3232235776U);
    TEST_ASSERT(addr.getPlen() == 24);
}

void IP4AddrTest::badstr_goodplen24()
{
    IP4Addr::IP4Addr addr("1", 24);
    invalid_addr(addr);
}

void IP4AddrTest::goodstr_badmask()
{
    IP4Addr::IP4Addr addr("1.1.1.1", 64);
    invalid_addr(addr);
}

void IP4AddrTest::goodstr_goodplen_default()
{
    IP4Addr::IP4Addr addr("0.0.0.0", 0);

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "0.0.0.0", 0);
    addr_equals(addr.getMask(), "0.0.0.0", 0);
    addr_equals(addr.getHostmask(), "255.255.255.255", 4294967295U);
    addr_equals(addr.getBroadcast(), "255.255.255.255", 4294967295U);
    addr_equals(addr.getNetwork(), "0.0.0.0", 0);
    TEST_ASSERT(addr.getPlen() == 0);
}

void IP4AddrTest::goodstr_goodplen_hostroute()
{
    IP4Addr::IP4Addr addr("192.168.1.1", 32);

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getMask(), "255.255.255.255", 4294967295U);
    addr_equals(addr.getHostmask(), "0.0.0.0", 0);
    addr_equals(addr.getBroadcast(), "192.168.1.1", 3232235777U);
    addr_equals(addr.getNetwork(), "192.168.1.1", 3232235777U);
    TEST_ASSERT(addr.getPlen() == 32);
}

void IP4AddrTest::goodstr_badplen_toohigh()
{
    IP4Addr::IP4Addr addr("192.168.1.1", 33);
    invalid_addr(addr);
}

void IP4AddrTest::goodstr_badsnmask_toohigh()
{
    IP4Addr::IP4Addr addr("192.168.1.1", 4294967293U);
    invalid_addr(addr);
}

void IP4AddrTest::goodstr_goodplen30()
{
    IP4Addr::IP4Addr addr("10.0.4.2", 30);

    TEST_ASSERT(addr.isValid() == true);
    addr_equals(addr.getAddr(), "10.0.4.2", 167773186U);
    addr_equals(addr.getMask(), "255.255.255.252", 4294967292U);
    addr_equals(addr.getHostmask(), "0.0.0.3", 3);
    addr_equals(addr.getBroadcast(), "10.0.4.3", 167773187U);
    addr_equals(addr.getNetwork(), "10.0.4.0", 167773184U);
    TEST_ASSERT(addr.getPlen() == 30);
}

void IP4AddrTest::goodstr_badmask_nbo8()
{
    IP4Addr::IP4Addr addr("10.0.4.2", 4278190080U);
    invalid_addr(addr);
}
