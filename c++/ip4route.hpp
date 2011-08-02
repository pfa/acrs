/* ip4route.hpp */

#ifndef IP4ROUTE_H
#define IP4ROUTE_H

#include <string>

#include <inttypes.h>
#include <arpa/inet.h>

#include "ip4addr.hpp"

namespace IP4Route
{
    enum
    {
        MAX_METRIC = 65535,
        MIN_METRIC = 0
    };

    class IP4Route : public IP4Addr::IP4Addr
    {
    private:
        in_addr_t m_metric;
        bool m_metric_valid;
    
        bool setMetricFail();
        bool setMetricSuccess();
        bool isValidMetric(int metric);
    
    public:
        int getMetric();
        bool setMetric(int metric);
        bool isValid();
    
        /* Constructors */
        IP4Route(void);
        IP4Route(std::string addr_s, uint32_t mask, int metric = 0);
        IP4Route(std::string addr_s, std::string snmask_s, int metric = 0);
        IP4Route(in_addr_t addr_i, uint32_t mask, int metric = 0);
        IP4Route(in_addr_t addr_i, std::string snmask_s, int metric = 0);

        virtual ~IP4Route() {};
    
        void printAll(std::ostream & os);
        friend std::ostream & operator<<(std::ostream & os, IP4Route & rt);
    };
}

#endif /* IP4ROUTE_H */