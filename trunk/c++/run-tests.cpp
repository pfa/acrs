#include <cpptest.h>

#include "ip4addr-test.hpp"

int main(void)
{
    IP4AddrTest ip4addr_tests;
    Test::TextOutput output(Test::TextOutput::Verbose);
    ip4addr_tests.run(output);
    return 0;
}
