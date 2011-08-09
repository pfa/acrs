/* acrs4-test.hpp */

#ifndef ACRS4TEST_H
#define ACRS4TEST_H

#include <cpptest.h>

#include "acrs4.hpp"
#include "acrs4-test.hpp"

class Acrs4Test : public Test::Suite
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
    Acrs4Test()
    {
        TEST_ADD(Acrs4Test::no_routes);
        TEST_ADD(Acrs4Test::one_good_route);
        TEST_ADD(Acrs4Test::one_bad_route);
        TEST_ADD(Acrs4Test::lower_not_possible);
        TEST_ADD(Acrs4Test::basic_overlap);
        TEST_ADD(Acrs4Test::basic_main);
    }
};

#endif /* ACRS4TEST_H */
