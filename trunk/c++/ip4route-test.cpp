/* ip4route-test.cpp */

#include <cpptest.h>

#include "ip4route.hpp"
#include "ip4route-test.hpp"

void IP4RouteTest::invalid_route(const IP4Route::IP4Route & rt)
{
    TEST_ASSERT(rt.isValid() == false);
    TEST_ASSERT(rt.getMetric() == 0);
    invalid_addr(rt);
}

void IP4RouteTest::goodmetric(void)
{
    IP4Route::IP4Route rt("192.168.1.1", 24);
    TEST_ASSERT(rt.isValid() == true);
    TEST_ASSERT(rt.getMetric() == 0);
    addr_equals(rt.getAddr(), "192.168.1.1", 3232235777U);
    addr_equals(rt.getMask(), "255.255.255.0", 4294967040U);
    addr_equals(rt.getHostmask(), "0.0.0.255", 255);
    addr_equals(rt.getBroadcast(), "192.168.1.255", 3232236031U);
    addr_equals(rt.getNetwork(), "192.168.1.0", 3232235776U);
    TEST_ASSERT(rt.getPlen() == 24);
}

void IP4RouteTest::badmetric_toohigh(void)
{
    IP4Route::IP4Route rt("192.168.1.1", 24, IP4Addr::IP4Addr::PLEN,
                          IP4Route::IP4Route::MAX_METRIC + 1);

    TEST_ASSERT(rt.isValid() == false);
    TEST_ASSERT(rt.getMetric() == 0);
    TEST_ASSERT(rt.IP4Addr::isValid() == true);
}

void IP4RouteTest::badmetric_negative(void)
{
    IP4Route::IP4Route rt("192.168.1.1", 24, IP4Addr::IP4Addr::PLEN,
                          IP4Route::IP4Route::MIN_METRIC - 1);

    TEST_ASSERT(rt.isValid() == false);
    TEST_ASSERT(rt.getMetric() == 0);
    TEST_ASSERT(rt.IP4Addr::isValid() == true);
}

void IP4RouteTest::setmetric_good(void)
{
    IP4Route::IP4Route rt("192.168.1.1", 24, IP4Addr::IP4Addr::PLEN, 1);
 
    TEST_ASSERT(rt.isValid() == true);
    rt.setPlen(23);
    TEST_ASSERT(rt.getPlen() == 23);
}

void IP4RouteTest::implied_metric(void)
{
    IP4Route::IP4Route rt("192.168.1.1", 32);
    TEST_ASSERT(rt.isValid() == true);
    TEST_ASSERT(rt.getMetric() == 0);
    TEST_ASSERT(rt.getPlen() == 32);
}

void IP4RouteTest::negplen_goodmetric(void)
{
    IP4Route::IP4Route rt("192.168.1.1", -1);
    TEST_ASSERT(rt.isValid() == true);
    TEST_ASSERT(rt.IP4Addr::isValid() == true);
    TEST_ASSERT(rt.getMetric() == 0);
}
