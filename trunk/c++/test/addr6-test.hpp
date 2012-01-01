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
    void opLessThan();
    void getFuncs();

    /* Helper functions */
    static bool addr_equals(const IP::Addr6 & addr1,
                            const IP::Addr6 & addr2);

public:
    Addr6Test()
    {
        TEST_ADD(Addr6Test::setAddrGood);
        TEST_ADD(Addr6Test::opLessThan);
        TEST_ADD(Addr6Test::getFuncs);
    }
};

#endif /* ADDR6TEST_H */
