/* route4.hpp
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

#ifndef IPROUTE4_H
#define IPROUTE4_H

#include <string>

#include "route.hpp"
#include "addr4.hpp"

namespace IP
{
    class Route4 : public virtual Route, public virtual Addr4
    {
        private:
        public:
            void printAll(std::ostream & os) const;

            /* Constructors */
            Route4(const std::string & addr, const uint32_t mask,
                   const MaskType type = UNSPEC, const int metric = 0);
            Route4(const std::string & addr, const std::string & mask,
                   const int metric = 0);
            Route4(const in_addr_t addr, const uint32_t mask,
                   const MaskType type = UNSPEC, const int metric = 0);
            Route4(const in_addr_t addr, const std::string & mask,
                   const int metric = 0);

            /* Destructor */
            virtual ~Route4() {};
    };
}

#endif /* IPROUTE4_H */
