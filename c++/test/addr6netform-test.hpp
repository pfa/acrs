/* addr6netform-test.hpp */

#ifndef ADDR6NETFORM_TEST
#define ADDR6NETFORM_TEST

#include <netinet/in.h>
#include <cpptest.h>

#include "../addr6netform.hpp"

class Addr6NetFormTest : public Test::Suite
{
private:
    /* Tests */
    void setAddrGood();
    void opAnd();
    void opOr();
    void op1Complement();

    /* Helper functions */
    static bool addr_equals(const in6_addr & addr1, const in6_addr & addr2);

public:
    Addr6NetFormTest()
    {
        TEST_ADD(Addr6NetFormTest::setAddrGood);
        TEST_ADD(Addr6NetFormTest::opAnd);
        TEST_ADD(Addr6NetFormTest::opOr);
        TEST_ADD(Addr6NetFormTest::op1Complement);
    }
};

#endif /* ADDR6NETFORM_TEST */
