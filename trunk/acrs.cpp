/* acrs.cpp - Automatic classless route summarization
 *
 * Copyright 2011 Patrick Allen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

#include "acrs.hpp"
#include "ip4addr.hpp"

namespace IP4Addr { namespace Acrs {
	/* Compare two AcrsRoute4 to determine which should come first.
	 *
	 * Highest prefix length always comes first.
	 * If prefix lengths match, lowest network address comes first.
	 * If prefix lengths and network addresses match, lowest metric comes
	 * first.
	 */
	bool acrsCmp(AcrsRoute4 & rt1, AcrsRoute4 & rt2)
	{
		if (rt1.getMetric() < rt2.getMetric()) {
			return true;
		} else if (rt1.getMetric() > rt2.getMetric()) {
			return false;
		}

		/* If reached, metrics are the same */

		if (rt1.getPlen() > rt2.getPlen()) {
			return true;
		} else if (rt1.getPlen() < rt2.getPlen()) {
			return false;
		}

		/* If reached, prefix lengths are the same */

		if (rt1.getNetwork(0) < rt2.getNetwork(0)) {
			return true;
		} else {
			return false;
		}
	}

	/* Summarize a route list; return true if any summarization was
	 * done, return false otherwise.
	 */
	bool Summarize(std::list<AcrsRoute4> & rtlist)
	{
		AcrsRoute4 * prev = NULL;
		bool summarized = false;

		rtlist.sort(acrsCmp);

		for (std::list<AcrsRoute4>::iterator cur = rtlist.begin();
		     cur != rtlist.end();
		     prev = &(*cur), cur++)
		{
			if (! prev)
			{
				continue;
			}

			/* Prefix lengths must match */
			if (prev->getPlen() != cur->getPlen())
			{
				continue;
			}

			/* Can't summarize non-even subnets
			 * (happens with host routes) */
			if (prev->getNetwork(0) & 1)
			{
				continue;
			}

			/* Round addresses down to the network id if necessary.
			 * 
			 * XXX This should be done once when the route is added.
			 */
			if (prev->getAddr(0) != prev->getNetwork(0))
			{
				prev->setAddr_i(prev->getNetwork(0));
			}

			if (prev->getBroadcast(0) + 1 == cur->getNetwork(0))
			{
				/* Can summarize these */
				rtlist.erase(cur);
				cur--;
				cur->setPlen(cur->getPlen() - 1);
				summarized = true;
			}
		}

		/* If we summarized at all on this iteration, go over the
		 * list again.
		 */
		if (summarized == true)
		{
			Summarize(rtlist);
			return true;
		}
		else
		{
			return false;
		}
	}

	void AcrsRoute4::init(int metric)
	{
		m_metric = metric;
	}

	AcrsRoute4::AcrsRoute4(int metric) : IP4Addr::IP4Addr()
	{
		AcrsRoute4::init(metric);
	}

	AcrsRoute4::AcrsRoute4(in_addr_t addr, in_addr_t mask, int metric)
	                       : IP4Addr::IP4Addr(addr, mask)
	{
		AcrsRoute4::init(metric);
	}

	AcrsRoute4::AcrsRoute4(char * addr, char * mask, int metric)
	                       : IP4Addr::IP4Addr(addr, mask)
	{
		AcrsRoute4::init(metric);
	}

	AcrsRoute4::AcrsRoute4(char * addr, plen_t plen, int metric)
	                       : IP4Addr::IP4Addr(addr, plen)
	{
		AcrsRoute4::init(metric);
	}

	std::ostream & operator<<(std::ostream & os, AcrsRoute4 & rt)
	{
		char addr[INET_ADDRSTRLEN];

		rt.getAddr(addr);

		os << addr << "/" << (int) rt.getPlen() << " in "
		   << rt.getMetric();

		return os;
	}
} }
