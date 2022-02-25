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

import nnpy
import struct
import argparse

parser = argparse.ArgumentParser(description='PTF Nanomsg tester 1')
parser.add_argument(
    "--socket", type=str, dest="socket")
parser.add_argument(
    "--receive", dest="receive", action='store_true', default=False)
args = parser.parse_args()

MSG_TYPE_PORT_ADD = 0
MSG_TYPE_PORT_REMOVE = 1
MSG_TYPE_PORT_SET_STATUS = 2
MSG_TYPE_PACKET_IN = 3
MSG_TYPE_PACKET_OUT = 4

def receive(socket):
    while True:
        msg = socket.recv()
        fmt = "<iii"
        msg_type, port_number, length = struct.unpack_from(fmt, msg)
        hdr_size = struct.calcsize(fmt)
        msg = msg[hdr_size:]
        assert (msg_type == MSG_TYPE_PACKET_OUT)
        assert (len(msg) == length)
        print "Received:", msg

def main():
    socket = nnpy.Socket(nnpy.AF_SP, nnpy.PAIR)
    socket.connect(args.socket)
    if args.receive:
        receive(socket)
    else:  # send one
        p = "ab" * 20
        port = 1
        msg = struct.pack("<iii{}s".format(len(p)), MSG_TYPE_PACKET_IN,
                          port, len(p), p)
        # because nnpy expects unicode when using str
        msg = list(msg)
        socket.send(msg)

if __name__ == '__main__':
    main()
