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
import scapy.all as sc

parser = argparse.ArgumentParser(description='PTF Nanomsg tester 2')
parser.add_argument(
    "--interface", type=str, dest="interface")
parser.add_argument(
    "--receive", dest="receive", action='store_true', default=False)
args = parser.parse_args()

def receive(interface):
    def printp(p):
        print "Received:", str(p)
    sc.sniff(iface=interface, prn=lambda x: printp(x))

def main():
    if args.receive:
        receive(args.interface)
    else:  # send one
        p = "ab" * 20
        sc.sendp(p, iface=args.interface, verbose=0)

if __name__ == '__main__':
    main()
