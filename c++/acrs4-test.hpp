/* acrs4-test.hpp */

#ifndef ACRS4TEST_H
#define ACRS4TEST_H

#include <cpptest.h>

#include "acrs4.hpp"
#include "acrs4-test.hpp"

class AcrsTest : public Test::Suite
{
private:
    /* Tests */
    void no_routes(void);
    void one_good_route(void);
    void one_bad_route(void);
    void lower_not_possible(void);
    void basic_overlap(void);
    void basic_main(void);

public:
    AcrsTest()
    {
        TEST_ADD(AcrsTest::no_routes);
        TEST_ADD(AcrsTest::one_good_route);
        TEST_ADD(AcrsTest::one_bad_route);
        TEST_ADD(AcrsTest::lower_not_possible);
        TEST_ADD(AcrsTest::basic_overlap);
        TEST_ADD(AcrsTest::basic_main);
    }
};

#endif /* ACRS4TEST_H */
