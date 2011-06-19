#!/usr/bin/env python
#
# IP version 4 address class
# Copyright (C) 2011  Patrick F. Allen
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.

# This uses inet_pton, ntohl, etc, which may or may not be available in
# Windows cpython.
#
# Addresses and mask are set either using a string in dotted decimal format,
# a host order integer, or (for masks) an integer prefix length.
#
# Once an address and mask is set, use the isValid() method to check if
# everything is valid. If it returns True, you can use the get*() methods
# to retrieve other info like the host mask, prefix length, broadcast and
# network addresses, etc., in both string and network order integer form.
#
# Each get*() method returns a tuple. The first item is
# a string in dotted decimal format, and the second is a *network* order
# integer. (Don't be thrown off that you set it in host order and retrieve
# in network order. This is the most useful way to do it for network
# programming. If you want to use values from one IP4Addr to make a new
# IP4Addr, use the string item in the tuple.)

from _struct import pack
from socket import ntohl, htonl, inet_ntop, inet_pton, AF_INET, error

class IP4Addr:
    def getAddr(self):
        if (self.isValid() == False):
            return None, None

        #hboaddr = ntohl(self._addr_i)

        return self._addr_s, self._addr_i

    def getMask(self, buf = None):
        if (self.isValid() == False):
            return None, None

        return self._mask_s, self._mask_i

    def getBroadcast(self):
        if (self.isValid() == False):
            return None, None

        bc_i = self._addr_i | self.getHostmask()[1]
        bc_s = ntohl(bc_i)
        bc_s = inet_ntop(AF_INET, pack("L", bc_s))

        return bc_s, bc_i

    def getHostmask(self):
        if (self.isValid() == False):
            return None, None

        # AND with 0xFFFFFFFF in case of different length integers
        hmask_i = ~ self.getMask()[1] & 0xFFFFFFFF
        hmask_s = ntohl(hmask_i)
        hmask_s = inet_ntop(AF_INET, pack("L", hmask_s))

        return hmask_s, hmask_i

    def getNetwork(self):
        if (self.isValid() == False):
            return None, None

        net_i = self._mask_i & self._addr_i
        net_s = ntohl(net_i)
        net_s = inet_ntop(AF_INET, pack("L", net_s))

        return net_s, net_i

    def getPlen(self):
        if (self.isValid() == False):
            return None

        return self._plen

    @staticmethod
    def add_octets(addrstr):
        addrbytes = bytearray(addrstr)
        return (addrbytes[0] << 24) + \
               (addrbytes[1] << 16) + \
               (addrbytes[2] << 8)  + \
               (addrbytes[3])

    # Mask may be either a subnet mask (in dotted decimal form as a string,
    # or a packed 32 bit address), or a prefix length.
    #
    # We'll determine if it's a mask or prefix length by attempting to use
    # it as a subnet mask first; if that fails, we'll try using it as a
    # prefix length.
    #
    # So, the following are all valid and equivalent:
    #    IP4Addr("192.168.0.1", 24)
    #    IP4Addr("192.168.0.1", "255.255.255.0")
    #    IP4Addr(16820416, 16777215)   # Host order integers
    #
    # If no mask is given, a 32 bit mask is assumed
    #    IP4Addr("192.168.0.1") -> Assumes 32 bit mask
    #    IP4Addr(16820416) ->  # Host order integer with assumed 32 bit mask
    # 
    def __init__(self, addr, mask = 32):
        self.setAddr(addr)
        self.setMask(mask)

    def setMask(self, mask):
        if (isinstance(mask, int) or isinstance(mask, long)):
            if (mask < 0):
                return self._setMaskFail()

            if ((IP4Addr.isValidPlen(mask)) == True):
                self._plen = mask
                mask_hostorder = IP4Addr.pltosm(self._plen)
                self._mask_s = inet_ntop(AF_INET, pack("L", mask_hostorder))
                self._mask_i = htonl(mask_hostorder)
                return self._setMaskSuccess()
            else:
                # It's not a prefix length, try using as a subnet mask
                try:
                    self._mask_s = inet_ntop(AF_INET, pack("L", mask))
                    self._mask_i = htonl(mask & 0xFFFFFFFF)
                    if (IP4Addr.isValidMask(self._mask_i) == False):
                        return self._setMaskFail()

                    self._plen = IP4Addr.smtopl(self._mask_i)
                    return self._setMaskSuccess()
                except error:
                    return self._setMaskFail()
        elif (isinstance(mask, str)):
            try:
                maskbytes = inet_pton(AF_INET, mask)
                self._mask_i = IP4Addr.add_octets(maskbytes)
                self._mask_s = mask
                self._plen = IP4Addr.smtopl(self._mask_i)
                return self._setMaskSuccess()
            except error:
                return self._setMaskFail()
        else:
            # Bad type, should throw an exception
            return self._setMaskFail()

        # Not reached in setMask()
        assert(False)

    # Subnet mask to prefix length
    @staticmethod
    def smtopl(mask):
        plen = 0

        if ((isinstance(mask, int) != True) and
            (isinstance(mask, long) != True)):
            return None

        # Load the value of each byte into a list of bytes
        maskbytes = []
        maskbytes.append((mask & 0xFF000000) >> 24)
        maskbytes.append((mask & 0x00FF0000) >> 16)
        maskbytes.append((mask & 0x0000FF00) >> 8 )
        maskbytes.append(mask & 0x000000FF)

        for maskbyte in maskbytes:
            if (maskbyte == 255):
                # All bits were on
                plen += 8
            else:
                # Find which bits are on in this octet
                bits = [ 128, 64, 32, 16, 8, 4, 2, 1 ]

                for bit in bits:
                    if (bit & maskbyte == 0):
                        break
                    plen += 1

        return plen

    # Prefix length to subnet mask in host order
    @staticmethod
    def pltosm(plen):
        if (not IP4Addr.isValidPlen(plen)):
            return None

        assert(plen <= 32 and plen >= 0)

        if (plen == 0):
            return 0

        highbit = 1 << 31    # Highest bit flipped on

        # 1.  Begin with a mask of the highest bit flipped on
        # 2.  Right shift mask by one less than the prefix length
        # 3.  Subtract 1 from mask; now all bits we want are 0
        # 4.  Get the one's complement of mask
        # 4a. AND with 0xFFFFFFFF to counter python's type system
        #     (will appear as a negative number otherwise)
        # 5.  Mask is in network byte order, convert to host order
        return ntohl(~((highbit >> plen - 1) - 1) & 0xFFFFFFFF)

    @staticmethod
    def isValidMask(mask):
        if (isinstance(mask, int)):
            mask = long(mask)

        if (not isinstance(mask, long)):
            return False

        if (mask < 0 or mask > inet_pton(AF_INET, "255.255.255.255")):
            return False

        bits = [ 128, 64, 32, 16, 8, 4, 2, 1 ]
        hostbits = False

        # For every octet, check if mask is ok
        for maskbyte in [ ord(x) for x in pack(">I", mask) ]:
            for bit in bits:
                if (hostbits == False):
                    if (maskbyte & bit):
                        continue
                    else:
                        hostbits = True
                else:
                    if (maskbyte & bit):
                        # Bad mask
                        return False
                    else:
                        continue

        return True

    @staticmethod
    def isValidPlen(plen):
        if (not isinstance(plen, int)):
            return False

        if (plen > 32 or plen < 0):
            return False

        return True

    def setAddr(self, addr):
        if (isinstance(addr, int)):
            try:
                self._addr_s = inet_ntop(AF_INET, pack("L", addr))
                self._addr_i = htonl(addr)
                return self._setAddrSuccess()
            except error:
                return self._setAddrFail()
        elif (isinstance(addr, str)):
            try:
                #addrbytes = inet_pton(AF_INET, addr)
                #self._addr_i = self.add_octets(addrbytes)
                self._addr_s = addr
                self._addr_i = inet_pton(AF_INET, self._addr_s)
                self._addr_i = IP4Addr.add_octets(self._addr_i)

                return self._setAddrSuccess()
            except error:
                return self._setAddrFail()
        else:
            # Bad type, should throw an exception
            return self._setAddrFail()

    def _setAddrSuccess(self):
        self._addr_valid = True
        return True

    def _setAddrFail(self):
        self._addr_valid = False
        return False

    def _setMaskSuccess(self):
        self._mask_valid = True
        return True

    def _setMaskFail(self):
        self._mask_valid = False
        return False

    def isValid(self):
        return self._addr_valid and self._mask_valid

    def printAll(self):
        ret = self.isValid()

        print "Valid:", self.isValid()

        if (ret == True):
	        print "Addr:", self.getAddr()
	        print "Mask:", self.getMask()
	        print "Bcast:", self.getBroadcast()
	        print "Hmask:", self.getHostmask()
	        print "Net:", self.getNetwork()
	        print "Plen:", self.getPlen()
