/* ip4route-test.hpp */

#ifndef IP4ROUTE_TESTS
#define IP4ROUTE_TESTS

#include <arpa/inet.h>
#include <cpptest.h>

#include "ip4addr-test.hpp"
#include "ip4route.hpp"

class IP4RouteTest : public IP4AddrTest
{
private:
    /* Tests */
    void goodmetric(void);
    void badmetric_toohigh(void);
    void badmetric_negative(void);
    void setmetric_good(void);
    void implied_metric(void);
    void negplen_goodmetric(void);

    /* Helpers */
    void invalid_route(const IP4Route::IP4Route & rt);

public:
    IP4RouteTest()
    {
        TEST_ADD(IP4RouteTest::goodmetric);
        TEST_ADD(IP4RouteTest::badmetric_toohigh);
        TEST_ADD(IP4RouteTest::badmetric_negative);
        TEST_ADD(IP4RouteTest::setmetric_good);
        TEST_ADD(IP4RouteTest::implied_metric);
        TEST_ADD(IP4RouteTest::negplen_goodmetric);
    }
};

#endif /* IP4ROUTE_TESTS */
