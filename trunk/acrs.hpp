/* acrs.hpp */

#ifndef ACRS_ROUTE_H
#define ACRS_ROUTE_H

#include <iostream>
#include <list>

#include "ip4addr.hpp"

namespace IP4Addr { namespace Acrs {
	class AcrsRoute4 : public IP4Addr::IP4Addr
	{
	private:
		int m_metric;
		void init(int metric);

	public:
		AcrsRoute4(int metric);
		AcrsRoute4(in_addr_t addr, in_addr_t mask, int metric);
		AcrsRoute4(char * addr, char * mask, int metric);
		AcrsRoute4(char * addr, plen_t plen, int metric);

		friend std::ostream & operator<<(std::ostream & os,
		                                 AcrsRoute4 & rt);
		friend bool acrsCmp(AcrsRoute4& rt1, AcrsRoute4& rt2);
		friend bool Summarize(std::list<AcrsRoute4> & rtlist);
		bool setMetric(int metric);
		int getMetric() { return m_metric; };
	};

	bool acrsCmp(AcrsRoute4& rt1, AcrsRoute4& rt2);
	bool Summarize(std::list<AcrsRoute4> & rtlist);
	std::ostream & operator<<(std::ostream & os, IP4Addr::IP4Addr & ip);
} }

#endif /* ACRS_ROUTE_H */
