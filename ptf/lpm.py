import random

from ipaddress import ip_address, ip_network
from SubnetTree import SubnetTree

'''
NOTE: This module is pulled from sonic-mgmt repo

LpmDict is a class used in FIB test for LPM and IP segmentation.

This class contains a class variable SubnetTree() _subnet_tree to solve the
LPM search functionality, which is achieved using Patricia tree. In order to
have IP segmentation functionality: segment the whole IP space into different
segments from start to end according to the prefixes (networks) it reads.

Initially, the whole IP space contains only one range. After inserting
prefixes, the IP space is segmented into multiple ranges. The ranges()
function returns all ranges in the LpmDict with a list of IpIntervals. The
sub-class IpInterval then could be used to get the first/last/random IP within
this range. It could also check the length of the range and if an IP is within
this range.

To achieve the LPM functionality, use the LpmDict as a dictionary and use
[] operator to get the corresponding value using the key (IP).

Please check the test_lpm.py file to see the details of how this class works.
'''
class LpmDict():
    class IpInterval:
        def __init__(self, s):
            self._start = s
            self._end = s

        def __init__(self, s, e):
            assert s <= e
            self._start = s
            self._end = e

        # __len__ has hard limit on returning long int
        def length(self):
            return int(self._end) - int(self._start)

        def contains(self, ip):
            return ip >= self._start and ip <= self._end

        def get_first_ip(self):
            return str(self._start)

        def get_last_ip(self):
            return str(self._end)

        def get_random_ip(self):
            diff = self.length()
            return str(self._start + random.randint(0, diff))

        def __str__(self):
            return str(self._start) + ' - ' + str(self._end)

    def __init__(self, ipv4=True):
        self._ipv4 = ipv4
        self._prefix_set = set()
        self._subnet_tree = SubnetTree()
        # 0.0.0.0 is a non-routable meta-address that needs to be skipped
        self._boundaries = { ip_address(u'0.0.0.0') : 1} if ipv4 else { ip_address(u'::') : 1}

    def __setitem__(self, key, value):
        prefix = ip_network(six.ensure_text(key))
        # add the current key to self._prefix_set only when it is not the default route and it is not a duplicate key
        if prefix.prefixlen and key not in self._prefix_set:
            boundary = prefix[0]
            self._boundaries[boundary] = self._boundaries.get(boundary, 0) + 1
            if prefix[-1] != ip_address(u'255.255.255.255') and prefix[-1] != ip_address(u'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff'):
                next_boundary = prefix[-1] + 1
                self._boundaries[next_boundary] = self._boundaries.get(next_boundary, 0) + 1
            self._prefix_set.add(key)
        self._subnet_tree.__setitem__(key, value)

    def __getitem__(self, key):
        return self._subnet_tree[key]

    def __delitem__(self, key):
        if '/0' not in key:
            prefix = ip_network(six.ensure_text(key))
            boundary = prefix[0]
            next_boundary = prefix[-1] + 1
            self._boundaries[boundary] = self._boundaries.get(boundary) - 1
            if not self._boundaries[boundary]:
                del self._boundaries[boundary]
            self._boundaries[next_boundary] = self._boundaries.get(next_boundary) - 1
            if not self._boundaries[next_boundary]:
                del self._boundaries[next_boundary]
            self._prefix_set.remove(key)
        self._subnet_tree.__delitem__(key)

    def ranges(self):
        sorted_boundaries = sorted(self._boundaries.keys())
        ranges = []
        for index, boundary in enumerate(sorted_boundaries):
            if index != len(sorted_boundaries) - 1:
                interval = self.IpInterval(sorted_boundaries[index], sorted_boundaries[index + 1] - 1)
            else:
                if self._ipv4:
                    interval = self.IpInterval(sorted_boundaries[index], ip_address(u'255.255.255.255'))
                else:
                    interval = self.IpInterval(sorted_boundaries[index], ip_address(u'ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff'))
            ranges.append(interval)
        return ranges

    def contains(self, key):
        return key in self._subnet_tree
