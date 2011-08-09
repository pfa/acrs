#include <cpptest.h>

#include "ip4addr-test.hpp"
#include "ip4route-test.hpp"
#include "acrs4-test.hpp"

int main(void)
{
    IP4AddrTest ip4addr_tests;
    IP4RouteTest ip4rt_tests;
    Acrs4Test acrs4_tests;

    Test::TextOutput output(Test::TextOutput::Verbose);

    ip4addr_tests.run(output);
    ip4rt_tests.run(output);
    acrs4_tests.run(output);

    return 0;
}
