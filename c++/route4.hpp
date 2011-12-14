/* route4.hpp */

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
