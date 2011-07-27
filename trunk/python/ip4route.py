# IP version 4 route class
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

import ip4addr

class IP4Route(ip4addr.IP4Addr):
    MAX_METRIC = 65535

    def __init__(self, addr, mask = 32, metric = 0):
        super(self.__class__, self).__init__(addr, mask)
        self.setMetric(metric)

    def getMetric(self):
        if (self.isValid() == False):
            return None
        else:
            return self._metric

    def setMetric(self, metric):
        if (not self.isValidMetric(metric)):
            return self._setMetricFail()

        self._metric = metric
        return self._setMetricSuccess()

    @staticmethod
    def isValidMetric(metric):
        if (not (isinstance(metric, int) or isinstance(metric, long))):
            return False

        if (metric < 0 or metric > IP4Route.MAX_METRIC):
            return False

        return True

    def isValid(self):
        return (super(self.__class__, self).isValid() and self._metric_valid)

    def _setMetricFail(self):
        self._metric_valid = False
        return False

    def _setMetricSuccess(self):
        self._metric_valid = True
        return True

    def __str__(self):
        if (self.isValid() == False):
            return "Route not valid."

        return (super(self.__class__, self).__str__() + " in %s" %
                self.getMetric())

    def printAll(self):
        if (self.isValid() == False):
            print "Valid: False"
            return
        else:
            super(self.__class__, self).printAll()
            print "Metric:", self.getMetric()
