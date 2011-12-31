#include <cstdio>

#include <cpptest.h>

#include "addr6netform-test.hpp"
#include "addr6-test.hpp"

int main()
{
    Addr6NetFormTest addr6netform_test;
    Addr6Test addr6_test;

    Test::TextOutput output(Test::TextOutput::Verbose);

    addr6netform_test.run(output);
    addr6_test.run(output);

    return 0;
}
