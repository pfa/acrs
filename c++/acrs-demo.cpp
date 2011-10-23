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

#include <list>
#include <sstream>
#include <cstdio>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "acrs4.hpp"

#define OPTIONS "lhm"

bool getList(Acrs::Acrs<IP4Route::IP4Route> & summary, int numrts,
             char * p_rts[]);
void usage(void);

int main(int argc, char * argv[])
{
    Acrs::Acrs<IP4Route::IP4Route> summary;
    extern int optind;
    char c;
    bool printMetric = true;

    while ((c = getopt(argc, argv, OPTIONS)) != -1)
    {
        switch (c)
        {
            case 'm':
                printMetric = false;
                break;
            case 'l':
                summary.setLogging(true);
                break;
            case 'h': /* Fall through */
            default:
                usage();
                return 2;
                break;
        }
    }

    if (argc - optind == 0)
    {
        usage();
        fprintf(stderr, "Error: One or more prefixes required.\n");
        return 2;
    }

    if (getList(summary, argc - optind, &argv[optind]) == false)
    {
        fprintf(stderr, "Error: Bad list.\n");
        return 2;
    }

    int summarized = summary.summarize();

    if (printMetric == false)
    {
        for (std::list<IP4Route::IP4Route>::iterator iter = summary.begin();
             iter != summary.end();
             iter++)
        {
            /* Cast to an IP address if metrics are not desired. */
            std::cout << *((IP4Addr::IP4Addr *) &(*iter)) << std::endl;
        }
    }
    else
    {
        for (std::list<IP4Route::IP4Route>::iterator iter = summary.begin();
             iter != summary.end();
             iter++)
        {
            std::cout << *iter << std::endl;
        }
    }

    return summarized;
}

bool getList(Acrs::Acrs<IP4Route::IP4Route> & summary, int numrts,
             char * p_rts[])
{
    char * p_addr;
    char * p_plen;
    char * p_metric;
    char * p_maskmetric;
    char * p_maskchk;
    char * p_save;
    char * p_prefix;
    char ipstr[INET_ADDRSTRLEN];
    int plen_int;
    int metric_int;

    for (int i = 0; i < numrts; i++)
    {
        p_save = 0;
        p_prefix = p_rts[i];

        /* Get IP address */
        p_addr = strtok_r(p_prefix, "/", &p_save);
        if (p_addr == 0)
        {
            fprintf(stderr, "Could not find IP address, no slash found in: %s"
                    " (use CIDR notation: 1.1.1.0/24)\n", p_prefix);
            return false;
        }
        strncpy(ipstr, p_addr, sizeof(ipstr));

        /* Get prefix length (and metric, if specified) */
        p_maskmetric = strtok_r(NULL, "/", &p_save);
        if (p_maskmetric == 0)
        {
            fprintf(stderr, "Could not find prefix length in: %s\n", p_prefix);
            return false;
        }

        /* Check for a second slash, since the below error checking will not
         * catch it. Reusing p_addr for this as it's no longer needed.
         */
        p_addr = strtok_r(NULL, "/", &p_save);
        if (p_addr != 0)
        {
            fprintf(stderr, "Improperly formatted prefix (extra slash).\n");
            return false;
        }

        /* If an "m" is used to supply a metric, make sure it is preceded
         * by a prefix length -- otherwise when strtok tokenizes something like
         * "/m1", the "1" will be used as a prefix length instead of throwing
         * an error.
         *
         * Similarly, if "m" is supplied, make sure a metric actually follows.
         */
        p_maskchk = strchr(p_maskmetric, 'm');
        if (p_maskchk == p_maskmetric)
        {
            fprintf(stderr, "Missing prefix length.\n");
            return false;
        }

        if ((p_maskchk - (strlen(p_maskmetric) - 1)) == p_maskmetric)
        {
            fprintf(stderr, "An 'm' was supplied, but the metric is "
                    "missing.\n");
            return false;
        }

        p_plen = strtok_r(p_maskmetric, "m", &p_save);
        p_metric = strtok_r(NULL, "m", &p_save);

        /* Make sure prefix length is actually actually a number,
         * otherwise atoi will return 0 (which would convert an invalid length
         * into a valid one).
         */
        for (char * c = p_plen; *c != 0; c++)
        {
            if (isdigit(*c) == false)
            {
                fprintf(stderr, "Invalid prefix length: %s\n", p_plen);
                return false;
            }
        }

        plen_int = atoi(p_plen);

        /* Metric isn't required. If not given, set to 0 */
        if (p_metric == 0)
        {
            metric_int = 0;
        }
        else
        {
            for (char * c = p_metric; *c != 0; c++)
            {
                if (isdigit(*c) == false)
                {
                    fprintf(stderr, "Invalid metric: %s\n", p_metric);
                    return false;
                }
            }

            metric_int = atoi(p_metric);
        }

        IP4Route::IP4Route newrt(ipstr, plen_int, IP4Addr::IP4Addr::PLEN,
                                 IP4Addr::IP4Addr::HBO, metric_int);
        if (newrt.isValid() == false)
        {
            std::stringstream ssPlen;
            std::stringstream ssMetric;

            /* Get the actual prefix length and metric passed to the
             * constructor instead of using the old strings, in case
             * the conversion happened incorrectly or the metric wasn't
             * specified.
             */
            ssPlen << plen_int;
            ssMetric << metric_int;

            fprintf(stderr, "Invalid route with attributes:\n"
                    "Address:         %s\n"
                    "Prefix length:   %s\n"
                    "Metric:          %s\n",
                    ipstr, ssPlen.str().c_str(), ssMetric.str().c_str());

            if (metric_int > IP4Route::IP4Route::MAX_METRIC)
            {
                fprintf(stderr, "Note: Maximum metric was "
                        "compiled as %d\n", IP4Route::IP4Route::MAX_METRIC);
            }
            else if (metric_int < IP4Route::IP4Route::MIN_METRIC)
            {
                fprintf(stderr, "Note: Minimum metric was "
                        "compiled as %d\n", IP4Route::IP4Route::MIN_METRIC);
            }

            return false;
        }
        summary.insert(summary.end(), newrt);
    }

    return true;
}

void usage(void)
{
    fprintf(stderr,
            "Automatic classless route summarization (ACRS) demo program\n"
            "Usage:\n"
            "\n"
            "       ./acrs-demo [-lmh] <PREFIX>[m<METRIC>] [PREFIX...]\n"
            "       PREFIX is an IPv4 address and mask in CIDR form. For example:\n"
            "            192.168.1.0/24\n"
            "       A metric of 0 is assumed unless 'm<NUMBER>' is appended to the prefix:\n"
            "            192.168.1.0/24m1\n"
            "       Use -l to enable logging\n"
            "       Use -m to suppress metric printing (\"in 0...\")\n"
            "       Use -h to display this help message\n"
            "\n"
            "Other useful information is available on the wiki at: acrs.googlecode.com\n");
    return;
}

