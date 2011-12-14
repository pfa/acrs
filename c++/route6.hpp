/* route6.hpp */

#ifndef IPROUTE6_H
#define IPROUTE6_H

#include <string>

#include "route.hpp"
#include "addr6.hpp"

namespace IP
{
    class Route6 : public virtual Route, public virtual Addr6
    {
        private:
        public:
            void printAll(std::ostream & os) const;

            /* Constructors */
            Route6(const std::string & addr, const uint32_t plen,
                   const MaskType type = PLEN, const int metric = 0);
            Route6(const std::string & addr, const in6_addr & mask,
                   const MaskType type = SNMASK, const int metric = 0);
            Route6(const std::string & addr, const std::string & mask,
                   const int metric = 0);
            Route6(const in6_addr & addr, const uint32_t plen,
                   const MaskType type = PLEN, const int metric = 0);
            Route6(const in6_addr & addr, const in6_addr & mask,
                   const MaskType type = SNMASK, const int metric = 0);
            Route6(const in6_addr & addr, const std::string & mask,
                   const int metric = 0);

            /* Destructor */
            virtual ~Route6() {};
    };
}

#endif /* IPROUTE6_H */
