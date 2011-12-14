/* route.hpp */

#ifndef ROUTE_H
#define ROUTE_H

#include <iostream>
#include <string>

#include "addr.hpp"

namespace IP
{
    class Route : public virtual Addr
    {
        private:
        protected:
            int m_metric;
            bool m_metric_valid;

            void setMetricFail();
            void setMetricSuccess();

        public:
            enum
            {
                MIN_METRIC = 0,
                MAX_METRIC = 65535
            };

            int getMetric() const;
            bool setMetric(int metric);
            bool isValid() const;
            bool isValidMetric(int metric) const;

            virtual void printAll(std::ostream & os) const {};
            friend std::ostream & operator<<(std::ostream & os,
                                             const Route & rt);
            std::string operator*() const;
            std::string str() const;

            /* Constructors */
            Route();

            /* Destructor */
            virtual ~Route() = 0;

            static int getMaxMetric();
            static int getMinMetric();
    };
}

#endif /* ROUTE_H */
