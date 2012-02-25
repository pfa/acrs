/* acrs-demo.cpp - Demo program for the ACRS library
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
#include <arpa/inet.h>
#include <assert.h>

#include "acrs.hpp"
#include "route.hpp"
#include "route4.hpp"
#include "route6.hpp"
#include "addr.hpp"

#define OPTIONS "lh46m:"

template <class T> bool getList(T & rt_list, int numrts, char * p_rts[],
                                int ipstr_len, int addr_family);
template <class T> int runSummary(T & rt_list, int numrts, char * p_rts[],
                                  bool logging, int metric_style,
                                  int ipstr_len, int addr_family);
bool getRoute(char * p_prefix, char * ipstr, int * plen_int, int * metric_int,
              int ipstr_len, int addr_family);
bool validateRoute(const char * p_prefix);
int findMetricStyle(const char * metric);
std::string getMetricStyleString(int spaces);
void usage();

#define METRIC_STYLES \
    STYLE(METRIC_STYLE_NONE, none, \
          "Do not print a metric") \
    STYLE(METRIC_STYLE_FULL, full, \
          "Print metric X as '... in X' (default)") \
    STYLE(METRIC_STYLE_BRIEF, brief, \
          "Print metric X as '...mX'")

#define STYLE(longname, shortname, desc) longname,
enum
{
    METRIC_STYLES
    METRIC_STYLE_INVALID
};
#undef STYLE

typedef struct metricType
{
    std::string name;
    std::string desc;
} metricType;

#define STYLE(longname, shortname, desc) { # shortname, desc },
static metricType METRIC_TYPES[] =
{
    METRIC_STYLES
};
#undef STYLE

int main(int argc, char * argv[])
{
    extern int optind;
    char c;
    bool logging = false;
    bool ipv4 = false;
    bool ipv6 = false;
    int metric_style = METRIC_STYLE_FULL;

    while ((c = getopt(argc, argv, OPTIONS)) != -1)
    {
        switch (c)
        {
        case 'm':
            metric_style = findMetricStyle(optarg);
            if (metric_style == METRIC_STYLE_INVALID)
            {
                fprintf(stderr, "Invalid metric style: %s\n"
                                "%s", optarg, getMetricStyleString(2).c_str());
                return 2;
            }
            break;
        case 'l':
            logging = true;
            break;
        case '4':
            if (ipv6 == true)
            {
                fprintf(stderr, "Error: -4 and -6 are mutually "
                                "exclusive.\n");
                return 2;
            }
            else
            {
                ipv4 = true;
            }
            break;
        case '6':
            if (ipv4 == true)
            {
                fprintf(stderr, "Error: -4 and -6 are mutually "
                                "exclusive.\n");
                return 2;
            }
            else
            {
                ipv6 = true;
            }
            break;
        case 'h': /* Fall through */
        default:
            usage();
            return 2;
            break;
        }
    }

    /* Default to using IPv4 if not specified on the command line */
    if (ipv4 == false && ipv6 == false)
    {
        ipv4 = true;
    }

    if (argc - optind == 0)
    {
        usage();
        fprintf(stderr, "Error: One or more prefixes required.\n");
        return 2;
    }

    int retval;

    if (ipv4)
    {
        std::list<IP::Route4> rt_list;
        retval = runSummary(rt_list, argc - optind, &argv[optind], logging,
                            metric_style, INET_ADDRSTRLEN, AF_INET);
    }
    else if (ipv6)
    {
        std::list<IP::Route6> rt_list;
        retval = runSummary(rt_list, argc - optind, &argv[optind], logging,
                            metric_style, INET6_ADDRSTRLEN, AF_INET6);
    }
    else
    {
        assert(false);
    }

    if (retval == 1)
    {
        /* Return 0 if anything was summarized */
        return 0;
    }
    else if (retval == 0)
    {
        /* Return 1 if nothing was summarized but there were no errors */
        return 1;
    }
    else
    {
        /* Return 2 if there was an error */
        return 2;
    }
}

