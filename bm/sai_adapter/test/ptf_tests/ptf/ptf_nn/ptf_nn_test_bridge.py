#!/usr/bin/env python

# Copyright 2013-present Barefoot Networks, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# Antonin Bas (antonin@barefootnetworks.com)
#
#

import argparse
import threading
import scapy.all as sc
import time

parser = argparse.ArgumentParser(description='PTF Nanomsg tester bridge')
parser.add_argument("-ifrom", type=str)
parser.add_argument("-ito", type=str)
args = parser.parse_args()

forwarders = {}

class Forwarder(threading.Thread):
    def __init__(self, iface_name, other):
        threading.Thread.__init__(self)
        self.daemon = True
        self.iface_name = iface_name
        self.other = other
        forwarders[iface_name] = self

    def forward(self, p):
        print "forwarding", p, "---", self.other, "->", self.iface_name
        sc.sendp(p, iface=self.iface_name, verbose=0)

    def run(self):
        other_fwd = forwarders[self.other]
        sc.sniff(iface=self.iface_name, prn=lambda x: other_fwd.forward(str(x)))

def main():
    f1 = Forwarder(args.ifrom, args.ito)
    f2 = Forwarder(args.ito, args.ifrom)
    time.sleep(2)
    f1.start()
    print "READY"
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        return

if __name__ == '__main__':
    main()
