/* addr4netform.hpp -- See XXX comments in acrs.hpp */

#ifndef IP_ADDR4NETFORM_H
#define IP_ADDR4NETFORM_H

#include <string>

#include <inttypes.h>
#include <arpa/inet.h>

#include "addrnetform.hpp"

namespace IP
{
    class Addr4NetForm : public AddrNetForm
    {
        private:
        protected:
            in_addr_t m_addr_net_form;

        public:
            in_addr_t getAddr() const { return m_addr_net_form; };
            bool setAddr(const in_addr_t & addr);

            Addr4NetForm operator&(const Addr4NetForm & other) const;
            Addr4NetForm operator|(const Addr4NetForm & other) const;
            Addr4NetForm operator~() const;
            bool operator<(const Addr4NetForm & other) const;
            bool operator>(const Addr4NetForm & other) const;
            bool operator==(const Addr4NetForm & other) const;
            bool operator!=(const Addr4NetForm & other) const;
            Addr4NetForm nbo() const;
            Addr4NetForm hbo() const;

            Addr4NetForm();
            Addr4NetForm(const in_addr_t addr);
            virtual ~Addr4NetForm();
    };
}

#endif /* IP_ADDR4NETFORM_H */
