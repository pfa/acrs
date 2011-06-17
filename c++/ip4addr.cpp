/* ip4addr.cpp - IP address utility
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

/* Stores integers in NBO */

#include <arpa/inet.h>
#include <inttypes.h>
#include <cstring>
#include <iostream>

#include "ip4addr.hpp"

namespace IP4Addr
{
        hash_t IP4Addr::makeHash(in_addr_t addr, plen_t plen)
        {
                return ((addr << 31) + plen);
        }

	hash_t IP4Addr::makeHash(in_addr_t addr, in_addr_t snmask)
	{
		/* Since the hash is a network address + prefix length,
		 * if we are given the subnet mask we must first convert it
		 * to a prefix length. */
		plen_t plen = smtopl(snmask);

		/* Call the other hash function */
		return makeHash(addr, plen);
	}
	
	/* valid_snmask_i
	 * Make sure all the bits flipped on are to the 'left' side of the
	 * number.  Otherwise it's not a subnet mask.
	 */
	bool IP4Addr::valid_snmask_i(in_addr_t snmask) const
	{
		char maskchk[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
		char * chkptr = maskchk;
		uint8_t * octet;
		bool hostbits = false;
	
		//snmask = htonl(snmask);
	 	octet = (uint8_t *) &snmask;
	
		/* For every octet, check if the mask is ok */
		for (int i = 0; i < 4; i++, octet++)
		{
			for (chkptr = maskchk; chkptr <= &maskchk[7]; chkptr++)
			{
				if (! hostbits)
				{
					if (*chkptr & *octet)
					{
						continue;
					}
					else
					{
						hostbits++;
					}
				}
				else
				{
					if (*chkptr & *octet)
					{
						/* Bad mask */
						return false;
					}
					else
					{
						continue;
					}
				}
			}
		}

		return true;
	}
	
	/* pltosn
	 *
	 * Prefix length to subnet mask.
	 */
	in_addr_t IP4Addr::pltosm(plen_t plen) const
	{
		uint32_t highbit = 1 << 31;  /* Leftmost bit flipped on */

		/* The following won't work with a prefix length of 0,
		 * so handle that now */
		if (plen == 0)
		{
			return 0;
		}

		/* If the prefix length was greater than the max,
		 * reduce it to 32. */
		if (plen > 32)
		{
			plen = 32;
		}

		/*
	 	 * 1. Zero out MASK, then set MASK's highest order bit equal to one
	 	 * 2. Right shift MASK by one less than the PREFIX LENGTH
	 	 * 3. Subtract 1 from MASK; now all the bits we want are flipped to 0
	 	 * 4. Get the one's complement of MASK (flip all 0s to 1s, vice versa)
	 	 * 5. MASK is now in network byte order; convert it to host order
	 	 */
		return ntohl(~((highbit >> plen - 1) - 1));
	}

	/* smtopl
	 * Subnet mask (as a host order integer) to prefix length conversion.
	 */ 
	plen_t IP4Addr::smtopl(in_addr_t snmask)
	{
		uint8_t * cur = (uint8_t *) &snmask;
		plen_t plen = 0;
	
		/* Convert to network byte order */
		//snmask = htonl(snmask);
	
		/* Check # of bits on in each octet */
		for (int i = 0; i < 4; i++, cur++) {
			if (*cur == 255) {
				/* All bits were on */
				plen += 8;
				continue;
			} else {
				/* Find consecutive leftmost bits on within this byte */
				char bits[] = { 128, 64, 32, 16, 8, 4, 2, 1 };
				char * bptr = &bits[0];
				char * startbit = &bits[0];
	
				while (*cur & *bptr)
					bptr++;
				plen += (bptr - startbit);
				break;
			}
		}
	
		return plen;
	}
	
	/* Get functions */
	
	in_addr_t IP4Addr::getAddr(char * buf)
	{
		if (! isValid())
		{
			return 0;
		}

		if (buf != NULL)
		{
			strncpy(buf, m_addr_s, INET_ADDRSTRLEN);
		}

		return m_addr_i;
	}

	in_addr_t IP4Addr::getSnmask(char * buf)
	{
		if (! isValid())
		{
			return NULL;
		}

		if (buf != NULL)
		{
			strncpy(buf, m_snmask_s, INET_ADDRSTRLEN);
		}

		return m_snmask_i;
	}

	hash_t IP4Addr::getHash()
	{
		if (! isValid())
		{
			return 0;
		}

		return m_hash;
	}

	plen_t IP4Addr::getPlen()
	{
		if (! isValid())
		{
			return 0;
		}

		return m_plen;
	}

	bool IP4Addr::isValid()
	{
		return (addrValid && maskValid);
	}

	/* Gives both the string and integer (NBO) representation of a
	 * broadcast address. String goes in the buf if it is not NULL.
	 * Integer is returned normally.
	 */
	in_addr_t IP4Addr::getBroadcast(char * buf)
	{
		in_addr_t bc;

		if (! isValid())
		{
			return 0;
		}

		bc = m_addr_i | getHostmask(NULL);

		if (buf != NULL)
		{
			int hboBc = ntohl(bc);
			inet_ntop(AF_INET, &hboBc, buf, INET_ADDRSTRLEN);
		}

		return bc;
	}

	in_addr_t IP4Addr::getHostmask(char * buf)
	{
		in_addr_t hmask;

		if (! isValid())
		{
			return 0;
		}

		hmask = ~ getSnmask(0);

		if (buf != NULL)
		{
			int hboHmask = ntohl(hmask);
			inet_ntop(AF_INET, &hboHmask, buf, INET_ADDRSTRLEN);
		}

		return hmask;
	}

	in_addr_t IP4Addr::getNetwork(char * buf)
	{
		in_addr_t net;

		if (! isValid())
		{
			return 0;
		}

		net = m_snmask_i & m_addr_i;

		if (buf != NULL)
		{
			int hboNet = ntohl(net);
			inet_ntop(AF_INET, &hboNet, buf, INET_ADDRSTRLEN);
		}

		return net;
	}

	bool IP4Addr::setAddr_s(char * addr_s)
	{
		if (! addr_s)
		{
			return setAddrFail();
		}

		if (! inet_pton(AF_INET, addr_s, &m_addr_i))
		{
			return setAddrFail();
		}

		/* Convert address to NBO */
		m_addr_i = htonl(m_addr_i);

		strncpy(m_addr_s, addr_s, INET_ADDRSTRLEN);
		return setAddrSuccess();
	}

	bool IP4Addr::setAddr_i(in_addr_t addr_i)
	{
		if (! (inet_ntop(AF_INET, &addr_i, m_addr_s, INET_ADDRSTRLEN)))
		{
			return setAddrFail();
		}

		m_addr_i = htonl(addr_i);
		return setAddrSuccess();
	}

	bool IP4Addr::setSnmask_s(char * snmask_s)
	{
		if (! inet_pton(AF_INET, snmask_s, &m_snmask_i))
		{
			return setMaskFail();
		}

		strncpy(m_snmask_s, snmask_s, INET_ADDRSTRLEN);
		m_plen = smtopl(m_snmask_i);
		return setMaskSuccess();
	}

	bool IP4Addr::setSnmask_i(in_addr_t snmask_i)
	{
		if (! (inet_ntop(AF_INET, &snmask_i, m_snmask_s,
		       INET_ADDRSTRLEN)))
		{
			return setMaskFail();
		}

		m_plen = smtopl(snmask_i);
		m_snmask_i = htonl(snmask_i);
		return setMaskSuccess();
	}

	bool IP4Addr::setPlen(int plen)
	{
		if (plen < 0 || plen > 32)
		{
			return setMaskFail();
		}

		setSnmask_i(pltosm(plen));

		return setMaskSuccess();
	}

	bool IP4Addr::setAddrSuccess()
	{
		if (maskValid == true)
		{
			updateNetInfo();
		}

		return addrValid = true;
	}

	bool IP4Addr::setAddrFail()
	{
		return addrValid = false;
	}

	bool IP4Addr::setMaskSuccess()
	{
		if (addrValid == true)
		{
			updateNetInfo();
		}

		return maskValid = true;
	}

	bool IP4Addr::setMaskFail()
	{
		return maskValid = false;
	}

	/* Compute any members that depend on the mask and address */
	void IP4Addr::updateNetInfo()
	{
		makeHash(m_addr_i, m_snmask_i);
		return;
	}

	/* CTORs -- as above */
	IP4Addr::IP4Addr(void)
	{
		setAddrFail();
		setMaskFail();
	}

	IP4Addr::IP4Addr(char * addr_s, char * snmask_s)
	{
		setAddr_s(addr_s);
		setSnmask_s(snmask_s);
	}

	IP4Addr::IP4Addr(in_addr_t addr_i, in_addr_t snmask_i)
	{
		setAddr_i(addr_i);
		setSnmask_i(snmask_i);
	}

	IP4Addr::IP4Addr(char * addr_s, plen_t plen)
	{
		setAddr_s(addr_s);
		setPlen(plen);
	}

	IP4Addr::IP4Addr(in_addr_t addr_i, plen_t plen)
	{
		setAddr_i(addr_i);
		setPlen(plen);
	}

	/* Other */

	std::ostream & operator<<(std::ostream & os, IP4Addr & ip)
	{
		char addr[INET_ADDRSTRLEN];

		if (ip.isValid() == false)
		{
			os << "IP and/or mask is not valid." << std::endl;
			return os;
		}

		ip.getAddr(addr);

		os << "IP Address:    " << addr << " (" << ip.getAddr(NULL)
		   << ")" << std::endl;

		os << "Prefix length: " << (int) ip.getPlen() << std::endl;

		ip.getSnmask(addr);

		os << "Subnet Mask:   " << addr << " (" << ip.getSnmask(NULL)
		   << ")" << std::endl;

		ip.getBroadcast(addr);

		os << "Broadcast:     " << addr << " (" << ip.getBroadcast(NULL)
		   << ")" << std::endl;

		ip.getNetwork(addr);

		os << "Network:       " << addr << " (" << ip.getNetwork(NULL)
		   << ")" << std::endl;

		ip.getHostmask(addr);

		os << "Hostmask:      " << addr << " (" << ip.getHostmask(NULL)
		   << ")" << std::endl;

		return os;
	}

	/* with_snmask
	 * Return the network ANDed with a new network mask.
	 */
	in_addr_t IP4Addr::withSnmask(in_addr_t snmask_i) const
	{
		if (! valid_snmask_i(snmask_i)) {
			return 0;
		}
	
		return m_addr_i & snmask_i;
	}
	
	/* with_snmask
	 * Return the network ANDed with a new network mask.
	 */
	in_addr_t IP4Addr::withSnmask(plen_t plen) const
	{
		if (plen > 32) {
			plen = 32;
		}
	
		in_addr_t snmask = pltosm(plen);
	
		return withSnmask(snmask);
	}
};