template <class T> int runSummary(T & rt_list, int numrts, char * p_rts[],
                                  bool logging, int metric_style,
                                  int ipstr_len, int addr_family)
{
    Acrs::Acrs summary;
    summary.setLogging(logging);

    /* Fill a list with routes based on user input */
    if (getList(rt_list, numrts, p_rts, ipstr_len, addr_family) == false)
    {
        fprintf(stderr, "Error: One or more invalid routes entered.\n");
        return 2;
    }

    /* Summarize the route list */
    int summarized = summary.summarize(rt_list);

    /* Print the results */
    switch (metric_style)
    {
    case METRIC_STYLE_NONE:
        for (typename T::iterator iter = rt_list.begin();
             iter != rt_list.end();
             iter++)
        {
            /* Cast to an IP address if metrics are not desired */
            std::cout << *(dynamic_cast<IP::Addr*>(&(*iter))) << std::endl;
        }

        break;
    case METRIC_STYLE_FULL:
        for (typename T::iterator iter = rt_list.begin();
             iter != rt_list.end();
             iter++)
        {
            std::cout << *iter << std::endl;
        }

        break;
    case METRIC_STYLE_BRIEF:
        for (typename T::iterator iter = rt_list.begin();
             iter != rt_list.end();
             iter++)
        {
            std::cout << iter->getAddrP()
                      << "/" << iter->getPlen()
                      << "m" << iter->getMetric()
                      << std::endl;
        }

        break;
    default:
        assert(false);
    }

    return summarized;
}

template <class T> bool getList(T & rt_list, int numrts, char * p_rts[],
                                int ipstr_len, int addr_family)
{
    char ipstr[ipstr_len];
    int plen_int;
    int metric_int;
    char * p_prefix;

    for (int i = 0; i < numrts; i++)
    {
        p_prefix = p_rts[i];

        if (getRoute(p_prefix, ipstr, &plen_int, &metric_int, sizeof(ipstr),
                     addr_family) == false)
        {
            return false;
        }

        typename T::value_type newrt(ipstr, plen_int, IP::PLEN, metric_int);
        if (newrt.isValid() == false)
        {
            std::stringstream ss_plen;
            std::stringstream ss_metric;

            /* Get the actual prefix length and metric passed to the
             * constructor instead of using the old strings, in case
             * the conversion happened incorrectly or the metric wasn't
             * specified.
             */
            ss_plen << plen_int;
            ss_metric << metric_int;

            fprintf(stderr, "Invalid IPv%d route with attributes:\n"
                    "Address:         %s\n"
                    "Prefix length:   %s\n"
                    "Metric:          %s\n",
                    addr_family == AF_INET ? 4 : 6, ipstr,
                    ss_plen.str().c_str(), ss_metric.str().c_str());

            if (metric_int > IP::Route::MAX_METRIC)
            {
                fprintf(stderr, "Note: Maximum metric was "
                        "compiled as %d\n", IP::Route::MAX_METRIC);
            }
            else if (metric_int < IP::Route::MIN_METRIC)
            {
                fprintf(stderr, "Note: Minimum metric was "
                        "compiled as %d\n", IP::Route::MIN_METRIC);
            }

            return false;
        }
        else
        {
            rt_list.push_back(newrt);
        }
    }

    return true;
}

