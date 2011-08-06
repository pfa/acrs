/* ip4addr-test.hpp */

#ifndef IP4ADDR_TESTS
#define IP4ADDR_TESTS

#include <arpa/inet.h>
#include <cpptest.h>

#include "ip4addr.hpp"

class IP4AddrTest : public Test::Suite
{
private:
    /* Tests */
    void goodstr_goodint24(void);
    void goodstr_goodstr24(void);
    void goodstr_goodplen24(void);
    void goodint_goodplen24(void);
    void goodint_goodstr24(void);
    void goodint_goodint24(void);
    void badint_goodplen24(void);
    void goodstr_badmask(void);
    void goodstr_goodplen_default(void);
    void goodstr_goodplen_hostroute(void);
    void goodstr_badplen_toohigh(void);
    void badstr_goodplen24(void);
    void goodstr_badmask_nbo8(void);
    void goodstr_badsnmask_toohigh(void);
    void goodstr_goodplen30(void);

public:
    IP4AddrTest()
    {
        TEST_ADD(IP4AddrTest::goodstr_goodint24);
        TEST_ADD(IP4AddrTest::goodstr_goodstr24);
        TEST_ADD(IP4AddrTest::goodstr_goodstr24);
        TEST_ADD(IP4AddrTest::goodstr_goodplen24);
        TEST_ADD(IP4AddrTest::goodint_goodplen24);
        TEST_ADD(IP4AddrTest::goodint_goodstr24);
        TEST_ADD(IP4AddrTest::goodint_goodint24);
        TEST_ADD(IP4AddrTest::goodstr_badmask);
        TEST_ADD(IP4AddrTest::goodstr_goodplen_default);
        TEST_ADD(IP4AddrTest::goodstr_goodplen_hostroute);
        TEST_ADD(IP4AddrTest::goodstr_badplen_toohigh);
        TEST_ADD(IP4AddrTest::goodstr_badmask_nbo8);
        TEST_ADD(IP4AddrTest::badstr_goodplen24);
        TEST_ADD(IP4AddrTest::goodstr_badsnmask_toohigh);
        TEST_ADD(IP4AddrTest::goodstr_goodplen30);
    }

    /* Helpers */
    void invalid_addr(const IP4Addr::IP4Addr & addr);
    void addr_equals(const std::pair<std::string, in_addr_t> & ippair,
                     const std::string & s,
                     const in_addr_t i);
};

#endif /* IP4ADDR_TESTS */
