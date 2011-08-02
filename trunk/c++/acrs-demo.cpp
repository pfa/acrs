/* acrs-demo.cpp - demo of acrs as-is
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

#include "acrs4.hpp"

int get_list(std::list<IP4Route::IP4Route> & rtlist, int numrts,
             char * rts[]);
void usage(void);

int main(int argc, char * argv[])
{
    std::list<IP4Route::IP4Route> rtlist;
    int startind = 1;
    bool logging = false;

    if (argc == 1)
    {
        usage();
        return 1;
    }

    /* Check for log flag, must be first argument */
    if (strlen(argv[1]) == 2)
    {
        if (argv[1][0] == '-' && argv[1][1] == 'l')
        {
            if (argc == 2)
            {
                usage();
                return 1;
            }

            startind = 2;    
            logging = true;
        }
    }

    if (get_list(rtlist, argc - startind, &argv[startind]) == false)
    {
        std::cerr << "Bad list." << std::endl;
        return 2;
    }

        Acrs4::Summarize(rtlist, logging, std::cout);

        for (std::list<IP4Route::IP4Route>::iterator iter = rtlist.begin();
             iter != rtlist.end();
             iter++)
        {
                std::cout << *iter << std::endl;
        }

    return 0;
}

void usage(void)
{
    std::cerr << "Usage: acrs-demo [-l] <PREFIX> [PREFIX ...]" << std::endl
              << "PREFIX is an IPv4 address and mask in CIDR form (e.g. "
              << "192.168.1.1/24)" << std::endl;
    return;
}

int get_list(std::list<IP4Route::IP4Route> & rtlist, int numrts,
             char * rts[])
{
    for (int i = 0; i < numrts; i++)
    {
        char * ptr1, * ptr2;
        char * saveptr = 0;
        char ipstr[INET_ADDRSTRLEN];
        char * prefix = rts[i];

        /* Validation */
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
        
        uint8_t preflen = atoi(ptr2);
        int metric = 0;
        IP4Route::IP4Route newrt(ipstr, preflen, metric);
        if (newrt.isValid() == false)
        {
            return false;
        }
        rtlist.insert(rtlist.end(), newrt);
    }

    return true;
}
