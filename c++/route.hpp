/* route.hpp
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
 */

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
