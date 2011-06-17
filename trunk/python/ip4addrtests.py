#!/usr/bin/env python

from ip4addr import IP4Addr
import sys

def assert_equals(res, good):
	if (res != good):
		print "Failed.\n***\nDetails:\nResult was: ", res, "\n", \
		      "Should have been:", good
		print "***"
		raise AssertionError
	else:
		return

def ip4addr_test_good_str_int24():
	addr = IP4Addr("192.168.1.1", 16777215)

	assert_equals(addr.isValid(), True)
	assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
	assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
	assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
	assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
	assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
	assert_equals(addr.getPlen(), 24)

	return

def ip4addr_test_good_str_str24():
	addr = IP4Addr("192.168.1.1", "255.255.255.0")

	assert_equals(addr.isValid(), True)
	assert_equals(addr.getAddr(), ("192.168.1.1", 3232235777))
	assert_equals(addr.getMask(), ("255.255.255.0", 4294967040))
	assert_equals(addr.getHostmask(), ("0.0.0.255", 255))
	assert_equals(addr.getBroadcast(), ("192.168.1.255", 3232236031))
	assert_equals(addr.getNetwork(), ("192.168.1.0", 3232235776))
	assert_equals(addr.getPlen(), 24)

	return

def ip4addr_test_another():
	assert True

if (__name__ == "__main__"):
	# Find all methods starting with "ip4addr_test" and run them.
	for test in dir(sys.modules[__name__]):
		if (test.startswith("ip4addr_test")):
			sys.stdout.write("Running " + test + "... ")
			getattr(sys.modules[__name__], test)()
			sys.stdout.write("OK\n")

	print "All tests succeeded."