bool getRoute(char * p_prefix, char * ipstr, int * plen_int, int * metric_int,
              int ipstr_len, int addr_family)
{
    char * p_addr;
    char * p_plen;
    char * p_metric;
    char * p_maskmetric;
    char * p_maskchk;
    char * p_save = 0;

    /* Get IP address */
    p_addr = strtok_r(p_prefix, "/", &p_save);
    if (p_addr == 0)
    {
        std::string addr_example;

        switch (addr_family)
        {
        case AF_INET:
            addr_example = "1.1.1.0/24";
            break;
        case AF_INET6:
            addr_example = "2001:db8::/64";
            break;
        default:
            assert(false);
            break;
        }

        fprintf(stderr, "Could not find IP address, no slash found in: %s"
                " (use CIDR notation: %s)\n", p_prefix, addr_example.c_str());

        return false;
    }
    strncpy(ipstr, p_addr, ipstr_len);

    /* Check for duplicate slashes */
    if (p_save && (strchr(p_save, '/') != 0))
    {
        fprintf(stderr, "Improperly formatted prefix (extra slash).\n");
        return false;
    }

    /* Get prefix length (and metric, if specified) */
    p_maskmetric = strtok_r(NULL, "/", &p_save);
    if (p_maskmetric == 0)
    {
        fprintf(stderr, "Could not find prefix length in: %s\n", p_prefix);
        return false;
    }

    /* If an "m" is used to supply a metric, make sure it is preceded
     * by a prefix length -- otherwise when strtok tokenizes something like
     * "/m1", the "1" will be used as a prefix length instead of throwing
     * an error.
     */
    p_maskchk = strchr(p_maskmetric, 'm');
    if (p_maskchk == p_maskmetric)
    {
        fprintf(stderr, "Missing prefix length.\n");
        return false;
    }

    /*
     * If "m" is supplied, make sure a metric actually follows.
     *
     * The starting address of the potential mask (maskchk) minus
     * the length of the full mask and metric string (everything after
     * the slash) minus 1 should NOT be equal to the starting address of
     * the full mask and metric string. This would mean either an 'm'
     * was specified and no metric is present, or there are two 'm' chars.
     */
    if ((p_maskchk - (strlen(p_maskmetric) - 1)) == p_maskmetric)
    {
        fprintf(stderr, "An 'm' was supplied, but the metric was "
                "missing.\n");
        return false;
    }

    /* Check for a second 'm' and error out if present, which the above
     * won't catch
     */
    if (p_maskchk && (strchr((p_maskchk + 1), 'm') != 0))
    {
        fprintf(stderr, "Duplicate 'm' character entered.\n");
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

    *plen_int = atoi(p_plen);

    /* Metric isn't required. If not given, set to 0 */
    if (p_metric == 0)
    {
        *metric_int = 0;
    }
    else
    {
        /* Make sure metric is a number (as above) */
        for (char * c = p_metric; *c != 0; c++)
        {
            if (isdigit(*c) == false)
            {
                fprintf(stderr, "Invalid metric: %s\n", p_metric);
                return false;
            }
        }

        *metric_int = atoi(p_metric);
    }

    return true;
}

int findMetricStyle(const char * requested_name)
{
    int i;

    for (i = 0; i != METRIC_STYLE_INVALID; i++)
    {
        const char * valid_name = METRIC_TYPES[i].name.c_str();
        if (strcasecmp(requested_name, valid_name) == 0)
        {
            break;
        }
    }

    /* Return the index of the style (maps to the enum value) */
    return i;
}

std::string getMetricStyleString(int spaces)
{
    std::string s = "";

    for (int i = 0; i != METRIC_STYLE_INVALID; i++)
    {
        for (int count = 0; count <= spaces; count++)
        {
            s += " ";
        }

        s += METRIC_TYPES[i].name + ": " + METRIC_TYPES[i].desc + "\n";
    }

    return s;
}

void usage()
{
    fprintf(stderr,
            "Automatic classless route summarization (ACRS) demo program\n"
            "Usage:\n"
            "\n"
            "       ./acrs-demo [-46lh] [-m STYLE] PREFIX [PREFIX ...]\n"
            "\n"
            "       PREFIX consists of <NETWORK>/<PREFLEN>[m<METRIC>]\n"
            "\n"
            "       NETWORK is an IPv4 or IPv6 address.\n"
            "       PREFLEN is the prefix length.\n"
            "       METRIC is the route's metric (this is optional, default is 0).\n"
            "\n"
            "       Example usage:  ./acrs-demo 192.168.0.0/24m1 192.168.1.0/24\n"
            "                       ./acrs-demo -6 2001:db8::/128 2001:db8::1/128\n"
            "\n"
            "       Options:\n"
            "       -l    Enables logging\n"
            "       -4    Input routes are IPv4 (default)\n"
            "       -6    Input routes are IPv6\n"
            "       -h    Displays this help message\n"
            "       -m STYLE  Modifies the format of the metric message (\"... in 0\")\n"
            "             in summary output. Does not affect logging messages or how routes\n"
            "             are summarized. Valid metric styles:\n"
            "%s\n"
            "Other useful information is available on the wiki at: acrs.googlecode.com\n", getMetricStyleString(15).c_str());
    return;
}
