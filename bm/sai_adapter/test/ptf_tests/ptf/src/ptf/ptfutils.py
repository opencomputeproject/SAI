
"""
Utilities for the OpenFlow test framework
"""

import random
import time
import os
import fcntl
import logging
import signal

default_timeout = None # set by ptf
default_negative_timeout = None # set by ptf

def gen_xid():
    return random.randrange(1,0xffffffff)

"""
Wait on a condition variable until the given function returns non-None or a timeout expires.
The condition variable must already be acquired.
The timeout value -1 means use the default timeout.
There is deliberately no support for an infinite timeout.
"""
def timed_wait(cv, fn, timeout=-1):
    if timeout == -1:
        timeout = default_timeout

    end_time = time.time() + timeout
    while True:
        val = fn()
        if val != None:
            return val

        remaining_time = end_time - time.time()
        cv.wait(remaining_time)

        if time.time() > end_time:
            return None

class EventDescriptor():
    """
    Similar to a condition variable, but can be passed to select().
    Only supports one waiter.
    """

    def __init__(self):
        self.pipe_rd, self.pipe_wr = os.pipe()
        fcntl.fcntl(self.pipe_wr, fcntl.F_SETFL, os.O_NONBLOCK)

    def __del__(self):
        os.close(self.pipe_rd)
        os.close(self.pipe_wr)

    def notify(self):
        try:
            os.write(self.pipe_wr, "x")
        except OSError as e:
            logging.warn("Failed to notify EventDescriptor: %s", e)

    def wait(self):
        os.read(self.pipe_rd, 1)

    def fileno(self):
        return self.pipe_rd

# inspired from http://stackoverflow.com/questions/8464391/what-should-i-do-if-socket-setdefaulttimeout-is-not-working
class Timeout():
    """Timeout class using ALARM signal"""

    class TimeoutError(Exception):
        pass

    def __init__(self, sec):
        try:
            from signal import alarm
            self.supported = True
        except ImportError:
            logging.warn("Your platform does not support alarm signals, "
                         "the Timeout feature is therefore not supported")
            self.supported = False
            return
        self.sec = sec
        if sec > 0:
            self.valid = True
        else:
            self.valid = False
            logging.warn("Invalid timeout requested")

    def __enter__(self):
        if not self.supported or not self.valid:
            return
        signal.signal(signal.SIGALRM, self.raise_timeout)
        signal.alarm(self.sec)

    def __exit__(self, *args):
        if not self.supported or not self.valid:
            return
        signal.alarm(0) # disable alarm

    def raise_timeout(self, *args):
        raise Timeout.TimeoutError()
