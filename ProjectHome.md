# About #

This project presents and explains a simple algorithm to automatically summarize lists of IPv4 or IPv6 routes or prefixes, classlessly. Source code is provided for an IPv4 and IPv6 implementation in C++, and an IPv4 implementation in Python, along with a demo program that uses the library.

Example from the demo program that uses the C++ library:

```
# ./acrs-demo 10.0.0.0/26 10.0.0.64/26 10.0.0.128/25
10.0.0.0/24 in 0

# ./acrs-demo -6 2001:db8::1/128m1 2001:db8::/128m1
2001:db8::/127 in 1
```

For general information about ACRS, see the Getting Started page: http://code.google.com/p/acrs/wiki/GettingStarted

Download the code (get the highest revision): http://code.google.com/p/acrs/downloads/list

Browse the source online here: http://code.google.com/p/acrs/source/browse/trunk

For help setting up the web-based demo (there is a separate command line demo with the source), see this page: http://code.google.com/p/acrs/wiki/WebDemoSetup

# Updates #

_1 January 2012_

The C++ Acrs library now fully supports classless IPv6 route summarization, and the command line demo program has been updated to accept both IPv4 and IPv6 routes (specify -4 or -6 on the command line, otherwise the default of -4 is used).

A new tar file is available on the downloads tab. G++ version 4.4 or greater is required to compile since some c++0x features are used.

_13 December 2011_

Finished the first round of refactoring to support IPv6 in the C++ code. There is now a base IP Address class, a base Route class, and derived classes for IPv4 and IPv6 addresses and routes. The ACRS template supports both IPv4 and IPv6 route summarization. This is all in the latest subversion revision.

Right now the acrs-demo program still only supports IPv4.

Metric support was added into the demo program a while ago (its always been supported by the library), so that's also new.

There's still some cleaning up to do, and the unit tests have yet to be updated.

_8 August 2011_

Cleaned up the C++ code structure quite a lot over the past few days. Acrs4 is now a proper class and inherits from std::list. Basic unit tests for IP4Route and Acrs4 are complete.

_1 August 2011_

Updated C++ code and put up a new tar file.

_23 June 2011_

New tar file of the latest code on the downloads tab.

_22 June 2011_

Added a python CGI script that will allow the acrs library to be demonstrated from a web server. I also added a rudimentary web page that will submit data to the script, as an example (feel free to use your own). Any other web page should work with the script as long as there is an html form named "summary-input" which submits text (the route list) to acrs-web-demo.py.

A few other notes for using the web demo version:

  1. The web server needs CGI enabled and must have Python 2.6 installed (won't work with 2.5, haven't tested with anything else)
  1. The acrs class files (acrs.py, ip4addr.py, ip4route.py) all need to be in the Python class search path (such as /usr/lib/python2.6/ or the same directory as acrs-web-demo.py).
  1. By default, the script will print diagnostic messages to the web page if an exception is thrown. Along with a stack trace, this will print the version of python you have installed and a timestamp. This information aids in debugging. If you don't want the python version or time advertised from your web server, open the script and comment out the line "cgitb.enable()" (around line 33).
  1. The number of routes allowed to be summarized is limited in the web version to prevent misuse. It's currently set to 50. This can be increased by setting the ARGS\_LIMIT variable near the top of acrs-web-demo.py.

_18 June 2011_

Mostly finished with the Python implementation. The current subversion has a usable demo program and supporting classes (acrs, ip4route, ip4addr). It seems to be summarizing basic input properly, though I have not yet rigorously tested it. At this point, the ip4addr class unit tests mainly target input possible from the demo program (so a string address plus a prefix length), so those tests need to be extended for other types of input.

I'll bundle a new tar file together after I've done some more testing.

_17 June 2011_

I'm in the process of implementing this in Python. I decided to upload the current Python code to subversion just to get it into the light of day. It's very much still a work in progress -- I don't even have the IP address class completely finished yet. So, while the summarization script (acrs.py) is included, it's not functional at the moment. It can all be found here: http://code.google.com/p/acrs/source/browse/#svn%2Ftrunk%2Fpython.

_17 May 2011_

Initial C++ commit.