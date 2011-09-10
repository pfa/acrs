/* acrs4-test.cpp */

#include <cpptest.h>

#include "acrs4.hpp"
#include "acrs4-test.hpp"

void AcrsTest::no_routes(void)
{
    Acrs::Acrs<IP4Route::IP4Route> summary;
    bool summarized = summary.summarize();

    TEST_ASSERT(summarized == false);
    TEST_ASSERT(summary.empty() == true);
}

void AcrsTest::one_good_route(void)
{
    Acrs::Acrs<IP4Route::IP4Route> summary;
    IP4Route::IP4Route rt("192.168.1.0", 24);

    summary.push_front(rt);
    bool summarized = summary.summarize();
    TEST_ASSERT(summarized == false);
    TEST_ASSERT(summary.begin()->getAddr() == rt.getAddr());
    TEST_ASSERT(summary.begin()->getMask() == rt.getMask());
}

void AcrsTest::one_bad_route(void)
{
    Acrs::Acrs<IP4Route::IP4Route> summary;
    IP4Route::IP4Route rt("256.256.256.256", 24);

    summary.push_front(rt);
    bool summarized = summary.summarize();
    TEST_ASSERT(summarized == false);
    TEST_ASSERT(summary.empty() == false);
}

void AcrsTest::lower_not_possible(void)
{
    Acrs::Acrs<IP4Route::IP4Route> summary;
    IP4Route::IP4Route rt1("192.168.1.1", 32);
    IP4Route::IP4Route rt2("192.168.1.2", 32);

    summary.push_front(rt1);
    summary.push_front(rt2);

    bool summarized = summary.summarize();
    TEST_ASSERT(summarized == false);

    /* Check sort order */
    TEST_ASSERT(summary.front().str() == rt1.str());
    TEST_ASSERT(summary.back().str() == rt2.str());
}

void AcrsTest::basic_overlap(void)
{
    Acrs::Acrs<IP4Route::IP4Route> summary;
    IP4Route::IP4Route rt1("192.168.1.1", 32);
    IP4Route::IP4Route rt2("192.168.1.1", 32);

    summary.push_front(rt1);
    summary.push_front(rt2);

    bool summarized = summary.summarize();
    TEST_ASSERT(summarized == true);
    TEST_ASSERT(summary.size() == 1);
    TEST_ASSERT(summary.front().str() == rt1.str());
}

void AcrsTest::basic_main(void)
{
    Acrs::Acrs<IP4Route::IP4Route> summary;
    IP4Route::IP4Route rt1("192.168.1.0", 32);
    IP4Route::IP4Route rt2("192.168.1.1", 32);

    summary.push_front(rt1);
    summary.push_front(rt2);

    bool summarized = summary.summarize();
    TEST_ASSERT(summarized == true);
    TEST_ASSERT(summary.size() == 1);
    TEST_ASSERT(summary.front().str() ==
                IP4Route::IP4Route("192.168.1.0", 31).str());
}
