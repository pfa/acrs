/* acrs-demo.cpp - demo of acrs as-is
 *
 * Copyright 2011 Patrick Allen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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
 * ./acrs-parser 10.0.0.0/26 10.0.0.64/26 10.0.0.128/25
 *
 * And I'll summarize them and print to standard out. Example output for
 * the above:
 *
 * 10.0.0.0/24
 *
 * At the moment, metrics cannot be passed to acrs using this parser. All
 * routes are considered to have equal metrics.
 *
 */

#include <list>
#include <iostream>

#include <stdlib.h>
#include <string.h>

#include "acrs.hpp"

int get_list(std::list<IP4Addr::Acrs::AcrsRoute4> & rtlist, int numrts, char * rts[]);
int check_network(char * netstr);
int check_octet(char * octetp);
int check_preflen(char * s);
int is_number(char * s);

int main(int argc, char * argv[])
{
	std::list<IP4Addr::Acrs::AcrsRoute4> rtlist;

	if (argc == 1)
	{
		std::cerr << "Usage: acrs-parser <ROUTE> [ROUTE ROUTE ...]" << std::endl;
		return 1;
	}

	if (get_list(rtlist, argc - 1, &argv[1]) == 1)
	{
		std::cerr << "Bad list." << std::endl;
		return 2;
	}

        IP4Addr::Acrs::Summarize(rtlist);

        for (std::list<IP4Addr::Acrs::AcrsRoute4>::iterator iter = rtlist.begin();
             iter != rtlist.end();
             iter++)
        {
                std::cout << *iter << std::endl;
        }

	return 0;
}

int get_list(std::list<IP4Addr::Acrs::AcrsRoute4> & rtlist, int numrts, char * rts[])
{
	for (int i = 0; i < numrts; i++)
	{
		char * ptr1, * ptr2;
		char * saveptr;
		char ipstr[INET_ADDRSTRLEN];
		in_addr_t network;
		uint8_t preflen;
		uint64_t hash;
		int metric = 0;
		char * prefix = rts[i];
		IP4Addr::Acrs::AcrsRoute4 * newrt;

		/* Validation */
		ptr1 = strtok_r(prefix, "/", &saveptr);
		strncpy(ipstr, ptr1, sizeof(ipstr));
		ptr2 = strtok_r(NULL, "/", &saveptr);
		if (ptr1)
		{
			if (check_network(ptr1))
			{
				std::cerr << "Invalid network ID" << std::endl;
				return 1;
			}
		}
		else
		{
			std::cerr << "No network ID given" << std::endl;
			return 1;
		}
	
		if (ptr2)
		{
			if (check_preflen(ptr2) < 0)
			{
				std::cerr << "Invalid prefix length" << std::endl;
				return 1;
			}
		}
		else
		{
			std::cerr << "No prefix length given" << std::endl;
			return 1;
		}

		preflen = atoi(ptr2);

		newrt = new IP4Addr::Acrs::AcrsRoute4(ipstr, preflen, metric);
		rtlist.insert(rtlist.end(), *newrt);
	}

	return 0;
}

/* check_network
 * Make sure this is a valid network in dotted decimal notation.
 * Return 1 on error, otherwise return 0.
 */
int check_network(char * netstr)
{
	char * octetp, * savep;
	int valid = 0;                 /* Number of valid octets found */

	octetp = strtok_r(netstr, ".", &savep);

	if (check_octet(octetp))
		return 1;
	else
		valid++;
	
	while (octetp = strtok_r(NULL, ".", &savep))
		if (check_octet(octetp))
			return 1;
		else
			valid++;

	return ! (valid == 4);
}

int check_octet(char * octetp)
{
	int num;

	if (strlen(octetp) < 4 && is_number(octetp)) {
		num = atoi(octetp);

		if (num > 255 && num < 0) {
			return 1;
		}
	} else {
		return 1;
	}
}

/* check_preflen
 * Determine if the given string is a valid prefix length.
 * Return -1 if there is a problem, otherwise return the prefix length as an
 * integer.
 */
int check_preflen(char * s)
{
	int plen;

	if (is_number(s))
		atoi(s);
	else
		return -1;

	plen = atoi(s);

	if (plen > 32 || plen < 0)
		return -1;
	else
		return plen;
}

/* is_number
 * Return 1 if the string s contains all numeric characters, otherwise return 
 * 0. No negative numbers.
 */
int is_number(char * s)
{
	int i, first;

	/* No negative numbers */
	if (s[0] == '-')
		return 0;

	/* Are any chars in the string not a number? */
	for (i = 0; s[i] != '\0'; i++)
		if (! isdigit(s[i]))
			return 0;

	/* If the string length was greater than 0, this was a number */
	return (i > 0);
}
