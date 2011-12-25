/* addr6netform-test.hpp */

#ifndef ADDR6NETFORM_TEST
#define ADDR6NETFORM_TEST

#include <cpptest.h>

#include "../addr6netform.hpp"

class Addr6NetFormTest : public Test::Suite
{
private:
    void setAddrGood();

public:
    Addr6NetFormTest()
    {
        TEST_ADD(Addr6NetFormTest::setAddrGood);
    }
};

#endif /* ADDR6NETFORM_TEST */
