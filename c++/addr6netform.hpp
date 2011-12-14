/* addr6netform.hpp */

#ifndef IP_ADDR6NETFORM
#define IP_ADDR6NETFORM

#include <string>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "addrnetform.hpp"

namespace IP
{
    class Addr6NetForm : public AddrNetForm
    {
        private:
        protected:
            in6_addr m_addr_net_form;

        public:
            in6_addr getAddr() const { return m_addr_net_form; };
            bool setAddr(const in6_addr & addr);

            Addr6NetForm operator&(const Addr6NetForm & other) const;
            Addr6NetForm operator|(const Addr6NetForm & other) const;
            Addr6NetForm operator~() const;
            bool operator<(const Addr6NetForm & other) const;
            bool operator>(const Addr6NetForm & other) const;
            bool operator==(const Addr6NetForm & other) const;
            Addr6NetForm nbo() const;
            Addr6NetForm hbo() const;

            Addr6NetForm();
            Addr6NetForm(const in6_addr & addr);
            virtual ~Addr6NetForm();
    };
}

#endif /* IP_ADDR6NETFORM */
