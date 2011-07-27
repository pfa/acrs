#!/usr/bin/env python
# Unit tests.
# To add a new test, write a function beginning with "ip4addr_test".

import ip4addr
import ip4route
import sys

def assert_equals(res, good):
    if (res != good):
        raise AssertionError("***\n"
                             " Details:\n"
                             " Result was:", res, "\n",
                             " Should have been:", good, "\n",
                             "***")
    else:
        return

def check_version():
    # Only tested on python 2.6. Print a helpful warning message if this
    # system has a different version.
    if (sys.hexversion < 0x02060000 or sys.hexversion >= 0x02070000):
        sys.stderr.write("/*\n"
                         " *   WARNING: This was written for python 2.6.\n"
                         " *   Probably won't work with lower versions, and\n"
                         " *   has not been tested with higher versions.\n"
                         " */\n")

def sanity():
    assert_equals(1, 1)

    try:
        assert_equals(1, 0)
    except AssertionError:
        # Expected
        return

    # Should not reach
    raise AssertionError

def ip4addr_test_goodstr_goodint24():
    addr = ip4addr.IP4Addr("192.168.1.1", 16777215)

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
    assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
    assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
    assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
    assert_equals(addr.getPlen(), 24)

def ip4addr_test_goodstr_goodstr24():
    addr = ip4addr.IP4Addr("192.168.1.1", "255.255.255.0")

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
    assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
    assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
    assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
    assert_equals(addr.getPlen(), 24)

def ip4addr_test_goodstr_goodplen24():
    addr = ip4addr.IP4Addr("192.168.1.1", 24)

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
    assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
    assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
    assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
    assert_equals(addr.getPlen(), 24)

def ip4addr_test_goodint_goodplen24():
    addr = ip4addr.IP4Addr(16885952, 24)

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
    assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
    assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
    assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
    assert_equals(addr.getPlen(), 24)

def ip4addr_test_goodint_goodstr24():
    addr = ip4addr.IP4Addr(16885952, "255.255.255.0")

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
    assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
    assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
    assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
    assert_equals(addr.getPlen(), 24)

def ip4addr_test_goodint_goodint24():
    addr = ip4addr.IP4Addr(16885952, 16777215)

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
    assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
    assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
    assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
    assert_equals(addr.getPlen(), 24)

def ip4addr_test_badstr_goodplen24():
    addr = ip4addr.IP4Addr("1", 24)
    invalid_addr(addr)

def ip4addr_test_goodstr_badmask():
    addr = ip4addr.IP4Addr("1.1.1.1", 64)
    invalid_addr(addr)

def ip4addr_test_goodstr_goodplen_default():
    addr = ip4addr.IP4Addr("0.0.0.0", 0)

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("0.0.0.0", 0))
    assert_equals(addr.getMask(), ("0.0.0.0", 0))
    assert_equals(addr.getHostmask(), ("255.255.255.255", 4294967295))
    assert_equals(addr.getBroadcast(), ("255.255.255.255", 4294967295))
    assert_equals(addr.getNetwork(), ("0.0.0.0", 0))
    assert_equals(addr.getPlen(), 0)

