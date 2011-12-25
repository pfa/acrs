#include <cstdio>

#include <cpptest.h>

#include "addr6netform-test.hpp"

int main()
{
    Addr6NetFormTest addr6_test;
    Test::TextOutput output(Test::TextOutput::Verbose);

    addr6_test.run(output);

    return 0;
}
