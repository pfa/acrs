/* addr6-test.hpp */

#ifndef ADDR6TEST_H
#define ADDR6TEST_H

#include <netinet/in.h>
#include <cpptest.h>

#include "../addr6.hpp"

class Addr6Test : public Test::Suite
{
private:
    /* Tests */
    void setAddrGood();
    void opAnd();
    void opOr();
    void op1Complement();
    void opLessThan();
    void opGreaterThan();
    void opEqual();
    void opNotEqual();
    void ordering();

    /* Helper functions */
    static bool addr_equals(const IP::Addr6 & addr1,
                            const IP::Addr6 & addr2);

public:
    Addr6Test()
    {
        TEST_ADD(Addr6Test::setAddrGood);
        TEST_ADD(Addr6Test::opAnd);
        TEST_ADD(Addr6Test::opOr);
        TEST_ADD(Addr6Test::op1Complement);
        TEST_ADD(Addr6Test::opLessThan);
        TEST_ADD(Addr6Test::opGreaterThan);
        TEST_ADD(Addr6Test::opEqual);
        TEST_ADD(Addr6Test::opNotEqual);
        TEST_ADD(Addr6Test::ordering);
    }
};

#endif /* ADDR6TEST_H */
