/* acrs-demo.cpp - demo of the acrs4 library
 *
 * Copyright 2011 Patrick F. Allen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* Usage
 *
 * Give me command line args like the following:
 *
 * ./acrs-demo 10.0.0.0/26 10.0.0.64/26 10.0.0.128/25
 *
 * And I'll summarize them and print to standard out. Example output for
 * the above:
 *
 * 10.0.0.0/24 in 0
 *
 * At the moment, metrics cannot be passed to acrs using this parser. All
 * routes are considered to have equal metrics.
 *
 */

#include <list>
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "acrs4.hpp"

#define OPTIONS "lh"

int get_list(Acrs4::Acrs4 & summary, int numrts, char * rts[]);
void usage(void);

int main(int argc, char * argv[])
{
    Acrs4::Acrs4 summary;
    int startind = 1;
    char c;

    if (argc == 1)
    {
        usage();
        return 2;
    }

    while ((c = getopt(argc, argv, OPTIONS)) != -1)
    {
        switch (c)
        {
            case 'l':
                summary.setLogging(true);
                startind += 1;
                break;
            case 'h':
                usage();
                return 2;
                break;
            default:
                usage();
                return 2;
                break;
        }
    }

    if (argc - startind == 0)
    {
        usage();
        std::cerr << "Error: One or more prefixes required." << std::endl;
        return 2;
    }

    if (get_list(summary, argc - startind, &argv[startind]) == false)
    {
        std::cerr << "Error: Bad list." << std::endl;
        return 2;
    }

    int summarized = summary.summarize();

    for (std::list<IP4Route::IP4Route>::iterator iter = summary.begin();
         iter != summary.end();
         iter++)
    {
        std::cout << *iter << std::endl;
    }

    return summarized;
}

void usage(void)
{
    std::cerr << "Usage: acrs-demo [-l] <PREFIX> [PREFIX ...]" << std::endl
              << "PREFIX is an IPv4 address and mask in CIDR form (e.g. "
              << "192.168.1.1/24)" << std::endl
              << "Use -l to enable logging"
              << std::endl;
    return;
}

int get_list(Acrs4::Acrs4 & summary, int numrts, char * rts[])
{
    for (int i = 0; i < numrts; i++)
    {
        char * ptr1, * ptr2;
        char * saveptr = 0;
        char ipstr[INET_ADDRSTRLEN];
        char * prefix = rts[i];

        ptr1 = strtok_r(prefix, "/", &saveptr);
        if (ptr1 == 0)
        {
            return false;
        }
        strncpy(ipstr, ptr1, sizeof(ipstr));

        ptr2 = strtok_r(NULL, "/", &saveptr);
        if (ptr2 == 0)
        {
            return false;
        }

        /* Make sure prefix length is actually a number, otherwise atoi will return 0 */
        for (char * c = ptr2; *c != 0; c++)
        {
            if (isdigit(*c) == false)
            {
                return false;
            }
        }

        uint8_t preflen = atoi(ptr2);
        int metric = 0;
        IP4Route::IP4Route newrt(ipstr, preflen, MaskType::PLEN);
        if (newrt.isValid() == false)
        {
            return false;
        }
        summary.insert(summary.end(), newrt);
    }

    return true;
}