def ip4addr_test_goodstr_goodplen_hostroute():
    addr = ip4addr.IP4Addr("192.168.1.1", 32)

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getMask(), ("255.255.255.255", 4294967295))
    assert_equals(addr.getHostmask(), ("0.0.0.0", 0))
    assert_equals(addr.getBroadcast(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getNetwork(), ("192.168.1.1", 3232235777))
    assert_equals(addr.getPlen(), 32)

def ip4addr_test_goodstr_badplen_negative():
    addr = ip4addr.IP4Addr("192.168.1.1", -1)
    invalid_addr(addr)

def ip4addr_test_goodstr_badplen_toohigh():
    addr = ip4addr.IP4Addr("192.168.1.1", 33)
    invalid_addr(addr)

def ip4addr_test_goodstr_badsnmask_toohigh():
    addr = ip4addr.IP4Addr("192.168.1.1", 4294967293)
    invalid_addr(addr)

def ip4addr_test_goodstr_goodplen30():
    addr = ip4addr.IP4Addr("10.0.4.2", 30)

    assert_equals(addr.isValid(), True)
    assert_equals(addr.getAddr(), ("10.0.4.2", 167773186))
    assert_equals(addr.getMask(), ("255.255.255.252", 4294967292))
    assert_equals(addr.getHostmask(), ("0.0.0.3", 3))
    assert_equals(addr.getBroadcast(), ("10.0.4.3", 167773187))
    assert_equals(addr.getNetwork(), ("10.0.4.0", 167773184))
    assert_equals(addr.getPlen(), 30)

def ip4addr_test_goodstr_badmask_nbo8():
    addr = ip4addr.IP4Addr("10.0.4.2", 4278190080)
    invalid_addr(addr)

def invalid_addr(addr):
    if (addr.__class__.__name__ != "IP4Addr"):
        raise TypeError

    assert_equals(addr.isValid(), False)
    assert_equals(addr.getAddr(), (None, None))
    assert_equals(addr.getMask(), (None, None))
    assert_equals(addr.getHostmask(), (None, None))
    assert_equals(addr.getBroadcast(), (None, None))
    assert_equals(addr.getNetwork(), (None, None))
    assert_equals(addr.getPlen(), None)

def ip4route_test_goodstr_goodplen_goodmetric():
    rt = ip4route.IP4Route("192.168.1.1", 24, 0)

    assert_equals(rt.isValid(), True)
    assert_equals(rt.getMetric(), 0)
    assert_equals(rt.getAddr(), ("192.168.1.1", 3232235777))
    assert_equals(rt.getMask(), ("255.255.255.0", 4294967040))
    assert_equals(rt.getHostmask(), ("0.0.0.255", 255))
    assert_equals(rt.getBroadcast(), ("192.168.1.255", 3232236031))
    assert_equals(rt.getNetwork(), ("192.168.1.0", 3232235776))
    assert_equals(rt.getPlen(), 24)

def ip4route_test_goodstr_goodplen_badmetric_toohigh():
    rt = ip4route.IP4Route("192.168.1.1", 24, 65536)

    assert_equals(rt.isValid(), False)
    assert_equals(rt.getMetric(), None)

def ip4route_test_set_plen_good():
    rt = ip4route.IP4Route("192.168.1.1", 24)

    assert_equals(rt.isValid(), True)
    rt.setPlen(23)
    assert_equals(rt.getPlen(), 23)

def ip4route_test_set_metric_good():
    rt = ip4route.IP4Route("192.168.1.1", 24, 1)

    assert_equals(rt.isValid(), True)
    rt.setMetric(2)
    assert_equals(rt.getMetric(), 2)

def ip4route_test_goodstr_goodplen_impliedmetric():
    rt = ip4route.IP4Route("192.168.1.1", 24)

    assert_equals(rt.isValid(), True)
    assert_equals(rt.getMetric(), 0)
    assert_equals(rt.getAddr()[0], "192.168.1.1")
    assert_equals(rt.getPlen(), 24)
 
def ip4route_test_goodstr_goodplen_badmetric_negative():
    rt = ip4route.IP4Route("192.168.1.1", 24, -1)

    assert_equals(rt.isValid(), False)
    assert_equals(rt.getMetric(), None)

def ip4route_test_goodstr_badplen_goodmetric():
    rt = ip4route.IP4Route("192.168.1.1", -1, 0)

    assert_equals(rt.isValid(), False)
    assert_equals(rt.getMetric(), None)

def main():
    check_version()

    print "Running sanity check...",
    sanity()
    print "OK"

    run_tests_by_name("ip4addr")
    run_tests_by_name("ip4route")
    print "* All tests succeeded."

def run_tests_by_name(name):
    print "* Beginning unit tests for " + name + " class."
    try:
        # Find all methods starting with NAME_test and run them
        for test in dir(sys.modules[__name__]):
            if (test.startswith(name + "_test")):
                sys.stdout.write("Running " + test + "... ")
                getattr(sys.modules[__name__], test)()
                print "OK"
    except AssertionError as e:
        print "Failed"
        for arg in e.args:
            print arg,
        e.args = ""
        raise

    print "* All " + name + " tests succeeded."

if (__name__ == "__main__"):
    main()
