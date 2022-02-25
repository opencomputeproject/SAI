'''Wrapper for sai.h

Generated with:
/usr/local/bin/ctypesgen.py ../../../inc/sai.h ../../../inc/saiacl.h ../../../inc/saibridge.h ../../../inc/saibuffer.h ../../../inc/saifdb.h ../../../inc/saihash.h ../../../inc/saihostif.h ../../../inc/saiipmc.h ../../../inc/saiipmcgroup.h ../../../inc/sail2mc.h ../../../inc/sail2mcgroup.h ../../../inc/sailag.h ../../../inc/saimcastfdb.h ../../../inc/saimirror.h ../../../inc/saineighbor.h ../../../inc/sainexthop.h ../../../inc/sainexthopgroup.h ../../../inc/saiobject.h ../../../inc/saipolicer.h ../../../inc/saiport.h ../../../inc/saiqosmap.h ../../../inc/saiqueue.h ../../../inc/sairoute.h ../../../inc/sairouterinterface.h ../../../inc/sairpfgroup.h ../../../inc/saisamplepacket.h ../../../inc/saischeduler.h ../../../inc/saischedulergroup.h ../../../inc/saistatus.h ../../../inc/saistp.h ../../../inc/saiswitch.h ../../../inc/saitunnel.h ../../../inc/saitypes.h ../../../inc/saiudf.h ../../../inc/saivirtualrouter.h ../../../inc/saivlan.h ../../../inc/saiwred.h -o sai_types.py -I../../../inc

Do not modify this file.
'''

__docformat__ =  'restructuredtext'

# Begin preamble

import ctypes, os, sys
from ctypes import *

_int_types = (c_int16, c_int32)
if hasattr(ctypes, 'c_int64'):
    # Some builds of ctypes apparently do not have c_int64
    # defined; it's a pretty good bet that these builds do not
    # have 64-bit pointers.
    _int_types += (c_int64,)
for t in _int_types:
    if sizeof(t) == sizeof(c_size_t):
        c_ptrdiff_t = t
del t
del _int_types

class c_void(Structure):
    # c_void_p is a buggy return type, converting to int, so
    # POINTER(None) == c_void_p is actually written as
    # POINTER(c_void), so it can be treated as a real pointer.
    _fields_ = [('dummy', c_int)]

def POINTER(obj):
    p = ctypes.POINTER(obj)

    # Convert None to a real NULL pointer to work around bugs
    # in how ctypes handles None on 64-bit platforms
    if not isinstance(p.from_param, classmethod):
        def from_param(cls, x):
            if x is None:
                return cls()
            else:
                return x
        p.from_param = classmethod(from_param)

    return p

class UserString:
    def __init__(self, seq):
        if isinstance(seq, basestring):
            self.data = seq
        elif isinstance(seq, UserString):
            self.data = seq.data[:]
        else:
            self.data = str(seq)
    def __str__(self): return str(self.data)
    def __repr__(self): return repr(self.data)
    def __int__(self): return int(self.data)
    def __long__(self): return long(self.data)
    def __float__(self): return float(self.data)
    def __complex__(self): return complex(self.data)
    def __hash__(self): return hash(self.data)

    def __cmp__(self, string):
        if isinstance(string, UserString):
            return cmp(self.data, string.data)
        else:
            return cmp(self.data, string)
    def __contains__(self, char):
        return char in self.data

    def __len__(self): return len(self.data)
    def __getitem__(self, index): return self.__class__(self.data[index])
    def __getslice__(self, start, end):
        start = max(start, 0); end = max(end, 0)
        return self.__class__(self.data[start:end])

    def __add__(self, other):
        if isinstance(other, UserString):
            return self.__class__(self.data + other.data)
        elif isinstance(other, basestring):
            return self.__class__(self.data + other)
        else:
            return self.__class__(self.data + str(other))
    def __radd__(self, other):
        if isinstance(other, basestring):
            return self.__class__(other + self.data)
        else:
            return self.__class__(str(other) + self.data)
    def __mul__(self, n):
        return self.__class__(self.data*n)
    __rmul__ = __mul__
    def __mod__(self, args):
        return self.__class__(self.data % args)

    # the following methods are defined in alphabetical order:
    def capitalize(self): return self.__class__(self.data.capitalize())
    def center(self, width, *args):
        return self.__class__(self.data.center(width, *args))
    def count(self, sub, start=0, end=sys.maxint):
        return self.data.count(sub, start, end)
    def decode(self, encoding=None, errors=None): # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.decode(encoding, errors))
            else:
                return self.__class__(self.data.decode(encoding))
        else:
            return self.__class__(self.data.decode())
    def encode(self, encoding=None, errors=None): # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.encode(encoding, errors))
            else:
                return self.__class__(self.data.encode(encoding))
        else:
            return self.__class__(self.data.encode())
    def endswith(self, suffix, start=0, end=sys.maxint):
        return self.data.endswith(suffix, start, end)
    def expandtabs(self, tabsize=8):
        return self.__class__(self.data.expandtabs(tabsize))
    def find(self, sub, start=0, end=sys.maxint):
        return self.data.find(sub, start, end)
    def index(self, sub, start=0, end=sys.maxint):
        return self.data.index(sub, start, end)
    def isalpha(self): return self.data.isalpha()
    def isalnum(self): return self.data.isalnum()
    def isdecimal(self): return self.data.isdecimal()
    def isdigit(self): return self.data.isdigit()
    def islower(self): return self.data.islower()
    def isnumeric(self): return self.data.isnumeric()
    def isspace(self): return self.data.isspace()
    def istitle(self): return self.data.istitle()
    def isupper(self): return self.data.isupper()
    def join(self, seq): return self.data.join(seq)
    def ljust(self, width, *args):
        return self.__class__(self.data.ljust(width, *args))
    def lower(self): return self.__class__(self.data.lower())
    def lstrip(self, chars=None): return self.__class__(self.data.lstrip(chars))
    def partition(self, sep):
        return self.data.partition(sep)
    def replace(self, old, new, maxsplit=-1):
        return self.__class__(self.data.replace(old, new, maxsplit))
    def rfind(self, sub, start=0, end=sys.maxint):
        return self.data.rfind(sub, start, end)
    def rindex(self, sub, start=0, end=sys.maxint):
        return self.data.rindex(sub, start, end)
    def rjust(self, width, *args):
        return self.__class__(self.data.rjust(width, *args))
    def rpartition(self, sep):
        return self.data.rpartition(sep)
    def rstrip(self, chars=None): return self.__class__(self.data.rstrip(chars))
    def split(self, sep=None, maxsplit=-1):
        return self.data.split(sep, maxsplit)
    def rsplit(self, sep=None, maxsplit=-1):
        return self.data.rsplit(sep, maxsplit)
    def splitlines(self, keepends=0): return self.data.splitlines(keepends)
    def startswith(self, prefix, start=0, end=sys.maxint):
        return self.data.startswith(prefix, start, end)
    def strip(self, chars=None): return self.__class__(self.data.strip(chars))
    def swapcase(self): return self.__class__(self.data.swapcase())
    def title(self): return self.__class__(self.data.title())
    def translate(self, *args):
        return self.__class__(self.data.translate(*args))
    def upper(self): return self.__class__(self.data.upper())
    def zfill(self, width): return self.__class__(self.data.zfill(width))

class MutableString(UserString):
    """mutable string objects

    Python strings are immutable objects.  This has the advantage, that
    strings may be used as dictionary keys.  If this property isn't needed
    and you insist on changing string values in place instead, you may cheat
    and use MutableString.

    But the purpose of this class is an educational one: to prevent
    people from inventing their own mutable string class derived
    from UserString and than forget thereby to remove (override) the
    __hash__ method inherited from UserString.  This would lead to
    errors that would be very hard to track down.

    A faster and better solution is to rewrite your program using lists."""
    def __init__(self, string=""):
        self.data = string
    def __hash__(self):
        raise TypeError("unhashable type (it is mutable)")
    def __setitem__(self, index, sub):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data): raise IndexError
        self.data = self.data[:index] + sub + self.data[index+1:]
    def __delitem__(self, index):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data): raise IndexError
        self.data = self.data[:index] + self.data[index+1:]
    def __setslice__(self, start, end, sub):
        start = max(start, 0); end = max(end, 0)
        if isinstance(sub, UserString):
            self.data = self.data[:start]+sub.data+self.data[end:]
        elif isinstance(sub, basestring):
            self.data = self.data[:start]+sub+self.data[end:]
        else:
            self.data =  self.data[:start]+str(sub)+self.data[end:]
    def __delslice__(self, start, end):
        start = max(start, 0); end = max(end, 0)
        self.data = self.data[:start] + self.data[end:]
    def immutable(self):
        return UserString(self.data)
    def __iadd__(self, other):
        if isinstance(other, UserString):
            self.data += other.data
        elif isinstance(other, basestring):
            self.data += other
        else:
            self.data += str(other)
        return self
    def __imul__(self, n):
        self.data *= n
        return self

class String(MutableString, Union):

    _fields_ = [('raw', POINTER(c_char)),
                ('data', c_char_p)]

    def __init__(self, obj=""):
        if isinstance(obj, (str, unicode, UserString)):
            self.data = str(obj)
        else:
            self.raw = obj

    def __len__(self):
        return self.data and len(self.data) or 0

    def from_param(cls, obj):
        # Convert None or 0
        if obj is None or obj == 0:
            return cls(POINTER(c_char)())

        # Convert from String
        elif isinstance(obj, String):
            return obj

        # Convert from str
        elif isinstance(obj, str):
            return cls(obj)

        # Convert from c_char_p
        elif isinstance(obj, c_char_p):
            return obj

        # Convert from POINTER(c_char)
        elif isinstance(obj, POINTER(c_char)):
            return obj

        # Convert from raw pointer
        elif isinstance(obj, int):
            return cls(cast(obj, POINTER(c_char)))

        # Convert from object
        else:
            return String.from_param(obj._as_parameter_)
    from_param = classmethod(from_param)

def ReturnString(obj, func=None, arguments=None):
    return String.from_param(obj)

# As of ctypes 1.0, ctypes does not support custom error-checking
# functions on callbacks, nor does it support custom datatypes on
# callbacks, so we must ensure that all callbacks return
# primitive datatypes.
#
# Non-primitive return values wrapped with UNCHECKED won't be
# typechecked, and will be converted to c_void_p.
def UNCHECKED(type):
    if (hasattr(type, "_type_") and isinstance(type._type_, str)
        and type._type_ != "P"):
        return type
    else:
        return c_void_p

# ctypes doesn't have direct support for variadic functions, so we have to write
# our own wrapper class
class _variadic_function(object):
    def __init__(self,func,restype,argtypes):
        self.func=func
        self.func.restype=restype
        self.argtypes=argtypes
    def _as_parameter_(self):
        # So we can pass this variadic function as a function pointer
        return self.func
    def __call__(self,*args):
        fixed_args=[]
        i=0
        for argtype in self.argtypes:
            # Typecheck what we can
            fixed_args.append(argtype.from_param(args[i]))
            i+=1
        return self.func(*fixed_args+list(args[i:]))

# End preamble

_libs = {}
_libdirs = []

# Begin loader

# ----------------------------------------------------------------------------
# Copyright (c) 2008 David James
# Copyright (c) 2006-2008 Alex Holkner
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#  * Neither the name of pyglet nor the names of its
#    contributors may be used to endorse or promote products
#    derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

import os.path, re, sys, glob
import ctypes
import ctypes.util

def _environ_path(name):
    if name in os.environ:
        return os.environ[name].split(":")
    else:
        return []

class LibraryLoader(object):
    def __init__(self):
        self.other_dirs=[]

    def load_library(self,libname):
        """Given the name of a library, load it."""
        paths = self.getpaths(libname)

        for path in paths:
            if os.path.exists(path):
                return self.load(path)

        raise ImportError("%s not found." % libname)

    def load(self,path):
        """Given a path to a library, load it."""
        try:
            # Darwin requires dlopen to be called with mode RTLD_GLOBAL instead
            # of the default RTLD_LOCAL.  Without this, you end up with
            # libraries not being loadable, resulting in "Symbol not found"
            # errors
            if sys.platform == 'darwin':
                return ctypes.CDLL(path, ctypes.RTLD_GLOBAL)
            else:
                return ctypes.cdll.LoadLibrary(path)
        except OSError,e:
            raise ImportError(e)

    def getpaths(self,libname):
        """Return a list of paths where the library might be found."""
        if os.path.isabs(libname):
            yield libname

        else:
            for path in self.getplatformpaths(libname):
                yield path

            path = ctypes.util.find_library(libname)
            if path: yield path

    def getplatformpaths(self, libname):
        return []

# Darwin (Mac OS X)

class DarwinLibraryLoader(LibraryLoader):
    name_formats = ["lib%s.dylib", "lib%s.so", "lib%s.bundle", "%s.dylib",
                "%s.so", "%s.bundle", "%s"]

    def getplatformpaths(self,libname):
        if os.path.pathsep in libname:
            names = [libname]
        else:
            names = [format % libname for format in self.name_formats]

        for dir in self.getdirs(libname):
            for name in names:
                yield os.path.join(dir,name)

    def getdirs(self,libname):
        '''Implements the dylib search as specified in Apple documentation:

        http://developer.apple.com/documentation/DeveloperTools/Conceptual/
            DynamicLibraries/Articles/DynamicLibraryUsageGuidelines.html

        Before commencing the standard search, the method first checks
        the bundle's ``Frameworks`` directory if the application is running
        within a bundle (OS X .app).
        '''

        dyld_fallback_library_path = _environ_path("DYLD_FALLBACK_LIBRARY_PATH")
        if not dyld_fallback_library_path:
            dyld_fallback_library_path = [os.path.expanduser('~/lib'),
                                          '/usr/local/lib', '/usr/lib']

        dirs = []

        if '/' in libname:
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))
        else:
            dirs.extend(_environ_path("LD_LIBRARY_PATH"))
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))

        dirs.extend(self.other_dirs)
        dirs.append(".")

        if hasattr(sys, 'frozen') and sys.frozen == 'macosx_app':
            dirs.append(os.path.join(
                os.environ['RESOURCEPATH'],
                '..',
                'Frameworks'))

        dirs.extend(dyld_fallback_library_path)

        return dirs

# Posix

class PosixLibraryLoader(LibraryLoader):
    _ld_so_cache = None

    def _create_ld_so_cache(self):
        # Recreate search path followed by ld.so.  This is going to be
        # slow to build, and incorrect (ld.so uses ld.so.cache, which may
        # not be up-to-date).  Used only as fallback for distros without
        # /sbin/ldconfig.
        #
        # We assume the DT_RPATH and DT_RUNPATH binary sections are omitted.

        directories = []
        for name in ("LD_LIBRARY_PATH",
                     "SHLIB_PATH", # HPUX
                     "LIBPATH", # OS/2, AIX
                     "LIBRARY_PATH", # BE/OS
                    ):
            if name in os.environ:
                directories.extend(os.environ[name].split(os.pathsep))
        directories.extend(self.other_dirs)
        directories.append(".")

        try: directories.extend([dir.strip() for dir in open('/etc/ld.so.conf')])
        except IOError: pass

        directories.extend(['/lib', '/usr/lib', '/lib64', '/usr/lib64'])

        cache = {}
        lib_re = re.compile(r'lib(.*)\.s[ol]')
        ext_re = re.compile(r'\.s[ol]$')
        for dir in directories:
            try:
                for path in glob.glob("%s/*.s[ol]*" % dir):
                    file = os.path.basename(path)

                    # Index by filename
                    if file not in cache:
                        cache[file] = path

                    # Index by library name
                    match = lib_re.match(file)
                    if match:
                        library = match.group(1)
                        if library not in cache:
                            cache[library] = path
            except OSError:
                pass

        self._ld_so_cache = cache

    def getplatformpaths(self, libname):
        if self._ld_so_cache is None:
            self._create_ld_so_cache()

        result = self._ld_so_cache.get(libname)
        if result: yield result

        path = ctypes.util.find_library(libname)
        if path: yield os.path.join("/lib",path)

# Windows

class _WindowsLibrary(object):
    def __init__(self, path):
        self.cdll = ctypes.cdll.LoadLibrary(path)
        self.windll = ctypes.windll.LoadLibrary(path)

    def __getattr__(self, name):
        try: return getattr(self.cdll,name)
        except AttributeError:
            try: return getattr(self.windll,name)
            except AttributeError:
                raise

class WindowsLibraryLoader(LibraryLoader):
    name_formats = ["%s.dll", "lib%s.dll", "%slib.dll"]

    def load_library(self, libname):
        try:
            result = LibraryLoader.load_library(self, libname)
        except ImportError:
            result = None
            if os.path.sep not in libname:
                for name in self.name_formats:
                    try:
                        result = getattr(ctypes.cdll, name % libname)
                        if result:
                            break
                    except WindowsError:
                        result = None
            if result is None:
                try:
                    result = getattr(ctypes.cdll, libname)
                except WindowsError:
                    result = None
            if result is None:
                raise ImportError("%s not found." % libname)
        return result

    def load(self, path):
        return _WindowsLibrary(path)

    def getplatformpaths(self, libname):
        if os.path.sep not in libname:
            for name in self.name_formats:
                dll_in_current_dir = os.path.abspath(name % libname)
                if os.path.exists(dll_in_current_dir):
                    yield dll_in_current_dir
                path = ctypes.util.find_library(name % libname)
                if path:
                    yield path

# Platform switching

# If your value of sys.platform does not appear in this dict, please contact
# the Ctypesgen maintainers.

loaderclass = {
    "darwin":   DarwinLibraryLoader,
    "cygwin":   WindowsLibraryLoader,
    "win32":    WindowsLibraryLoader
}

loader = loaderclass.get(sys.platform, PosixLibraryLoader)()

def add_library_search_dirs(other_dirs):
    loader.other_dirs = other_dirs

load_library = loader.load_library

del loaderclass

# End loader

add_library_search_dirs([])

# No libraries

# No modules

sai_status_t = c_int32 # /home/omer/P4/SAI/inc/saitypes.h: 84

sai_switch_profile_id_t = c_uint32 # /home/omer/P4/SAI/inc/saitypes.h: 85

sai_vlan_id_t = c_uint16 # /home/omer/P4/SAI/inc/saitypes.h: 86

sai_attr_id_t = c_uint32 # /home/omer/P4/SAI/inc/saitypes.h: 87

sai_cos_t = c_uint8 # /home/omer/P4/SAI/inc/saitypes.h: 88

sai_queue_index_t = c_uint8 # /home/omer/P4/SAI/inc/saitypes.h: 89

sai_mac_t = c_uint8 * 6 # /home/omer/P4/SAI/inc/saitypes.h: 90

sai_ip4_t = c_uint32 # /home/omer/P4/SAI/inc/saitypes.h: 91

sai_ip6_t = c_uint8 * 16 # /home/omer/P4/SAI/inc/saitypes.h: 92

sai_switch_hash_seed_t = c_uint32 # /home/omer/P4/SAI/inc/saitypes.h: 93

sai_uint64_t = c_uint64 # /home/omer/P4/SAI/inc/saitypes.h: 107

sai_int64_t = c_int64 # /home/omer/P4/SAI/inc/saitypes.h: 108

sai_uint32_t = c_uint32 # /home/omer/P4/SAI/inc/saitypes.h: 109

sai_int32_t = c_int32 # /home/omer/P4/SAI/inc/saitypes.h: 110

sai_uint16_t = c_uint16 # /home/omer/P4/SAI/inc/saitypes.h: 111

sai_int16_t = c_int16 # /home/omer/P4/SAI/inc/saitypes.h: 112

sai_uint8_t = c_uint8 # /home/omer/P4/SAI/inc/saitypes.h: 113

sai_int8_t = c_int8 # /home/omer/P4/SAI/inc/saitypes.h: 114

sai_size_t = c_size_t # /home/omer/P4/SAI/inc/saitypes.h: 115

sai_object_id_t = c_uint64 # /home/omer/P4/SAI/inc/saitypes.h: 116

sai_pointer_t = POINTER(None) # /home/omer/P4/SAI/inc/saitypes.h: 117

# /home/omer/P4/SAI/inc/saitypes.h: 143
class struct__sai_object_list_t(Structure):
    pass

struct__sai_object_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_object_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(sai_object_id_t)),
]

sai_object_list_t = struct__sai_object_list_t # /home/omer/P4/SAI/inc/saitypes.h: 143

enum__sai_common_api_t = c_int # /home/omer/P4/SAI/inc/saitypes.h: 154

SAI_COMMON_API_CREATE = 0 # /home/omer/P4/SAI/inc/saitypes.h: 154

SAI_COMMON_API_REMOVE = 1 # /home/omer/P4/SAI/inc/saitypes.h: 154

SAI_COMMON_API_SET = 2 # /home/omer/P4/SAI/inc/saitypes.h: 154

SAI_COMMON_API_GET = 3 # /home/omer/P4/SAI/inc/saitypes.h: 154

SAI_COMMON_API_MAX = 4 # /home/omer/P4/SAI/inc/saitypes.h: 154

sai_common_api_t = enum__sai_common_api_t # /home/omer/P4/SAI/inc/saitypes.h: 154

enum__sai_object_type_t = c_int # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_NULL = 0 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_PORT = 1 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_LAG = 2 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_VIRTUAL_ROUTER = 3 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_NEXT_HOP = 4 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_NEXT_HOP_GROUP = 5 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ROUTER_INTERFACE = 6 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ACL_TABLE = 7 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ACL_ENTRY = 8 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ACL_COUNTER = 9 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ACL_RANGE = 10 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ACL_TABLE_GROUP = 11 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ACL_TABLE_GROUP_MEMBER = 12 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_HOSTIF = 13 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_MIRROR_SESSION = 14 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_SAMPLEPACKET = 15 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_STP = 16 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_HOSTIF_TRAP_GROUP = 17 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_POLICER = 18 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_WRED = 19 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_QOS_MAP = 20 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_QUEUE = 21 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_SCHEDULER = 22 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_SCHEDULER_GROUP = 23 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_BUFFER_POOL = 24 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_BUFFER_PROFILE = 25 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_INGRESS_PRIORITY_GROUP = 26 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_LAG_MEMBER = 27 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_HASH = 28 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_UDF = 29 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_UDF_MATCH = 30 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_UDF_GROUP = 31 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_FDB_ENTRY = 32 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_SWITCH = 33 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_HOSTIF_TRAP = 34 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_HOSTIF_TABLE_ENTRY = 35 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_NEIGHBOR_ENTRY = 36 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_ROUTE_ENTRY = 37 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_VLAN = 38 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_VLAN_MEMBER = 39 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_HOSTIF_PACKET = 40 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_TUNNEL_MAP = 41 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_TUNNEL = 42 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY = 43 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_FDB_FLUSH = 44 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_NEXT_HOP_GROUP_MEMBER = 45 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_STP_PORT = 46 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_RPF_GROUP = 47 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_RPF_GROUP_MEMBER = 48 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_L2MC_GROUP = 49 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_L2MC_GROUP_MEMBER = 50 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_IPMC_GROUP = 51 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_IPMC_GROUP_MEMBER = 52 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_L2MC_ENTRY = 53 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_IPMC_ENTRY = 54 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_MCAST_FDB_ENTRY = 55 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_HOSTIF_USER_DEFINED_TRAP = 56 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_BRIDGE = 57 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_BRIDGE_PORT = 58 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_TUNNEL_MAP_ENTRY = 59 # /home/omer/P4/SAI/inc/saitypes.h: 221

SAI_OBJECT_TYPE_MAX = 60 # /home/omer/P4/SAI/inc/saitypes.h: 221

sai_object_type_t = enum__sai_object_type_t # /home/omer/P4/SAI/inc/saitypes.h: 221

# /home/omer/P4/SAI/inc/saitypes.h: 226
class struct__sai_u8_list_t(Structure):
    pass

struct__sai_u8_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_u8_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(c_uint8)),
]

sai_u8_list_t = struct__sai_u8_list_t # /home/omer/P4/SAI/inc/saitypes.h: 226

# /home/omer/P4/SAI/inc/saitypes.h: 235
class struct__sai_s8_list_t(Structure):
    pass

struct__sai_s8_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_s8_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(c_int8)),
]

sai_s8_list_t = struct__sai_s8_list_t # /home/omer/P4/SAI/inc/saitypes.h: 235

# /home/omer/P4/SAI/inc/saitypes.h: 240
class struct__sai_u16_list_t(Structure):
    pass

struct__sai_u16_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_u16_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(c_uint16)),
]

sai_u16_list_t = struct__sai_u16_list_t # /home/omer/P4/SAI/inc/saitypes.h: 240

# /home/omer/P4/SAI/inc/saitypes.h: 245
class struct__sai_s16_list_t(Structure):
    pass

struct__sai_s16_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_s16_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(c_int16)),
]

sai_s16_list_t = struct__sai_s16_list_t # /home/omer/P4/SAI/inc/saitypes.h: 245

# /home/omer/P4/SAI/inc/saitypes.h: 250
class struct__sai_u32_list_t(Structure):
    pass

struct__sai_u32_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_u32_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(c_uint32)),
]

sai_u32_list_t = struct__sai_u32_list_t # /home/omer/P4/SAI/inc/saitypes.h: 250

# /home/omer/P4/SAI/inc/saitypes.h: 255
class struct__sai_s32_list_t(Structure):
    pass

struct__sai_s32_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_s32_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(c_int32)),
]

sai_s32_list_t = struct__sai_s32_list_t # /home/omer/P4/SAI/inc/saitypes.h: 255

# /home/omer/P4/SAI/inc/saitypes.h: 260
class struct__sai_u32_range_t(Structure):
    pass

struct__sai_u32_range_t.__slots__ = [
    'min',
    'max',
]
struct__sai_u32_range_t._fields_ = [
    ('min', c_uint32),
    ('max', c_uint32),
]

sai_u32_range_t = struct__sai_u32_range_t # /home/omer/P4/SAI/inc/saitypes.h: 260

# /home/omer/P4/SAI/inc/saitypes.h: 265
class struct__sai_s32_range_t(Structure):
    pass

struct__sai_s32_range_t.__slots__ = [
    'min',
    'max',
]
struct__sai_s32_range_t._fields_ = [
    ('min', c_int32),
    ('max', c_int32),
]

sai_s32_range_t = struct__sai_s32_range_t # /home/omer/P4/SAI/inc/saitypes.h: 265

# /home/omer/P4/SAI/inc/saitypes.h: 278
class struct__sai_vlan_list_t(Structure):
    pass

struct__sai_vlan_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_vlan_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(sai_vlan_id_t)),
]

sai_vlan_list_t = struct__sai_vlan_list_t # /home/omer/P4/SAI/inc/saitypes.h: 278

enum__sai_ip_addr_family_t = c_int # /home/omer/P4/SAI/inc/saitypes.h: 286

SAI_IP_ADDR_FAMILY_IPV4 = 0 # /home/omer/P4/SAI/inc/saitypes.h: 286

SAI_IP_ADDR_FAMILY_IPV6 = (SAI_IP_ADDR_FAMILY_IPV4 + 1) # /home/omer/P4/SAI/inc/saitypes.h: 286

sai_ip_addr_family_t = enum__sai_ip_addr_family_t # /home/omer/P4/SAI/inc/saitypes.h: 286

# /home/omer/P4/SAI/inc/saitypes.h: 290
class union_anon_14(Union):
    pass

union_anon_14.__slots__ = [
    'ip4',
    'ip6',
]
union_anon_14._fields_ = [
    ('ip4', sai_ip4_t),
    ('ip6', sai_ip6_t),
]

# /home/omer/P4/SAI/inc/saitypes.h: 294
class struct__sai_ip_address_t(Structure):
    pass

struct__sai_ip_address_t.__slots__ = [
    'addr_family',
    'addr',
]
struct__sai_ip_address_t._fields_ = [
    ('addr_family', sai_ip_addr_family_t),
    ('addr', union_anon_14),
]

sai_ip_address_t = struct__sai_ip_address_t # /home/omer/P4/SAI/inc/saitypes.h: 294

# /home/omer/P4/SAI/inc/saitypes.h: 298
class union_anon_15(Union):
    pass

union_anon_15.__slots__ = [
    'ip4',
    'ip6',
]
union_anon_15._fields_ = [
    ('ip4', sai_ip4_t),
    ('ip6', sai_ip6_t),
]

# /home/omer/P4/SAI/inc/saitypes.h: 302
class union_anon_16(Union):
    pass

union_anon_16.__slots__ = [
    'ip4',
    'ip6',
]
union_anon_16._fields_ = [
    ('ip4', sai_ip4_t),
    ('ip6', sai_ip6_t),
]

# /home/omer/P4/SAI/inc/saitypes.h: 306
class struct__sai_ip_prefix_t(Structure):
    pass

struct__sai_ip_prefix_t.__slots__ = [
    'addr_family',
    'addr',
    'mask',
]
struct__sai_ip_prefix_t._fields_ = [
    ('addr_family', sai_ip_addr_family_t),
    ('addr', union_anon_15),
    ('mask', union_anon_16),
]

sai_ip_prefix_t = struct__sai_ip_prefix_t # /home/omer/P4/SAI/inc/saitypes.h: 306

# /home/omer/P4/SAI/inc/saitypes.h: 323
class union_anon_17(Union):
    pass

union_anon_17.__slots__ = [
    'u8',
    's8',
    'u16',
    's16',
    'u32',
    's32',
    'mac',
    'ip4',
    'ip6',
    'u8list',
]
union_anon_17._fields_ = [
    ('u8', sai_uint8_t),
    ('s8', sai_int8_t),
    ('u16', sai_uint16_t),
    ('s16', sai_int16_t),
    ('u32', sai_uint32_t),
    ('s32', sai_int32_t),
    ('mac', sai_mac_t),
    ('ip4', sai_ip4_t),
    ('ip6', sai_ip6_t),
    ('u8list', sai_u8_list_t),
]

# /home/omer/P4/SAI/inc/saitypes.h: 339
class union_anon_18(Union):
    pass

union_anon_18.__slots__ = [
    'booldata',
    'u8',
    's8',
    'u16',
    's16',
    'u32',
    's32',
    'mac',
    'ip4',
    'ip6',
    'oid',
    'objlist',
    'u8list',
]
union_anon_18._fields_ = [
    ('booldata', c_uint8),
    ('u8', sai_uint8_t),
    ('s8', sai_int8_t),
    ('u16', sai_uint16_t),
    ('s16', sai_int16_t),
    ('u32', sai_uint32_t),
    ('s32', sai_int32_t),
    ('mac', sai_mac_t),
    ('ip4', sai_ip4_t),
    ('ip6', sai_ip6_t),
    ('oid', sai_object_id_t),
    ('objlist', sai_object_list_t),
    ('u8list', sai_u8_list_t),
]

# /home/omer/P4/SAI/inc/saitypes.h: 354
class struct__sai_acl_field_data_t(Structure):
    pass

struct__sai_acl_field_data_t.__slots__ = [
    'enable',
    'mask',
    'data',
]
struct__sai_acl_field_data_t._fields_ = [
    ('enable', c_uint8),
    ('mask', union_anon_17),
    ('data', union_anon_18),
]

sai_acl_field_data_t = struct__sai_acl_field_data_t # /home/omer/P4/SAI/inc/saitypes.h: 354

# /home/omer/P4/SAI/inc/saitypes.h: 371
class union_anon_19(Union):
    pass

union_anon_19.__slots__ = [
    'u8',
    's8',
    'u16',
    's16',
    'u32',
    's32',
    'mac',
    'ip4',
    'ip6',
    'oid',
    'objlist',
]
union_anon_19._fields_ = [
    ('u8', sai_uint8_t),
    ('s8', sai_int8_t),
    ('u16', sai_uint16_t),
    ('s16', sai_int16_t),
    ('u32', sai_uint32_t),
    ('s32', sai_int32_t),
    ('mac', sai_mac_t),
    ('ip4', sai_ip4_t),
    ('ip6', sai_ip6_t),
    ('oid', sai_object_id_t),
    ('objlist', sai_object_list_t),
]

# /home/omer/P4/SAI/inc/saitypes.h: 385
class struct__sai_acl_action_data_t(Structure):
    pass

struct__sai_acl_action_data_t.__slots__ = [
    'enable',
    'parameter',
]
struct__sai_acl_action_data_t._fields_ = [
    ('enable', c_uint8),
    ('parameter', union_anon_19),
]

sai_acl_action_data_t = struct__sai_acl_action_data_t # /home/omer/P4/SAI/inc/saitypes.h: 385

enum__sai_packet_color_t = c_int # /home/omer/P4/SAI/inc/saitypes.h: 407

SAI_PACKET_COLOR_GREEN = 0 # /home/omer/P4/SAI/inc/saitypes.h: 407

SAI_PACKET_COLOR_YELLOW = (SAI_PACKET_COLOR_GREEN + 1) # /home/omer/P4/SAI/inc/saitypes.h: 407

SAI_PACKET_COLOR_RED = (SAI_PACKET_COLOR_YELLOW + 1) # /home/omer/P4/SAI/inc/saitypes.h: 407

sai_packet_color_t = enum__sai_packet_color_t # /home/omer/P4/SAI/inc/saitypes.h: 407

# /home/omer/P4/SAI/inc/saitypes.h: 447
class struct__sai_qos_map_params_t(Structure):
    pass

struct__sai_qos_map_params_t.__slots__ = [
    'tc',
    'dscp',
    'dot1p',
    'prio',
    'pg',
    'queue_index',
    'color',
]
struct__sai_qos_map_params_t._fields_ = [
    ('tc', sai_cos_t),
    ('dscp', sai_uint8_t),
    ('dot1p', sai_uint8_t),
    ('prio', sai_uint8_t),
    ('pg', sai_uint8_t),
    ('queue_index', sai_queue_index_t),
    ('color', sai_packet_color_t),
]

sai_qos_map_params_t = struct__sai_qos_map_params_t # /home/omer/P4/SAI/inc/saitypes.h: 447

# /home/omer/P4/SAI/inc/saitypes.h: 457
class struct__sai_qos_map_t(Structure):
    pass

struct__sai_qos_map_t.__slots__ = [
    'key',
    'value',
]
struct__sai_qos_map_t._fields_ = [
    ('key', sai_qos_map_params_t),
    ('value', sai_qos_map_params_t),
]

sai_qos_map_t = struct__sai_qos_map_t # /home/omer/P4/SAI/inc/saitypes.h: 457

# /home/omer/P4/SAI/inc/saitypes.h: 466
class struct__sai_qos_map_list_t(Structure):
    pass

struct__sai_qos_map_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_qos_map_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(sai_qos_map_t)),
]

sai_qos_map_list_t = struct__sai_qos_map_list_t # /home/omer/P4/SAI/inc/saitypes.h: 466

# /home/omer/P4/SAI/inc/saitypes.h: 482
class struct__sai_tunnel_map_params_t(Structure):
    pass

struct__sai_tunnel_map_params_t.__slots__ = [
    'oecn',
    'uecn',
    'vlan_id',
    'vni_id',
]
struct__sai_tunnel_map_params_t._fields_ = [
    ('oecn', sai_uint8_t),
    ('uecn', sai_uint8_t),
    ('vlan_id', sai_vlan_id_t),
    ('vni_id', sai_uint32_t),
]

sai_tunnel_map_params_t = struct__sai_tunnel_map_params_t # /home/omer/P4/SAI/inc/saitypes.h: 482

# /home/omer/P4/SAI/inc/saitypes.h: 492
class struct__sai_tunnel_map_t(Structure):
    pass

struct__sai_tunnel_map_t.__slots__ = [
    'key',
    'value',
]
struct__sai_tunnel_map_t._fields_ = [
    ('key', sai_tunnel_map_params_t),
    ('value', sai_tunnel_map_params_t),
]

sai_tunnel_map_t = struct__sai_tunnel_map_t # /home/omer/P4/SAI/inc/saitypes.h: 492

# /home/omer/P4/SAI/inc/saitypes.h: 502
class struct__sai_tunnel_map_list_t(Structure):
    pass

struct__sai_tunnel_map_list_t.__slots__ = [
    'count',
    'list',
]
struct__sai_tunnel_map_list_t._fields_ = [
    ('count', c_uint32),
    ('list', POINTER(sai_tunnel_map_t)),
]

sai_tunnel_map_list_t = struct__sai_tunnel_map_list_t # /home/omer/P4/SAI/inc/saitypes.h: 502

# /home/omer/P4/SAI/inc/saitypes.h: 524
class struct__sai_acl_capability_t(Structure):
    pass

struct__sai_acl_capability_t.__slots__ = [
    'is_action_list_mandatory',
    'action_list',
]
struct__sai_acl_capability_t._fields_ = [
    ('is_action_list_mandatory', c_uint8),
    ('action_list', sai_s32_list_t),
]

sai_acl_capability_t = struct__sai_acl_capability_t # /home/omer/P4/SAI/inc/saitypes.h: 524

enum__sai_fdb_entry_bridge_type_t = c_int # /home/omer/P4/SAI/inc/saitypes.h: 537

SAI_FDB_ENTRY_BRIDGE_TYPE_1Q = 0 # /home/omer/P4/SAI/inc/saitypes.h: 537

SAI_FDB_ENTRY_BRIDGE_TYPE_1D = (SAI_FDB_ENTRY_BRIDGE_TYPE_1Q + 1) # /home/omer/P4/SAI/inc/saitypes.h: 537

sai_fdb_entry_bridge_type_t = enum__sai_fdb_entry_bridge_type_t # /home/omer/P4/SAI/inc/saitypes.h: 537

# /home/omer/P4/SAI/inc/saitypes.h: 576
class union_anon_20(Union):
    pass

union_anon_20.__slots__ = [
    'booldata',
    'chardata',
    'u8',
    's8',
    'u16',
    's16',
    'u32',
    's32',
    'u64',
    's64',
    'ptr',
    'mac',
    'ip4',
    'ip6',
    'ipaddr',
    'ipprefix',
    'oid',
    'objlist',
    'u8list',
    's8list',
    'u16list',
    's16list',
    'u32list',
    's32list',
    'u32range',
    's32range',
    'vlanlist',
    'aclfield',
    'aclaction',
    'qosmap',
    'tunnelmap',
    'aclcapability',
]
union_anon_20._fields_ = [
    ('booldata', c_uint8),
    ('chardata', c_char * 32),
    ('u8', sai_uint8_t),
    ('s8', sai_int8_t),
    ('u16', sai_uint16_t),
    ('s16', sai_int16_t),
    ('u32', sai_uint32_t),
    ('s32', sai_int32_t),
    ('u64', sai_uint64_t),
    ('s64', sai_int64_t),
    ('ptr', sai_pointer_t),
    ('mac', sai_mac_t),
    ('ip4', sai_ip4_t),
    ('ip6', sai_ip6_t),
    ('ipaddr', sai_ip_address_t),
    ('ipprefix', sai_ip_prefix_t),
    ('oid', sai_object_id_t),
    ('objlist', sai_object_list_t),
    ('u8list', sai_u8_list_t),
    ('s8list', sai_s8_list_t),
    ('u16list', sai_u16_list_t),
    ('s16list', sai_s16_list_t),
    ('u32list', sai_u32_list_t),
    ('s32list', sai_s32_list_t),
    ('u32range', sai_u32_range_t),
    ('s32range', sai_s32_range_t),
    ('vlanlist', sai_vlan_list_t),
    ('aclfield', sai_acl_field_data_t),
    ('aclaction', sai_acl_action_data_t),
    ('qosmap', sai_qos_map_list_t),
    ('tunnelmap', sai_tunnel_map_list_t),
    ('aclcapability', sai_acl_capability_t),
]

sai_attribute_value_t = union_anon_20 # /home/omer/P4/SAI/inc/saitypes.h: 576

# /home/omer/P4/SAI/inc/saitypes.h: 581
class struct__sai_attribute_t(Structure):
    pass

struct__sai_attribute_t.__slots__ = [
    'id',
    'value',
]
struct__sai_attribute_t._fields_ = [
    ('id', sai_attr_id_t),
    ('value', sai_attribute_value_t),
]

sai_attribute_t = struct__sai_attribute_t # /home/omer/P4/SAI/inc/saitypes.h: 581

enum__sai_bulk_op_type_t = c_int # /home/omer/P4/SAI/inc/saitypes.h: 591

SAI_BULK_OP_TYPE_STOP_ON_ERROR = 0 # /home/omer/P4/SAI/inc/saitypes.h: 591

SAI_BULK_OP_TYPE_INGORE_ERROR = (SAI_BULK_OP_TYPE_STOP_ON_ERROR + 1) # /home/omer/P4/SAI/inc/saitypes.h: 591

sai_bulk_op_type_t = enum__sai_bulk_op_type_t # /home/omer/P4/SAI/inc/saitypes.h: 591

sai_bulk_object_create_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(c_uint32), POINTER(POINTER(sai_attribute_t)), sai_bulk_op_type_t, POINTER(sai_object_id_t), POINTER(sai_status_t)) # /home/omer/P4/SAI/inc/saitypes.h: 611

sai_bulk_object_remove_fn = CFUNCTYPE(UNCHECKED(sai_status_t), c_uint32, POINTER(sai_object_id_t), sai_bulk_op_type_t, POINTER(sai_status_t)) # /home/omer/P4/SAI/inc/saitypes.h: 633

enum__sai_acl_stage_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 47

SAI_ACL_STAGE_INGRESS = 0 # /home/omer/P4/SAI/inc/saiacl.h: 47

SAI_ACL_STAGE_EGRESS = (SAI_ACL_STAGE_INGRESS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 47

sai_acl_stage_t = enum__sai_acl_stage_t # /home/omer/P4/SAI/inc/saiacl.h: 47

enum__sai_acl_bind_point_type_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 69

SAI_ACL_BIND_POINT_TYPE_PORT = 0 # /home/omer/P4/SAI/inc/saiacl.h: 69

SAI_ACL_BIND_POINT_TYPE_LAG = (SAI_ACL_BIND_POINT_TYPE_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 69

SAI_ACL_BIND_POINT_TYPE_VLAN = (SAI_ACL_BIND_POINT_TYPE_LAG + 1) # /home/omer/P4/SAI/inc/saiacl.h: 69

SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF = (SAI_ACL_BIND_POINT_TYPE_VLAN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 69

SAI_ACL_BIND_POINT_TYPE_SWITCH = (SAI_ACL_BIND_POINT_TYPE_ROUTER_INTF + 1) # /home/omer/P4/SAI/inc/saiacl.h: 69

sai_acl_bind_point_type_t = enum__sai_acl_bind_point_type_t # /home/omer/P4/SAI/inc/saiacl.h: 69

enum__sai_acl_ip_type_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_ANY = 0 # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_IP = (SAI_ACL_IP_TYPE_ANY + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_NON_IP = (SAI_ACL_IP_TYPE_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_IPV4ANY = (SAI_ACL_IP_TYPE_NON_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_NON_IPV4 = (SAI_ACL_IP_TYPE_IPV4ANY + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_IPV6ANY = (SAI_ACL_IP_TYPE_NON_IPV4 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_NON_IPV6 = (SAI_ACL_IP_TYPE_IPV6ANY + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_ARP = (SAI_ACL_IP_TYPE_NON_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_ARP_REQUEST = (SAI_ACL_IP_TYPE_ARP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

SAI_ACL_IP_TYPE_ARP_REPLY = (SAI_ACL_IP_TYPE_ARP_REQUEST + 1) # /home/omer/P4/SAI/inc/saiacl.h: 106

sai_acl_ip_type_t = enum__sai_acl_ip_type_t # /home/omer/P4/SAI/inc/saiacl.h: 106

enum__sai_acl_ip_frag_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 128

SAI_ACL_IP_FRAG_ANY = 0 # /home/omer/P4/SAI/inc/saiacl.h: 128

SAI_ACL_IP_FRAG_NON_FRAG = (SAI_ACL_IP_FRAG_ANY + 1) # /home/omer/P4/SAI/inc/saiacl.h: 128

SAI_ACL_IP_FRAG_NON_FRAG_OR_HEAD = (SAI_ACL_IP_FRAG_NON_FRAG + 1) # /home/omer/P4/SAI/inc/saiacl.h: 128

SAI_ACL_IP_FRAG_HEAD = (SAI_ACL_IP_FRAG_NON_FRAG_OR_HEAD + 1) # /home/omer/P4/SAI/inc/saiacl.h: 128

SAI_ACL_IP_FRAG_NON_HEAD = (SAI_ACL_IP_FRAG_HEAD + 1) # /home/omer/P4/SAI/inc/saiacl.h: 128

sai_acl_ip_frag_t = enum__sai_acl_ip_frag_t # /home/omer/P4/SAI/inc/saiacl.h: 128

enum__sai_acl_action_type_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_REDIRECT = 0 # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_REDIRECT_LIST = (SAI_ACL_ACTION_TYPE_REDIRECT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_PACKET_ACTION = (SAI_ACL_ACTION_TYPE_REDIRECT_LIST + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_FLOOD = (SAI_ACL_ACTION_TYPE_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_COUNTER = (SAI_ACL_ACTION_TYPE_FLOOD + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_MIRROR_INGRESS = (SAI_ACL_ACTION_TYPE_COUNTER + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_MIRROR_EGRESS = (SAI_ACL_ACTION_TYPE_MIRROR_INGRESS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_POLICER = (SAI_ACL_ACTION_TYPE_MIRROR_EGRESS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_DECREMENT_TTL = (SAI_ACL_ACTION_TYPE_SET_POLICER + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_TC = (SAI_ACL_ACTION_TYPE_DECREMENT_TTL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_PACKET_COLOR = (SAI_ACL_ACTION_TYPE_SET_TC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_INNER_VLAN_ID = (SAI_ACL_ACTION_TYPE_SET_PACKET_COLOR + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_INNER_VLAN_PRI = (SAI_ACL_ACTION_TYPE_SET_INNER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_OUTER_VLAN_ID = (SAI_ACL_ACTION_TYPE_SET_INNER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_OUTER_VLAN_PRI = (SAI_ACL_ACTION_TYPE_SET_OUTER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_SRC_MAC = (SAI_ACL_ACTION_TYPE_SET_OUTER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_DST_MAC = (SAI_ACL_ACTION_TYPE_SET_SRC_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_SRC_IP = (SAI_ACL_ACTION_TYPE_SET_DST_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_DST_IP = (SAI_ACL_ACTION_TYPE_SET_SRC_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_SRC_IPV6 = (SAI_ACL_ACTION_TYPE_SET_DST_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_DST_IPV6 = (SAI_ACL_ACTION_TYPE_SET_SRC_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_DSCP = (SAI_ACL_ACTION_TYPE_SET_DST_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_ECN = (SAI_ACL_ACTION_TYPE_SET_DSCP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_L4_SRC_PORT = (SAI_ACL_ACTION_TYPE_SET_ECN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_L4_DST_PORT = (SAI_ACL_ACTION_TYPE_SET_L4_SRC_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_INGRESS_SAMPLEPACKET_ENABLE = (SAI_ACL_ACTION_TYPE_SET_L4_DST_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_EGRESS_SAMPLEPACKET_ENABLE = (SAI_ACL_ACTION_TYPE_INGRESS_SAMPLEPACKET_ENABLE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_CPU_QUEUE = (SAI_ACL_ACTION_TYPE_EGRESS_SAMPLEPACKET_ENABLE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_ACL_META_DATA = (SAI_ACL_ACTION_TYPE_SET_CPU_QUEUE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_EGRESS_BLOCK_PORT_LIST = (SAI_ACL_ACTION_TYPE_SET_ACL_META_DATA + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_USER_TRAP_ID = (SAI_ACL_ACTION_TYPE_EGRESS_BLOCK_PORT_LIST + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

SAI_ACL_ACTION_TYPE_SET_DO_NOT_LEARN = (SAI_ACL_ACTION_TYPE_SET_USER_TRAP_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 231

sai_acl_action_type_t = enum__sai_acl_action_type_t # /home/omer/P4/SAI/inc/saiacl.h: 231

enum__sai_acl_table_group_type_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 244

SAI_ACL_TABLE_GROUP_TYPE_SEQUENTIAL = 0 # /home/omer/P4/SAI/inc/saiacl.h: 244

SAI_ACL_TABLE_GROUP_TYPE_PARALLEL = (SAI_ACL_TABLE_GROUP_TYPE_SEQUENTIAL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 244

sai_acl_table_group_type_t = enum__sai_acl_table_group_type_t # /home/omer/P4/SAI/inc/saiacl.h: 244

enum__sai_acl_table_group_attr_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 313

SAI_ACL_TABLE_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiacl.h: 313

SAI_ACL_TABLE_GROUP_ATTR_ACL_STAGE = SAI_ACL_TABLE_GROUP_ATTR_START # /home/omer/P4/SAI/inc/saiacl.h: 313

SAI_ACL_TABLE_GROUP_ATTR_ACL_BIND_POINT_TYPE_LIST = (SAI_ACL_TABLE_GROUP_ATTR_ACL_STAGE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 313

SAI_ACL_TABLE_GROUP_ATTR_TYPE = (SAI_ACL_TABLE_GROUP_ATTR_ACL_BIND_POINT_TYPE_LIST + 1) # /home/omer/P4/SAI/inc/saiacl.h: 313

SAI_ACL_TABLE_GROUP_ATTR_END = (SAI_ACL_TABLE_GROUP_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 313

SAI_ACL_TABLE_GROUP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiacl.h: 313

SAI_ACL_TABLE_GROUP_ATTR_CUSTOM_RANGE_END = (SAI_ACL_TABLE_GROUP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiacl.h: 313

sai_acl_table_group_attr_t = enum__sai_acl_table_group_attr_t # /home/omer/P4/SAI/inc/saiacl.h: 313

enum__sai_acl_table_group_member_attr_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 382

SAI_ACL_TABLE_GROUP_MEMBER_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiacl.h: 382

SAI_ACL_TABLE_GROUP_MEMBER_ATTR_ACL_TABLE_GROUP_ID = SAI_ACL_TABLE_GROUP_MEMBER_ATTR_START # /home/omer/P4/SAI/inc/saiacl.h: 382

SAI_ACL_TABLE_GROUP_MEMBER_ATTR_ACL_TABLE_ID = (SAI_ACL_TABLE_GROUP_MEMBER_ATTR_ACL_TABLE_GROUP_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 382

SAI_ACL_TABLE_GROUP_MEMBER_ATTR_PRIORITY = (SAI_ACL_TABLE_GROUP_MEMBER_ATTR_ACL_TABLE_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 382

SAI_ACL_TABLE_GROUP_MEMBER_ATTR_END = (SAI_ACL_TABLE_GROUP_MEMBER_ATTR_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiacl.h: 382

SAI_ACL_TABLE_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiacl.h: 382

SAI_ACL_TABLE_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END = (SAI_ACL_TABLE_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiacl.h: 382

sai_acl_table_group_member_attr_t = enum__sai_acl_table_group_member_attr_t # /home/omer/P4/SAI/inc/saiacl.h: 382

enum__sai_acl_table_attr_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_ACL_STAGE = SAI_ACL_TABLE_ATTR_START # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST = (SAI_ACL_TABLE_ATTR_ACL_STAGE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_SIZE = (SAI_ACL_TABLE_ATTR_ACL_BIND_POINT_TYPE_LIST + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_ACL_ACTION_TYPE_LIST = (SAI_ACL_TABLE_ATTR_SIZE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_START = 4096 # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_SRC_IPV6 = SAI_ACL_TABLE_ATTR_FIELD_START # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_DST_IPV6 = (SAI_ACL_TABLE_ATTR_FIELD_SRC_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_INNER_SRC_IPV6 = (SAI_ACL_TABLE_ATTR_FIELD_DST_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_INNER_DST_IPV6 = (SAI_ACL_TABLE_ATTR_FIELD_INNER_SRC_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC = (SAI_ACL_TABLE_ATTR_FIELD_INNER_DST_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_DST_MAC = (SAI_ACL_TABLE_ATTR_FIELD_SRC_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_SRC_IP = (SAI_ACL_TABLE_ATTR_FIELD_DST_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_DST_IP = (SAI_ACL_TABLE_ATTR_FIELD_SRC_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_INNER_SRC_IP = (SAI_ACL_TABLE_ATTR_FIELD_DST_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_INNER_DST_IP = (SAI_ACL_TABLE_ATTR_FIELD_INNER_SRC_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS = (SAI_ACL_TABLE_ATTR_FIELD_INNER_DST_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_OUT_PORTS = (SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_IN_PORT = (SAI_ACL_TABLE_ATTR_FIELD_OUT_PORTS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT = (SAI_ACL_TABLE_ATTR_FIELD_IN_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_SRC_PORT = (SAI_ACL_TABLE_ATTR_FIELD_OUT_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_ID = (SAI_ACL_TABLE_ATTR_FIELD_SRC_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_PRI = (SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_CFI = (SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_ID = (SAI_ACL_TABLE_ATTR_FIELD_OUTER_VLAN_CFI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_PRI = (SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_CFI = (SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT = (SAI_ACL_TABLE_ATTR_FIELD_INNER_VLAN_CFI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT = (SAI_ACL_TABLE_ATTR_FIELD_L4_SRC_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE = (SAI_ACL_TABLE_ATTR_FIELD_L4_DST_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL = (SAI_ACL_TABLE_ATTR_FIELD_ETHER_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_IP_IDENTIFICATION = (SAI_ACL_TABLE_ATTR_FIELD_IP_PROTOCOL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_DSCP = (SAI_ACL_TABLE_ATTR_FIELD_IP_IDENTIFICATION + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ECN = (SAI_ACL_TABLE_ATTR_FIELD_DSCP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_TTL = (SAI_ACL_TABLE_ATTR_FIELD_ECN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_TOS = (SAI_ACL_TABLE_ATTR_FIELD_TTL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_IP_FLAGS = (SAI_ACL_TABLE_ATTR_FIELD_TOS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS = (SAI_ACL_TABLE_ATTR_FIELD_IP_FLAGS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE = (SAI_ACL_TABLE_ATTR_FIELD_TCP_FLAGS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_FRAG = (SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_IPV6_FLOW_LABEL = (SAI_ACL_TABLE_ATTR_FIELD_ACL_IP_FRAG + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_TC = (SAI_ACL_TABLE_ATTR_FIELD_IPV6_FLOW_LABEL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ICMP_TYPE = (SAI_ACL_TABLE_ATTR_FIELD_TC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ICMP_CODE = (SAI_ACL_TABLE_ATTR_FIELD_ICMP_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_PACKET_VLAN = (SAI_ACL_TABLE_ATTR_FIELD_ICMP_CODE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_FDB_DST_USER_META = (SAI_ACL_TABLE_ATTR_FIELD_PACKET_VLAN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ROUTE_DST_USER_META = (SAI_ACL_TABLE_ATTR_FIELD_FDB_DST_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_NEIGHBOR_DST_USER_META = (SAI_ACL_TABLE_ATTR_FIELD_ROUTE_DST_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_PORT_USER_META = (SAI_ACL_TABLE_ATTR_FIELD_NEIGHBOR_DST_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_VLAN_USER_META = (SAI_ACL_TABLE_ATTR_FIELD_PORT_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ACL_USER_META = (SAI_ACL_TABLE_ATTR_FIELD_VLAN_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_FDB_NPU_META_DST_HIT = (SAI_ACL_TABLE_ATTR_FIELD_ACL_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_NEIGHBOR_NPU_META_DST_HIT = (SAI_ACL_TABLE_ATTR_FIELD_FDB_NPU_META_DST_HIT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ROUTE_NPU_META_DST_HIT = (SAI_ACL_TABLE_ATTR_FIELD_NEIGHBOR_NPU_META_DST_HIT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN = (SAI_ACL_TABLE_ATTR_FIELD_ROUTE_NPU_META_DST_HIT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MAX = (SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MIN + 255) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_ACL_RANGE_TYPE = (SAI_ACL_TABLE_ATTR_USER_DEFINED_FIELD_GROUP_MAX + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_IPV6_NEXT_HEADER = (SAI_ACL_TABLE_ATTR_FIELD_ACL_RANGE_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_FIELD_END = SAI_ACL_TABLE_ATTR_FIELD_IPV6_NEXT_HEADER # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_END = (SAI_ACL_TABLE_ATTR_FIELD_END + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiacl.h: 959

SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_END = (SAI_ACL_TABLE_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiacl.h: 959

sai_acl_table_attr_t = enum__sai_acl_table_attr_t # /home/omer/P4/SAI/inc/saiacl.h: 959

enum__sai_acl_entry_attr_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_TABLE_ID = SAI_ACL_ENTRY_ATTR_START # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_PRIORITY = (SAI_ACL_ENTRY_ATTR_TABLE_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ADMIN_STATE = (SAI_ACL_ENTRY_ATTR_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_START = 4096 # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPV6 = SAI_ACL_ENTRY_ATTR_FIELD_START # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_DST_IPV6 = (SAI_ACL_ENTRY_ATTR_FIELD_SRC_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_INNER_SRC_IPV6 = (SAI_ACL_ENTRY_ATTR_FIELD_DST_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_INNER_DST_IPV6 = (SAI_ACL_ENTRY_ATTR_FIELD_INNER_SRC_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC = (SAI_ACL_ENTRY_ATTR_FIELD_INNER_DST_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC = (SAI_ACL_ENTRY_ATTR_FIELD_SRC_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP = (SAI_ACL_ENTRY_ATTR_FIELD_DST_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_DST_IP = (SAI_ACL_ENTRY_ATTR_FIELD_SRC_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_INNER_SRC_IP = (SAI_ACL_ENTRY_ATTR_FIELD_DST_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_INNER_DST_IP = (SAI_ACL_ENTRY_ATTR_FIELD_INNER_SRC_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS = (SAI_ACL_ENTRY_ATTR_FIELD_INNER_DST_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS = (SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT = (SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORTS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT = (SAI_ACL_ENTRY_ATTR_FIELD_IN_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_SRC_PORT = (SAI_ACL_ENTRY_ATTR_FIELD_OUT_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID = (SAI_ACL_ENTRY_ATTR_FIELD_SRC_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI = (SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI = (SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID = (SAI_ACL_ENTRY_ATTR_FIELD_OUTER_VLAN_CFI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_PRI = (SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_CFI = (SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT = (SAI_ACL_ENTRY_ATTR_FIELD_INNER_VLAN_CFI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT = (SAI_ACL_ENTRY_ATTR_FIELD_L4_SRC_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE = (SAI_ACL_ENTRY_ATTR_FIELD_L4_DST_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL = (SAI_ACL_ENTRY_ATTR_FIELD_ETHER_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_IP_IDENTIFICATION = (SAI_ACL_ENTRY_ATTR_FIELD_IP_PROTOCOL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_DSCP = (SAI_ACL_ENTRY_ATTR_FIELD_IP_IDENTIFICATION + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ECN = (SAI_ACL_ENTRY_ATTR_FIELD_DSCP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_TTL = (SAI_ACL_ENTRY_ATTR_FIELD_ECN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_TOS = (SAI_ACL_ENTRY_ATTR_FIELD_TTL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_IP_FLAGS = (SAI_ACL_ENTRY_ATTR_FIELD_TOS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS = (SAI_ACL_ENTRY_ATTR_FIELD_IP_FLAGS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE = (SAI_ACL_ENTRY_ATTR_FIELD_TCP_FLAGS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_FRAG = (SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_IPV6_FLOW_LABEL = (SAI_ACL_ENTRY_ATTR_FIELD_ACL_IP_FRAG + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_TC = (SAI_ACL_ENTRY_ATTR_FIELD_IPV6_FLOW_LABEL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE = (SAI_ACL_ENTRY_ATTR_FIELD_TC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE = (SAI_ACL_ENTRY_ATTR_FIELD_ICMP_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_PACKET_VLAN = (SAI_ACL_ENTRY_ATTR_FIELD_ICMP_CODE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_FDB_DST_USER_META = (SAI_ACL_ENTRY_ATTR_FIELD_PACKET_VLAN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ROUTE_DST_USER_META = (SAI_ACL_ENTRY_ATTR_FIELD_FDB_DST_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_NEIGHBOR_DST_USER_META = (SAI_ACL_ENTRY_ATTR_FIELD_ROUTE_DST_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_PORT_USER_META = (SAI_ACL_ENTRY_ATTR_FIELD_NEIGHBOR_DST_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_VLAN_USER_META = (SAI_ACL_ENTRY_ATTR_FIELD_PORT_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ACL_USER_META = (SAI_ACL_ENTRY_ATTR_FIELD_VLAN_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_FDB_NPU_META_DST_HIT = (SAI_ACL_ENTRY_ATTR_FIELD_ACL_USER_META + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_NEIGHBOR_NPU_META_DST_HIT = (SAI_ACL_ENTRY_ATTR_FIELD_FDB_NPU_META_DST_HIT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ROUTE_NPU_META_DST_HIT = (SAI_ACL_ENTRY_ATTR_FIELD_NEIGHBOR_NPU_META_DST_HIT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_MIN = (SAI_ACL_ENTRY_ATTR_FIELD_ROUTE_NPU_META_DST_HIT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_MAX = (SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_MIN + 255) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_ACL_RANGE_TYPE = (SAI_ACL_ENTRY_ATTR_USER_DEFINED_FIELD_MAX + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_IPV6_NEXT_HEADER = (SAI_ACL_ENTRY_ATTR_FIELD_ACL_RANGE_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_FIELD_END = SAI_ACL_ENTRY_ATTR_FIELD_IPV6_NEXT_HEADER # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_START = 8192 # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT = SAI_ACL_ENTRY_ATTR_ACTION_START # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT_LIST = (SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_PACKET_ACTION = (SAI_ACL_ENTRY_ATTR_ACTION_REDIRECT_LIST + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_FLOOD = (SAI_ACL_ENTRY_ATTR_ACTION_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_COUNTER = (SAI_ACL_ENTRY_ATTR_ACTION_FLOOD + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS = (SAI_ACL_ENTRY_ATTR_ACTION_COUNTER + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS = (SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_INGRESS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_POLICER = (SAI_ACL_ENTRY_ATTR_ACTION_MIRROR_EGRESS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_DECREMENT_TTL = (SAI_ACL_ENTRY_ATTR_ACTION_SET_POLICER + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_TC = (SAI_ACL_ENTRY_ATTR_ACTION_DECREMENT_TTL + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_PACKET_COLOR = (SAI_ACL_ENTRY_ATTR_ACTION_SET_TC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_ID = (SAI_ACL_ENTRY_ATTR_ACTION_SET_PACKET_COLOR + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_PRI = (SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_ID = (SAI_ACL_ENTRY_ATTR_ACTION_SET_INNER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_PRI = (SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC = (SAI_ACL_ENTRY_ATTR_ACTION_SET_OUTER_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC = (SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IP = (SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_MAC + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IP = (SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IPV6 = (SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IPV6 = (SAI_ACL_ENTRY_ATTR_ACTION_SET_SRC_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP = (SAI_ACL_ENTRY_ATTR_ACTION_SET_DST_IPV6 + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_ECN = (SAI_ACL_ENTRY_ATTR_ACTION_SET_DSCP + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_SRC_PORT = (SAI_ACL_ENTRY_ATTR_ACTION_SET_ECN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_DST_PORT = (SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_SRC_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_INGRESS_SAMPLEPACKET_ENABLE = (SAI_ACL_ENTRY_ATTR_ACTION_SET_L4_DST_PORT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_EGRESS_SAMPLEPACKET_ENABLE = (SAI_ACL_ENTRY_ATTR_ACTION_INGRESS_SAMPLEPACKET_ENABLE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE = (SAI_ACL_ENTRY_ATTR_ACTION_EGRESS_SAMPLEPACKET_ENABLE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_ACL_META_DATA = (SAI_ACL_ENTRY_ATTR_ACTION_SET_CPU_QUEUE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_EGRESS_BLOCK_PORT_LIST = (SAI_ACL_ENTRY_ATTR_ACTION_SET_ACL_META_DATA + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_USER_TRAP_ID = (SAI_ACL_ENTRY_ATTR_ACTION_EGRESS_BLOCK_PORT_LIST + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_SET_DO_NOT_LEARN = (SAI_ACL_ENTRY_ATTR_ACTION_SET_USER_TRAP_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_ACTION_END = SAI_ACL_ENTRY_ATTR_ACTION_SET_DO_NOT_LEARN # /home/omer/P4/SAI/inc/saiacl.h: 1781

SAI_ACL_ENTRY_ATTR_END = (SAI_ACL_ENTRY_ATTR_ACTION_END + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1781

sai_acl_entry_attr_t = enum__sai_acl_entry_attr_t # /home/omer/P4/SAI/inc/saiacl.h: 1781

enum__sai_acl_counter_attr_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 1851

SAI_ACL_COUNTER_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiacl.h: 1851

SAI_ACL_COUNTER_ATTR_TABLE_ID = SAI_ACL_COUNTER_ATTR_START # /home/omer/P4/SAI/inc/saiacl.h: 1851

SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT = (SAI_ACL_COUNTER_ATTR_TABLE_ID + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1851

SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT = (SAI_ACL_COUNTER_ATTR_ENABLE_PACKET_COUNT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1851

SAI_ACL_COUNTER_ATTR_PACKETS = (SAI_ACL_COUNTER_ATTR_ENABLE_BYTE_COUNT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1851

SAI_ACL_COUNTER_ATTR_BYTES = (SAI_ACL_COUNTER_ATTR_PACKETS + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1851

SAI_ACL_COUNTER_ATTR_END = (SAI_ACL_COUNTER_ATTR_BYTES + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1851

sai_acl_counter_attr_t = enum__sai_acl_counter_attr_t # /home/omer/P4/SAI/inc/saiacl.h: 1851

enum__sai_acl_range_type_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 1873

SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE = 0 # /home/omer/P4/SAI/inc/saiacl.h: 1873

SAI_ACL_RANGE_TYPE_L4_DST_PORT_RANGE = (SAI_ACL_RANGE_TYPE_L4_SRC_PORT_RANGE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1873

SAI_ACL_RANGE_TYPE_OUTER_VLAN = (SAI_ACL_RANGE_TYPE_L4_DST_PORT_RANGE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1873

SAI_ACL_RANGE_TYPE_INNER_VLAN = (SAI_ACL_RANGE_TYPE_OUTER_VLAN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1873

SAI_ACL_RANGE_TYPE_PACKET_LENGTH = (SAI_ACL_RANGE_TYPE_INNER_VLAN + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1873

sai_acl_range_type_t = enum__sai_acl_range_type_t # /home/omer/P4/SAI/inc/saiacl.h: 1873

enum__sai_acl_range_attr_t = c_int # /home/omer/P4/SAI/inc/saiacl.h: 1913

SAI_ACL_RANGE_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiacl.h: 1913

SAI_ACL_RANGE_ATTR_TYPE = SAI_ACL_RANGE_ATTR_START # /home/omer/P4/SAI/inc/saiacl.h: 1913

SAI_ACL_RANGE_ATTR_LIMIT = (SAI_ACL_RANGE_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1913

SAI_ACL_RANGE_ATTR_END = (SAI_ACL_RANGE_ATTR_LIMIT + 1) # /home/omer/P4/SAI/inc/saiacl.h: 1913

sai_acl_range_attr_t = enum__sai_acl_range_attr_t # /home/omer/P4/SAI/inc/saiacl.h: 1913

sai_create_acl_table_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 1925

sai_remove_acl_table_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiacl.h: 1938

sai_set_acl_table_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 1949

sai_get_acl_table_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 1962

sai_create_acl_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 1977

sai_remove_acl_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiacl.h: 1990

sai_set_acl_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2001

sai_get_acl_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2014

sai_create_acl_counter_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2029

sai_remove_acl_counter_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiacl.h: 2042

sai_set_acl_counter_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2053

sai_get_acl_counter_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2066

sai_create_acl_range_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2081

sai_remove_acl_range_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiacl.h: 2094

sai_set_acl_range_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2104

sai_get_acl_range_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2117

sai_create_acl_table_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2131

sai_remove_acl_table_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiacl.h: 2144

sai_set_acl_table_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2155

sai_get_acl_table_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2168

sai_create_acl_table_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2182

sai_remove_acl_table_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiacl.h: 2195

sai_set_acl_table_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2206

sai_get_acl_table_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiacl.h: 2219

# /home/omer/P4/SAI/inc/saiacl.h: 2253
class struct__sai_acl_api_t(Structure):
    pass

struct__sai_acl_api_t.__slots__ = [
    'create_acl_table',
    'remove_acl_table',
    'set_acl_table_attribute',
    'get_acl_table_attribute',
    'create_acl_entry',
    'remove_acl_entry',
    'set_acl_entry_attribute',
    'get_acl_entry_attribute',
    'create_acl_counter',
    'remove_acl_counter',
    'set_acl_counter_attribute',
    'get_acl_counter_attribute',
    'create_acl_range',
    'remove_acl_range',
    'set_acl_range_attribute',
    'get_acl_range_attribute',
    'create_acl_table_group',
    'remove_acl_table_group',
    'set_acl_table_group_attribute',
    'get_acl_table_group_attribute',
    'create_acl_table_group_member',
    'remove_acl_table_group_member',
    'set_acl_table_group_member_attribute',
    'get_acl_table_group_member_attribute',
]
struct__sai_acl_api_t._fields_ = [
    ('create_acl_table', sai_create_acl_table_fn),
    ('remove_acl_table', sai_remove_acl_table_fn),
    ('set_acl_table_attribute', sai_set_acl_table_attribute_fn),
    ('get_acl_table_attribute', sai_get_acl_table_attribute_fn),
    ('create_acl_entry', sai_create_acl_entry_fn),
    ('remove_acl_entry', sai_remove_acl_entry_fn),
    ('set_acl_entry_attribute', sai_set_acl_entry_attribute_fn),
    ('get_acl_entry_attribute', sai_get_acl_entry_attribute_fn),
    ('create_acl_counter', sai_create_acl_counter_fn),
    ('remove_acl_counter', sai_remove_acl_counter_fn),
    ('set_acl_counter_attribute', sai_set_acl_counter_attribute_fn),
    ('get_acl_counter_attribute', sai_get_acl_counter_attribute_fn),
    ('create_acl_range', sai_create_acl_range_fn),
    ('remove_acl_range', sai_remove_acl_range_fn),
    ('set_acl_range_attribute', sai_set_acl_range_attribute_fn),
    ('get_acl_range_attribute', sai_get_acl_range_attribute_fn),
    ('create_acl_table_group', sai_create_acl_table_group_fn),
    ('remove_acl_table_group', sai_remove_acl_table_group_fn),
    ('set_acl_table_group_attribute', sai_set_acl_table_group_attribute_fn),
    ('get_acl_table_group_attribute', sai_get_acl_table_group_attribute_fn),
    ('create_acl_table_group_member', sai_create_acl_table_group_member_fn),
    ('remove_acl_table_group_member', sai_remove_acl_table_group_member_fn),
    ('set_acl_table_group_member_attribute', sai_set_acl_table_group_member_attribute_fn),
    ('get_acl_table_group_member_attribute', sai_get_acl_table_group_member_attribute_fn),
]

sai_acl_api_t = struct__sai_acl_api_t # /home/omer/P4/SAI/inc/saiacl.h: 2253

enum__sai_ingress_priority_group_attr_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 64

SAI_INGRESS_PRIORITY_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 64

SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE = SAI_INGRESS_PRIORITY_GROUP_ATTR_START # /home/omer/P4/SAI/inc/saibuffer.h: 64

SAI_INGRESS_PRIORITY_GROUP_ATTR_END = (SAI_INGRESS_PRIORITY_GROUP_ATTR_BUFFER_PROFILE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 64

sai_ingress_priority_group_attr_t = enum__sai_ingress_priority_group_attr_t # /home/omer/P4/SAI/inc/saibuffer.h: 64

enum__sai_ingress_priority_group_stat_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_PACKETS = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_BYTES = 1 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_CURR_OCCUPANCY_BYTES = 2 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_WATERMARK_BYTES = 3 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_CURR_OCCUPANCY_BYTES = 4 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_SHARED_WATERMARK_BYTES = 5 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_XOFF_ROOM_CURR_OCCUPANCY_BYTES = 6 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_XOFF_ROOM_WATERMARK_BYTES = 7 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_DROPPED_PACKETS = 8 # /home/omer/P4/SAI/inc/saibuffer.h: 101

SAI_INGRESS_PRIORITY_GROUP_STAT_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saibuffer.h: 101

sai_ingress_priority_group_stat_t = enum__sai_ingress_priority_group_stat_t # /home/omer/P4/SAI/inc/saibuffer.h: 101

sai_create_ingress_priority_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 113

sai_remove_ingress_priority_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saibuffer.h: 126

sai_set_ingress_priority_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 137

sai_get_ingress_priority_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 150

sai_get_ingress_priority_group_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_ingress_priority_group_stat_t), c_uint32, POINTER(c_uint64)) # /home/omer/P4/SAI/inc/saibuffer.h: 165

sai_clear_ingress_priority_group_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_ingress_priority_group_stat_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 180

enum__sai_buffer_pool_type_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 196

SAI_BUFFER_POOL_TYPE_INGRESS = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 196

SAI_BUFFER_POOL_TYPE_EGRESS = (SAI_BUFFER_POOL_TYPE_INGRESS + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 196

sai_buffer_pool_type_t = enum__sai_buffer_pool_type_t # /home/omer/P4/SAI/inc/saibuffer.h: 196

enum__sai_buffer_pool_threshold_mode_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 209

SAI_BUFFER_POOL_THRESHOLD_MODE_STATIC = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 209

SAI_BUFFER_POOL_THRESHOLD_MODE_DYNAMIC = (SAI_BUFFER_POOL_THRESHOLD_MODE_STATIC + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 209

sai_buffer_pool_threshold_mode_t = enum__sai_buffer_pool_threshold_mode_t # /home/omer/P4/SAI/inc/saibuffer.h: 209

enum__sai_buffer_pool_attr_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 273

SAI_BUFFER_POOL_ATTR_START = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 273

SAI_BUFFER_POOL_ATTR_SHARED_SIZE = SAI_BUFFER_POOL_ATTR_START # /home/omer/P4/SAI/inc/saibuffer.h: 273

SAI_BUFFER_POOL_ATTR_TYPE = (SAI_BUFFER_POOL_ATTR_SHARED_SIZE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 273

SAI_BUFFER_POOL_ATTR_SIZE = (SAI_BUFFER_POOL_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 273

SAI_BUFFER_POOL_ATTR_THRESHOLD_MODE = (SAI_BUFFER_POOL_ATTR_SIZE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 273

SAI_BUFFER_POOL_ATTR_XOFF_SIZE = (SAI_BUFFER_POOL_ATTR_THRESHOLD_MODE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 273

SAI_BUFFER_POOL_ATTR_END = (SAI_BUFFER_POOL_ATTR_XOFF_SIZE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 273

sai_buffer_pool_attr_t = enum__sai_buffer_pool_attr_t # /home/omer/P4/SAI/inc/saibuffer.h: 273

enum__sai_buffer_pool_stat_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 292

SAI_BUFFER_POOL_STAT_CURR_OCCUPANCY_BYTES = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 292

SAI_BUFFER_POOL_STAT_WATERMARK_BYTES = 1 # /home/omer/P4/SAI/inc/saibuffer.h: 292

SAI_BUFFER_POOL_STAT_DROPPED_PACKETS = 2 # /home/omer/P4/SAI/inc/saibuffer.h: 292

SAI_BUFFER_POOL_STAT_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saibuffer.h: 292

sai_buffer_pool_stat_t = enum__sai_buffer_pool_stat_t # /home/omer/P4/SAI/inc/saibuffer.h: 292

sai_create_buffer_pool_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 304

sai_remove_buffer_pool_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saibuffer.h: 317

sai_set_buffer_pool_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 328

sai_get_buffer_pool_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 341

sai_get_buffer_pool_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_buffer_pool_stat_t), c_uint32, POINTER(c_uint64)) # /home/omer/P4/SAI/inc/saibuffer.h: 356

sai_clear_buffer_pool_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_buffer_pool_stat_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 371

enum__sai_buffer_profile_threshold_mode_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 390

SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 390

SAI_BUFFER_PROFILE_THRESHOLD_MODE_DYNAMIC = (SAI_BUFFER_PROFILE_THRESHOLD_MODE_STATIC + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 390

SAI_BUFFER_PROFILE_THRESHOLD_MODE_INHERIT_BUFFER_POOL_MODE = (SAI_BUFFER_PROFILE_THRESHOLD_MODE_DYNAMIC + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 390

sai_buffer_profile_threshold_mode_t = enum__sai_buffer_profile_threshold_mode_t # /home/omer/P4/SAI/inc/saibuffer.h: 390

enum__sai_buffer_profile_attr_t = c_int # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_START = 0 # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_POOL_ID = SAI_BUFFER_PROFILE_ATTR_START # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE = (SAI_BUFFER_PROFILE_ATTR_POOL_ID + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_THRESHOLD_MODE = (SAI_BUFFER_PROFILE_ATTR_BUFFER_SIZE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_SHARED_DYNAMIC_TH = (SAI_BUFFER_PROFILE_ATTR_THRESHOLD_MODE + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_SHARED_STATIC_TH = (SAI_BUFFER_PROFILE_ATTR_SHARED_DYNAMIC_TH + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_XOFF_TH = (SAI_BUFFER_PROFILE_ATTR_SHARED_STATIC_TH + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_XON_TH = (SAI_BUFFER_PROFILE_ATTR_XOFF_TH + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_XON_OFFSET_TH = (SAI_BUFFER_PROFILE_ATTR_XON_TH + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

SAI_BUFFER_PROFILE_ATTR_END = (SAI_BUFFER_PROFILE_ATTR_XON_OFFSET_TH + 1) # /home/omer/P4/SAI/inc/saibuffer.h: 514

sai_buffer_profile_attr_t = enum__sai_buffer_profile_attr_t # /home/omer/P4/SAI/inc/saibuffer.h: 514

sai_create_buffer_profile_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 526

sai_remove_buffer_profile_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saibuffer.h: 539

sai_set_buffer_profile_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 550

sai_get_buffer_profile_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibuffer.h: 563

# /home/omer/P4/SAI/inc/saibuffer.h: 589
class struct__sai_buffer_api_t(Structure):
    pass

struct__sai_buffer_api_t.__slots__ = [
    'create_buffer_pool',
    'remove_buffer_pool',
    'set_buffer_pool_attribute',
    'get_buffer_pool_attribute',
    'get_buffer_pool_stats',
    'clear_buffer_pool_stats',
    'create_ingress_priority_group',
    'remove_ingress_priority_group',
    'set_ingress_priority_group_attribute',
    'get_ingress_priority_group_attribute',
    'get_ingress_priority_group_stats',
    'clear_ingress_priority_group_stats',
    'create_buffer_profile',
    'remove_buffer_profile',
    'set_buffer_profile_attribute',
    'get_buffer_profile_attribute',
]
struct__sai_buffer_api_t._fields_ = [
    ('create_buffer_pool', sai_create_buffer_pool_fn),
    ('remove_buffer_pool', sai_remove_buffer_pool_fn),
    ('set_buffer_pool_attribute', sai_set_buffer_pool_attribute_fn),
    ('get_buffer_pool_attribute', sai_get_buffer_pool_attribute_fn),
    ('get_buffer_pool_stats', sai_get_buffer_pool_stats_fn),
    ('clear_buffer_pool_stats', sai_clear_buffer_pool_stats_fn),
    ('create_ingress_priority_group', sai_create_ingress_priority_group_fn),
    ('remove_ingress_priority_group', sai_remove_ingress_priority_group_fn),
    ('set_ingress_priority_group_attribute', sai_set_ingress_priority_group_attribute_fn),
    ('get_ingress_priority_group_attribute', sai_get_ingress_priority_group_attribute_fn),
    ('get_ingress_priority_group_stats', sai_get_ingress_priority_group_stats_fn),
    ('clear_ingress_priority_group_stats', sai_clear_ingress_priority_group_stats_fn),
    ('create_buffer_profile', sai_create_buffer_profile_fn),
    ('remove_buffer_profile', sai_remove_buffer_profile_fn),
    ('set_buffer_profile_attribute', sai_set_buffer_profile_attribute_fn),
    ('get_buffer_profile_attribute', sai_get_buffer_profile_attribute_fn),
]

sai_buffer_api_t = struct__sai_buffer_api_t # /home/omer/P4/SAI/inc/saibuffer.h: 589

enum__sai_fdb_entry_type_t = c_int # /home/omer/P4/SAI/inc/saifdb.h: 47

SAI_FDB_ENTRY_TYPE_DYNAMIC = 0 # /home/omer/P4/SAI/inc/saifdb.h: 47

SAI_FDB_ENTRY_TYPE_STATIC = (SAI_FDB_ENTRY_TYPE_DYNAMIC + 1) # /home/omer/P4/SAI/inc/saifdb.h: 47

sai_fdb_entry_type_t = enum__sai_fdb_entry_type_t # /home/omer/P4/SAI/inc/saifdb.h: 47

# /home/omer/P4/SAI/inc/saifdb.h: 77
class struct__sai_fdb_entry_t(Structure):
    pass

struct__sai_fdb_entry_t.__slots__ = [
    'switch_id',
    'mac_address',
    'bridge_type',
    'vlan_id',
    'bridge_id',
]
struct__sai_fdb_entry_t._fields_ = [
    ('switch_id', sai_object_id_t),
    ('mac_address', sai_mac_t),
    ('bridge_type', sai_fdb_entry_bridge_type_t),
    ('vlan_id', sai_vlan_id_t),
    ('bridge_id', sai_object_id_t),
]

sai_fdb_entry_t = struct__sai_fdb_entry_t # /home/omer/P4/SAI/inc/saifdb.h: 77

enum__sai_fdb_event_t = c_int # /home/omer/P4/SAI/inc/saifdb.h: 96

SAI_FDB_EVENT_LEARNED = 0 # /home/omer/P4/SAI/inc/saifdb.h: 96

SAI_FDB_EVENT_AGED = (SAI_FDB_EVENT_LEARNED + 1) # /home/omer/P4/SAI/inc/saifdb.h: 96

SAI_FDB_EVENT_MOVE = (SAI_FDB_EVENT_AGED + 1) # /home/omer/P4/SAI/inc/saifdb.h: 96

SAI_FDB_EVENT_FLUSHED = (SAI_FDB_EVENT_MOVE + 1) # /home/omer/P4/SAI/inc/saifdb.h: 96

sai_fdb_event_t = enum__sai_fdb_event_t # /home/omer/P4/SAI/inc/saifdb.h: 96

enum__sai_fdb_entry_attr_t = c_int # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_START = 0 # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_TYPE = SAI_FDB_ENTRY_ATTR_START # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_PACKET_ACTION = (SAI_FDB_ENTRY_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID = (SAI_FDB_ENTRY_ATTR_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_META_DATA = (SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID + 1) # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_ENDPOINT_IP = (SAI_FDB_ENTRY_ATTR_META_DATA + 1) # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_END = (SAI_FDB_ENTRY_ATTR_ENDPOINT_IP + 1) # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saifdb.h: 172

SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_FDB_ENTRY_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saifdb.h: 172

sai_fdb_entry_attr_t = enum__sai_fdb_entry_attr_t # /home/omer/P4/SAI/inc/saifdb.h: 172

enum__sai_fdb_flush_entry_type_t = c_int # /home/omer/P4/SAI/inc/saifdb.h: 185

SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC = 0 # /home/omer/P4/SAI/inc/saifdb.h: 185

SAI_FDB_FLUSH_ENTRY_TYPE_STATIC = (SAI_FDB_FLUSH_ENTRY_TYPE_DYNAMIC + 1) # /home/omer/P4/SAI/inc/saifdb.h: 185

sai_fdb_flush_entry_type_t = enum__sai_fdb_flush_entry_type_t # /home/omer/P4/SAI/inc/saifdb.h: 185

enum__sai_fdb_flush_attr_t = c_int # /home/omer/P4/SAI/inc/saifdb.h: 245

SAI_FDB_FLUSH_ATTR_START = 0 # /home/omer/P4/SAI/inc/saifdb.h: 245

SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID = SAI_FDB_FLUSH_ATTR_START # /home/omer/P4/SAI/inc/saifdb.h: 245

SAI_FDB_FLUSH_ATTR_VLAN_ID = (SAI_FDB_FLUSH_ATTR_BRIDGE_PORT_ID + 1) # /home/omer/P4/SAI/inc/saifdb.h: 245

SAI_FDB_FLUSH_ATTR_ENTRY_TYPE = (SAI_FDB_FLUSH_ATTR_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saifdb.h: 245

SAI_FDB_FLUSH_ATTR_END = (SAI_FDB_FLUSH_ATTR_ENTRY_TYPE + 1) # /home/omer/P4/SAI/inc/saifdb.h: 245

sai_fdb_flush_attr_t = enum__sai_fdb_flush_attr_t # /home/omer/P4/SAI/inc/saifdb.h: 245

# /home/omer/P4/SAI/inc/saifdb.h: 264
class struct__sai_fdb_event_notification_data_t(Structure):
    pass

struct__sai_fdb_event_notification_data_t.__slots__ = [
    'event_type',
    'fdb_entry',
    'attr_count',
    'attr',
]
struct__sai_fdb_event_notification_data_t._fields_ = [
    ('event_type', sai_fdb_event_t),
    ('fdb_entry', sai_fdb_entry_t),
    ('attr_count', c_uint32),
    ('attr', POINTER(sai_attribute_t)),
]

sai_fdb_event_notification_data_t = struct__sai_fdb_event_notification_data_t # /home/omer/P4/SAI/inc/saifdb.h: 264

sai_create_fdb_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_fdb_entry_t), c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saifdb.h: 275

sai_remove_fdb_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_fdb_entry_t)) # /home/omer/P4/SAI/inc/saifdb.h: 287

sai_set_fdb_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_fdb_entry_t), POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saifdb.h: 298

sai_get_fdb_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_fdb_entry_t), c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saifdb.h: 311

sai_flush_fdb_entries_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saifdb.h: 325

sai_fdb_event_notification_fn = CFUNCTYPE(UNCHECKED(None), c_uint32, POINTER(sai_fdb_event_notification_data_t)) # /home/omer/P4/SAI/inc/saifdb.h: 336

# /home/omer/P4/SAI/inc/saifdb.h: 351
class struct__sai_fdb_api_t(Structure):
    pass

struct__sai_fdb_api_t.__slots__ = [
    'create_fdb_entry',
    'remove_fdb_entry',
    'set_fdb_entry_attribute',
    'get_fdb_entry_attribute',
    'flush_fdb_entries',
]
struct__sai_fdb_api_t._fields_ = [
    ('create_fdb_entry', sai_create_fdb_entry_fn),
    ('remove_fdb_entry', sai_remove_fdb_entry_fn),
    ('set_fdb_entry_attribute', sai_set_fdb_entry_attribute_fn),
    ('get_fdb_entry_attribute', sai_get_fdb_entry_attribute_fn),
    ('flush_fdb_entries', sai_flush_fdb_entries_fn),
]

sai_fdb_api_t = struct__sai_fdb_api_t # /home/omer/P4/SAI/inc/saifdb.h: 351

enum__sai_native_hash_field_t = c_int # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_SRC_IP = 0 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_DST_IP = 1 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_INNER_SRC_IP = 2 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_INNER_DST_IP = 3 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_VLAN_ID = 4 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_IP_PROTOCOL = 5 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_ETHERTYPE = 6 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_L4_SRC_PORT = 7 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_L4_DST_PORT = 8 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_SRC_MAC = 9 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_DST_MAC = 10 # /home/omer/P4/SAI/inc/saihash.h: 85

SAI_NATIVE_HASH_FIELD_IN_PORT = 11 # /home/omer/P4/SAI/inc/saihash.h: 85

sai_native_hash_field_t = enum__sai_native_hash_field_t # /home/omer/P4/SAI/inc/saihash.h: 85

enum__sai_hash_attr_t = c_int # /home/omer/P4/SAI/inc/saihash.h: 121

SAI_HASH_ATTR_START = 0 # /home/omer/P4/SAI/inc/saihash.h: 121

SAI_HASH_ATTR_NATIVE_HASH_FIELD_LIST = SAI_HASH_ATTR_START # /home/omer/P4/SAI/inc/saihash.h: 121

SAI_HASH_ATTR_UDF_GROUP_LIST = (SAI_HASH_ATTR_NATIVE_HASH_FIELD_LIST + 1) # /home/omer/P4/SAI/inc/saihash.h: 121

SAI_HASH_ATTR_END = (SAI_HASH_ATTR_UDF_GROUP_LIST + 1) # /home/omer/P4/SAI/inc/saihash.h: 121

sai_hash_attr_t = enum__sai_hash_attr_t # /home/omer/P4/SAI/inc/saihash.h: 121

sai_create_hash_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihash.h: 133

sai_remove_hash_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saihash.h: 146

sai_set_hash_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihash.h: 157

sai_get_hash_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihash.h: 170

# /home/omer/P4/SAI/inc/saihash.h: 185
class struct__sai_hash_api_t(Structure):
    pass

struct__sai_hash_api_t.__slots__ = [
    'create_hash',
    'remove_hash',
    'set_hash_attribute',
    'get_hash_attribute',
]
struct__sai_hash_api_t._fields_ = [
    ('create_hash', sai_create_hash_fn),
    ('remove_hash', sai_remove_hash_fn),
    ('set_hash_attribute', sai_set_hash_attribute_fn),
    ('get_hash_attribute', sai_get_hash_attribute_fn),
]

sai_hash_api_t = struct__sai_hash_api_t # /home/omer/P4/SAI/inc/saihash.h: 185

enum__sai_hostif_trap_group_attr_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 99

SAI_HOSTIF_TRAP_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 99

SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE = SAI_HOSTIF_TRAP_GROUP_ATTR_START # /home/omer/P4/SAI/inc/saihostif.h: 99

SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE = (SAI_HOSTIF_TRAP_GROUP_ATTR_ADMIN_STATE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 99

SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER = (SAI_HOSTIF_TRAP_GROUP_ATTR_QUEUE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 99

SAI_HOSTIF_TRAP_GROUP_ATTR_END = (SAI_HOSTIF_TRAP_GROUP_ATTR_POLICER + 1) # /home/omer/P4/SAI/inc/saihostif.h: 99

SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saihostif.h: 99

SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_END = (SAI_HOSTIF_TRAP_GROUP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saihostif.h: 99

sai_hostif_trap_group_attr_t = enum__sai_hostif_trap_group_attr_t # /home/omer/P4/SAI/inc/saihostif.h: 99

sai_create_hostif_trap_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 111

sai_remove_hostif_trap_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saihostif.h: 124

sai_set_hostif_trap_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 135

sai_get_hostif_trap_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 148

enum__sai_hostif_trap_type_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_STP = SAI_HOSTIF_TRAP_TYPE_START # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_LACP = 1 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_EAPOL = 2 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_LLDP = 3 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_PVRST = 4 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_QUERY = 5 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_LEAVE = 6 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_V1_REPORT = 7 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_V2_REPORT = 8 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IGMP_TYPE_V3_REPORT = 9 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_SAMPLEPACKET = 10 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_SWITCH_CUSTOM_RANGE_BASE = 4096 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_ARP_REQUEST = 8192 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_ARP_RESPONSE = 8193 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_DHCP = 8194 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_OSPF = 8195 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_PIM = 8196 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_VRRP = 8197 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_DHCPV6 = 8198 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_OSPFV6 = 8199 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_VRRPV6 = 8200 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IPV6_NEIGHBOR_DISCOVERY = 8201 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_V2 = 8202 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_REPORT = 8203 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IPV6_MLD_V1_DONE = 8204 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_MLD_V2_REPORT = 8205 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_UNKNOWN_L3_MULTICAST = 8206 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_ROUTER_CUSTOM_RANGE_BASE = 12288 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_IP2ME = 16384 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_SSH = 16385 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_SNMP = 16386 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_BGP = 16387 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_BGPV6 = 16388 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_LOCAL_IP_CUSTOM_RANGE_BASE = 20480 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_L3_MTU_ERROR = 24576 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_TTL_ERROR = 24577 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_CUSTOM_EXCEPTION_RANGE_BASE = 28672 # /home/omer/P4/SAI/inc/saihostif.h: 314

SAI_HOSTIF_TRAP_TYPE_END = 32768 # /home/omer/P4/SAI/inc/saihostif.h: 314

sai_hostif_trap_type_t = enum__sai_hostif_trap_type_t # /home/omer/P4/SAI/inc/saihostif.h: 314

enum__sai_hostif_trap_attr_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_TRAP_TYPE = SAI_HOSTIF_TRAP_ATTR_START # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION = (SAI_HOSTIF_TRAP_ATTR_TRAP_TYPE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY = (SAI_HOSTIF_TRAP_ATTR_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_EXCLUDE_PORT_LIST = (SAI_HOSTIF_TRAP_ATTR_TRAP_PRIORITY + 1) # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP = (SAI_HOSTIF_TRAP_ATTR_EXCLUDE_PORT_LIST + 1) # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_END = (SAI_HOSTIF_TRAP_ATTR_TRAP_GROUP + 1) # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saihostif.h: 386

SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_END = (SAI_HOSTIF_TRAP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saihostif.h: 386

sai_hostif_trap_attr_t = enum__sai_hostif_trap_attr_t # /home/omer/P4/SAI/inc/saihostif.h: 386

sai_create_hostif_trap_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 398

sai_remove_hostif_trap_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saihostif.h: 411

sai_set_hostif_trap_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 422

sai_get_hostif_trap_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 435

enum__sai_hostif_user_defined_trap_type_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 470

SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 470

SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_ROUTER = SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_START # /home/omer/P4/SAI/inc/saihostif.h: 470

SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_NEIGH = (SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_ROUTER + 1) # /home/omer/P4/SAI/inc/saihostif.h: 470

SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_ACL = (SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_NEIGH + 1) # /home/omer/P4/SAI/inc/saihostif.h: 470

SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_FDB = (SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_ACL + 1) # /home/omer/P4/SAI/inc/saihostif.h: 470

SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_CUSTOM_RANGE_BASE = 4096 # /home/omer/P4/SAI/inc/saihostif.h: 470

SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_END = (SAI_HOSTIF_USER_DEFINED_TRAP_TYPE_CUSTOM_RANGE_BASE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 470

sai_hostif_user_defined_trap_type_t = enum__sai_hostif_user_defined_trap_type_t # /home/omer/P4/SAI/inc/saihostif.h: 470

enum__sai_hostif_user_defined_trap_attr_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 523

SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 523

SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TYPE = SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_START # /home/omer/P4/SAI/inc/saihostif.h: 523

SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_PRIORITY = (SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 523

SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_GROUP = (SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_PRIORITY + 1) # /home/omer/P4/SAI/inc/saihostif.h: 523

SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_END = (SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_TRAP_GROUP + 1) # /home/omer/P4/SAI/inc/saihostif.h: 523

SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saihostif.h: 523

SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_END = (SAI_HOSTIF_USER_DEFINED_TRAP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saihostif.h: 523

sai_hostif_user_defined_trap_attr_t = enum__sai_hostif_user_defined_trap_attr_t # /home/omer/P4/SAI/inc/saihostif.h: 523

sai_create_hostif_user_defined_trap_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 535

sai_remove_hostif_user_defined_trap_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saihostif.h: 548

sai_set_hostif_user_defined_trap_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 559

sai_get_hostif_user_defined_trap_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 572

enum__sai_hostif_type_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 588

SAI_HOSTIF_TYPE_NETDEV = 0 # /home/omer/P4/SAI/inc/saihostif.h: 588

SAI_HOSTIF_TYPE_FD = (SAI_HOSTIF_TYPE_NETDEV + 1) # /home/omer/P4/SAI/inc/saihostif.h: 588

sai_hostif_type_t = enum__sai_hostif_type_t # /home/omer/P4/SAI/inc/saihostif.h: 588

enum__sai_hostif_attr_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_TYPE = SAI_HOSTIF_ATTR_START # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_OBJ_ID = (SAI_HOSTIF_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_NAME = (SAI_HOSTIF_ATTR_OBJ_ID + 1) # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_OPER_STATUS = (SAI_HOSTIF_ATTR_NAME + 1) # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_QUEUE = (SAI_HOSTIF_ATTR_OPER_STATUS + 1) # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_END = (SAI_HOSTIF_ATTR_QUEUE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saihostif.h: 666

SAI_HOSTIF_ATTR_CUSTOM_RANGE_END = (SAI_HOSTIF_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saihostif.h: 666

sai_hostif_attr_t = enum__sai_hostif_attr_t # /home/omer/P4/SAI/inc/saihostif.h: 666

sai_create_hostif_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 678

sai_remove_hostif_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saihostif.h: 691

sai_set_hostif_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 702

sai_get_hostif_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 715

enum__sai_hostif_table_entry_type_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 740

SAI_HOSTIF_TABLE_ENTRY_TYPE_PORT = 0 # /home/omer/P4/SAI/inc/saihostif.h: 740

SAI_HOSTIF_TABLE_ENTRY_TYPE_LAG = (SAI_HOSTIF_TABLE_ENTRY_TYPE_PORT + 1) # /home/omer/P4/SAI/inc/saihostif.h: 740

SAI_HOSTIF_TABLE_ENTRY_TYPE_VLAN = (SAI_HOSTIF_TABLE_ENTRY_TYPE_LAG + 1) # /home/omer/P4/SAI/inc/saihostif.h: 740

SAI_HOSTIF_TABLE_ENTRY_TYPE_TRAP_ID = (SAI_HOSTIF_TABLE_ENTRY_TYPE_VLAN + 1) # /home/omer/P4/SAI/inc/saihostif.h: 740

SAI_HOSTIF_TABLE_ENTRY_TYPE_WILDCARD = (SAI_HOSTIF_TABLE_ENTRY_TYPE_TRAP_ID + 1) # /home/omer/P4/SAI/inc/saihostif.h: 740

sai_hostif_table_entry_type_t = enum__sai_hostif_table_entry_type_t # /home/omer/P4/SAI/inc/saihostif.h: 740

enum__sai_hostif_table_entry_channel_type_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 762

SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_CB = 0 # /home/omer/P4/SAI/inc/saihostif.h: 762

SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_FD = (SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_CB + 1) # /home/omer/P4/SAI/inc/saihostif.h: 762

SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT = (SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_FD + 1) # /home/omer/P4/SAI/inc/saihostif.h: 762

SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_LOGICAL_PORT = (SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_PHYSICAL_PORT + 1) # /home/omer/P4/SAI/inc/saihostif.h: 762

SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_L3 = (SAI_HOSTIF_TABLE_ENTRY_CHANNEL_TYPE_NETDEV_LOGICAL_PORT + 1) # /home/omer/P4/SAI/inc/saihostif.h: 762

sai_hostif_table_entry_channel_type_t = enum__sai_hostif_table_entry_channel_type_t # /home/omer/P4/SAI/inc/saihostif.h: 762

enum__sai_hostif_table_entry_attr_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE = SAI_HOSTIF_ATTR_START # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_OBJ_ID = (SAI_HOSTIF_TABLE_ENTRY_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_TRAP_ID = (SAI_HOSTIF_TABLE_ENTRY_ATTR_OBJ_ID + 1) # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE = (SAI_HOSTIF_TABLE_ENTRY_ATTR_TRAP_ID + 1) # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_HOST_IF = (SAI_HOSTIF_TABLE_ENTRY_ATTR_CHANNEL_TYPE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_END = (SAI_HOSTIF_TABLE_ENTRY_ATTR_HOST_IF + 1) # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saihostif.h: 843

SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_HOSTIF_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saihostif.h: 843

sai_hostif_table_entry_attr_t = enum__sai_hostif_table_entry_attr_t # /home/omer/P4/SAI/inc/saihostif.h: 843

sai_create_hostif_table_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 855

sai_remove_hostif_table_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saihostif.h: 868

sai_set_hostif_table_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 879

sai_get_hostif_table_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 892

enum__sai_hostif_tx_type_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 914

SAI_HOSTIF_TX_TYPE_PIPELINE_BYPASS = 0 # /home/omer/P4/SAI/inc/saihostif.h: 914

SAI_HOSTIF_TX_TYPE_PIPELINE_LOOKUP = (SAI_HOSTIF_TX_TYPE_PIPELINE_BYPASS + 1) # /home/omer/P4/SAI/inc/saihostif.h: 914

SAI_HOSTIF_TX_TYPE_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saihostif.h: 914

sai_hostif_tx_type_t = enum__sai_hostif_tx_type_t # /home/omer/P4/SAI/inc/saihostif.h: 914

enum__sai_hostif_packet_attr_t = c_int # /home/omer/P4/SAI/inc/saihostif.h: 980

SAI_HOSTIF_PACKET_ATTR_START = 0 # /home/omer/P4/SAI/inc/saihostif.h: 980

SAI_HOSTIF_PACKET_ATTR_HOSTIF_TRAP_ID = SAI_HOSTIF_PACKET_ATTR_START # /home/omer/P4/SAI/inc/saihostif.h: 980

SAI_HOSTIF_PACKET_ATTR_INGRESS_PORT = (SAI_HOSTIF_PACKET_ATTR_HOSTIF_TRAP_ID + 1) # /home/omer/P4/SAI/inc/saihostif.h: 980

SAI_HOSTIF_PACKET_ATTR_INGRESS_LAG = (SAI_HOSTIF_PACKET_ATTR_INGRESS_PORT + 1) # /home/omer/P4/SAI/inc/saihostif.h: 980

SAI_HOSTIF_PACKET_ATTR_HOSTIF_TX_TYPE = (SAI_HOSTIF_PACKET_ATTR_INGRESS_LAG + 1) # /home/omer/P4/SAI/inc/saihostif.h: 980

SAI_HOSTIF_PACKET_ATTR_EGRESS_PORT_OR_LAG = (SAI_HOSTIF_PACKET_ATTR_HOSTIF_TX_TYPE + 1) # /home/omer/P4/SAI/inc/saihostif.h: 980

SAI_HOSTIF_PACKET_ATTR_END = (SAI_HOSTIF_PACKET_ATTR_EGRESS_PORT_OR_LAG + 1) # /home/omer/P4/SAI/inc/saihostif.h: 980

sai_hostif_packet_attr_t = enum__sai_hostif_packet_attr_t # /home/omer/P4/SAI/inc/saihostif.h: 980

sai_recv_hostif_packet_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(None), POINTER(sai_size_t), POINTER(c_uint32), POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 996

sai_send_hostif_packet_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(None), sai_size_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 1016

sai_packet_event_notification_fn = CFUNCTYPE(UNCHECKED(None), sai_object_id_t, POINTER(None), sai_size_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saihostif.h: 1032

# /home/omer/P4/SAI/inc/saihostif.h: 1066
class struct__sai_hostif_api_t(Structure):
    pass

struct__sai_hostif_api_t.__slots__ = [
    'create_hostif',
    'remove_hostif',
    'set_hostif_attribute',
    'get_hostif_attribute',
    'create_hostif_table_entry',
    'remove_hostif_table_entry',
    'set_hostif_table_entry_attribute',
    'get_hostif_table_entry_attribute',
    'create_hostif_trap_group',
    'remove_hostif_trap_group',
    'set_hostif_trap_group_attribute',
    'get_hostif_trap_group_attribute',
    'create_hostif_trap',
    'remove_hostif_trap',
    'set_hostif_trap_attribute',
    'get_hostif_trap_attribute',
    'create_hostif_user_defined_trap',
    'remove_hostif_user_defined_trap',
    'set_hostif_user_defined_trap_attribute',
    'get_hostif_user_defined_trap_attribute',
    'recv_hostif_packet',
    'send_hostif_packet',
]
struct__sai_hostif_api_t._fields_ = [
    ('create_hostif', sai_create_hostif_fn),
    ('remove_hostif', sai_remove_hostif_fn),
    ('set_hostif_attribute', sai_set_hostif_attribute_fn),
    ('get_hostif_attribute', sai_get_hostif_attribute_fn),
    ('create_hostif_table_entry', sai_create_hostif_table_entry_fn),
    ('remove_hostif_table_entry', sai_remove_hostif_table_entry_fn),
    ('set_hostif_table_entry_attribute', sai_set_hostif_table_entry_attribute_fn),
    ('get_hostif_table_entry_attribute', sai_get_hostif_table_entry_attribute_fn),
    ('create_hostif_trap_group', sai_create_hostif_trap_group_fn),
    ('remove_hostif_trap_group', sai_remove_hostif_trap_group_fn),
    ('set_hostif_trap_group_attribute', sai_set_hostif_trap_group_attribute_fn),
    ('get_hostif_trap_group_attribute', sai_get_hostif_trap_group_attribute_fn),
    ('create_hostif_trap', sai_create_hostif_trap_fn),
    ('remove_hostif_trap', sai_remove_hostif_trap_fn),
    ('set_hostif_trap_attribute', sai_set_hostif_trap_attribute_fn),
    ('get_hostif_trap_attribute', sai_get_hostif_trap_attribute_fn),
    ('create_hostif_user_defined_trap', sai_create_hostif_user_defined_trap_fn),
    ('remove_hostif_user_defined_trap', sai_remove_hostif_user_defined_trap_fn),
    ('set_hostif_user_defined_trap_attribute', sai_set_hostif_user_defined_trap_attribute_fn),
    ('get_hostif_user_defined_trap_attribute', sai_get_hostif_user_defined_trap_attribute_fn),
    ('recv_hostif_packet', sai_recv_hostif_packet_fn),
    ('send_hostif_packet', sai_send_hostif_packet_fn),
]

sai_hostif_api_t = struct__sai_hostif_api_t # /home/omer/P4/SAI/inc/saihostif.h: 1066

enum__sai_lag_attr_t = c_int # /home/omer/P4/SAI/inc/sailag.h: 100

SAI_LAG_ATTR_START = 0 # /home/omer/P4/SAI/inc/sailag.h: 100

SAI_LAG_ATTR_PORT_LIST = SAI_LAG_ATTR_START # /home/omer/P4/SAI/inc/sailag.h: 100

SAI_LAG_ATTR_INGRESS_ACL = (SAI_LAG_ATTR_PORT_LIST + 1) # /home/omer/P4/SAI/inc/sailag.h: 100

SAI_LAG_ATTR_EGRESS_ACL = (SAI_LAG_ATTR_INGRESS_ACL + 1) # /home/omer/P4/SAI/inc/sailag.h: 100

SAI_LAG_ATTR_END = (SAI_LAG_ATTR_EGRESS_ACL + 1) # /home/omer/P4/SAI/inc/sailag.h: 100

SAI_LAG_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sailag.h: 100

SAI_LAG_ATTR_CUSTOM_RANGE_END = (SAI_LAG_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sailag.h: 100

sai_lag_attr_t = enum__sai_lag_attr_t # /home/omer/P4/SAI/inc/sailag.h: 100

sai_create_lag_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sailag.h: 112

sai_remove_lag_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sailag.h: 125

sai_set_lag_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sailag.h: 136

sai_get_lag_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sailag.h: 149

enum__sai_lag_member_attr_t = c_int # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_START = 0 # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_LAG_ID = SAI_LAG_MEMBER_ATTR_START # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_PORT_ID = (SAI_LAG_MEMBER_ATTR_LAG_ID + 1) # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE = (SAI_LAG_MEMBER_ATTR_PORT_ID + 1) # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_INGRESS_DISABLE = (SAI_LAG_MEMBER_ATTR_EGRESS_DISABLE + 1) # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_END = (SAI_LAG_MEMBER_ATTR_INGRESS_DISABLE + 1) # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sailag.h: 211

SAI_LAG_MEMBER_ATTR_CUSTOM_RANGE_END = (SAI_LAG_MEMBER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sailag.h: 211

sai_lag_member_attr_t = enum__sai_lag_member_attr_t # /home/omer/P4/SAI/inc/sailag.h: 211

sai_create_lag_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sailag.h: 223

sai_remove_lag_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sailag.h: 236

sai_set_lag_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sailag.h: 247

sai_get_lag_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sailag.h: 260

# /home/omer/P4/SAI/inc/sailag.h: 280
class struct__sai_lag_api_t(Structure):
    pass

struct__sai_lag_api_t.__slots__ = [
    'create_lag',
    'remove_lag',
    'set_lag_attribute',
    'get_lag_attribute',
    'create_lag_member',
    'remove_lag_member',
    'set_lag_member_attribute',
    'get_lag_member_attribute',
    'create_lag_members',
    'remove_lag_members',
]
struct__sai_lag_api_t._fields_ = [
    ('create_lag', sai_create_lag_fn),
    ('remove_lag', sai_remove_lag_fn),
    ('set_lag_attribute', sai_set_lag_attribute_fn),
    ('get_lag_attribute', sai_get_lag_attribute_fn),
    ('create_lag_member', sai_create_lag_member_fn),
    ('remove_lag_member', sai_remove_lag_member_fn),
    ('set_lag_member_attribute', sai_set_lag_member_attribute_fn),
    ('get_lag_member_attribute', sai_get_lag_member_attribute_fn),
    ('create_lag_members', sai_bulk_object_create_fn),
    ('remove_lag_members', sai_bulk_object_remove_fn),
]

sai_lag_api_t = struct__sai_lag_api_t # /home/omer/P4/SAI/inc/sailag.h: 280

enum__sai_mirror_session_type_t = c_int # /home/omer/P4/SAI/inc/saimirror.h: 50

SAI_MIRROR_SESSION_TYPE_LOCAL = 0 # /home/omer/P4/SAI/inc/saimirror.h: 50

SAI_MIRROR_SESSION_TYPE_REMOTE = (SAI_MIRROR_SESSION_TYPE_LOCAL + 1) # /home/omer/P4/SAI/inc/saimirror.h: 50

SAI_MIRROR_SESSION_TYPE_ENHANCED_REMOTE = (SAI_MIRROR_SESSION_TYPE_REMOTE + 1) # /home/omer/P4/SAI/inc/saimirror.h: 50

sai_mirror_session_type_t = enum__sai_mirror_session_type_t # /home/omer/P4/SAI/inc/saimirror.h: 50

enum__sai_erspan_encapsulation_type_t = c_int # /home/omer/P4/SAI/inc/saimirror.h: 62

SAI_ERSPAN_ENCAPSULATION_TYPE_MIRROR_L3_GRE_TUNNEL = 0 # /home/omer/P4/SAI/inc/saimirror.h: 62

sai_erspan_encapsulation_type_t = enum__sai_erspan_encapsulation_type_t # /home/omer/P4/SAI/inc/saimirror.h: 62

enum__sai_mirror_session_attr_t = c_int # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_START = 0 # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_TYPE = SAI_MIRROR_SESSION_ATTR_START # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_MONITOR_PORT = (SAI_MIRROR_SESSION_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE = (SAI_MIRROR_SESSION_ATTR_MONITOR_PORT + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_TC = (SAI_MIRROR_SESSION_ATTR_TRUNCATE_SIZE + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_VLAN_TPID = (SAI_MIRROR_SESSION_ATTR_TC + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_VLAN_ID = (SAI_MIRROR_SESSION_ATTR_VLAN_TPID + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_VLAN_PRI = (SAI_MIRROR_SESSION_ATTR_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_VLAN_CFI = (SAI_MIRROR_SESSION_ATTR_VLAN_PRI + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE = (SAI_MIRROR_SESSION_ATTR_VLAN_CFI + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_IPHDR_VERSION = (SAI_MIRROR_SESSION_ATTR_ERSPAN_ENCAPSULATION_TYPE + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_TOS = (SAI_MIRROR_SESSION_ATTR_IPHDR_VERSION + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_TTL = (SAI_MIRROR_SESSION_ATTR_TOS + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_SRC_IP_ADDRESS = (SAI_MIRROR_SESSION_ATTR_TTL + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_DST_IP_ADDRESS = (SAI_MIRROR_SESSION_ATTR_SRC_IP_ADDRESS + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_SRC_MAC_ADDRESS = (SAI_MIRROR_SESSION_ATTR_DST_IP_ADDRESS + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_DST_MAC_ADDRESS = (SAI_MIRROR_SESSION_ATTR_SRC_MAC_ADDRESS + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_GRE_PROTOCOL_TYPE = (SAI_MIRROR_SESSION_ATTR_DST_MAC_ADDRESS + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

SAI_MIRROR_SESSION_ATTR_END = (SAI_MIRROR_SESSION_ATTR_GRE_PROTOCOL_TYPE + 1) # /home/omer/P4/SAI/inc/saimirror.h: 244

sai_mirror_session_attr_t = enum__sai_mirror_session_attr_t # /home/omer/P4/SAI/inc/saimirror.h: 244

sai_create_mirror_session_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saimirror.h: 257

sai_remove_mirror_session_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saimirror.h: 271

sai_set_mirror_session_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saimirror.h: 283

sai_get_mirror_session_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saimirror.h: 297

# /home/omer/P4/SAI/inc/saimirror.h: 312
class struct__sai_mirror_api_t(Structure):
    pass

struct__sai_mirror_api_t.__slots__ = [
    'create_mirror_session',
    'remove_mirror_session',
    'set_mirror_session_attribute',
    'get_mirror_session_attribute',
]
struct__sai_mirror_api_t._fields_ = [
    ('create_mirror_session', sai_create_mirror_session_fn),
    ('remove_mirror_session', sai_remove_mirror_session_fn),
    ('set_mirror_session_attribute', sai_set_mirror_session_attribute_fn),
    ('get_mirror_session_attribute', sai_get_mirror_session_attribute_fn),
]

sai_mirror_api_t = struct__sai_mirror_api_t # /home/omer/P4/SAI/inc/saimirror.h: 312

enum__sai_neighbor_entry_attr_t = c_int # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_START = 0 # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS = SAI_NEIGHBOR_ENTRY_ATTR_START # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_PACKET_ACTION = (SAI_NEIGHBOR_ENTRY_ATTR_DST_MAC_ADDRESS + 1) # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE = (SAI_NEIGHBOR_ENTRY_ATTR_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_META_DATA = (SAI_NEIGHBOR_ENTRY_ATTR_NO_HOST_ROUTE + 1) # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_END = (SAI_NEIGHBOR_ENTRY_ATTR_META_DATA + 1) # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saineighbor.h: 104

SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_NEIGHBOR_ENTRY_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saineighbor.h: 104

sai_neighbor_entry_attr_t = enum__sai_neighbor_entry_attr_t # /home/omer/P4/SAI/inc/saineighbor.h: 104

# /home/omer/P4/SAI/inc/saineighbor.h: 130
class struct__sai_neighbor_entry_t(Structure):
    pass

struct__sai_neighbor_entry_t.__slots__ = [
    'switch_id',
    'rif_id',
    'ip_address',
]
struct__sai_neighbor_entry_t._fields_ = [
    ('switch_id', sai_object_id_t),
    ('rif_id', sai_object_id_t),
    ('ip_address', sai_ip_address_t),
]

sai_neighbor_entry_t = struct__sai_neighbor_entry_t # /home/omer/P4/SAI/inc/saineighbor.h: 130

sai_create_neighbor_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_neighbor_entry_t), c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saineighbor.h: 143

sai_remove_neighbor_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_neighbor_entry_t)) # /home/omer/P4/SAI/inc/saineighbor.h: 157

sai_set_neighbor_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_neighbor_entry_t), POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saineighbor.h: 168

sai_get_neighbor_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_neighbor_entry_t), c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saineighbor.h: 181

sai_remove_all_neighbor_entries_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saineighbor.h: 192

# /home/omer/P4/SAI/inc/saineighbor.h: 206
class struct__sai_neighbor_api_t(Structure):
    pass

struct__sai_neighbor_api_t.__slots__ = [
    'create_neighbor_entry',
    'remove_neighbor_entry',
    'set_neighbor_entry_attribute',
    'get_neighbor_entry_attribute',
    'remove_all_neighbor_entries',
]
struct__sai_neighbor_api_t._fields_ = [
    ('create_neighbor_entry', sai_create_neighbor_entry_fn),
    ('remove_neighbor_entry', sai_remove_neighbor_entry_fn),
    ('set_neighbor_entry_attribute', sai_set_neighbor_entry_attribute_fn),
    ('get_neighbor_entry_attribute', sai_get_neighbor_entry_attribute_fn),
    ('remove_all_neighbor_entries', sai_remove_all_neighbor_entries_fn),
]

sai_neighbor_api_t = struct__sai_neighbor_api_t # /home/omer/P4/SAI/inc/saineighbor.h: 206

enum__sai_next_hop_group_type_t = c_int # /home/omer/P4/SAI/inc/sainexthopgroup.h: 46

SAI_NEXT_HOP_GROUP_TYPE_ECMP = 0 # /home/omer/P4/SAI/inc/sainexthopgroup.h: 46

sai_next_hop_group_type_t = enum__sai_next_hop_group_type_t # /home/omer/P4/SAI/inc/sainexthopgroup.h: 46

enum__sai_next_hop_group_attr_t = c_int # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

SAI_NEXT_HOP_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT = SAI_NEXT_HOP_GROUP_ATTR_START # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_MEMBER_LIST = (SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_COUNT + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

SAI_NEXT_HOP_GROUP_ATTR_TYPE = (SAI_NEXT_HOP_GROUP_ATTR_NEXT_HOP_MEMBER_LIST + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

SAI_NEXT_HOP_GROUP_ATTR_END = (SAI_NEXT_HOP_GROUP_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

SAI_NEXT_HOP_GROUP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

SAI_NEXT_HOP_GROUP_ATTR_CUSTOM_RANGE_END = (SAI_NEXT_HOP_GROUP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

sai_next_hop_group_attr_t = enum__sai_next_hop_group_attr_t # /home/omer/P4/SAI/inc/sainexthopgroup.h: 94

enum__sai_next_hop_group_member_attr_t = c_int # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

SAI_NEXT_HOP_GROUP_MEMBER_ATTR_START = 0 # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID = SAI_NEXT_HOP_GROUP_MEMBER_ATTR_START # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID = (SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_GROUP_ID + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

SAI_NEXT_HOP_GROUP_MEMBER_ATTR_WEIGHT = (SAI_NEXT_HOP_GROUP_MEMBER_ATTR_NEXT_HOP_ID + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

SAI_NEXT_HOP_GROUP_MEMBER_ATTR_END = (SAI_NEXT_HOP_GROUP_MEMBER_ATTR_WEIGHT + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END = (SAI_NEXT_HOP_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

sai_next_hop_group_member_attr_t = enum__sai_next_hop_group_member_attr_t # /home/omer/P4/SAI/inc/sainexthopgroup.h: 141

sai_create_next_hop_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 153

sai_remove_next_hop_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 166

sai_set_next_hop_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 177

sai_get_next_hop_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 190

sai_create_next_hop_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 204

sai_remove_next_hop_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 217

sai_set_next_hop_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 228

sai_get_next_hop_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthopgroup.h: 241

# /home/omer/P4/SAI/inc/sainexthopgroup.h: 261
class struct__sai_next_hop_group_api_t(Structure):
    pass

struct__sai_next_hop_group_api_t.__slots__ = [
    'create_next_hop_group',
    'remove_next_hop_group',
    'set_next_hop_group_attribute',
    'get_next_hop_group_attribute',
    'create_next_hop_group_member',
    'remove_next_hop_group_member',
    'set_next_hop_group_member_attribute',
    'get_next_hop_group_member_attribute',
    'create_next_hop_group_members',
    'remove_next_hop_group_members',
]
struct__sai_next_hop_group_api_t._fields_ = [
    ('create_next_hop_group', sai_create_next_hop_group_fn),
    ('remove_next_hop_group', sai_remove_next_hop_group_fn),
    ('set_next_hop_group_attribute', sai_set_next_hop_group_attribute_fn),
    ('get_next_hop_group_attribute', sai_get_next_hop_group_attribute_fn),
    ('create_next_hop_group_member', sai_create_next_hop_group_member_fn),
    ('remove_next_hop_group_member', sai_remove_next_hop_group_member_fn),
    ('set_next_hop_group_member_attribute', sai_set_next_hop_group_member_attribute_fn),
    ('get_next_hop_group_member_attribute', sai_get_next_hop_group_member_attribute_fn),
    ('create_next_hop_group_members', sai_bulk_object_create_fn),
    ('remove_next_hop_group_members', sai_bulk_object_remove_fn),
]

sai_next_hop_group_api_t = struct__sai_next_hop_group_api_t # /home/omer/P4/SAI/inc/sainexthopgroup.h: 261

enum__sai_next_hop_type_t = c_int # /home/omer/P4/SAI/inc/sainexthop.h: 50

SAI_NEXT_HOP_TYPE_IP = 0 # /home/omer/P4/SAI/inc/sainexthop.h: 50

SAI_NEXT_HOP_TYPE_MPLS = (SAI_NEXT_HOP_TYPE_IP + 1) # /home/omer/P4/SAI/inc/sainexthop.h: 50

SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP = (SAI_NEXT_HOP_TYPE_MPLS + 1) # /home/omer/P4/SAI/inc/sainexthop.h: 50

sai_next_hop_type_t = enum__sai_next_hop_type_t # /home/omer/P4/SAI/inc/sainexthop.h: 50

enum__sai_next_hop_attr_t = c_int # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_START = 0 # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_TYPE = SAI_NEXT_HOP_ATTR_START # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_IP = (SAI_NEXT_HOP_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID = (SAI_NEXT_HOP_ATTR_IP + 1) # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_TUNNEL_ID = (SAI_NEXT_HOP_ATTR_ROUTER_INTERFACE_ID + 1) # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_END = (SAI_NEXT_HOP_ATTR_TUNNEL_ID + 1) # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sainexthop.h: 109

SAI_NEXT_HOP_ATTR_CUSTOM_RANGE_END = (SAI_NEXT_HOP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sainexthop.h: 109

sai_next_hop_attr_t = enum__sai_next_hop_attr_t # /home/omer/P4/SAI/inc/sainexthop.h: 109

sai_create_next_hop_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthop.h: 123

sai_remove_next_hop_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sainexthop.h: 136

sai_set_next_hop_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthop.h: 147

sai_get_next_hop_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sainexthop.h: 160

# /home/omer/P4/SAI/inc/sainexthop.h: 175
class struct__sai_next_hop_api_t(Structure):
    pass

struct__sai_next_hop_api_t.__slots__ = [
    'create_next_hop',
    'remove_next_hop',
    'set_next_hop_attribute',
    'get_next_hop_attribute',
]
struct__sai_next_hop_api_t._fields_ = [
    ('create_next_hop', sai_create_next_hop_fn),
    ('remove_next_hop', sai_remove_next_hop_fn),
    ('set_next_hop_attribute', sai_set_next_hop_attribute_fn),
    ('get_next_hop_attribute', sai_get_next_hop_attribute_fn),
]

sai_next_hop_api_t = struct__sai_next_hop_api_t # /home/omer/P4/SAI/inc/sainexthop.h: 175

# ../../../inc/saimcastfdb.h: 54
class struct__sai_mcast_fdb_entry_t(Structure):
    pass

struct__sai_mcast_fdb_entry_t.__slots__ = [
    'switch_id',
    'mac_address',
    'vlan_id',
]
struct__sai_mcast_fdb_entry_t._fields_ = [
    ('switch_id', sai_object_id_t),
    ('mac_address', sai_mac_t),
    ('vlan_id', sai_vlan_id_t),
]

sai_mcast_fdb_entry_t = struct__sai_mcast_fdb_entry_t # ../../../inc/saimcastfdb.h: 54

enum__sai_mcast_fdb_entry_attr_t = c_int # ../../../inc/saimcastfdb.h: 108

SAI_MCAST_FDB_ENTRY_ATTR_START = 0 # ../../../inc/saimcastfdb.h: 108

SAI_MCAST_FDB_ENTRY_ATTR_GROUP_ID = SAI_MCAST_FDB_ENTRY_ATTR_START # ../../../inc/saimcastfdb.h: 108

SAI_MCAST_FDB_ENTRY_ATTR_PACKET_ACTION = (SAI_MCAST_FDB_ENTRY_ATTR_GROUP_ID + 1) # ../../../inc/saimcastfdb.h: 108

SAI_MCAST_FDB_ENTRY_ATTR_META_DATA = (SAI_MCAST_FDB_ENTRY_ATTR_PACKET_ACTION + 1) # ../../../inc/saimcastfdb.h: 108

SAI_MCAST_FDB_ENTRY_ATTR_END = (SAI_MCAST_FDB_ENTRY_ATTR_META_DATA + 1) # ../../../inc/saimcastfdb.h: 108

SAI_MCAST_FDB_ENTRY_ATTR_CUSTOM_RANGE_START = 268435456 # ../../../inc/saimcastfdb.h: 108

SAI_MCAST_FDB_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_MCAST_FDB_ENTRY_ATTR_CUSTOM_RANGE_START + 1) # ../../../inc/saimcastfdb.h: 108

sai_mcast_fdb_entry_attr_t = enum__sai_mcast_fdb_entry_attr_t # ../../../inc/saimcastfdb.h: 108

sai_create_mcast_fdb_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_mcast_fdb_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/saimcastfdb.h: 119

sai_remove_mcast_fdb_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_mcast_fdb_entry_t)) # ../../../inc/saimcastfdb.h: 131

sai_set_mcast_fdb_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_mcast_fdb_entry_t), POINTER(sai_attribute_t)) # ../../../inc/saimcastfdb.h: 142

sai_get_mcast_fdb_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_mcast_fdb_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/saimcastfdb.h: 155

# ../../../inc/saimcastfdb.h: 170
class struct__sai_mcast_fdb_api_t(Structure):
    pass

struct__sai_mcast_fdb_api_t.__slots__ = [
    'create_mcast_fdb_entry',
    'remove_mcast_fdb_entry',
    'set_mcast_fdb_entry_attribute',
    'get_mcast_fdb_entry_attribute',
]
struct__sai_mcast_fdb_api_t._fields_ = [
    ('create_mcast_fdb_entry', sai_create_mcast_fdb_entry_fn),
    ('remove_mcast_fdb_entry', sai_remove_mcast_fdb_entry_fn),
    ('set_mcast_fdb_entry_attribute', sai_set_mcast_fdb_entry_attribute_fn),
    ('get_mcast_fdb_entry_attribute', sai_get_mcast_fdb_entry_attribute_fn),
]

sai_mcast_fdb_api_t = struct__sai_mcast_fdb_api_t # ../../../inc/saimcastfdb.h: 170

enum__sai_l2mc_entry_type_t = c_int # ../../../inc/sail2mc.h: 47

SAI_L2MC_ENTRY_TYPE_SG = 0 # ../../../inc/sail2mc.h: 47

SAI_L2MC_ENTRY_TYPE_XG = (SAI_L2MC_ENTRY_TYPE_SG + 1) # ../../../inc/sail2mc.h: 47

sai_l2mc_entry_type_t = enum__sai_l2mc_entry_type_t # ../../../inc/sail2mc.h: 47

# ../../../inc/sail2mc.h: 82
class struct__sai_l2mc_entry_t(Structure):
    pass

struct__sai_l2mc_entry_t.__slots__ = [
    'switch_id',
    'bridge_type',
    'vlan_id',
    'bridge_id',
    'type',
    'destination',
    'source',
]
struct__sai_l2mc_entry_t._fields_ = [
    ('switch_id', sai_object_id_t),
    ('bridge_type', sai_fdb_entry_bridge_type_t),
    ('vlan_id', sai_vlan_id_t),
    ('bridge_id', sai_object_id_t),
    ('type', sai_l2mc_entry_type_t),
    ('destination', sai_ip_address_t),
    ('source', sai_ip_address_t),
]

sai_l2mc_entry_t = struct__sai_l2mc_entry_t # ../../../inc/sail2mc.h: 82

enum__sai_l2mc_entry_attr_t = c_int # ../../../inc/sail2mc.h: 128

SAI_L2MC_ENTRY_ATTR_START = 0 # ../../../inc/sail2mc.h: 128

SAI_L2MC_ENTRY_ATTR_PACKET_ACTION = SAI_L2MC_ENTRY_ATTR_START # ../../../inc/sail2mc.h: 128

SAI_L2MC_ENTRY_ATTR_OUTPUT_GROUP_ID = (SAI_L2MC_ENTRY_ATTR_PACKET_ACTION + 1) # ../../../inc/sail2mc.h: 128

SAI_L2MC_ENTRY_ATTR_END = (SAI_L2MC_ENTRY_ATTR_OUTPUT_GROUP_ID + 1) # ../../../inc/sail2mc.h: 128

SAI_L2MC_ENTRY_ATTR_CUSTOM_RANGE_BASE = 268435456 # ../../../inc/sail2mc.h: 128

SAI_L2MC_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_L2MC_ENTRY_ATTR_CUSTOM_RANGE_BASE + 1) # ../../../inc/sail2mc.h: 128

sai_l2mc_entry_attr_t = enum__sai_l2mc_entry_attr_t # ../../../inc/sail2mc.h: 128

sai_create_l2mc_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_l2mc_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/sail2mc.h: 139

sai_remove_l2mc_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_l2mc_entry_t)) # ../../../inc/sail2mc.h: 151

sai_set_l2mc_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_l2mc_entry_t), POINTER(sai_attribute_t)) # ../../../inc/sail2mc.h: 162

sai_get_l2mc_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_l2mc_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/sail2mc.h: 175

# ../../../inc/sail2mc.h: 190
class struct__sai_l2mc_api_t(Structure):
    pass

struct__sai_l2mc_api_t.__slots__ = [
    'create_l2mc_entry',
    'remove_l2mc_entry',
    'set_l2mc_entry_attribute',
    'get_l2mc_entry_attribute',
]
struct__sai_l2mc_api_t._fields_ = [
    ('create_l2mc_entry', sai_create_l2mc_entry_fn),
    ('remove_l2mc_entry', sai_remove_l2mc_entry_fn),
    ('set_l2mc_entry_attribute', sai_set_l2mc_entry_attribute_fn),
    ('get_l2mc_entry_attribute', sai_get_l2mc_entry_attribute_fn),
]

sai_l2mc_api_t = struct__sai_l2mc_api_t # ../../../inc/sail2mc.h: 190

enum__sai_ipmc_entry_type_t = c_int # ../../../inc/saiipmc.h: 47

SAI_IPMC_ENTRY_TYPE_SG = 0 # ../../../inc/saiipmc.h: 47

SAI_IPMC_ENTRY_TYPE_XG = (SAI_IPMC_ENTRY_TYPE_SG + 1) # ../../../inc/saiipmc.h: 47

sai_ipmc_entry_type_t = enum__sai_ipmc_entry_type_t # ../../../inc/saiipmc.h: 47

# ../../../inc/saiipmc.h: 76
class struct__sai_ipmc_entry_t(Structure):
    pass

struct__sai_ipmc_entry_t.__slots__ = [
    'switch_id',
    'vr_id',
    'type',
    'destination',
    'source',
]
struct__sai_ipmc_entry_t._fields_ = [
    ('switch_id', sai_object_id_t),
    ('vr_id', sai_object_id_t),
    ('type', sai_ipmc_entry_type_t),
    ('destination', sai_ip_address_t),
    ('source', sai_ip_address_t),
]

sai_ipmc_entry_t = struct__sai_ipmc_entry_t # ../../../inc/saiipmc.h: 76

enum__sai_ipmc_entry_attr_t = c_int # ../../../inc/saiipmc.h: 133

SAI_IPMC_ENTRY_ATTR_START = 0 # ../../../inc/saiipmc.h: 133

SAI_IPMC_ENTRY_ATTR_PACKET_ACTION = SAI_IPMC_ENTRY_ATTR_START # ../../../inc/saiipmc.h: 133

SAI_IPMC_ENTRY_ATTR_OUTPUT_GROUP_ID = (SAI_IPMC_ENTRY_ATTR_PACKET_ACTION + 1) # ../../../inc/saiipmc.h: 133

SAI_IPMC_ENTRY_ATTR_RPF_GROUP_ID = (SAI_IPMC_ENTRY_ATTR_OUTPUT_GROUP_ID + 1) # ../../../inc/saiipmc.h: 133

SAI_IPMC_ENTRY_ATTR_END = (SAI_IPMC_ENTRY_ATTR_RPF_GROUP_ID + 1) # ../../../inc/saiipmc.h: 133

SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_BASE = 268435456 # ../../../inc/saiipmc.h: 133

SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_IPMC_ENTRY_ATTR_CUSTOM_RANGE_BASE + 1) # ../../../inc/saiipmc.h: 133

sai_ipmc_entry_attr_t = enum__sai_ipmc_entry_attr_t # ../../../inc/saiipmc.h: 133

sai_create_ipmc_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_ipmc_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/saiipmc.h: 144

sai_remove_ipmc_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_ipmc_entry_t)) # ../../../inc/saiipmc.h: 156

sai_set_ipmc_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_ipmc_entry_t), POINTER(sai_attribute_t)) # ../../../inc/saiipmc.h: 167

sai_get_ipmc_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_ipmc_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/saiipmc.h: 180

# ../../../inc/saiipmc.h: 195
class struct__sai_ipmc_api_t(Structure):
    pass

struct__sai_ipmc_api_t.__slots__ = [
    'create_ipmc_entry',
    'remove_ipmc_entry',
    'set_ipmc_entry_attribute',
    'get_ipmc_entry_attribute',
]
struct__sai_ipmc_api_t._fields_ = [
    ('create_ipmc_entry', sai_create_ipmc_entry_fn),
    ('remove_ipmc_entry', sai_remove_ipmc_entry_fn),
    ('set_ipmc_entry_attribute', sai_set_ipmc_entry_attribute_fn),
    ('get_ipmc_entry_attribute', sai_get_ipmc_entry_attribute_fn),
]

sai_ipmc_api_t = struct__sai_ipmc_api_t # ../../../inc/saiipmc.h: 195

enum__sai_route_entry_attr_t = c_int # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_START = 0 # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_PACKET_ACTION = SAI_ROUTE_ENTRY_ATTR_START # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_TRAP_PRIORITY = (SAI_ROUTE_ENTRY_ATTR_PACKET_ACTION + 1) # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID = (SAI_ROUTE_ENTRY_ATTR_TRAP_PRIORITY + 1) # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_META_DATA = (SAI_ROUTE_ENTRY_ATTR_NEXT_HOP_ID + 1) # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_END = (SAI_ROUTE_ENTRY_ATTR_META_DATA + 1) # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_START = 268435456 # ../../../inc/sairoute.h: 112

SAI_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_ROUTE_ENTRY_ATTR_CUSTOM_RANGE_START + 1) # ../../../inc/sairoute.h: 112

sai_route_entry_attr_t = enum__sai_route_entry_attr_t # ../../../inc/sairoute.h: 112

# ../../../inc/sairoute.h: 138
class struct__sai_route_entry_t(Structure):
    pass

struct__sai_route_entry_t.__slots__ = [
    'switch_id',
    'vr_id',
    'destination',
]
struct__sai_route_entry_t._fields_ = [
    ('switch_id', sai_object_id_t),
    ('vr_id', sai_object_id_t),
    ('destination', sai_ip_prefix_t),
]

sai_route_entry_t = struct__sai_route_entry_t # ../../../inc/sairoute.h: 138

sai_create_route_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_route_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/sairoute.h: 151

sai_remove_route_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_route_entry_t)) # ../../../inc/sairoute.h: 165

sai_set_route_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_route_entry_t), POINTER(sai_attribute_t)) # ../../../inc/sairoute.h: 176

sai_get_route_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_route_entry_t), c_uint32, POINTER(sai_attribute_t)) # ../../../inc/sairoute.h: 189

# ../../../inc/sairoute.h: 204
class struct__sai_route_api_t(Structure):
    pass

struct__sai_route_api_t.__slots__ = [
    'create_route_entry',
    'remove_route_entry',
    'set_route_entry_attribute',
    'get_route_entry_attribute',
]
struct__sai_route_api_t._fields_ = [
    ('create_route_entry', sai_create_route_entry_fn),
    ('remove_route_entry', sai_remove_route_entry_fn),
    ('set_route_entry_attribute', sai_set_route_entry_attribute_fn),
    ('get_route_entry_attribute', sai_get_route_entry_attribute_fn),
]

sai_route_api_t = struct__sai_route_api_t # ../../../inc/sairoute.h: 204

# /home/omer/P4/SAI/inc/saiobject.h: 49
class union_anon_21(Union):
    pass

union_anon_21.__slots__ = [
    'object_id',
    'fdb_entry',
    'neighbor_entry',
    'route_entry',
    'mcast_fdb_entry',
    'l2mc_entry',
    'ipmc_entry',
]
union_anon_21._fields_ = [
    ('object_id', sai_object_id_t),
    ('fdb_entry', sai_fdb_entry_t),
    ('neighbor_entry', sai_neighbor_entry_t),
    ('route_entry', sai_route_entry_t),
    ('mcast_fdb_entry', sai_mcast_fdb_entry_t),
    ('l2mc_entry', sai_l2mc_entry_t),
    ('ipmc_entry', sai_ipmc_entry_t),
]

# /home/omer/P4/SAI/inc/saiobject.h: 61
class struct__sai_object_key_t(Structure):
    pass

struct__sai_object_key_t.__slots__ = [
    'key',
]
struct__sai_object_key_t._fields_ = [
    ('key', union_anon_21),
]

sai_object_key_t = struct__sai_object_key_t # /home/omer/P4/SAI/inc/saiobject.h: 61

# /home/omer/P4/SAI/inc/saiobject.h: 72
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_get_maximum_attribute_count'):
        continue
    sai_get_maximum_attribute_count = _lib.sai_get_maximum_attribute_count
    sai_get_maximum_attribute_count.argtypes = [sai_object_id_t, sai_object_type_t, POINTER(c_uint32)]
    sai_get_maximum_attribute_count.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/saiobject.h: 86
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_get_object_count'):
        continue
    sai_get_object_count = _lib.sai_get_object_count
    sai_get_object_count.argtypes = [sai_object_id_t, sai_object_type_t, POINTER(c_uint32)]
    sai_get_object_count.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/saiobject.h: 101
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_get_object_key'):
        continue
    sai_get_object_key = _lib.sai_get_object_key
    sai_get_object_key.argtypes = [sai_object_id_t, sai_object_type_t, c_uint32, POINTER(sai_object_key_t)]
    sai_get_object_key.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/saiobject.h: 136
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_bulk_get_attribute'):
        continue
    sai_bulk_get_attribute = _lib.sai_bulk_get_attribute
    sai_bulk_get_attribute.argtypes = [sai_object_id_t, sai_object_type_t, c_uint32, POINTER(sai_object_key_t), POINTER(c_uint32), POINTER(POINTER(sai_attribute_t)), POINTER(sai_status_t)]
    sai_bulk_get_attribute.restype = sai_status_t
    break

enum__sai_meter_type_t = c_int # /home/omer/P4/SAI/inc/saipolicer.h: 50

SAI_METER_TYPE_PACKETS = 0 # /home/omer/P4/SAI/inc/saipolicer.h: 50

SAI_METER_TYPE_BYTES = 1 # /home/omer/P4/SAI/inc/saipolicer.h: 50

SAI_METER_TYPE_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saipolicer.h: 50

sai_meter_type_t = enum__sai_meter_type_t # /home/omer/P4/SAI/inc/saipolicer.h: 50

enum__sai_policer_mode_t = c_int # /home/omer/P4/SAI/inc/saipolicer.h: 69

SAI_POLICER_MODE_SR_TCM = 0 # /home/omer/P4/SAI/inc/saipolicer.h: 69

SAI_POLICER_MODE_TR_TCM = 1 # /home/omer/P4/SAI/inc/saipolicer.h: 69

SAI_POLICER_MODE_STORM_CONTROL = 2 # /home/omer/P4/SAI/inc/saipolicer.h: 69

SAI_POLICER_MODE_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saipolicer.h: 69

sai_policer_mode_t = enum__sai_policer_mode_t # /home/omer/P4/SAI/inc/saipolicer.h: 69

enum__sai_policer_color_source_t = c_int # /home/omer/P4/SAI/inc/saipolicer.h: 85

SAI_POLICER_COLOR_SOURCE_BLIND = 0 # /home/omer/P4/SAI/inc/saipolicer.h: 85

SAI_POLICER_COLOR_SOURCE_AWARE = 1 # /home/omer/P4/SAI/inc/saipolicer.h: 85

SAI_POLICER_COLOR_SOURCE_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saipolicer.h: 85

sai_policer_color_source_t = enum__sai_policer_color_source_t # /home/omer/P4/SAI/inc/saipolicer.h: 85

enum__sai_policer_attr_t = c_int # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_START = 0 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_METER_TYPE = SAI_POLICER_ATTR_START # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_MODE = 1 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_COLOR_SOURCE = 2 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_CBS = 3 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_CIR = 4 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_PBS = 5 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_PIR = 6 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_GREEN_PACKET_ACTION = 7 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_YELLOW_PACKET_ACTION = 8 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_RED_PACKET_ACTION = 9 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_ENABLE_COUNTER_PACKET_ACTION_LIST = 10 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_END = (SAI_POLICER_ATTR_ENABLE_COUNTER_PACKET_ACTION_LIST + 1) # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saipolicer.h: 213

SAI_POLICER_ATTR_CUSTOM_RANGE_END = (SAI_POLICER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saipolicer.h: 213

sai_policer_attr_t = enum__sai_policer_attr_t # /home/omer/P4/SAI/inc/saipolicer.h: 213

enum__sai_policer_stat_t = c_int # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_PACKETS = 0 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_ATTR_BYTES = 1 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_GREEN_PACKETS = 2 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_GREEN_BYTES = 3 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_YELLOW_PACKETS = 4 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_YELLOW_BYTES = 5 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_RED_PACKETS = 6 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_RED_BYTES = 7 # /home/omer/P4/SAI/inc/saipolicer.h: 247

SAI_POLICER_STAT_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saipolicer.h: 247

sai_policer_stat_t = enum__sai_policer_stat_t # /home/omer/P4/SAI/inc/saipolicer.h: 247

sai_create_policer_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saipolicer.h: 259

sai_remove_policer_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saipolicer.h: 272

sai_set_policer_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saipolicer.h: 283

sai_get_policer_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saipolicer.h: 296

sai_get_policer_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_policer_stat_t), c_uint32, POINTER(c_uint64)) # /home/omer/P4/SAI/inc/saipolicer.h: 311

sai_clear_policer_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_policer_stat_t)) # /home/omer/P4/SAI/inc/saipolicer.h: 327

# /home/omer/P4/SAI/inc/saipolicer.h: 344
class struct__sai_policer_api_t(Structure):
    pass

struct__sai_policer_api_t.__slots__ = [
    'create_policer',
    'remove_policer',
    'set_policer_attribute',
    'get_policer_attribute',
    'get_policer_stats',
    'clear_policer_stats',
]
struct__sai_policer_api_t._fields_ = [
    ('create_policer', sai_create_policer_fn),
    ('remove_policer', sai_remove_policer_fn),
    ('set_policer_attribute', sai_set_policer_attribute_fn),
    ('get_policer_attribute', sai_get_policer_attribute_fn),
    ('get_policer_stats', sai_get_policer_stats_fn),
    ('clear_policer_stats', sai_clear_policer_stats_fn),
]

sai_policer_api_t = struct__sai_policer_api_t # /home/omer/P4/SAI/inc/saipolicer.h: 344

enum__sai_port_type_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 47

SAI_PORT_TYPE_LOGICAL = 0 # /home/omer/P4/SAI/inc/saiport.h: 47

SAI_PORT_TYPE_CPU = (SAI_PORT_TYPE_LOGICAL + 1) # /home/omer/P4/SAI/inc/saiport.h: 47

sai_port_type_t = enum__sai_port_type_t # /home/omer/P4/SAI/inc/saiport.h: 47

enum__sai_port_bind_mode_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 60

SAI_PORT_BIND_MODE_PORT = 0 # /home/omer/P4/SAI/inc/saiport.h: 60

SAI_PORT_BIND_MODE_SUB_PORT = (SAI_PORT_BIND_MODE_PORT + 1) # /home/omer/P4/SAI/inc/saiport.h: 60

sai_port_bind_mode_t = enum__sai_port_bind_mode_t # /home/omer/P4/SAI/inc/saiport.h: 60

enum__sai_port_oper_status_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 82

SAI_PORT_OPER_STATUS_UNKNOWN = 0 # /home/omer/P4/SAI/inc/saiport.h: 82

SAI_PORT_OPER_STATUS_UP = (SAI_PORT_OPER_STATUS_UNKNOWN + 1) # /home/omer/P4/SAI/inc/saiport.h: 82

SAI_PORT_OPER_STATUS_DOWN = (SAI_PORT_OPER_STATUS_UP + 1) # /home/omer/P4/SAI/inc/saiport.h: 82

SAI_PORT_OPER_STATUS_TESTING = (SAI_PORT_OPER_STATUS_DOWN + 1) # /home/omer/P4/SAI/inc/saiport.h: 82

SAI_PORT_OPER_STATUS_NOT_PRESENT = (SAI_PORT_OPER_STATUS_TESTING + 1) # /home/omer/P4/SAI/inc/saiport.h: 82

sai_port_oper_status_t = enum__sai_port_oper_status_t # /home/omer/P4/SAI/inc/saiport.h: 82

# /home/omer/P4/SAI/inc/saiport.h: 95
class struct__sai_port_oper_status_notification_t(Structure):
    pass

struct__sai_port_oper_status_notification_t.__slots__ = [
    'port_id',
    'port_state',
]
struct__sai_port_oper_status_notification_t._fields_ = [
    ('port_id', sai_object_id_t),
    ('port_state', sai_port_oper_status_t),
]

sai_port_oper_status_notification_t = struct__sai_port_oper_status_notification_t # /home/omer/P4/SAI/inc/saiport.h: 95

enum__sai_port_flow_control_mode_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 114

SAI_PORT_FLOW_CONTROL_MODE_DISABLE = 0 # /home/omer/P4/SAI/inc/saiport.h: 114

SAI_PORT_FLOW_CONTROL_MODE_TX_ONLY = (SAI_PORT_FLOW_CONTROL_MODE_DISABLE + 1) # /home/omer/P4/SAI/inc/saiport.h: 114

SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY = (SAI_PORT_FLOW_CONTROL_MODE_TX_ONLY + 1) # /home/omer/P4/SAI/inc/saiport.h: 114

SAI_PORT_FLOW_CONTROL_MODE_BOTH_ENABLE = (SAI_PORT_FLOW_CONTROL_MODE_RX_ONLY + 1) # /home/omer/P4/SAI/inc/saiport.h: 114

sai_port_flow_control_mode_t = enum__sai_port_flow_control_mode_t # /home/omer/P4/SAI/inc/saiport.h: 114

enum__sai_port_internal_loopback_mode_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 130

SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE = 0 # /home/omer/P4/SAI/inc/saiport.h: 130

SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY = (SAI_PORT_INTERNAL_LOOPBACK_MODE_NONE + 1) # /home/omer/P4/SAI/inc/saiport.h: 130

SAI_PORT_INTERNAL_LOOPBACK_MODE_MAC = (SAI_PORT_INTERNAL_LOOPBACK_MODE_PHY + 1) # /home/omer/P4/SAI/inc/saiport.h: 130

sai_port_internal_loopback_mode_t = enum__sai_port_internal_loopback_mode_t # /home/omer/P4/SAI/inc/saiport.h: 130

enum__sai_port_media_type_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 148

SAI_PORT_MEDIA_TYPE_NOT_PRESENT = 0 # /home/omer/P4/SAI/inc/saiport.h: 148

SAI_PORT_MEDIA_TYPE_UNKNONWN = (SAI_PORT_MEDIA_TYPE_NOT_PRESENT + 1) # /home/omer/P4/SAI/inc/saiport.h: 148

SAI_PORT_MEDIA_TYPE_FIBER = (SAI_PORT_MEDIA_TYPE_UNKNONWN + 1) # /home/omer/P4/SAI/inc/saiport.h: 148

SAI_PORT_MEDIA_TYPE_COPPER = (SAI_PORT_MEDIA_TYPE_FIBER + 1) # /home/omer/P4/SAI/inc/saiport.h: 148

sai_port_media_type_t = enum__sai_port_media_type_t # /home/omer/P4/SAI/inc/saiport.h: 148

enum__sai_port_breakout_mode_type_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 167

SAI_PORT_BREAKOUT_MODE_TYPE_1_LANE = 0 # /home/omer/P4/SAI/inc/saiport.h: 167

SAI_PORT_BREAKOUT_MODE_TYPE_2_LANE = 1 # /home/omer/P4/SAI/inc/saiport.h: 167

SAI_PORT_BREAKOUT_MODE_TYPE_4_LANE = 2 # /home/omer/P4/SAI/inc/saiport.h: 167

SAI_PORT_BREAKOUT_MODE_TYPE_MAX = (SAI_PORT_BREAKOUT_MODE_TYPE_4_LANE + 1) # /home/omer/P4/SAI/inc/saiport.h: 167

sai_port_breakout_mode_type_t = enum__sai_port_breakout_mode_type_t # /home/omer/P4/SAI/inc/saiport.h: 167

enum__sai_port_fec_mode_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 182

SAI_PORT_FEC_MODE_NONE = 0 # /home/omer/P4/SAI/inc/saiport.h: 182

SAI_PORT_FEC_MODE_RS = (SAI_PORT_FEC_MODE_NONE + 1) # /home/omer/P4/SAI/inc/saiport.h: 182

SAI_PORT_FEC_MODE_FC = (SAI_PORT_FEC_MODE_RS + 1) # /home/omer/P4/SAI/inc/saiport.h: 182

sai_port_fec_mode_t = enum__sai_port_fec_mode_t # /home/omer/P4/SAI/inc/saiport.h: 182

enum__sai_port_attr_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_TYPE = SAI_PORT_ATTR_START # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_OPER_STATUS = (SAI_PORT_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE_TYPE = (SAI_PORT_ATTR_OPER_STATUS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE_TYPE = (SAI_PORT_ATTR_SUPPORTED_BREAKOUT_MODE_TYPE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES = (SAI_PORT_ATTR_CURRENT_BREAKOUT_MODE_TYPE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_QUEUE_LIST = (SAI_PORT_ATTR_QOS_NUMBER_OF_QUEUES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS = (SAI_PORT_ATTR_QOS_QUEUE_LIST + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST = (SAI_PORT_ATTR_QOS_NUMBER_OF_SCHEDULER_GROUPS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_SPEED = (SAI_PORT_ATTR_QOS_SCHEDULER_GROUP_LIST + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_FEC_MODE = (SAI_PORT_ATTR_SUPPORTED_SPEED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_HALF_DUPLEX_SPEED = (SAI_PORT_ATTR_SUPPORTED_FEC_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_AUTO_NEG_MODE = (SAI_PORT_ATTR_SUPPORTED_HALF_DUPLEX_SPEED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_FLOW_CONTROL_MODE = (SAI_PORT_ATTR_SUPPORTED_AUTO_NEG_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_ASYMMETRIC_PAUSE_MODE = (SAI_PORT_ATTR_SUPPORTED_FLOW_CONTROL_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SUPPORTED_MEDIA_TYPE = (SAI_PORT_ATTR_SUPPORTED_ASYMMETRIC_PAUSE_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_REMOTE_ADVERTISED_SPEED = (SAI_PORT_ATTR_SUPPORTED_MEDIA_TYPE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_REMOTE_ADVERTISED_FEC_MODE = (SAI_PORT_ATTR_REMOTE_ADVERTISED_SPEED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_REMOTE_ADVERTISED_HALF_DUPLEX_SPEED = (SAI_PORT_ATTR_REMOTE_ADVERTISED_FEC_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_REMOTE_ADVERTISED_AUTO_NEG_MODE = (SAI_PORT_ATTR_REMOTE_ADVERTISED_HALF_DUPLEX_SPEED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_REMOTE_ADVERTISED_FLOW_CONTROL_MODE = (SAI_PORT_ATTR_REMOTE_ADVERTISED_AUTO_NEG_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_REMOTE_ADVERTISED_ASYMMETRIC_PAUSE_MODE = (SAI_PORT_ATTR_REMOTE_ADVERTISED_FLOW_CONTROL_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_REMOTE_ADVERTISED_MEDIA_TYPE = (SAI_PORT_ATTR_REMOTE_ADVERTISED_ASYMMETRIC_PAUSE_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_NUMBER_OF_INGRESS_PRIORITY_GROUPS = (SAI_PORT_ATTR_REMOTE_ADVERTISED_MEDIA_TYPE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_INGRESS_PRIORITY_GROUP_LIST = (SAI_PORT_ATTR_NUMBER_OF_INGRESS_PRIORITY_GROUPS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_HW_LANE_LIST = (SAI_PORT_ATTR_INGRESS_PRIORITY_GROUP_LIST + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_SPEED = (SAI_PORT_ATTR_HW_LANE_LIST + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_FULL_DUPLEX_MODE = (SAI_PORT_ATTR_SPEED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_AUTO_NEG_MODE = (SAI_PORT_ATTR_FULL_DUPLEX_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADMIN_STATE = (SAI_PORT_ATTR_AUTO_NEG_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_MEDIA_TYPE = (SAI_PORT_ATTR_ADMIN_STATE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADVERTISED_SPEED = (SAI_PORT_ATTR_MEDIA_TYPE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADVERTISED_FEC_MODE = (SAI_PORT_ATTR_ADVERTISED_SPEED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADVERTISED_HALF_DUPLEX_SPEED = (SAI_PORT_ATTR_ADVERTISED_FEC_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADVERTISED_AUTO_NEG_MODE = (SAI_PORT_ATTR_ADVERTISED_HALF_DUPLEX_SPEED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADVERTISED_FLOW_CONTROL_MODE = (SAI_PORT_ATTR_ADVERTISED_AUTO_NEG_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADVERTISED_ASYMMETRIC_PAUSE_MODE = (SAI_PORT_ATTR_ADVERTISED_FLOW_CONTROL_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_ADVERTISED_MEDIA_TYPE = (SAI_PORT_ATTR_ADVERTISED_ASYMMETRIC_PAUSE_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_PORT_VLAN_ID = (SAI_PORT_ATTR_ADVERTISED_MEDIA_TYPE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY = (SAI_PORT_ATTR_PORT_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_INGRESS_FILTERING = (SAI_PORT_ATTR_DEFAULT_VLAN_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_DROP_UNTAGGED = (SAI_PORT_ATTR_INGRESS_FILTERING + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_DROP_TAGGED = (SAI_PORT_ATTR_DROP_UNTAGGED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE = (SAI_PORT_ATTR_DROP_TAGGED + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_FEC_MODE = (SAI_PORT_ATTR_INTERNAL_LOOPBACK_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_UPDATE_DSCP = (SAI_PORT_ATTR_FEC_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_MTU = (SAI_PORT_ATTR_UPDATE_DSCP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID = (SAI_PORT_ATTR_MTU + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID = (SAI_PORT_ATTR_FLOOD_STORM_CONTROL_POLICER_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID = (SAI_PORT_ATTR_BROADCAST_STORM_CONTROL_POLICER_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE = (SAI_PORT_ATTR_MULTICAST_STORM_CONTROL_POLICER_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_INGRESS_ACL = (SAI_PORT_ATTR_GLOBAL_FLOW_CONTROL_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_EGRESS_ACL = (SAI_PORT_ATTR_INGRESS_ACL + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_INGRESS_MIRROR_SESSION = (SAI_PORT_ATTR_EGRESS_ACL + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_EGRESS_MIRROR_SESSION = (SAI_PORT_ATTR_INGRESS_MIRROR_SESSION + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE = (SAI_PORT_ATTR_EGRESS_MIRROR_SESSION + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE = (SAI_PORT_ATTR_INGRESS_SAMPLEPACKET_ENABLE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_POLICER_ID = (SAI_PORT_ATTR_EGRESS_SAMPLEPACKET_ENABLE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_DEFAULT_TC = (SAI_PORT_ATTR_POLICER_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_DOT1P_TO_TC_MAP = (SAI_PORT_ATTR_QOS_DEFAULT_TC + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_DOT1P_TO_COLOR_MAP = (SAI_PORT_ATTR_QOS_DOT1P_TO_TC_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_DSCP_TO_TC_MAP = (SAI_PORT_ATTR_QOS_DOT1P_TO_COLOR_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_DSCP_TO_COLOR_MAP = (SAI_PORT_ATTR_QOS_DSCP_TO_TC_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP = (SAI_PORT_ATTR_QOS_DSCP_TO_COLOR_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP = (SAI_PORT_ATTR_QOS_TC_TO_QUEUE_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP = (SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_TC_TO_PRIORITY_GROUP_MAP = (SAI_PORT_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_PRIORITY_GROUP_MAP = (SAI_PORT_ATTR_QOS_TC_TO_PRIORITY_GROUP_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_QUEUE_MAP = (SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_PRIORITY_GROUP_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_WRED_PROFILE_ID = (SAI_PORT_ATTR_QOS_PFC_PRIORITY_TO_QUEUE_MAP + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID = (SAI_PORT_ATTR_QOS_WRED_PROFILE_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_INGRESS_BUFFER_PROFILE_LIST = (SAI_PORT_ATTR_QOS_SCHEDULER_PROFILE_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_QOS_EGRESS_BUFFER_PROFILE_LIST = (SAI_PORT_ATTR_QOS_INGRESS_BUFFER_PROFILE_LIST + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL = (SAI_PORT_ATTR_QOS_EGRESS_BUFFER_PROFILE_LIST + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_META_DATA = (SAI_PORT_ATTR_PRIORITY_FLOW_CONTROL + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST = (SAI_PORT_ATTR_META_DATA + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_HW_PROFILE_ID = (SAI_PORT_ATTR_EGRESS_BLOCK_PORT_LIST + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_EEE_ENABLE = (SAI_PORT_ATTR_HW_PROFILE_ID + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_EEE_IDLE_TIME = (SAI_PORT_ATTR_EEE_ENABLE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_EEE_WAKE_TIME = (SAI_PORT_ATTR_EEE_IDLE_TIME + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_BIND_MODE = (SAI_PORT_ATTR_EEE_WAKE_TIME + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_END = (SAI_PORT_ATTR_BIND_MODE + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiport.h: 1051

SAI_PORT_ATTR_CUSTOM_RANGE_END = (SAI_PORT_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiport.h: 1051

sai_port_attr_t = enum__sai_port_attr_t # /home/omer/P4/SAI/inc/saiport.h: 1051

enum__sai_port_stat_t = c_int # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_OCTETS = 0 # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_UCAST_PKTS = (SAI_PORT_STAT_IF_IN_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_NON_UCAST_PKTS = (SAI_PORT_STAT_IF_IN_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_DISCARDS = (SAI_PORT_STAT_IF_IN_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_ERRORS = (SAI_PORT_STAT_IF_IN_DISCARDS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_UNKNOWN_PROTOS = (SAI_PORT_STAT_IF_IN_ERRORS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_BROADCAST_PKTS = (SAI_PORT_STAT_IF_IN_UNKNOWN_PROTOS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_MULTICAST_PKTS = (SAI_PORT_STAT_IF_IN_BROADCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_IN_VLAN_DISCARDS = (SAI_PORT_STAT_IF_IN_MULTICAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_OCTETS = (SAI_PORT_STAT_IF_IN_VLAN_DISCARDS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_UCAST_PKTS = (SAI_PORT_STAT_IF_OUT_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_NON_UCAST_PKTS = (SAI_PORT_STAT_IF_OUT_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_DISCARDS = (SAI_PORT_STAT_IF_OUT_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_ERRORS = (SAI_PORT_STAT_IF_OUT_DISCARDS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_QLEN = (SAI_PORT_STAT_IF_OUT_ERRORS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_BROADCAST_PKTS = (SAI_PORT_STAT_IF_OUT_QLEN + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IF_OUT_MULTICAST_PKTS = (SAI_PORT_STAT_IF_OUT_BROADCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_DROP_EVENTS = (SAI_PORT_STAT_IF_OUT_MULTICAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_MULTICAST_PKTS = (SAI_PORT_STAT_ETHER_STATS_DROP_EVENTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_BROADCAST_PKTS = (SAI_PORT_STAT_ETHER_STATS_MULTICAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_UNDERSIZE_PKTS = (SAI_PORT_STAT_ETHER_STATS_BROADCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_FRAGMENTS = (SAI_PORT_STAT_ETHER_STATS_UNDERSIZE_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_64_OCTETS = (SAI_PORT_STAT_ETHER_STATS_FRAGMENTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_65_TO_127_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_64_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_128_TO_255_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_65_TO_127_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_256_TO_511_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_128_TO_255_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_512_TO_1023_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_256_TO_511_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_1024_TO_1518_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_512_TO_1023_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_1519_TO_2047_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_1024_TO_1518_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_2048_TO_4095_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_1519_TO_2047_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_4096_TO_9216_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_2048_TO_4095_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS_9217_TO_16383_OCTETS = (SAI_PORT_STAT_ETHER_STATS_PKTS_4096_TO_9216_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_OVERSIZE_PKTS = (SAI_PORT_STAT_ETHER_STATS_PKTS_9217_TO_16383_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_RX_OVERSIZE_PKTS = (SAI_PORT_STAT_ETHER_STATS_OVERSIZE_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_TX_OVERSIZE_PKTS = (SAI_PORT_STAT_ETHER_RX_OVERSIZE_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_JABBERS = (SAI_PORT_STAT_ETHER_TX_OVERSIZE_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_OCTETS = (SAI_PORT_STAT_ETHER_STATS_JABBERS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_PKTS = (SAI_PORT_STAT_ETHER_STATS_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_COLLISIONS = (SAI_PORT_STAT_ETHER_STATS_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_CRC_ALIGN_ERRORS = (SAI_PORT_STAT_ETHER_STATS_COLLISIONS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_TX_NO_ERRORS = (SAI_PORT_STAT_ETHER_STATS_CRC_ALIGN_ERRORS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_STATS_RX_NO_ERRORS = (SAI_PORT_STAT_ETHER_STATS_TX_NO_ERRORS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_IN_RECEIVES = (SAI_PORT_STAT_ETHER_STATS_RX_NO_ERRORS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_IN_OCTETS = (SAI_PORT_STAT_IP_IN_RECEIVES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_IN_UCAST_PKTS = (SAI_PORT_STAT_IP_IN_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_IN_NON_UCAST_PKTS = (SAI_PORT_STAT_IP_IN_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_IN_DISCARDS = (SAI_PORT_STAT_IP_IN_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_OUT_OCTETS = (SAI_PORT_STAT_IP_IN_DISCARDS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_OUT_UCAST_PKTS = (SAI_PORT_STAT_IP_OUT_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_OUT_NON_UCAST_PKTS = (SAI_PORT_STAT_IP_OUT_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IP_OUT_DISCARDS = (SAI_PORT_STAT_IP_OUT_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_IN_RECEIVES = (SAI_PORT_STAT_IP_OUT_DISCARDS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_IN_OCTETS = (SAI_PORT_STAT_IPV6_IN_RECEIVES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_IN_UCAST_PKTS = (SAI_PORT_STAT_IPV6_IN_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_IN_NON_UCAST_PKTS = (SAI_PORT_STAT_IPV6_IN_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_IN_MCAST_PKTS = (SAI_PORT_STAT_IPV6_IN_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_IN_DISCARDS = (SAI_PORT_STAT_IPV6_IN_MCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_OUT_OCTETS = (SAI_PORT_STAT_IPV6_IN_DISCARDS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_OUT_UCAST_PKTS = (SAI_PORT_STAT_IPV6_OUT_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_OUT_NON_UCAST_PKTS = (SAI_PORT_STAT_IPV6_OUT_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_OUT_MCAST_PKTS = (SAI_PORT_STAT_IPV6_OUT_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IPV6_OUT_DISCARDS = (SAI_PORT_STAT_IPV6_OUT_MCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_GREEN_DISCARD_DROPPED_PACKETS = (SAI_PORT_STAT_IPV6_OUT_DISCARDS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_GREEN_DISCARD_DROPPED_BYTES = (SAI_PORT_STAT_GREEN_DISCARD_DROPPED_PACKETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_PACKETS = (SAI_PORT_STAT_GREEN_DISCARD_DROPPED_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_BYTES = (SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_PACKETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_RED_DISCARD_DROPPED_PACKETS = (SAI_PORT_STAT_YELLOW_DISCARD_DROPPED_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_RED_DISCARD_DROPPED_BYTES = (SAI_PORT_STAT_RED_DISCARD_DROPPED_PACKETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_DISCARD_DROPPED_PACKETS = (SAI_PORT_STAT_RED_DISCARD_DROPPED_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_DISCARD_DROPPED_BYTES = (SAI_PORT_STAT_DISCARD_DROPPED_PACKETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ECN_MARKED_PACKETS = (SAI_PORT_STAT_DISCARD_DROPPED_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_64_OCTETS = (SAI_PORT_STAT_ECN_MARKED_PACKETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_65_TO_127_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_64_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_128_TO_255_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_65_TO_127_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_256_TO_511_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_128_TO_255_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_512_TO_1023_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_256_TO_511_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_1024_TO_1518_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_512_TO_1023_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_1519_TO_2047_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_1024_TO_1518_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_2048_TO_4095_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_1519_TO_2047_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_4096_TO_9216_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_2048_TO_4095_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_IN_PKTS_9217_TO_16383_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_4096_TO_9216_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_64_OCTETS = (SAI_PORT_STAT_ETHER_IN_PKTS_9217_TO_16383_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_65_TO_127_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_64_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_128_TO_255_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_65_TO_127_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_256_TO_511_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_128_TO_255_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_512_TO_1023_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_256_TO_511_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_1024_TO_1518_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_512_TO_1023_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_1519_TO_2047_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_1024_TO_1518_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_2048_TO_4095_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_1519_TO_2047_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_4096_TO_9216_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_2048_TO_4095_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_ETHER_OUT_PKTS_9217_TO_16383_OCTETS = (SAI_PORT_STAT_ETHER_OUT_PKTS_4096_TO_9216_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IN_CURR_OCCUPANCY_BYTES = (SAI_PORT_STAT_ETHER_OUT_PKTS_9217_TO_16383_OCTETS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IN_WATERMARK_BYTES = (SAI_PORT_STAT_IN_CURR_OCCUPANCY_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IN_SHARED_CURR_OCCUPANCY_BYTES = (SAI_PORT_STAT_IN_WATERMARK_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IN_SHARED_WATERMARK_BYTES = (SAI_PORT_STAT_IN_SHARED_CURR_OCCUPANCY_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_OUT_CURR_OCCUPANCY_BYTES = (SAI_PORT_STAT_IN_SHARED_WATERMARK_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_OUT_WATERMARK_BYTES = (SAI_PORT_STAT_OUT_CURR_OCCUPANCY_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_OUT_SHARED_CURR_OCCUPANCY_BYTES = (SAI_PORT_STAT_OUT_WATERMARK_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_OUT_SHARED_WATERMARK_BYTES = (SAI_PORT_STAT_OUT_SHARED_CURR_OCCUPANCY_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_IN_DROPPED_PKTS = (SAI_PORT_STAT_OUT_SHARED_WATERMARK_BYTES + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_OUT_DROPPED_PKTS = (SAI_PORT_STAT_IN_DROPPED_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PAUSE_RX_PKTS = (SAI_PORT_STAT_OUT_DROPPED_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PAUSE_TX_PKTS = (SAI_PORT_STAT_PAUSE_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_0_RX_PKTS = (SAI_PORT_STAT_PAUSE_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_0_TX_PKTS = (SAI_PORT_STAT_PFC_0_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_1_RX_PKTS = (SAI_PORT_STAT_PFC_0_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_1_TX_PKTS = (SAI_PORT_STAT_PFC_1_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_2_RX_PKTS = (SAI_PORT_STAT_PFC_1_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_2_TX_PKTS = (SAI_PORT_STAT_PFC_2_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_3_RX_PKTS = (SAI_PORT_STAT_PFC_2_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_3_TX_PKTS = (SAI_PORT_STAT_PFC_3_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_4_RX_PKTS = (SAI_PORT_STAT_PFC_3_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_4_TX_PKTS = (SAI_PORT_STAT_PFC_4_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_5_RX_PKTS = (SAI_PORT_STAT_PFC_4_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_5_TX_PKTS = (SAI_PORT_STAT_PFC_5_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_6_RX_PKTS = (SAI_PORT_STAT_PFC_5_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_6_TX_PKTS = (SAI_PORT_STAT_PFC_6_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_7_RX_PKTS = (SAI_PORT_STAT_PFC_6_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_7_TX_PKTS = (SAI_PORT_STAT_PFC_7_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_0_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_7_TX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_1_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_0_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_2_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_1_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_3_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_2_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_4_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_3_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_5_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_4_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_6_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_5_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_PFC_7_ON2OFF_RX_PKTS = (SAI_PORT_STAT_PFC_6_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_EEE_TX_EVENT_COUNT = (SAI_PORT_STAT_PFC_7_ON2OFF_RX_PKTS + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_EEE_RX_EVENT_COUNT = (SAI_PORT_STAT_EEE_TX_EVENT_COUNT + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_EEE_TX_DURATION = (SAI_PORT_STAT_EEE_RX_EVENT_COUNT + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

SAI_PORT_STAT_EEE_RX_DURATION = (SAI_PORT_STAT_EEE_TX_DURATION + 1) # /home/omer/P4/SAI/inc/saiport.h: 1465

sai_port_stat_t = enum__sai_port_stat_t # /home/omer/P4/SAI/inc/saiport.h: 1465

sai_create_port_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiport.h: 1477

sai_remove_port_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiport.h: 1489

sai_set_port_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiport.h: 1500

sai_get_port_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiport.h: 1513

sai_get_port_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_port_stat_t), c_uint32, POINTER(c_uint64)) # /home/omer/P4/SAI/inc/saiport.h: 1528

sai_clear_port_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_port_stat_t), c_uint32) # /home/omer/P4/SAI/inc/saiport.h: 1543

sai_clear_port_all_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiport.h: 1555

sai_port_state_change_notification_fn = CFUNCTYPE(UNCHECKED(None), c_uint32, POINTER(sai_port_oper_status_notification_t)) # /home/omer/P4/SAI/inc/saiport.h: 1566

# /home/omer/P4/SAI/inc/saiport.h: 1583
class struct__sai_port_api_t(Structure):
    pass

struct__sai_port_api_t.__slots__ = [
    'create_port',
    'remove_port',
    'set_port_attribute',
    'get_port_attribute',
    'get_port_stats',
    'clear_port_stats',
    'clear_port_all_stats',
]
struct__sai_port_api_t._fields_ = [
    ('create_port', sai_create_port_fn),
    ('remove_port', sai_remove_port_fn),
    ('set_port_attribute', sai_set_port_attribute_fn),
    ('get_port_attribute', sai_get_port_attribute_fn),
    ('get_port_stats', sai_get_port_stats_fn),
    ('clear_port_stats', sai_clear_port_stats_fn),
    ('clear_port_all_stats', sai_clear_port_all_stats_fn),
]

sai_port_api_t = struct__sai_port_api_t # /home/omer/P4/SAI/inc/saiport.h: 1583

enum__sai_qos_map_type_t = c_int # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_DOT1P_TO_TC = 0 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_DOT1P_TO_COLOR = 1 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_DSCP_TO_TC = 2 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_DSCP_TO_COLOR = 3 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_TC_TO_QUEUE = 4 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DSCP = 5 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_TC_AND_COLOR_TO_DOT1P = 6 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_TC_TO_PRIORITY_GROUP = 7 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_PRIORITY_GROUP = 8 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_PFC_PRIORITY_TO_QUEUE = 9 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

SAI_QOS_MAP_TYPE_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saiqosmap.h: 74

sai_qos_map_type_t = enum__sai_qos_map_type_t # /home/omer/P4/SAI/inc/saiqosmap.h: 74

enum__sai_qos_map_attr_t = c_int # /home/omer/P4/SAI/inc/saiqosmap.h: 118

SAI_QOS_MAP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiqosmap.h: 118

SAI_QOS_MAP_ATTR_TYPE = SAI_QOS_MAP_ATTR_START # /home/omer/P4/SAI/inc/saiqosmap.h: 118

SAI_QOS_MAP_ATTR_MAP_TO_VALUE_LIST = 1 # /home/omer/P4/SAI/inc/saiqosmap.h: 118

SAI_QOS_MAP_ATTR_END = (SAI_QOS_MAP_ATTR_MAP_TO_VALUE_LIST + 1) # /home/omer/P4/SAI/inc/saiqosmap.h: 118

SAI_QOS_MAP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiqosmap.h: 118

SAI_QOS_MAP_ATTR_CUSTOM_RANGE_END = (SAI_QOS_MAP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiqosmap.h: 118

sai_qos_map_attr_t = enum__sai_qos_map_attr_t # /home/omer/P4/SAI/inc/saiqosmap.h: 118

sai_create_qos_map_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiqosmap.h: 130

sai_remove_qos_map_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiqosmap.h: 143

sai_set_qos_map_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiqosmap.h: 154

sai_get_qos_map_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiqosmap.h: 167

# /home/omer/P4/SAI/inc/saiqosmap.h: 182
class struct__sai_qos_map_api_t(Structure):
    pass

struct__sai_qos_map_api_t.__slots__ = [
    'create_qos_map',
    'remove_qos_map',
    'set_qos_map_attribute',
    'get_qos_map_attribute',
]
struct__sai_qos_map_api_t._fields_ = [
    ('create_qos_map', sai_create_qos_map_fn),
    ('remove_qos_map', sai_remove_qos_map_fn),
    ('set_qos_map_attribute', sai_set_qos_map_attribute_fn),
    ('get_qos_map_attribute', sai_get_qos_map_attribute_fn),
]

sai_qos_map_api_t = struct__sai_qos_map_api_t # /home/omer/P4/SAI/inc/saiqosmap.h: 182

enum__sai_queue_type_t = c_int # /home/omer/P4/SAI/inc/saiqueue.h: 53

SAI_QUEUE_TYPE_ALL = 0 # /home/omer/P4/SAI/inc/saiqueue.h: 53

SAI_QUEUE_TYPE_UNICAST = 1 # /home/omer/P4/SAI/inc/saiqueue.h: 53

SAI_QUEUE_TYPE_MULTICAST = 2 # /home/omer/P4/SAI/inc/saiqueue.h: 53

SAI_QUEUE_TYPE_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saiqueue.h: 53

sai_queue_type_t = enum__sai_queue_type_t # /home/omer/P4/SAI/inc/saiqueue.h: 53

enum__sai_queue_attr_t = c_int # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_TYPE = SAI_QUEUE_ATTR_START # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_PORT = 1 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_INDEX = 2 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_PARENT_SCHEDULER_NODE = 3 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_WRED_PROFILE_ID = 4 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_BUFFER_PROFILE_ID = 5 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_SCHEDULER_PROFILE_ID = 6 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_PAUSE_STATUS = 7 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_END = (SAI_QUEUE_ATTR_PAUSE_STATUS + 1) # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiqueue.h: 164

SAI_QUEUE_ATTR_CUSTOM_RANGE_END = (SAI_QUEUE_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiqueue.h: 164

sai_queue_attr_t = enum__sai_queue_attr_t # /home/omer/P4/SAI/inc/saiqueue.h: 164

enum__sai_queue_stat_t = c_int # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_PACKETS = 0 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_BYTES = 1 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_DROPPED_PACKETS = 2 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_DROPPED_BYTES = 3 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_GREEN_PACKETS = 4 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_GREEN_BYTES = 5 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_GREEN_DROPPED_PACKETS = 6 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_GREEN_DROPPED_BYTES = 7 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_YELLOW_PACKETS = 8 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_YELLOW_BYTES = 9 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_YELLOW_DROPPED_PACKETS = 10 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_YELLOW_DROPPED_BYTES = 11 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_RED_PACKETS = 12 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_RED_BYTES = 13 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_RED_DROPPED_PACKETS = 14 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_RED_DROPPED_BYTES = 15 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_GREEN_DISCARD_DROPPED_PACKETS = 16 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_GREEN_DISCARD_DROPPED_BYTES = 17 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_YELLOW_DISCARD_DROPPED_PACKETS = 18 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_YELLOW_DISCARD_DROPPED_BYTES = 19 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_RED_DISCARD_DROPPED_PACKETS = 20 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_RED_DISCARD_DROPPED_BYTES = 21 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_DISCARD_DROPPED_PACKETS = 22 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_DISCARD_DROPPED_BYTES = 23 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_CURR_OCCUPANCY_BYTES = 24 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_WATERMARK_BYTES = 25 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_SHARED_CURR_OCCUPANCY_BYTES = 26 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_SHARED_WATERMARK_BYTES = 27 # /home/omer/P4/SAI/inc/saiqueue.h: 258

SAI_QUEUE_STAT_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saiqueue.h: 258

sai_queue_stat_t = enum__sai_queue_stat_t # /home/omer/P4/SAI/inc/saiqueue.h: 258

sai_create_queue_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiqueue.h: 270

sai_remove_queue_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiqueue.h: 283

sai_set_queue_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiqueue.h: 294

sai_get_queue_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiqueue.h: 307

sai_get_queue_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_queue_stat_t), c_uint32, POINTER(c_uint64)) # /home/omer/P4/SAI/inc/saiqueue.h: 322

sai_clear_queue_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_queue_stat_t), c_uint32) # /home/omer/P4/SAI/inc/saiqueue.h: 337

# /home/omer/P4/SAI/inc/saiqueue.h: 354
class struct__sai_queue_api_t(Structure):
    pass

struct__sai_queue_api_t.__slots__ = [
    'create_queue',
    'remove_queue',
    'set_queue_attribute',
    'get_queue_attribute',
    'get_queue_stats',
    'clear_queue_stats',
]
struct__sai_queue_api_t._fields_ = [
    ('create_queue', sai_create_queue_fn),
    ('remove_queue', sai_remove_queue_fn),
    ('set_queue_attribute', sai_set_queue_attribute_fn),
    ('get_queue_attribute', sai_get_queue_attribute_fn),
    ('get_queue_stats', sai_get_queue_stats_fn),
    ('clear_queue_stats', sai_clear_queue_stats_fn),
]

sai_queue_api_t = struct__sai_queue_api_t # /home/omer/P4/SAI/inc/saiqueue.h: 354

enum__sai_virtual_router_attr_t = c_int # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_START = 0 # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE = SAI_VIRTUAL_ROUTER_ATTR_START # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE = (SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V4_STATE + 1) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS = (SAI_VIRTUAL_ROUTER_ATTR_ADMIN_V6_STATE + 1) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_PACKET_ACTION = (SAI_VIRTUAL_ROUTER_ATTR_SRC_MAC_ADDRESS + 1) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS_PACKET_ACTION = (SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_TTL1_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_UNKNOWN_L3_MULTICAST_PACKET_ACTION = (SAI_VIRTUAL_ROUTER_ATTR_VIOLATION_IP_OPTIONS_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_END = (SAI_VIRTUAL_ROUTER_ATTR_UNKNOWN_L3_MULTICAST_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

SAI_VIRTUAL_ROUTER_ATTR_CUSTOM_RANGE_END = (SAI_VIRTUAL_ROUTER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

sai_virtual_router_attr_t = enum__sai_virtual_router_attr_t # /home/omer/P4/SAI/inc/saivirtualrouter.h: 122

sai_create_virtual_router_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 136

sai_remove_virtual_router_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 149

sai_set_virtual_router_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 160

sai_get_virtual_router_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivirtualrouter.h: 173

# /home/omer/P4/SAI/inc/saivirtualrouter.h: 188
class struct__sai_virtual_router_api_t(Structure):
    pass

struct__sai_virtual_router_api_t.__slots__ = [
    'create_virtual_router',
    'remove_virtual_router',
    'set_virtual_router_attribute',
    'get_virtual_router_attribute',
]
struct__sai_virtual_router_api_t._fields_ = [
    ('create_virtual_router', sai_create_virtual_router_fn),
    ('remove_virtual_router', sai_remove_virtual_router_fn),
    ('set_virtual_router_attribute', sai_set_virtual_router_attribute_fn),
    ('get_virtual_router_attribute', sai_get_virtual_router_attribute_fn),
]

sai_virtual_router_api_t = struct__sai_virtual_router_api_t # /home/omer/P4/SAI/inc/saivirtualrouter.h: 188

enum__sai_router_interface_type_t = c_int # /home/omer/P4/SAI/inc/sairouterinterface.h: 56

SAI_ROUTER_INTERFACE_TYPE_PORT = 0 # /home/omer/P4/SAI/inc/sairouterinterface.h: 56

SAI_ROUTER_INTERFACE_TYPE_VLAN = (SAI_ROUTER_INTERFACE_TYPE_PORT + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 56

SAI_ROUTER_INTERFACE_TYPE_LOOPBACK = (SAI_ROUTER_INTERFACE_TYPE_VLAN + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 56

SAI_ROUTER_INTERFACE_TYPE_SUB_PORT = (SAI_ROUTER_INTERFACE_TYPE_LOOPBACK + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 56

SAI_ROUTER_INTERFACE_TYPE_BRIDGE = (SAI_ROUTER_INTERFACE_TYPE_SUB_PORT + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 56

sai_router_interface_type_t = enum__sai_router_interface_type_t # /home/omer/P4/SAI/inc/sairouterinterface.h: 56

enum__sai_router_interface_attr_t = c_int # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_START = 0 # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID = SAI_ROUTER_INTERFACE_ATTR_START # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_TYPE = (SAI_ROUTER_INTERFACE_ATTR_VIRTUAL_ROUTER_ID + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_PORT_ID = (SAI_ROUTER_INTERFACE_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_VLAN_ID = (SAI_ROUTER_INTERFACE_ATTR_PORT_ID + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS = (SAI_ROUTER_INTERFACE_ATTR_VLAN_ID + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE = (SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE = (SAI_ROUTER_INTERFACE_ATTR_ADMIN_V4_STATE + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_MTU = (SAI_ROUTER_INTERFACE_ATTR_ADMIN_V6_STATE + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL = (SAI_ROUTER_INTERFACE_ATTR_MTU + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_EGRESS_ACL = (SAI_ROUTER_INTERFACE_ATTR_INGRESS_ACL + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_NEIGHBOR_MISS_PACKET_ACTION = (SAI_ROUTER_INTERFACE_ATTR_EGRESS_ACL + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_V4_MCAST_ENABLE = (SAI_ROUTER_INTERFACE_ATTR_NEIGHBOR_MISS_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_V6_MCAST_ENABLE = (SAI_ROUTER_INTERFACE_ATTR_V4_MCAST_ENABLE + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_END = (SAI_ROUTER_INTERFACE_ATTR_V6_MCAST_ENABLE + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_END = (SAI_ROUTER_INTERFACE_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

sai_router_interface_attr_t = enum__sai_router_interface_attr_t # /home/omer/P4/SAI/inc/sairouterinterface.h: 218

sai_create_router_interface_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairouterinterface.h: 230

sai_remove_router_interface_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sairouterinterface.h: 243

sai_set_router_interface_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairouterinterface.h: 254

sai_get_router_interface_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairouterinterface.h: 267

# /home/omer/P4/SAI/inc/sairouterinterface.h: 282
class struct__sai_router_interface_api_t(Structure):
    pass

struct__sai_router_interface_api_t.__slots__ = [
    'create_router_interface',
    'remove_router_interface',
    'set_router_interface_attribute',
    'get_router_interface_attribute',
]
struct__sai_router_interface_api_t._fields_ = [
    ('create_router_interface', sai_create_router_interface_fn),
    ('remove_router_interface', sai_remove_router_interface_fn),
    ('set_router_interface_attribute', sai_set_router_interface_attribute_fn),
    ('get_router_interface_attribute', sai_get_router_interface_attribute_fn),
]

sai_router_interface_api_t = struct__sai_router_interface_api_t # /home/omer/P4/SAI/inc/sairouterinterface.h: 282

enum__sai_samplepacket_type_t = c_int # /home/omer/P4/SAI/inc/saisamplepacket.h: 44

SAI_SAMPLEPACKET_TYPE_SLOW_PATH = 0 # /home/omer/P4/SAI/inc/saisamplepacket.h: 44

sai_samplepacket_type_t = enum__sai_samplepacket_type_t # /home/omer/P4/SAI/inc/saisamplepacket.h: 44

enum__sai_samplepacket_mode_t = c_int # /home/omer/P4/SAI/inc/saisamplepacket.h: 69

SAI_SAMPLEPACKET_MODE_EXCLUSIVE = 0 # /home/omer/P4/SAI/inc/saisamplepacket.h: 69

SAI_SAMPLEPACKET_MODE_SHARED = (SAI_SAMPLEPACKET_MODE_EXCLUSIVE + 1) # /home/omer/P4/SAI/inc/saisamplepacket.h: 69

sai_samplepacket_mode_t = enum__sai_samplepacket_mode_t # /home/omer/P4/SAI/inc/saisamplepacket.h: 69

enum__sai_samplepacket_attr_t = c_int # /home/omer/P4/SAI/inc/saisamplepacket.h: 117

SAI_SAMPLEPACKET_ATTR_START = 0 # /home/omer/P4/SAI/inc/saisamplepacket.h: 117

SAI_SAMPLEPACKET_ATTR_SAMPLE_RATE = SAI_SAMPLEPACKET_ATTR_START # /home/omer/P4/SAI/inc/saisamplepacket.h: 117

SAI_SAMPLEPACKET_ATTR_TYPE = (SAI_SAMPLEPACKET_ATTR_SAMPLE_RATE + 1) # /home/omer/P4/SAI/inc/saisamplepacket.h: 117

SAI_SAMPLEPACKET_ATTR_MODE = (SAI_SAMPLEPACKET_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saisamplepacket.h: 117

SAI_SAMPLEPACKET_ATTR_END = (SAI_SAMPLEPACKET_ATTR_MODE + 1) # /home/omer/P4/SAI/inc/saisamplepacket.h: 117

sai_samplepacket_attr_t = enum__sai_samplepacket_attr_t # /home/omer/P4/SAI/inc/saisamplepacket.h: 117

sai_create_samplepacket_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saisamplepacket.h: 130

sai_remove_samplepacket_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saisamplepacket.h: 144

sai_set_samplepacket_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saisamplepacket.h: 156

sai_get_samplepacket_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saisamplepacket.h: 170

# /home/omer/P4/SAI/inc/saisamplepacket.h: 185
class struct__sai_samplepacket_api_t(Structure):
    pass

struct__sai_samplepacket_api_t.__slots__ = [
    'create_samplepacket',
    'remove_samplepacket',
    'set_samplepacket_attribute',
    'get_samplepacket_attribute',
]
struct__sai_samplepacket_api_t._fields_ = [
    ('create_samplepacket', sai_create_samplepacket_fn),
    ('remove_samplepacket', sai_remove_samplepacket_fn),
    ('set_samplepacket_attribute', sai_set_samplepacket_attribute_fn),
    ('get_samplepacket_attribute', sai_get_samplepacket_attribute_fn),
]

sai_samplepacket_api_t = struct__sai_samplepacket_api_t # /home/omer/P4/SAI/inc/saisamplepacket.h: 185

enum__sai_scheduler_group_attr_t = c_int # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_CHILD_COUNT = SAI_SCHEDULER_GROUP_ATTR_START # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_CHILD_LIST = 1 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_PORT_ID = 2 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_LEVEL = 3 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_MAX_CHILDS = 4 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_SCHEDULER_PROFILE_ID = 5 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_PARENT_NODE = 6 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_END = (SAI_SCHEDULER_GROUP_ATTR_PARENT_NODE + 1) # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

SAI_SCHEDULER_GROUP_ATTR_CUSTOM_RANGE_END = (SAI_SCHEDULER_GROUP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

sai_scheduler_group_attr_t = enum__sai_scheduler_group_attr_t # /home/omer/P4/SAI/inc/saischedulergroup.h: 119

sai_create_scheduler_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saischedulergroup.h: 131

sai_remove_scheduler_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saischedulergroup.h: 144

sai_set_scheduler_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saischedulergroup.h: 155

sai_get_scheduler_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saischedulergroup.h: 168

# /home/omer/P4/SAI/inc/saischedulergroup.h: 183
class struct__sai_scheduler_group_api_t(Structure):
    pass

struct__sai_scheduler_group_api_t.__slots__ = [
    'create_scheduler_group',
    'remove_scheduler_group',
    'set_scheduler_group_attribute',
    'get_scheduler_group_attribute',
]
struct__sai_scheduler_group_api_t._fields_ = [
    ('create_scheduler_group', sai_create_scheduler_group_fn),
    ('remove_scheduler_group', sai_remove_scheduler_group_fn),
    ('set_scheduler_group_attribute', sai_set_scheduler_group_attribute_fn),
    ('get_scheduler_group_attribute', sai_get_scheduler_group_attribute_fn),
]

sai_scheduler_group_api_t = struct__sai_scheduler_group_api_t # /home/omer/P4/SAI/inc/saischedulergroup.h: 183

enum__sai_scheduling_type_t = c_int # /home/omer/P4/SAI/inc/saischeduler.h: 50

SAI_SCHEDULING_TYPE_STRICT = 0 # /home/omer/P4/SAI/inc/saischeduler.h: 50

SAI_SCHEDULING_TYPE_WRR = 1 # /home/omer/P4/SAI/inc/saischeduler.h: 50

SAI_SCHEDULING_TYPE_DWRR = 2 # /home/omer/P4/SAI/inc/saischeduler.h: 50

sai_scheduling_type_t = enum__sai_scheduling_type_t # /home/omer/P4/SAI/inc/saischeduler.h: 50

enum__sai_scheduler_attr_t = c_int # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_START = 0 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_SCHEDULING_TYPE = SAI_SCHEDULER_ATTR_START # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_SCHEDULING_WEIGHT = 1 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_METER_TYPE = 2 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_RATE = 3 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_MIN_BANDWIDTH_BURST_RATE = 4 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_RATE = 5 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE = 6 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_END = (SAI_SCHEDULER_ATTR_MAX_BANDWIDTH_BURST_RATE + 1) # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saischeduler.h: 143

SAI_SCHEDULER_ATTR_CUSTOM_RANGE_END = (SAI_SCHEDULER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saischeduler.h: 143

sai_scheduler_attr_t = enum__sai_scheduler_attr_t # /home/omer/P4/SAI/inc/saischeduler.h: 143

sai_create_scheduler_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saischeduler.h: 155

sai_remove_scheduler_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saischeduler.h: 168

sai_set_scheduler_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saischeduler.h: 179

sai_get_scheduler_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saischeduler.h: 192

# /home/omer/P4/SAI/inc/saischeduler.h: 207
class struct__sai_scheduler_api_t(Structure):
    pass

struct__sai_scheduler_api_t.__slots__ = [
    'create_scheduler',
    'remove_scheduler',
    'set_scheduler_attribute',
    'get_scheduler_attribute',
]
struct__sai_scheduler_api_t._fields_ = [
    ('create_scheduler', sai_create_scheduler_fn),
    ('remove_scheduler', sai_remove_scheduler_fn),
    ('set_scheduler_attribute', sai_set_scheduler_attribute_fn),
    ('get_scheduler_attribute', sai_get_scheduler_attribute_fn),
]

sai_scheduler_api_t = struct__sai_scheduler_api_t # /home/omer/P4/SAI/inc/saischeduler.h: 207

enum__sai_stp_port_state_t = c_int # /home/omer/P4/SAI/inc/saistp.h: 50

SAI_STP_PORT_STATE_LEARNING = 0 # /home/omer/P4/SAI/inc/saistp.h: 50

SAI_STP_PORT_STATE_FORWARDING = (SAI_STP_PORT_STATE_LEARNING + 1) # /home/omer/P4/SAI/inc/saistp.h: 50

SAI_STP_PORT_STATE_BLOCKING = (SAI_STP_PORT_STATE_FORWARDING + 1) # /home/omer/P4/SAI/inc/saistp.h: 50

sai_stp_port_state_t = enum__sai_stp_port_state_t # /home/omer/P4/SAI/inc/saistp.h: 50

enum__sai_stp_attr_t = c_int # /home/omer/P4/SAI/inc/saistp.h: 95

SAI_STP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saistp.h: 95

SAI_STP_ATTR_VLAN_LIST = SAI_STP_ATTR_START # /home/omer/P4/SAI/inc/saistp.h: 95

SAI_STP_ATTR_BRIDGE_ID = (SAI_STP_ATTR_VLAN_LIST + 1) # /home/omer/P4/SAI/inc/saistp.h: 95

SAI_STP_ATTR_PORT_LIST = (SAI_STP_ATTR_BRIDGE_ID + 1) # /home/omer/P4/SAI/inc/saistp.h: 95

SAI_STP_ATTR_END = (SAI_STP_ATTR_PORT_LIST + 1) # /home/omer/P4/SAI/inc/saistp.h: 95

sai_stp_attr_t = enum__sai_stp_attr_t # /home/omer/P4/SAI/inc/saistp.h: 95

enum__sai_stp_port_attr_t = c_int # /home/omer/P4/SAI/inc/saistp.h: 138

SAI_STP_PORT_ATTR_START = 0 # /home/omer/P4/SAI/inc/saistp.h: 138

SAI_STP_PORT_ATTR_STP = SAI_STP_PORT_ATTR_START # /home/omer/P4/SAI/inc/saistp.h: 138

SAI_STP_PORT_ATTR_BRIDGE_PORT = (SAI_STP_PORT_ATTR_STP + 1) # /home/omer/P4/SAI/inc/saistp.h: 138

SAI_STP_PORT_ATTR_STATE = (SAI_STP_PORT_ATTR_BRIDGE_PORT + 1) # /home/omer/P4/SAI/inc/saistp.h: 138

SAI_STP_PORT_ATTR_END = (SAI_STP_PORT_ATTR_STATE + 1) # /home/omer/P4/SAI/inc/saistp.h: 138

sai_stp_port_attr_t = enum__sai_stp_port_attr_t # /home/omer/P4/SAI/inc/saistp.h: 138

sai_create_stp_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saistp.h: 151

sai_remove_stp_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saistp.h: 165

sai_set_stp_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saistp.h: 176

sai_get_stp_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saistp.h: 189

sai_create_stp_port_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saistp.h: 203

sai_remove_stp_port_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saistp.h: 216

sai_set_stp_port_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saistp.h: 227

sai_get_stp_port_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saistp.h: 240

# /home/omer/P4/SAI/inc/saistp.h: 259
class struct__sai_stp_api_t(Structure):
    pass

struct__sai_stp_api_t.__slots__ = [
    'create_stp',
    'remove_stp',
    'set_stp_attribute',
    'get_stp_attribute',
    'create_stp_port',
    'remove_stp_port',
    'set_stp_port_attribute',
    'get_stp_port_attribute',
    'create_stp_ports',
    'remove_stp_ports',
]
struct__sai_stp_api_t._fields_ = [
    ('create_stp', sai_create_stp_fn),
    ('remove_stp', sai_remove_stp_fn),
    ('set_stp_attribute', sai_set_stp_attribute_fn),
    ('get_stp_attribute', sai_get_stp_attribute_fn),
    ('create_stp_port', sai_create_stp_port_fn),
    ('remove_stp_port', sai_remove_stp_port_fn),
    ('set_stp_port_attribute', sai_set_stp_port_attribute_fn),
    ('get_stp_port_attribute', sai_get_stp_port_attribute_fn),
    ('create_stp_ports', sai_bulk_object_create_fn),
    ('remove_stp_ports', sai_bulk_object_remove_fn),
]

sai_stp_api_t = struct__sai_stp_api_t # /home/omer/P4/SAI/inc/saistp.h: 259

enum__sai_switch_oper_status_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 66

SAI_SWITCH_OPER_STATUS_UNKNOWN = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 66

SAI_SWITCH_OPER_STATUS_UP = (SAI_SWITCH_OPER_STATUS_UNKNOWN + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 66

SAI_SWITCH_OPER_STATUS_DOWN = (SAI_SWITCH_OPER_STATUS_UP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 66

SAI_SWITCH_OPER_STATUS_FAILED = (SAI_SWITCH_OPER_STATUS_DOWN + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 66

sai_switch_oper_status_t = enum__sai_switch_oper_status_t # /home/omer/P4/SAI/inc/saiswitch.h: 66

enum__sai_packet_action_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_DROP = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_FORWARD = (SAI_PACKET_ACTION_DROP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_COPY = (SAI_PACKET_ACTION_FORWARD + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_COPY_CANCEL = (SAI_PACKET_ACTION_COPY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_TRAP = (SAI_PACKET_ACTION_COPY_CANCEL + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_LOG = (SAI_PACKET_ACTION_TRAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_DENY = (SAI_PACKET_ACTION_LOG + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 119

SAI_PACKET_ACTION_TRANSIT = (SAI_PACKET_ACTION_DENY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 119

sai_packet_action_t = enum__sai_packet_action_t # /home/omer/P4/SAI/inc/saiswitch.h: 119

enum__sai_packet_vlan_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 149

SAI_PACKET_VLAN_UNTAG = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 149

SAI_PACKET_VLAN_SINGLE_OUTER_TAG = (SAI_PACKET_VLAN_UNTAG + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 149

SAI_PACKET_VLAN_DOUBLE_TAG = (SAI_PACKET_VLAN_SINGLE_OUTER_TAG + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 149

sai_packet_vlan_t = enum__sai_packet_vlan_t # /home/omer/P4/SAI/inc/saiswitch.h: 149

enum__sai_switch_switching_mode_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 162

SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 162

SAI_SWITCH_SWITCHING_MODE_STORE_AND_FORWARD = (SAI_SWITCH_SWITCHING_MODE_CUT_THROUGH + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 162

sai_switch_switching_mode_t = enum__sai_switch_switching_mode_t # /home/omer/P4/SAI/inc/saiswitch.h: 162

enum__sai_hash_algorithm_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 191

SAI_HASH_ALGORITHM_CRC = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 191

SAI_HASH_ALGORITHM_XOR = 1 # /home/omer/P4/SAI/inc/saiswitch.h: 191

SAI_HASH_ALGORITHM_RANDOM = 2 # /home/omer/P4/SAI/inc/saiswitch.h: 191

SAI_HASH_ALGORITHM_CRC_32LO = 3 # /home/omer/P4/SAI/inc/saiswitch.h: 191

SAI_HASH_ALGORITHM_CRC_32HI = 4 # /home/omer/P4/SAI/inc/saiswitch.h: 191

SAI_HASH_ALGORITHM_CRC_CCITT = 5 # /home/omer/P4/SAI/inc/saiswitch.h: 191

SAI_HASH_ALGORITHM_CRC_XOR = 6 # /home/omer/P4/SAI/inc/saiswitch.h: 191

sai_hash_algorithm_t = enum__sai_hash_algorithm_t # /home/omer/P4/SAI/inc/saiswitch.h: 191

enum__sai_switch_restart_type_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 207

SAI_SWITCH_RESTART_TYPE_NONE = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 207

SAI_SWITCH_RESTART_TYPE_PLANNED = 1 # /home/omer/P4/SAI/inc/saiswitch.h: 207

SAI_SWITCH_RESTART_TYPE_ANY = 2 # /home/omer/P4/SAI/inc/saiswitch.h: 207

sai_switch_restart_type_t = enum__sai_switch_restart_type_t # /home/omer/P4/SAI/inc/saiswitch.h: 207

enum__sai_switch_mcast_snooping_capability_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 226

SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_NONE = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 226

SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_XG = 1 # /home/omer/P4/SAI/inc/saiswitch.h: 226

SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_SG = 2 # /home/omer/P4/SAI/inc/saiswitch.h: 226

SAI_SWITCH_MCAST_SNOOPING_CAPABILITY_XG_AND_SG = 3 # /home/omer/P4/SAI/inc/saiswitch.h: 226

sai_switch_mcast_snooping_capability_t = enum__sai_switch_mcast_snooping_capability_t # /home/omer/P4/SAI/inc/saiswitch.h: 226

enum__sai_switch_attr_t = c_int # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_PORT_NUMBER = SAI_SWITCH_ATTR_START # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_PORT_LIST = (SAI_SWITCH_ATTR_PORT_NUMBER + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_PORT_MAX_MTU = (SAI_SWITCH_ATTR_PORT_LIST + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_CPU_PORT = (SAI_SWITCH_ATTR_PORT_MAX_MTU + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS = (SAI_SWITCH_ATTR_CPU_PORT + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FDB_TABLE_SIZE = (SAI_SWITCH_ATTR_MAX_VIRTUAL_ROUTERS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_L3_NEIGHBOR_TABLE_SIZE = (SAI_SWITCH_ATTR_FDB_TABLE_SIZE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_L3_ROUTE_TABLE_SIZE = (SAI_SWITCH_ATTR_L3_NEIGHBOR_TABLE_SIZE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_MEMBERS = (SAI_SWITCH_ATTR_L3_ROUTE_TABLE_SIZE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NUMBER_OF_LAGS = (SAI_SWITCH_ATTR_LAG_MEMBERS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_MEMBERS = (SAI_SWITCH_ATTR_NUMBER_OF_LAGS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NUMBER_OF_ECMP_GROUPS = (SAI_SWITCH_ATTR_ECMP_MEMBERS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NUMBER_OF_UNICAST_QUEUES = (SAI_SWITCH_ATTR_NUMBER_OF_ECMP_GROUPS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NUMBER_OF_MULTICAST_QUEUES = (SAI_SWITCH_ATTR_NUMBER_OF_UNICAST_QUEUES + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NUMBER_OF_QUEUES = (SAI_SWITCH_ATTR_NUMBER_OF_MULTICAST_QUEUES + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NUMBER_OF_CPU_QUEUES = (SAI_SWITCH_ATTR_NUMBER_OF_QUEUES + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED = (SAI_SWITCH_ATTR_NUMBER_OF_CPU_QUEUES + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_OPER_STATUS = (SAI_SWITCH_ATTR_ON_LINK_ROUTE_SUPPORTED + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MAX_TEMP = (SAI_SWITCH_ATTR_OPER_STATUS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY = (SAI_SWITCH_ATTR_MAX_TEMP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY = (SAI_SWITCH_ATTR_ACL_TABLE_MINIMUM_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY = (SAI_SWITCH_ATTR_ACL_TABLE_MAXIMUM_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY = (SAI_SWITCH_ATTR_ACL_ENTRY_MINIMUM_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_TABLE_GROUP_MINIMUM_PRIORITY = (SAI_SWITCH_ATTR_ACL_ENTRY_MAXIMUM_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_TABLE_GROUP_MAXIMUM_PRIORITY = (SAI_SWITCH_ATTR_ACL_TABLE_GROUP_MINIMUM_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FDB_DST_USER_META_DATA_RANGE = (SAI_SWITCH_ATTR_ACL_TABLE_GROUP_MAXIMUM_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ROUTE_DST_USER_META_DATA_RANGE = (SAI_SWITCH_ATTR_FDB_DST_USER_META_DATA_RANGE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NEIGHBOR_DST_USER_META_DATA_RANGE = (SAI_SWITCH_ATTR_ROUTE_DST_USER_META_DATA_RANGE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_PORT_USER_META_DATA_RANGE = (SAI_SWITCH_ATTR_NEIGHBOR_DST_USER_META_DATA_RANGE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_VLAN_USER_META_DATA_RANGE = (SAI_SWITCH_ATTR_PORT_USER_META_DATA_RANGE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_USER_META_DATA_RANGE = (SAI_SWITCH_ATTR_VLAN_USER_META_DATA_RANGE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_USER_TRAP_ID_RANGE = (SAI_SWITCH_ATTR_ACL_USER_META_DATA_RANGE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_DEFAULT_VLAN_ID = (SAI_SWITCH_ATTR_ACL_USER_TRAP_ID_RANGE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID = (SAI_SWITCH_ATTR_DEFAULT_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID = (SAI_SWITCH_ATTR_DEFAULT_STP_INST_ID + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID = (SAI_SWITCH_ATTR_DEFAULT_VIRTUAL_ROUTER_ID + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_INGRESS_ACL = (SAI_SWITCH_ATTR_DEFAULT_1Q_BRIDGE_ID + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_EGRESS_ACL = (SAI_SWITCH_ATTR_INGRESS_ACL + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_TRAFFIC_CLASSES = (SAI_SWITCH_ATTR_EGRESS_ACL + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_HIERARCHY_LEVELS = (SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_TRAFFIC_CLASSES + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUPS_PER_HIERARCHY_LEVEL = (SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUP_HIERARCHY_LEVELS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_CHILDS_PER_SCHEDULER_GROUP = (SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_SCHEDULER_GROUPS_PER_HIERARCHY_LEVEL + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE = (SAI_SWITCH_ATTR_QOS_MAX_NUMBER_OF_CHILDS_PER_SCHEDULER_GROUP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM = (SAI_SWITCH_ATTR_TOTAL_BUFFER_SIZE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM = (SAI_SWITCH_ATTR_INGRESS_BUFFER_POOL_NUM + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP = (SAI_SWITCH_ATTR_EGRESS_BUFFER_POOL_NUM + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_HASH = (SAI_SWITCH_ATTR_DEFAULT_TRAP_GROUP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_HASH = (SAI_SWITCH_ATTR_ECMP_HASH + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_RESTART_WARM = (SAI_SWITCH_ATTR_LAG_HASH + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_RESTART_TYPE = (SAI_SWITCH_ATTR_RESTART_WARM + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MIN_PLANNED_RESTART_INTERVAL = (SAI_SWITCH_ATTR_RESTART_TYPE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_NV_STORAGE_SIZE = (SAI_SWITCH_ATTR_MIN_PLANNED_RESTART_INTERVAL + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MAX_ACL_ACTION_COUNT = (SAI_SWITCH_ATTR_NV_STORAGE_SIZE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MCAST_SNOOPING_CAPABILITY = (SAI_SWITCH_ATTR_MAX_ACL_ACTION_COUNT + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_SWITCHING_MODE = (SAI_SWITCH_ATTR_MCAST_SNOOPING_CAPABILITY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE = (SAI_SWITCH_ATTR_SWITCHING_MODE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE = (SAI_SWITCH_ATTR_BCAST_CPU_FLOOD_ENABLE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_SRC_MAC_ADDRESS = (SAI_SWITCH_ATTR_MCAST_CPU_FLOOD_ENABLE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES = (SAI_SWITCH_ATTR_SRC_MAC_ADDRESS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FDB_AGING_TIME = (SAI_SWITCH_ATTR_MAX_LEARNED_ADDRESSES + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION = (SAI_SWITCH_ATTR_FDB_AGING_TIME + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION = (SAI_SWITCH_ATTR_FDB_UNICAST_MISS_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_PACKET_ACTION = (SAI_SWITCH_ATTR_FDB_BROADCAST_MISS_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM = (SAI_SWITCH_ATTR_FDB_MULTICAST_MISS_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED = (SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_ALGORITHM + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_DEFAULT_SYMMETRIC_HASH = (SAI_SWITCH_ATTR_ECMP_DEFAULT_HASH_SEED + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_HASH_IPV4 = (SAI_SWITCH_ATTR_ECMP_DEFAULT_SYMMETRIC_HASH + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_HASH_IPV4_IN_IPV4 = (SAI_SWITCH_ATTR_ECMP_HASH_IPV4 + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ECMP_HASH_IPV6 = (SAI_SWITCH_ATTR_ECMP_HASH_IPV4_IN_IPV4 + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM = (SAI_SWITCH_ATTR_ECMP_HASH_IPV6 + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED = (SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_ALGORITHM + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_DEFAULT_SYMMETRIC_HASH = (SAI_SWITCH_ATTR_LAG_DEFAULT_HASH_SEED + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_HASH_IPV4 = (SAI_SWITCH_ATTR_LAG_DEFAULT_SYMMETRIC_HASH + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_HASH_IPV4_IN_IPV4 = (SAI_SWITCH_ATTR_LAG_HASH_IPV4 + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_LAG_HASH_IPV6 = (SAI_SWITCH_ATTR_LAG_HASH_IPV4_IN_IPV4 + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL = (SAI_SWITCH_ATTR_LAG_HASH_IPV6 + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_DEFAULT_TC = (SAI_SWITCH_ATTR_COUNTER_REFRESH_INTERVAL + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_DOT1P_TO_TC_MAP = (SAI_SWITCH_ATTR_QOS_DEFAULT_TC + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_DOT1P_TO_COLOR_MAP = (SAI_SWITCH_ATTR_QOS_DOT1P_TO_TC_MAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_DSCP_TO_TC_MAP = (SAI_SWITCH_ATTR_QOS_DOT1P_TO_COLOR_MAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_DSCP_TO_COLOR_MAP = (SAI_SWITCH_ATTR_QOS_DSCP_TO_TC_MAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP = (SAI_SWITCH_ATTR_QOS_DSCP_TO_COLOR_MAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP = (SAI_SWITCH_ATTR_QOS_TC_TO_QUEUE_MAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP = (SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DOT1P_MAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_SWITCH_SHELL_ENABLE = (SAI_SWITCH_ATTR_QOS_TC_AND_COLOR_TO_DSCP_MAP + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_SWITCH_PROFILE_ID = (SAI_SWITCH_ATTR_SWITCH_SHELL_ENABLE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO = (SAI_SWITCH_ATTR_SWITCH_PROFILE_ID + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FIRMWARE_PATH_NAME = (SAI_SWITCH_ATTR_SWITCH_HARDWARE_INFO + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_INIT_SWITCH = (SAI_SWITCH_ATTR_FIRMWARE_PATH_NAME + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_SWITCH_STATE_CHANGE_NOTIFY = (SAI_SWITCH_ATTR_INIT_SWITCH + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_SHUTDOWN_REQUEST_NOTIFY = (SAI_SWITCH_ATTR_SWITCH_STATE_CHANGE_NOTIFY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FDB_EVENT_NOTIFY = (SAI_SWITCH_ATTR_SHUTDOWN_REQUEST_NOTIFY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY = (SAI_SWITCH_ATTR_FDB_EVENT_NOTIFY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_PACKET_EVENT_NOTIFY = (SAI_SWITCH_ATTR_PORT_STATE_CHANGE_NOTIFY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_FAST_API_ENABLE = (SAI_SWITCH_ATTR_PACKET_EVENT_NOTIFY + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_MIRROR_TC = (SAI_SWITCH_ATTR_FAST_API_ENABLE + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_STAGE_INGRESS = (SAI_SWITCH_ATTR_MIRROR_TC + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_ACL_STAGE_EGRESS = (SAI_SWITCH_ATTR_ACL_STAGE_INGRESS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_END = (SAI_SWITCH_ATTR_ACL_STAGE_EGRESS + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiswitch.h: 1270

SAI_SWITCH_ATTR_CUSTOM_RANGE_END = (SAI_SWITCH_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiswitch.h: 1270

sai_switch_attr_t = enum__sai_switch_attr_t # /home/omer/P4/SAI/inc/saiswitch.h: 1270

sai_switch_shutdown_request_notification_fn = CFUNCTYPE(UNCHECKED(None), sai_object_id_t) # /home/omer/P4/SAI/inc/saiswitch.h: 1389

sai_switch_state_change_notification_fn = CFUNCTYPE(UNCHECKED(None), sai_object_id_t, sai_switch_oper_status_t) # /home/omer/P4/SAI/inc/saiswitch.h: 1398

sai_create_switch_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiswitch.h: 1415

sai_remove_switch_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiswitch.h: 1429

sai_set_switch_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiswitch.h: 1440

sai_get_switch_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, sai_uint32_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiswitch.h: 1453

# /home/omer/P4/SAI/inc/saiswitch.h: 1468
class struct__sai_switch_api_t(Structure):
    pass

struct__sai_switch_api_t.__slots__ = [
    'create_switch',
    'remove_switch',
    'set_switch_attribute',
    'get_switch_attribute',
]
struct__sai_switch_api_t._fields_ = [
    ('create_switch', sai_create_switch_fn),
    ('remove_switch', sai_remove_switch_fn),
    ('set_switch_attribute', sai_set_switch_attribute_fn),
    ('get_switch_attribute', sai_get_switch_attribute_fn),
]

sai_switch_api_t = struct__sai_switch_api_t # /home/omer/P4/SAI/inc/saiswitch.h: 1468

enum__sai_tunnel_map_type_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 62

SAI_TUNNEL_MAP_TYPE_OECN_TO_UECN = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 62

SAI_TUNNEL_MAP_TYPE_UECN_OECN_TO_OECN = 1 # /home/omer/P4/SAI/inc/saitunnel.h: 62

SAI_TUNNEL_MAP_TYPE_VNI_TO_VLAN_ID = 2 # /home/omer/P4/SAI/inc/saitunnel.h: 62

SAI_TUNNEL_MAP_TYPE_VLAN_ID_TO_VNI = 3 # /home/omer/P4/SAI/inc/saitunnel.h: 62

SAI_TUNNEL_MAP_TYPE_VNI_TO_BRIDGE_IF = 4 # /home/omer/P4/SAI/inc/saitunnel.h: 62

SAI_TUNNEL_MAP_TYPE_BRIDGE_IF_TO_VNI = 5 # /home/omer/P4/SAI/inc/saitunnel.h: 62

SAI_TUNNEL_MAP_TYPE_CUSTOM_RANGE_BASE = 268435456 # /home/omer/P4/SAI/inc/saitunnel.h: 62

sai_tunnel_map_type_t = enum__sai_tunnel_map_type_t # /home/omer/P4/SAI/inc/saitunnel.h: 62

enum__sai_tunnel_map_entry_attr_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_START = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP_TYPE = SAI_TUNNEL_MAP_ENTRY_ATTR_START # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_TUNNEL_MAP = 1 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_OECN_KEY = 2 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_OECN_VALUE = 3 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_UECN_KEY = 4 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_UECN_VALUE = 5 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_VLAN_ID_KEY = 6 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_VLAN_ID_VALUE = 7 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_KEY = 8 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_VNI_ID_VALUE = 9 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_KEY = 10 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE = 11 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_END = (SAI_TUNNEL_MAP_ENTRY_ATTR_BRIDGE_ID_VALUE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saitunnel.h: 193

SAI_TUNNEL_MAP_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_TUNNEL_MAP_ENTRY_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 193

sai_tunnel_map_entry_attr_t = enum__sai_tunnel_map_entry_attr_t # /home/omer/P4/SAI/inc/saitunnel.h: 193

enum__sai_tunnel_map_attr_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 235

SAI_TUNNEL_MAP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 235

SAI_TUNNEL_MAP_ATTR_TYPE = SAI_TUNNEL_MAP_ATTR_START # /home/omer/P4/SAI/inc/saitunnel.h: 235

SAI_TUNNEL_MAP_ATTR_MAP_TO_VALUE_LIST = 1 # /home/omer/P4/SAI/inc/saitunnel.h: 235

SAI_TUNNEL_MAP_ATTR_END = (SAI_TUNNEL_MAP_ATTR_MAP_TO_VALUE_LIST + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 235

SAI_TUNNEL_MAP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saitunnel.h: 235

SAI_TUNNEL_MAP_ATTR_CUSTOM_RANGE_END = (SAI_TUNNEL_MAP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 235

sai_tunnel_map_attr_t = enum__sai_tunnel_map_attr_t # /home/omer/P4/SAI/inc/saitunnel.h: 235

sai_create_tunnel_map_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 247

sai_remove_tunnel_map_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saitunnel.h: 260

sai_set_tunnel_map_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 271

sai_get_tunnel_map_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 284

enum__sai_tunnel_type_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 302

SAI_TUNNEL_TYPE_IPINIP = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 302

SAI_TUNNEL_TYPE_IPINIP_GRE = (SAI_TUNNEL_TYPE_IPINIP + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 302

SAI_TUNNEL_TYPE_VXLAN = (SAI_TUNNEL_TYPE_IPINIP_GRE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 302

SAI_TUNNEL_TYPE_MPLS = (SAI_TUNNEL_TYPE_VXLAN + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 302

sai_tunnel_type_t = enum__sai_tunnel_type_t # /home/omer/P4/SAI/inc/saitunnel.h: 302

enum__sai_tunnel_ttl_mode_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 330

SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 330

SAI_TUNNEL_TTL_MODE_PIPE_MODEL = (SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 330

sai_tunnel_ttl_mode_t = enum__sai_tunnel_ttl_mode_t # /home/omer/P4/SAI/inc/saitunnel.h: 330

enum__sai_tunnel_dscp_mode_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 358

SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 358

SAI_TUNNEL_DSCP_MODE_PIPE_MODEL = (SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 358

sai_tunnel_dscp_mode_t = enum__sai_tunnel_dscp_mode_t # /home/omer/P4/SAI/inc/saitunnel.h: 358

enum__sai_tunnel_encap_ecn_mode_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 378

SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 378

SAI_TUNNEL_ENCAP_ECN_MODE_USER_DEFINED = (SAI_TUNNEL_ENCAP_ECN_MODE_STANDARD + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 378

sai_tunnel_encap_ecn_mode_t = enum__sai_tunnel_encap_ecn_mode_t # /home/omer/P4/SAI/inc/saitunnel.h: 378

enum__sai_tunnel_decap_ecn_mode_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 402

SAI_TUNNEL_DECAP_ECN_MODE_STANDARD = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 402

SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER = (SAI_TUNNEL_DECAP_ECN_MODE_STANDARD + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 402

SAI_TUNNEL_DECAP_ECN_MODE_USER_DEFINED = (SAI_TUNNEL_DECAP_ECN_MODE_COPY_FROM_OUTER + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 402

sai_tunnel_decap_ecn_mode_t = enum__sai_tunnel_decap_ecn_mode_t # /home/omer/P4/SAI/inc/saitunnel.h: 402

enum__sai_tunnel_attr_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_START = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_TYPE = SAI_TUNNEL_ATTR_START # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE = (SAI_TUNNEL_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_OVERLAY_INTERFACE = (SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_SRC_IP = (SAI_TUNNEL_ATTR_OVERLAY_INTERFACE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_TTL_MODE = (SAI_TUNNEL_ATTR_ENCAP_SRC_IP + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_TTL_VAL = (SAI_TUNNEL_ATTR_ENCAP_TTL_MODE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE = (SAI_TUNNEL_ATTR_ENCAP_TTL_VAL + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL = (SAI_TUNNEL_ATTR_ENCAP_DSCP_MODE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID = (SAI_TUNNEL_ATTR_ENCAP_DSCP_VAL + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_GRE_KEY = (SAI_TUNNEL_ATTR_ENCAP_GRE_KEY_VALID + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_ECN_MODE = (SAI_TUNNEL_ATTR_ENCAP_GRE_KEY + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_ENCAP_MAPPERS = (SAI_TUNNEL_ATTR_ENCAP_ECN_MODE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_DECAP_ECN_MODE = (SAI_TUNNEL_ATTR_ENCAP_MAPPERS + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_DECAP_MAPPERS = (SAI_TUNNEL_ATTR_DECAP_ECN_MODE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_DECAP_TTL_MODE = (SAI_TUNNEL_ATTR_DECAP_MAPPERS + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_DECAP_DSCP_MODE = (SAI_TUNNEL_ATTR_DECAP_TTL_MODE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_END = (SAI_TUNNEL_ATTR_DECAP_DSCP_MODE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saitunnel.h: 586

SAI_TUNNEL_ATTR_CUSTOM_RANGE_END = (SAI_TUNNEL_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 586

sai_tunnel_attr_t = enum__sai_tunnel_attr_t # /home/omer/P4/SAI/inc/saitunnel.h: 586

sai_create_tunnel_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 598

sai_remove_tunnel_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saitunnel.h: 611

sai_set_tunnel_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 622

sai_get_tunnel_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 635

enum__sai_tunnel_term_table_entry_type_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 651

SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 651

SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2MP = (SAI_TUNNEL_TERM_TABLE_ENTRY_TYPE_P2P + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 651

sai_tunnel_term_table_entry_type_t = enum__sai_tunnel_term_table_entry_type_t # /home/omer/P4/SAI/inc/saitunnel.h: 651

enum__sai_tunnel_term_table_entry_attr_t = c_int # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_START = 0 # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID = SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_START # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE = (SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP = (SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP = (SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE = (SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID = (SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_END = (SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saitunnel.h: 726

SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_CUSTOM_RANGE_END = (SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saitunnel.h: 726

sai_tunnel_term_table_entry_attr_t = enum__sai_tunnel_term_table_entry_attr_t # /home/omer/P4/SAI/inc/saitunnel.h: 726

sai_create_tunnel_term_table_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 738

sai_remove_tunnel_term_table_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saitunnel.h: 751

sai_set_tunnel_term_table_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 762

sai_get_tunnel_term_table_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 775

sai_create_tunnel_map_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 790

sai_remove_tunnel_map_entry_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saitunnel.h: 803

sai_set_tunnel_map_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 814

sai_get_tunnel_map_entry_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saitunnel.h: 827

# /home/omer/P4/SAI/inc/saitunnel.h: 854
class struct__sai_tunnel_api_t(Structure):
    pass

struct__sai_tunnel_api_t.__slots__ = [
    'create_tunnel_map',
    'remove_tunnel_map',
    'set_tunnel_map_attribute',
    'get_tunnel_map_attribute',
    'create_tunnel',
    'remove_tunnel',
    'set_tunnel_attribute',
    'get_tunnel_attribute',
    'create_tunnel_term_table_entry',
    'remove_tunnel_term_table_entry',
    'set_tunnel_term_table_entry_attribute',
    'get_tunnel_term_table_entry_attribute',
    'create_tunnel_map_entry',
    'remove_tunnel_map_entry',
    'set_tunnel_map_entry_attribute',
    'get_tunnel_map_entry_attribute',
]
struct__sai_tunnel_api_t._fields_ = [
    ('create_tunnel_map', sai_create_tunnel_map_fn),
    ('remove_tunnel_map', sai_remove_tunnel_map_fn),
    ('set_tunnel_map_attribute', sai_set_tunnel_map_attribute_fn),
    ('get_tunnel_map_attribute', sai_get_tunnel_map_attribute_fn),
    ('create_tunnel', sai_create_tunnel_fn),
    ('remove_tunnel', sai_remove_tunnel_fn),
    ('set_tunnel_attribute', sai_set_tunnel_attribute_fn),
    ('get_tunnel_attribute', sai_get_tunnel_attribute_fn),
    ('create_tunnel_term_table_entry', sai_create_tunnel_term_table_entry_fn),
    ('remove_tunnel_term_table_entry', sai_remove_tunnel_term_table_entry_fn),
    ('set_tunnel_term_table_entry_attribute', sai_set_tunnel_term_table_entry_attribute_fn),
    ('get_tunnel_term_table_entry_attribute', sai_get_tunnel_term_table_entry_attribute_fn),
    ('create_tunnel_map_entry', sai_create_tunnel_map_entry_fn),
    ('remove_tunnel_map_entry', sai_remove_tunnel_map_entry_fn),
    ('set_tunnel_map_entry_attribute', sai_set_tunnel_map_entry_attribute_fn),
    ('get_tunnel_map_entry_attribute', sai_get_tunnel_map_entry_attribute_fn),
]

sai_tunnel_api_t = struct__sai_tunnel_api_t # /home/omer/P4/SAI/inc/saitunnel.h: 854

enum__sai_udf_base_t = c_int # /home/omer/P4/SAI/inc/saiudf.h: 50

SAI_UDF_BASE_L2 = 0 # /home/omer/P4/SAI/inc/saiudf.h: 50

SAI_UDF_BASE_L3 = (SAI_UDF_BASE_L2 + 1) # /home/omer/P4/SAI/inc/saiudf.h: 50

SAI_UDF_BASE_L4 = (SAI_UDF_BASE_L3 + 1) # /home/omer/P4/SAI/inc/saiudf.h: 50

sai_udf_base_t = enum__sai_udf_base_t # /home/omer/P4/SAI/inc/saiudf.h: 50

enum__sai_udf_attr_t = c_int # /home/omer/P4/SAI/inc/saiudf.h: 118

SAI_UDF_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiudf.h: 118

SAI_UDF_ATTR_MATCH_ID = SAI_UDF_ATTR_START # /home/omer/P4/SAI/inc/saiudf.h: 118

SAI_UDF_ATTR_GROUP_ID = (SAI_UDF_ATTR_MATCH_ID + 1) # /home/omer/P4/SAI/inc/saiudf.h: 118

SAI_UDF_ATTR_BASE = (SAI_UDF_ATTR_GROUP_ID + 1) # /home/omer/P4/SAI/inc/saiudf.h: 118

SAI_UDF_ATTR_OFFSET = (SAI_UDF_ATTR_BASE + 1) # /home/omer/P4/SAI/inc/saiudf.h: 118

SAI_UDF_ATTR_HASH_MASK = (SAI_UDF_ATTR_OFFSET + 1) # /home/omer/P4/SAI/inc/saiudf.h: 118

SAI_UDF_ATTR_END = (SAI_UDF_ATTR_HASH_MASK + 1) # /home/omer/P4/SAI/inc/saiudf.h: 118

sai_udf_attr_t = enum__sai_udf_attr_t # /home/omer/P4/SAI/inc/saiudf.h: 118

enum__sai_udf_match_attr_t = c_int # /home/omer/P4/SAI/inc/saiudf.h: 179

SAI_UDF_MATCH_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiudf.h: 179

SAI_UDF_MATCH_ATTR_L2_TYPE = SAI_UDF_MATCH_ATTR_START # /home/omer/P4/SAI/inc/saiudf.h: 179

SAI_UDF_MATCH_ATTR_L3_TYPE = (SAI_UDF_MATCH_ATTR_L2_TYPE + 1) # /home/omer/P4/SAI/inc/saiudf.h: 179

SAI_UDF_MATCH_ATTR_GRE_TYPE = (SAI_UDF_MATCH_ATTR_L3_TYPE + 1) # /home/omer/P4/SAI/inc/saiudf.h: 179

SAI_UDF_MATCH_ATTR_PRIORITY = (SAI_UDF_MATCH_ATTR_GRE_TYPE + 1) # /home/omer/P4/SAI/inc/saiudf.h: 179

SAI_UDF_MATCH_ATTR_END = (SAI_UDF_MATCH_ATTR_PRIORITY + 1) # /home/omer/P4/SAI/inc/saiudf.h: 179

sai_udf_match_attr_t = enum__sai_udf_match_attr_t # /home/omer/P4/SAI/inc/saiudf.h: 179

enum__sai_udf_group_type_t = c_int # /home/omer/P4/SAI/inc/saiudf.h: 198

SAI_UDF_GROUP_TYPE_START = 0 # /home/omer/P4/SAI/inc/saiudf.h: 198

SAI_UDF_GROUP_TYPE_GENERIC = SAI_UDF_GROUP_TYPE_START # /home/omer/P4/SAI/inc/saiudf.h: 198

SAI_UDF_GROUP_TYPE_HASH = (SAI_UDF_GROUP_TYPE_GENERIC + 1) # /home/omer/P4/SAI/inc/saiudf.h: 198

SAI_UDF_GROUP_TYPE_END = (SAI_UDF_GROUP_TYPE_HASH + 1) # /home/omer/P4/SAI/inc/saiudf.h: 198

sai_udf_group_type_t = enum__sai_udf_group_type_t # /home/omer/P4/SAI/inc/saiudf.h: 198

enum__sai_udf_group_attr_t = c_int # /home/omer/P4/SAI/inc/saiudf.h: 242

SAI_UDF_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiudf.h: 242

SAI_UDF_GROUP_ATTR_UDF_LIST = SAI_UDF_GROUP_ATTR_START # /home/omer/P4/SAI/inc/saiudf.h: 242

SAI_UDF_GROUP_ATTR_TYPE = (SAI_UDF_GROUP_ATTR_UDF_LIST + 1) # /home/omer/P4/SAI/inc/saiudf.h: 242

SAI_UDF_GROUP_ATTR_LENGTH = (SAI_UDF_GROUP_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saiudf.h: 242

SAI_UDF_GROUP_ATTR_END = (SAI_UDF_GROUP_ATTR_LENGTH + 1) # /home/omer/P4/SAI/inc/saiudf.h: 242

sai_udf_group_attr_t = enum__sai_udf_group_attr_t # /home/omer/P4/SAI/inc/saiudf.h: 242

sai_create_udf_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 254

sai_remove_udf_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiudf.h: 267

sai_set_udf_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 278

sai_get_udf_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 291

sai_create_udf_match_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 306

sai_remove_udf_match_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiudf.h: 319

sai_set_udf_match_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 330

sai_get_udf_match_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 343

sai_create_udf_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 358

sai_remove_udf_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiudf.h: 371

sai_set_udf_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 382

sai_get_udf_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiudf.h: 395

# /home/omer/P4/SAI/inc/saiudf.h: 418
class struct__sai_udf_api_t(Structure):
    pass

struct__sai_udf_api_t.__slots__ = [
    'create_udf',
    'remove_udf',
    'set_udf_attribute',
    'get_udf_attribute',
    'create_udf_match',
    'remove_udf_match',
    'set_udf_match_attribute',
    'get_udf_match_attribute',
    'create_udf_group',
    'remove_udf_group',
    'set_udf_group_attribute',
    'get_udf_group_attribute',
]
struct__sai_udf_api_t._fields_ = [
    ('create_udf', sai_create_udf_fn),
    ('remove_udf', sai_remove_udf_fn),
    ('set_udf_attribute', sai_set_udf_attribute_fn),
    ('get_udf_attribute', sai_get_udf_attribute_fn),
    ('create_udf_match', sai_create_udf_match_fn),
    ('remove_udf_match', sai_remove_udf_match_fn),
    ('set_udf_match_attribute', sai_set_udf_match_attribute_fn),
    ('get_udf_match_attribute', sai_get_udf_match_attribute_fn),
    ('create_udf_group', sai_create_udf_group_fn),
    ('remove_udf_group', sai_remove_udf_group_fn),
    ('set_udf_group_attribute', sai_set_udf_group_attribute_fn),
    ('get_udf_group_attribute', sai_get_udf_group_attribute_fn),
]

sai_udf_api_t = struct__sai_udf_api_t # /home/omer/P4/SAI/inc/saiudf.h: 418

enum__sai_vlan_tagging_mode_t = c_int # /home/omer/P4/SAI/inc/saivlan.h: 52

SAI_VLAN_TAGGING_MODE_UNTAGGED = 0 # /home/omer/P4/SAI/inc/saivlan.h: 52

SAI_VLAN_TAGGING_MODE_TAGGED = (SAI_VLAN_TAGGING_MODE_UNTAGGED + 1) # /home/omer/P4/SAI/inc/saivlan.h: 52

SAI_VLAN_TAGGING_MODE_PRIORITY_TAGGED = (SAI_VLAN_TAGGING_MODE_TAGGED + 1) # /home/omer/P4/SAI/inc/saivlan.h: 52

sai_vlan_tagging_mode_t = enum__sai_vlan_tagging_mode_t # /home/omer/P4/SAI/inc/saivlan.h: 52

enum__sai_vlan_mcast_lookup_key_type_t = c_int # /home/omer/P4/SAI/inc/saivlan.h: 67

SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_MAC_DA = 0 # /home/omer/P4/SAI/inc/saivlan.h: 67

SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_XG = (SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_MAC_DA + 1) # /home/omer/P4/SAI/inc/saivlan.h: 67

SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_SG = (SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_XG + 1) # /home/omer/P4/SAI/inc/saivlan.h: 67

SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_XG_AND_SG = (SAI_VLAN_MCAST_LOOKUP_KEY_TYPE_SG + 1) # /home/omer/P4/SAI/inc/saivlan.h: 67

sai_vlan_mcast_lookup_key_type_t = enum__sai_vlan_mcast_lookup_key_type_t # /home/omer/P4/SAI/inc/saivlan.h: 67

enum__sai_vlan_attr_t = c_int # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_START = 0 # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_VLAN_ID = SAI_VLAN_ATTR_START # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_MEMBER_LIST = (SAI_VLAN_ATTR_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES = (SAI_VLAN_ATTR_MEMBER_LIST + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_STP_INSTANCE = (SAI_VLAN_ATTR_MAX_LEARNED_ADDRESSES + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_LEARN_DISABLE = (SAI_VLAN_ATTR_STP_INSTANCE + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_IPV4_MCAST_LOOKUP_KEY_TYPE = (SAI_VLAN_ATTR_LEARN_DISABLE + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_IPV6_MCAST_LOOKUP_KEY_TYPE = (SAI_VLAN_ATTR_IPV4_MCAST_LOOKUP_KEY_TYPE + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_UNKNOWN_NON_IP_MCAST_OUTPUT_GROUP_ID = (SAI_VLAN_ATTR_IPV6_MCAST_LOOKUP_KEY_TYPE + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_UNKNOWN_IPV4_MCAST_OUTPUT_GROUP_ID = (SAI_VLAN_ATTR_UNKNOWN_NON_IP_MCAST_OUTPUT_GROUP_ID + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_UNKNOWN_IPV6_MCAST_OUTPUT_GROUP_ID = (SAI_VLAN_ATTR_UNKNOWN_IPV4_MCAST_OUTPUT_GROUP_ID + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_UNKNOWN_LINKLOCAL_MCAST_OUTPUT_GROUP_ID = (SAI_VLAN_ATTR_UNKNOWN_IPV6_MCAST_OUTPUT_GROUP_ID + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_INGRESS_ACL = (SAI_VLAN_ATTR_UNKNOWN_LINKLOCAL_MCAST_OUTPUT_GROUP_ID + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_EGRESS_ACL = (SAI_VLAN_ATTR_INGRESS_ACL + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_META_DATA = (SAI_VLAN_ATTR_EGRESS_ACL + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_END = (SAI_VLAN_ATTR_META_DATA + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saivlan.h: 277

SAI_VLAN_ATTR_CUSTOM_RANGE_END = (SAI_VLAN_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saivlan.h: 277

sai_vlan_attr_t = enum__sai_vlan_attr_t # /home/omer/P4/SAI/inc/saivlan.h: 277

enum__sai_vlan_member_attr_t = c_int # /home/omer/P4/SAI/inc/saivlan.h: 327

SAI_VLAN_MEMBER_ATTR_START = 0 # /home/omer/P4/SAI/inc/saivlan.h: 327

SAI_VLAN_MEMBER_ATTR_VLAN_ID = SAI_VLAN_MEMBER_ATTR_START # /home/omer/P4/SAI/inc/saivlan.h: 327

SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID = (SAI_VLAN_MEMBER_ATTR_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saivlan.h: 327

SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE = (SAI_VLAN_MEMBER_ATTR_BRIDGE_PORT_ID + 1) # /home/omer/P4/SAI/inc/saivlan.h: 327

SAI_VLAN_MEMBER_ATTR_END = (SAI_VLAN_MEMBER_ATTR_VLAN_TAGGING_MODE + 1) # /home/omer/P4/SAI/inc/saivlan.h: 327

SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saivlan.h: 327

SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_END = (SAI_VLAN_MEMBER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saivlan.h: 327

sai_vlan_member_attr_t = enum__sai_vlan_member_attr_t # /home/omer/P4/SAI/inc/saivlan.h: 327

enum__sai_vlan_stat_t = c_int # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_IN_OCTETS = 0 # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_IN_PACKETS = (SAI_VLAN_STAT_IN_OCTETS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_IN_UCAST_PKTS = (SAI_VLAN_STAT_IN_PACKETS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_IN_NON_UCAST_PKTS = (SAI_VLAN_STAT_IN_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_IN_DISCARDS = (SAI_VLAN_STAT_IN_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_IN_ERRORS = (SAI_VLAN_STAT_IN_DISCARDS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_IN_UNKNOWN_PROTOS = (SAI_VLAN_STAT_IN_ERRORS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_OUT_OCTETS = (SAI_VLAN_STAT_IN_UNKNOWN_PROTOS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_OUT_PACKETS = (SAI_VLAN_STAT_OUT_OCTETS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_OUT_UCAST_PKTS = (SAI_VLAN_STAT_OUT_PACKETS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_OUT_NON_UCAST_PKTS = (SAI_VLAN_STAT_OUT_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_OUT_DISCARDS = (SAI_VLAN_STAT_OUT_NON_UCAST_PKTS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_OUT_ERRORS = (SAI_VLAN_STAT_OUT_DISCARDS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

SAI_VLAN_STAT_OUT_QLEN = (SAI_VLAN_STAT_OUT_ERRORS + 1) # /home/omer/P4/SAI/inc/saivlan.h: 349

sai_vlan_stat_t = enum__sai_vlan_stat_t # /home/omer/P4/SAI/inc/saivlan.h: 349

sai_create_vlan_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivlan.h: 361

sai_remove_vlan_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saivlan.h: 374

sai_set_vlan_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivlan.h: 385

sai_get_vlan_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivlan.h: 398

sai_create_vlan_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivlan.h: 413

sai_remove_vlan_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saivlan.h: 426

sai_set_vlan_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivlan.h: 437

sai_get_vlan_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saivlan.h: 450

sai_get_vlan_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_vlan_stat_t), c_uint32, POINTER(c_uint64)) # /home/omer/P4/SAI/inc/saivlan.h: 465

sai_clear_vlan_stats_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_vlan_stat_t), c_uint32) # /home/omer/P4/SAI/inc/saivlan.h: 480

# /home/omer/P4/SAI/inc/saivlan.h: 503
class struct__sai_vlan_api_t(Structure):
    pass

struct__sai_vlan_api_t.__slots__ = [
    'create_vlan',
    'remove_vlan',
    'set_vlan_attribute',
    'get_vlan_attribute',
    'create_vlan_member',
    'remove_vlan_member',
    'set_vlan_member_attribute',
    'get_vlan_member_attribute',
    'create_vlan_members',
    'remove_vlan_members',
    'get_vlan_stats',
    'clear_vlan_stats',
]
struct__sai_vlan_api_t._fields_ = [
    ('create_vlan', sai_create_vlan_fn),
    ('remove_vlan', sai_remove_vlan_fn),
    ('set_vlan_attribute', sai_set_vlan_attribute_fn),
    ('get_vlan_attribute', sai_get_vlan_attribute_fn),
    ('create_vlan_member', sai_create_vlan_member_fn),
    ('remove_vlan_member', sai_remove_vlan_member_fn),
    ('set_vlan_member_attribute', sai_set_vlan_member_attribute_fn),
    ('get_vlan_member_attribute', sai_get_vlan_member_attribute_fn),
    ('create_vlan_members', sai_bulk_object_create_fn),
    ('remove_vlan_members', sai_bulk_object_remove_fn),
    ('get_vlan_stats', sai_get_vlan_stats_fn),
    ('clear_vlan_stats', sai_clear_vlan_stats_fn),
]

sai_vlan_api_t = struct__sai_vlan_api_t # /home/omer/P4/SAI/inc/saivlan.h: 503

enum__sai_ecn_mark_mode_t = c_int # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_NONE = 0 # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_GREEN = (SAI_ECN_MARK_MODE_NONE + 1) # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_YELLOW = (SAI_ECN_MARK_MODE_GREEN + 1) # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_RED = (SAI_ECN_MARK_MODE_YELLOW + 1) # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_GREEN_YELLOW = (SAI_ECN_MARK_MODE_RED + 1) # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_GREEN_RED = (SAI_ECN_MARK_MODE_GREEN_YELLOW + 1) # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_YELLOW_RED = (SAI_ECN_MARK_MODE_GREEN_RED + 1) # /home/omer/P4/SAI/inc/saiwred.h: 65

SAI_ECN_MARK_MODE_ALL = (SAI_ECN_MARK_MODE_YELLOW_RED + 1) # /home/omer/P4/SAI/inc/saiwred.h: 65

sai_ecn_mark_mode_t = enum__sai_ecn_mark_mode_t # /home/omer/P4/SAI/inc/saiwred.h: 65

enum__sai_wred_attr_t = c_int # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_GREEN_ENABLE = SAI_WRED_ATTR_START # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_GREEN_MIN_THRESHOLD = 1 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_GREEN_MAX_THRESHOLD = 2 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_GREEN_DROP_PROBABILITY = 3 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_YELLOW_ENABLE = 4 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_YELLOW_MIN_THRESHOLD = 5 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_YELLOW_MAX_THRESHOLD = 6 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_YELLOW_DROP_PROBABILITY = 7 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_RED_ENABLE = 8 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_RED_MIN_THRESHOLD = 9 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_RED_MAX_THRESHOLD = 10 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_RED_DROP_PROBABILITY = 11 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_WEIGHT = 12 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_ECN_MARK_MODE = 13 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_END = (SAI_WRED_ATTR_ECN_MARK_MODE + 1) # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiwred.h: 240

SAI_WRED_ATTR_CUSTOM_RANGE_END = (SAI_WRED_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiwred.h: 240

sai_wred_attr_t = enum__sai_wred_attr_t # /home/omer/P4/SAI/inc/saiwred.h: 240

sai_create_wred_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiwred.h: 252

sai_remove_wred_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiwred.h: 265

sai_set_wred_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiwred.h: 276

sai_get_wred_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiwred.h: 289

# /home/omer/P4/SAI/inc/saiwred.h: 304
class struct__sai_wred_api_t(Structure):
    pass

struct__sai_wred_api_t.__slots__ = [
    'create_wred',
    'remove_wred',
    'set_wred_attribute',
    'get_wred_attribute',
]
struct__sai_wred_api_t._fields_ = [
    ('create_wred', sai_create_wred_fn),
    ('remove_wred', sai_remove_wred_fn),
    ('set_wred_attribute', sai_set_wred_attribute_fn),
    ('get_wred_attribute', sai_get_wred_attribute_fn),
]

sai_wred_api_t = struct__sai_wred_api_t # /home/omer/P4/SAI/inc/saiwred.h: 304

enum__sai_bridge_port_fdb_learning_mode_t = c_int # /home/omer/P4/SAI/inc/saibridge.h: 66

SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DROP = 0 # /home/omer/P4/SAI/inc/saibridge.h: 66

SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE = (SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DROP + 1) # /home/omer/P4/SAI/inc/saibridge.h: 66

SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW = (SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE + 1) # /home/omer/P4/SAI/inc/saibridge.h: 66

SAI_BRIDGE_PORT_FDB_LEARNING_MODE_CPU_TRAP = (SAI_BRIDGE_PORT_FDB_LEARNING_MODE_HW + 1) # /home/omer/P4/SAI/inc/saibridge.h: 66

SAI_BRIDGE_PORT_FDB_LEARNING_MODE_CPU_LOG = (SAI_BRIDGE_PORT_FDB_LEARNING_MODE_CPU_TRAP + 1) # /home/omer/P4/SAI/inc/saibridge.h: 66

SAI_BRIDGE_PORT_FDB_LEARNING_MODE_FDB_NOTIFICATION = (SAI_BRIDGE_PORT_FDB_LEARNING_MODE_CPU_LOG + 1) # /home/omer/P4/SAI/inc/saibridge.h: 66

sai_bridge_port_fdb_learning_mode_t = enum__sai_bridge_port_fdb_learning_mode_t # /home/omer/P4/SAI/inc/saibridge.h: 66

enum__sai_bridge_port_type_t = c_int # /home/omer/P4/SAI/inc/saibridge.h: 88

SAI_BRIDGE_PORT_TYPE_PORT = 0 # /home/omer/P4/SAI/inc/saibridge.h: 88

SAI_BRIDGE_PORT_TYPE_SUB_PORT = (SAI_BRIDGE_PORT_TYPE_PORT + 1) # /home/omer/P4/SAI/inc/saibridge.h: 88

SAI_BRIDGE_PORT_TYPE_1Q_ROUTER = (SAI_BRIDGE_PORT_TYPE_SUB_PORT + 1) # /home/omer/P4/SAI/inc/saibridge.h: 88

SAI_BRIDGE_PORT_TYPE_1D_ROUTER = (SAI_BRIDGE_PORT_TYPE_1Q_ROUTER + 1) # /home/omer/P4/SAI/inc/saibridge.h: 88

SAI_BRIDGE_PORT_TYPE_TUNNEL = (SAI_BRIDGE_PORT_TYPE_1D_ROUTER + 1) # /home/omer/P4/SAI/inc/saibridge.h: 88

sai_bridge_port_type_t = enum__sai_bridge_port_type_t # /home/omer/P4/SAI/inc/saibridge.h: 88

enum__sai_bridge_port_attr_t = c_int # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_START = 0 # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_TYPE = SAI_BRIDGE_PORT_ATTR_START # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_PORT_ID = (SAI_BRIDGE_PORT_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_VLAN_ID = (SAI_BRIDGE_PORT_ATTR_PORT_ID + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_RIF_ID = (SAI_BRIDGE_PORT_ATTR_VLAN_ID + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_TUNNEL_ID = (SAI_BRIDGE_PORT_ATTR_RIF_ID + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_BRIDGE_ID = (SAI_BRIDGE_PORT_ATTR_TUNNEL_ID + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_MODE = (SAI_BRIDGE_PORT_ATTR_BRIDGE_ID + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_MAX_LEARNED_ADDRESSES = (SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_MODE + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION_PACKET_ACTION = (SAI_BRIDGE_PORT_ATTR_MAX_LEARNED_ADDRESSES + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_END = (SAI_BRIDGE_PORT_ATTR_FDB_LEARNING_LIMIT_VIOLATION_PACKET_ACTION + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saibridge.h: 200

SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_END = (SAI_BRIDGE_PORT_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saibridge.h: 200

sai_bridge_port_attr_t = enum__sai_bridge_port_attr_t # /home/omer/P4/SAI/inc/saibridge.h: 200

sai_create_bridge_port_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibridge.h: 212

sai_remove_bridge_port_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saibridge.h: 225

sai_set_bridge_port_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibridge.h: 236

sai_get_bridge_port_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibridge.h: 249

enum__sai_bridge_type_t = c_int # /home/omer/P4/SAI/inc/saibridge.h: 265

SAI_BRIDGE_TYPE_1Q = 0 # /home/omer/P4/SAI/inc/saibridge.h: 265

SAI_BRIDGE_TYPE_1D = (SAI_BRIDGE_TYPE_1Q + 1) # /home/omer/P4/SAI/inc/saibridge.h: 265

sai_bridge_type_t = enum__sai_bridge_type_t # /home/omer/P4/SAI/inc/saibridge.h: 265

enum__sai_bridge_attr_t = c_int # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_START = 0 # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_TYPE = SAI_BRIDGE_ATTR_START # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_PORT_LIST = (SAI_BRIDGE_ATTR_TYPE + 1) # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_MAX_LEARNED_ADDRESSES = (SAI_BRIDGE_ATTR_PORT_LIST + 1) # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_LEARN_DISABLE = (SAI_BRIDGE_ATTR_MAX_LEARNED_ADDRESSES + 1) # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_END = (SAI_BRIDGE_ATTR_LEARN_DISABLE + 1) # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saibridge.h: 325

SAI_BRIDGE_ATTR_CUSTOM_RANGE_END = (SAI_BRIDGE_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saibridge.h: 325

sai_bridge_attr_t = enum__sai_bridge_attr_t # /home/omer/P4/SAI/inc/saibridge.h: 325

sai_create_bridge_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibridge.h: 337

sai_remove_bridge_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saibridge.h: 350

sai_set_bridge_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibridge.h: 361

sai_get_bridge_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saibridge.h: 374

# /home/omer/P4/SAI/inc/saibridge.h: 392
class struct__sai_bridge_api_t(Structure):
    pass

struct__sai_bridge_api_t.__slots__ = [
    'create_bridge',
    'remove_bridge',
    'set_bridge_attribute',
    'get_bridge_attribute',
    'create_bridge_port',
    'remove_bridge_port',
    'set_bridge_port_attribute',
    'get_bridge_port_attribute',
]
struct__sai_bridge_api_t._fields_ = [
    ('create_bridge', sai_create_bridge_fn),
    ('remove_bridge', sai_remove_bridge_fn),
    ('set_bridge_attribute', sai_set_bridge_attribute_fn),
    ('get_bridge_attribute', sai_get_bridge_attribute_fn),
    ('create_bridge_port', sai_create_bridge_port_fn),
    ('remove_bridge_port', sai_remove_bridge_port_fn),
    ('set_bridge_port_attribute', sai_set_bridge_port_attribute_fn),
    ('get_bridge_port_attribute', sai_get_bridge_port_attribute_fn),
]

sai_bridge_api_t = struct__sai_bridge_api_t # /home/omer/P4/SAI/inc/saibridge.h: 392

enum__sai_rpf_group_attr_t = c_int # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

SAI_RPF_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

SAI_RPF_GROUP_ATTR_RPF_INTERFACE_COUNT = SAI_RPF_GROUP_ATTR_START # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

SAI_RPF_GROUP_ATTR_RPF_MEMBER_LIST = (SAI_RPF_GROUP_ATTR_RPF_INTERFACE_COUNT + 1) # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

SAI_RPF_GROUP_ATTR_END = (SAI_RPF_GROUP_ATTR_RPF_MEMBER_LIST + 1) # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_END = (SAI_RPF_GROUP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

sai_rpf_group_attr_t = enum__sai_rpf_group_attr_t # /home/omer/P4/SAI/inc/sairpfgroup.h: 74

enum__sai_rpf_group_member_attr_t = c_int # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

SAI_RPF_GROUP_MEMBER_ATTR_START = 0 # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

SAI_RPF_GROUP_MEMBER_ATTR_RPF_GROUP_ID = SAI_RPF_GROUP_MEMBER_ATTR_START # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

SAI_RPF_GROUP_MEMBER_ATTR_RPF_INTERFACE_ID = (SAI_RPF_GROUP_MEMBER_ATTR_RPF_GROUP_ID + 1) # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

SAI_RPF_GROUP_MEMBER_ATTR_END = (SAI_RPF_GROUP_MEMBER_ATTR_RPF_INTERFACE_ID + 1) # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END = (SAI_RPF_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

sai_rpf_group_member_attr_t = enum__sai_rpf_group_member_attr_t # /home/omer/P4/SAI/inc/sairpfgroup.h: 112

sai_create_rpf_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairpfgroup.h: 124

sai_remove_rpf_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sairpfgroup.h: 137

sai_set_rpf_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairpfgroup.h: 148

sai_get_rpf_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairpfgroup.h: 161

sai_create_rpf_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairpfgroup.h: 175

sai_remove_rpf_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sairpfgroup.h: 188

sai_set_rpf_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairpfgroup.h: 199

sai_get_rpf_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sairpfgroup.h: 212

# /home/omer/P4/SAI/inc/sairpfgroup.h: 231
class struct__sai_rpf_group_api_t(Structure):
    pass

struct__sai_rpf_group_api_t.__slots__ = [
    'create_rpf_group',
    'remove_rpf_group',
    'set_rpf_group_attribute',
    'get_rpf_group_attribute',
    'create_rpf_group_member',
    'remove_rpf_group_member',
    'set_rpf_group_member_attribute',
    'get_rpf_group_member_attribute',
]
struct__sai_rpf_group_api_t._fields_ = [
    ('create_rpf_group', sai_create_rpf_group_fn),
    ('remove_rpf_group', sai_remove_rpf_group_fn),
    ('set_rpf_group_attribute', sai_set_rpf_group_attribute_fn),
    ('get_rpf_group_attribute', sai_get_rpf_group_attribute_fn),
    ('create_rpf_group_member', sai_create_rpf_group_member_fn),
    ('remove_rpf_group_member', sai_remove_rpf_group_member_fn),
    ('set_rpf_group_member_attribute', sai_set_rpf_group_member_attribute_fn),
    ('get_rpf_group_member_attribute', sai_get_rpf_group_member_attribute_fn),
]

sai_rpf_group_api_t = struct__sai_rpf_group_api_t # /home/omer/P4/SAI/inc/sairpfgroup.h: 231

enum__sai_l2mc_group_attr_t = c_int # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

SAI_L2MC_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

SAI_L2MC_GROUP_ATTR_L2MC_OUTPUT_COUNT = SAI_L2MC_GROUP_ATTR_START # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

SAI_L2MC_GROUP_ATTR_L2MC_MEMBER_LIST = (SAI_L2MC_GROUP_ATTR_L2MC_OUTPUT_COUNT + 1) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

SAI_L2MC_GROUP_ATTR_END = (SAI_L2MC_GROUP_ATTR_L2MC_MEMBER_LIST + 1) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

SAI_L2MC_GROUP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

SAI_L2MC_GROUP_ATTR_CUSTOM_RANGE_END = (SAI_L2MC_GROUP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

sai_l2mc_group_attr_t = enum__sai_l2mc_group_attr_t # /home/omer/P4/SAI/inc/sail2mcgroup.h: 74

enum__sai_l2mc_group_member_attr_t = c_int # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

SAI_L2MC_GROUP_MEMBER_ATTR_START = 0 # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_GROUP_ID = SAI_L2MC_GROUP_MEMBER_ATTR_START # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_OUTPUT_ID = (SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_GROUP_ID + 1) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

SAI_L2MC_GROUP_MEMBER_ATTR_END = (SAI_L2MC_GROUP_MEMBER_ATTR_L2MC_OUTPUT_ID + 1) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

SAI_L2MC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

SAI_L2MC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END = (SAI_L2MC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

sai_l2mc_group_member_attr_t = enum__sai_l2mc_group_member_attr_t # /home/omer/P4/SAI/inc/sail2mcgroup.h: 112

sai_create_l2mc_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 124

sai_remove_l2mc_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 137

sai_set_l2mc_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 148

sai_get_l2mc_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 161

sai_create_l2mc_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 175

sai_remove_l2mc_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 188

sai_set_l2mc_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 199

sai_get_l2mc_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/sail2mcgroup.h: 212

# /home/omer/P4/SAI/inc/sail2mcgroup.h: 231
class struct__sai_l2mc_group_api_t(Structure):
    pass

struct__sai_l2mc_group_api_t.__slots__ = [
    'create_l2mc_group',
    'remove_l2mc_group',
    'set_l2mc_group_attribute',
    'get_l2mc_group_attribute',
    'create_l2mc_group_member',
    'remove_l2mc_group_member',
    'set_l2mc_group_member_attribute',
    'get_l2mc_group_member_attribute',
]
struct__sai_l2mc_group_api_t._fields_ = [
    ('create_l2mc_group', sai_create_l2mc_group_fn),
    ('remove_l2mc_group', sai_remove_l2mc_group_fn),
    ('set_l2mc_group_attribute', sai_set_l2mc_group_attribute_fn),
    ('get_l2mc_group_attribute', sai_get_l2mc_group_attribute_fn),
    ('create_l2mc_group_member', sai_create_l2mc_group_member_fn),
    ('remove_l2mc_group_member', sai_remove_l2mc_group_member_fn),
    ('set_l2mc_group_member_attribute', sai_set_l2mc_group_member_attribute_fn),
    ('get_l2mc_group_member_attribute', sai_get_l2mc_group_member_attribute_fn),
]

sai_l2mc_group_api_t = struct__sai_l2mc_group_api_t # /home/omer/P4/SAI/inc/sail2mcgroup.h: 231

enum__sai_ipmc_group_attr_t = c_int # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

SAI_IPMC_GROUP_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

SAI_IPMC_GROUP_ATTR_IPMC_OUTPUT_COUNT = SAI_IPMC_GROUP_ATTR_START # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

SAI_IPMC_GROUP_ATTR_IPMC_MEMBER_LIST = (SAI_IPMC_GROUP_ATTR_IPMC_OUTPUT_COUNT + 1) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

SAI_IPMC_GROUP_ATTR_END = (SAI_IPMC_GROUP_ATTR_IPMC_MEMBER_LIST + 1) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

SAI_IPMC_GROUP_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

SAI_IPMC_GROUP_ATTR_CUSTOM_RANGE_END = (SAI_IPMC_GROUP_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

sai_ipmc_group_attr_t = enum__sai_ipmc_group_attr_t # /home/omer/P4/SAI/inc/saiipmcgroup.h: 74

enum__sai_ipmc_group_member_attr_t = c_int # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

SAI_IPMC_GROUP_MEMBER_ATTR_START = 0 # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_GROUP_ID = SAI_IPMC_GROUP_MEMBER_ATTR_START # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_OUTPUT_ID = (SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_GROUP_ID + 1) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

SAI_IPMC_GROUP_MEMBER_ATTR_END = (SAI_IPMC_GROUP_MEMBER_ATTR_IPMC_OUTPUT_ID + 1) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

SAI_IPMC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START = 268435456 # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

SAI_IPMC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_END = (SAI_IPMC_GROUP_MEMBER_ATTR_CUSTOM_RANGE_START + 1) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

sai_ipmc_group_member_attr_t = enum__sai_ipmc_group_member_attr_t # /home/omer/P4/SAI/inc/saiipmcgroup.h: 112

sai_create_ipmc_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 124

sai_remove_ipmc_group_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 137

sai_set_ipmc_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 148

sai_get_ipmc_group_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 161

sai_create_ipmc_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), POINTER(sai_object_id_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 175

sai_remove_ipmc_group_member_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 188

sai_set_ipmc_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 199

sai_get_ipmc_group_member_attribute_fn = CFUNCTYPE(UNCHECKED(sai_status_t), sai_object_id_t, c_uint32, POINTER(sai_attribute_t)) # /home/omer/P4/SAI/inc/saiipmcgroup.h: 212

# /home/omer/P4/SAI/inc/saiipmcgroup.h: 231
class struct__sai_ipmc_group_api_t(Structure):
    pass

struct__sai_ipmc_group_api_t.__slots__ = [
    'create_ipmc_group',
    'remove_ipmc_group',
    'set_ipmc_group_attribute',
    'get_ipmc_group_attribute',
    'create_ipmc_group_member',
    'remove_ipmc_group_member',
    'set_ipmc_group_member_attribute',
    'get_ipmc_group_member_attribute',
]
struct__sai_ipmc_group_api_t._fields_ = [
    ('create_ipmc_group', sai_create_ipmc_group_fn),
    ('remove_ipmc_group', sai_remove_ipmc_group_fn),
    ('set_ipmc_group_attribute', sai_set_ipmc_group_attribute_fn),
    ('get_ipmc_group_attribute', sai_get_ipmc_group_attribute_fn),
    ('create_ipmc_group_member', sai_create_ipmc_group_member_fn),
    ('remove_ipmc_group_member', sai_remove_ipmc_group_member_fn),
    ('set_ipmc_group_member_attribute', sai_set_ipmc_group_member_attribute_fn),
    ('get_ipmc_group_member_attribute', sai_get_ipmc_group_member_attribute_fn),
]

sai_ipmc_group_api_t = struct__sai_ipmc_group_api_t # /home/omer/P4/SAI/inc/saiipmcgroup.h: 231

enum__sai_api_t = c_int # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_UNSPECIFIED = 0 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_SWITCH = 1 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_PORT = 2 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_FDB = 3 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_VLAN = 4 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_VIRTUAL_ROUTER = 5 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_ROUTE = 6 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_NEXT_HOP = 7 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_NEXT_HOP_GROUP = 8 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_ROUTER_INTERFACE = 9 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_NEIGHBOR = 10 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_ACL = 11 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_HOSTIF = 12 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_MIRROR = 13 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_SAMPLEPACKET = 14 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_STP = 15 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_LAG = 16 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_POLICER = 17 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_WRED = 18 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_QOS_MAP = 19 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_QUEUE = 20 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_SCHEDULER = 21 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_SCHEDULER_GROUP = 22 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_BUFFER = 23 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_HASH = 24 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_UDF = 25 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_TUNNEL = 26 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_L2MC = 27 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_IPMC = 28 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_RPF_GROUP = 29 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_L2MC_GROUP = 30 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_IPMC_GROUP = 31 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_MCAST_FDB = 32 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_BRIDGE = 33 # /home/omer/P4/SAI/inc/sai.h: 115

SAI_API_MAX = 34 # /home/omer/P4/SAI/inc/sai.h: 115

sai_api_t = enum__sai_api_t # /home/omer/P4/SAI/inc/sai.h: 115

enum__sai_log_level_t = c_int # /home/omer/P4/SAI/inc/sai.h: 140

SAI_LOG_LEVEL_DEBUG = 0 # /home/omer/P4/SAI/inc/sai.h: 140

SAI_LOG_LEVEL_INFO = 1 # /home/omer/P4/SAI/inc/sai.h: 140

SAI_LOG_LEVEL_NOTICE = 2 # /home/omer/P4/SAI/inc/sai.h: 140

SAI_LOG_LEVEL_WARN = 3 # /home/omer/P4/SAI/inc/sai.h: 140

SAI_LOG_LEVEL_ERROR = 4 # /home/omer/P4/SAI/inc/sai.h: 140

SAI_LOG_LEVEL_CRITICAL = 5 # /home/omer/P4/SAI/inc/sai.h: 140

sai_log_level_t = enum__sai_log_level_t # /home/omer/P4/SAI/inc/sai.h: 140

sai_profile_get_value_fn = CFUNCTYPE(UNCHECKED(String), sai_switch_profile_id_t, String) # /home/omer/P4/SAI/inc/sai.h: 142

sai_profile_get_next_value_fn = CFUNCTYPE(UNCHECKED(c_int), sai_switch_profile_id_t, POINTER(POINTER(c_char)), POINTER(POINTER(c_char))) # /home/omer/P4/SAI/inc/sai.h: 146

# /home/omer/P4/SAI/inc/sai.h: 170
class struct__service_method_table_t(Structure):
    pass

struct__service_method_table_t.__slots__ = [
    'profile_get_value',
    'profile_get_next_value',
]
struct__service_method_table_t._fields_ = [
    ('profile_get_value', sai_profile_get_value_fn),
    ('profile_get_next_value', sai_profile_get_next_value_fn),
]

service_method_table_t = struct__service_method_table_t # /home/omer/P4/SAI/inc/sai.h: 170

# /home/omer/P4/SAI/inc/sai.h: 180
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_api_initialize'):
        continue
    sai_api_initialize = _lib.sai_api_initialize
    sai_api_initialize.argtypes = [c_uint64, POINTER(service_method_table_t)]
    sai_api_initialize.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/sai.h: 194
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_api_query'):
        continue
    sai_api_query = _lib.sai_api_query
    sai_api_query.argtypes = [sai_api_t, POINTER(POINTER(None))]
    sai_api_query.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/sai.h: 204
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_api_uninitialize'):
        continue
    sai_api_uninitialize = _lib.sai_api_uninitialize
    sai_api_uninitialize.argtypes = []
    sai_api_uninitialize.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/sai.h: 214
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_log_set'):
        continue
    sai_log_set = _lib.sai_log_set
    sai_log_set.argtypes = [sai_api_t, sai_log_level_t]
    sai_log_set.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/sai.h: 226
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_object_type_query'):
        continue
    sai_object_type_query = _lib.sai_object_type_query
    sai_object_type_query.argtypes = [sai_object_id_t]
    sai_object_type_query.restype = sai_object_type_t
    break

# /home/omer/P4/SAI/inc/sai.h: 239
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_switch_id_query'):
        continue
    sai_switch_id_query = _lib.sai_switch_id_query
    sai_switch_id_query.argtypes = [sai_object_id_t]
    sai_switch_id_query.restype = sai_object_id_t
    break

# /home/omer/P4/SAI/inc/sai.h: 249
for _lib in _libs.itervalues():
    if not hasattr(_lib, 'sai_dbg_generate_dump'):
        continue
    sai_dbg_generate_dump = _lib.sai_dbg_generate_dump
    sai_dbg_generate_dump.argtypes = [String]
    sai_dbg_generate_dump.restype = sai_status_t
    break

# /home/omer/P4/SAI/inc/saitypes.h: 122
try:
    SAI_NULL_OBJECT_ID = 0L
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 43
def SAI_STATUS_CODE(_S_):
    return (-_S_)

# /home/omer/P4/SAI/inc/saistatus.h: 50
try:
    SAI_STATUS_SUCCESS = 0L
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 55
try:
    SAI_STATUS_FAILURE = (SAI_STATUS_CODE (1L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 60
try:
    SAI_STATUS_NOT_SUPPORTED = (SAI_STATUS_CODE (2L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 65
try:
    SAI_STATUS_NO_MEMORY = (SAI_STATUS_CODE (3L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 70
try:
    SAI_STATUS_INSUFFICIENT_RESOURCES = (SAI_STATUS_CODE (4L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 75
try:
    SAI_STATUS_INVALID_PARAMETER = (SAI_STATUS_CODE (5L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 81
try:
    SAI_STATUS_ITEM_ALREADY_EXISTS = (SAI_STATUS_CODE (6L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 87
try:
    SAI_STATUS_ITEM_NOT_FOUND = (SAI_STATUS_CODE (7L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 92
try:
    SAI_STATUS_BUFFER_OVERFLOW = (SAI_STATUS_CODE (8L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 97
try:
    SAI_STATUS_INVALID_PORT_NUMBER = (SAI_STATUS_CODE (9L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 102
try:
    SAI_STATUS_INVALID_PORT_MEMBER = (SAI_STATUS_CODE (10L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 107
try:
    SAI_STATUS_INVALID_VLAN_ID = (SAI_STATUS_CODE (11L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 112
try:
    SAI_STATUS_UNINITIALIZED = (SAI_STATUS_CODE (12L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 117
try:
    SAI_STATUS_TABLE_FULL = (SAI_STATUS_CODE (13L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 122
try:
    SAI_STATUS_MANDATORY_ATTRIBUTE_MISSING = (SAI_STATUS_CODE (14L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 127
try:
    SAI_STATUS_NOT_IMPLEMENTED = (SAI_STATUS_CODE (15L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 132
try:
    SAI_STATUS_ADDR_NOT_FOUND = (SAI_STATUS_CODE (16L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 137
try:
    SAI_STATUS_OBJECT_IN_USE = (SAI_STATUS_CODE (17L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 144
try:
    SAI_STATUS_INVALID_OBJECT_TYPE = (SAI_STATUS_CODE (18L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 152
try:
    SAI_STATUS_INVALID_OBJECT_ID = (SAI_STATUS_CODE (19L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 157
try:
    SAI_STATUS_INVALID_NV_STORAGE = (SAI_STATUS_CODE (20L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 162
try:
    SAI_STATUS_NV_STORAGE_FULL = (SAI_STATUS_CODE (21L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 167
try:
    SAI_STATUS_SW_UPGRADE_VERSION_MISMATCH = (SAI_STATUS_CODE (22L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 172
try:
    SAI_STATUS_NOT_EXECUTED = (SAI_STATUS_CODE (23L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 185
try:
    SAI_STATUS_INVALID_ATTRIBUTE_0 = (SAI_STATUS_CODE (65536L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 190
try:
    SAI_STATUS_INVALID_ATTRIBUTE_MAX = (SAI_STATUS_CODE (131071L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 195
try:
    SAI_STATUS_INVALID_ATTR_VALUE_0 = (SAI_STATUS_CODE (131072L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 199
try:
    SAI_STATUS_INVALID_ATTR_VALUE_MAX = (SAI_STATUS_CODE (196607L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 207
try:
    SAI_STATUS_ATTR_NOT_IMPLEMENTED_0 = (SAI_STATUS_CODE (196608L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 212
try:
    SAI_STATUS_ATTR_NOT_IMPLEMENTED_MAX = (SAI_STATUS_CODE (262143L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 220
try:
    SAI_STATUS_UNKNOWN_ATTRIBUTE_0 = (SAI_STATUS_CODE (262144L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 225
try:
    SAI_STATUS_UNKNOWN_ATTRIBUTE_MAX = (SAI_STATUS_CODE (327679L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 233
try:
    SAI_STATUS_ATTR_NOT_SUPPORTED_0 = (SAI_STATUS_CODE (327680L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 238
try:
    SAI_STATUS_ATTR_NOT_SUPPORTED_MAX = (SAI_STATUS_CODE (393215L))
except:
    pass

# /home/omer/P4/SAI/inc/saistatus.h: 247
def SAI_STATUS_IS_INVALID_ATTRIBUTE(x):
    return (x & ((~65535) == SAI_STATUS_INVALID_ATTRIBUTE_0))

# /home/omer/P4/SAI/inc/saistatus.h: 252
def SAI_STATUS_IS_INVALID_ATTR_VALUE(x):
    return (x & ((~65535) == SAI_STATUS_INVALID_ATTR_VALUE_0))

# /home/omer/P4/SAI/inc/saistatus.h: 257
def SAI_STATUS_IS_ATTR_NOT_IMPLEMENTED(x):
    return (x & ((~65535) == SAI_STATUS_ATTR_NOT_IMPLEMENTED_0))

# /home/omer/P4/SAI/inc/saistatus.h: 262
def SAI_STATUS_IS_UNKNOWN_ATTRIBUTE(x):
    return (x & ((~65535) == SAI_STATUS_INVALID_ATTRIBUTE_0))

# /home/omer/P4/SAI/inc/saistatus.h: 267
def SAI_STATUS_IS_ATTR_NOT_SUPPORTED(x):
    return (x & ((~65535) == SAI_STATUS_ATTR_NOT_SUPPORTED_0))

# /home/omer/P4/SAI/inc/saiacl.h: 387
try:
    SAI_ACL_USER_DEFINED_FIELD_ATTR_ID_RANGE = 255
except:
    pass

# /home/omer/P4/SAI/inc/saihostif.h: 47
try:
    HOSTIF_NAME_SIZE = 16
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 42
try:
    SAI_MAX_HARDWARE_ID_LEN = 255
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1276
try:
    SAI_SWITCH_ATTR_MAX_KEY_STRING_LEN = 64
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1285
try:
    SAI_SWITCH_ATTR_MAX_KEY_COUNT = 16
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1294
try:
    SAI_KEY_FDB_TABLE_SIZE = 'SAI_FDB_TABLE_SIZE'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1299
try:
    SAI_KEY_L3_ROUTE_TABLE_SIZE = 'SAI_L3_ROUTE_TABLE_SIZE'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1304
try:
    SAI_KEY_L3_NEIGHBOR_TABLE_SIZE = 'SAI_L3_NEIGHBOR_TABLE_SIZE'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1309
try:
    SAI_KEY_NUM_LAG_MEMBERS = 'SAI_NUM_LAG_MEMBERS'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1314
try:
    SAI_KEY_NUM_LAGS = 'SAI_NUM_LAGS'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1319
try:
    SAI_KEY_NUM_ECMP_MEMBERS = 'SAI_NUM_ECMP_MEMBERS'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1324
try:
    SAI_KEY_NUM_ECMP_GROUPS = 'SAI_NUM_ECMP_GROUPS'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1329
try:
    SAI_KEY_NUM_UNICAST_QUEUES = 'SAI_NUM_UNICAST_QUEUES'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1334
try:
    SAI_KEY_NUM_MULTICAST_QUEUES = 'SAI_NUM_MULTICAST_QUEUES'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1339
try:
    SAI_KEY_NUM_QUEUES = 'SAI_NUM_QUEUES'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1344
try:
    SAI_KEY_NUM_CPU_QUEUES = 'SAI_NUM_CPU_QUEUES'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1349
try:
    SAI_KEY_INIT_CONFIG_FILE = 'SAI_INIT_CONFIG_FILE'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1359
try:
    SAI_KEY_BOOT_TYPE = 'SAI_BOOT_TYPE'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1365
try:
    SAI_KEY_WARM_BOOT_READ_FILE = 'SAI_WARM_BOOT_READ_FILE'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1371
try:
    SAI_KEY_WARM_BOOT_WRITE_FILE = 'SAI_WARM_BOOT_WRITE_FILE'
except:
    pass

# /home/omer/P4/SAI/inc/saiswitch.h: 1379
try:
    SAI_KEY_HW_PORT_PROFILE_ID_CONFIG_FILE = 'SAI_HW_PORT_PROFILE_ID_CONFIG_FILE'
except:
    pass

# /home/omer/P4/SAI/inc/saivlan.h: 39
try:
    VLAN_COUNTER_SET_DEFAULT = 0
except:
    pass

_sai_object_list_t = struct__sai_object_list_t # /home/omer/P4/SAI/inc/saitypes.h: 143

_sai_u8_list_t = struct__sai_u8_list_t # /home/omer/P4/SAI/inc/saitypes.h: 226

_sai_s8_list_t = struct__sai_s8_list_t # /home/omer/P4/SAI/inc/saitypes.h: 235

_sai_u16_list_t = struct__sai_u16_list_t # /home/omer/P4/SAI/inc/saitypes.h: 240

_sai_s16_list_t = struct__sai_s16_list_t # /home/omer/P4/SAI/inc/saitypes.h: 245

_sai_u32_list_t = struct__sai_u32_list_t # /home/omer/P4/SAI/inc/saitypes.h: 250

_sai_s32_list_t = struct__sai_s32_list_t # /home/omer/P4/SAI/inc/saitypes.h: 255

_sai_u32_range_t = struct__sai_u32_range_t # /home/omer/P4/SAI/inc/saitypes.h: 260

_sai_s32_range_t = struct__sai_s32_range_t # /home/omer/P4/SAI/inc/saitypes.h: 265

_sai_vlan_list_t = struct__sai_vlan_list_t # /home/omer/P4/SAI/inc/saitypes.h: 278

_sai_ip_address_t = struct__sai_ip_address_t # /home/omer/P4/SAI/inc/saitypes.h: 294

_sai_ip_prefix_t = struct__sai_ip_prefix_t # /home/omer/P4/SAI/inc/saitypes.h: 306

_sai_acl_field_data_t = struct__sai_acl_field_data_t # /home/omer/P4/SAI/inc/saitypes.h: 354

_sai_acl_action_data_t = struct__sai_acl_action_data_t # /home/omer/P4/SAI/inc/saitypes.h: 385

_sai_qos_map_params_t = struct__sai_qos_map_params_t # /home/omer/P4/SAI/inc/saitypes.h: 447

_sai_qos_map_t = struct__sai_qos_map_t # /home/omer/P4/SAI/inc/saitypes.h: 457

_sai_qos_map_list_t = struct__sai_qos_map_list_t # /home/omer/P4/SAI/inc/saitypes.h: 466

_sai_tunnel_map_params_t = struct__sai_tunnel_map_params_t # /home/omer/P4/SAI/inc/saitypes.h: 482

_sai_tunnel_map_t = struct__sai_tunnel_map_t # /home/omer/P4/SAI/inc/saitypes.h: 492

_sai_tunnel_map_list_t = struct__sai_tunnel_map_list_t # /home/omer/P4/SAI/inc/saitypes.h: 502

_sai_acl_capability_t = struct__sai_acl_capability_t # /home/omer/P4/SAI/inc/saitypes.h: 524

_sai_attribute_t = struct__sai_attribute_t # /home/omer/P4/SAI/inc/saitypes.h: 581

_sai_acl_api_t = struct__sai_acl_api_t # /home/omer/P4/SAI/inc/saiacl.h: 2253

_sai_buffer_api_t = struct__sai_buffer_api_t # /home/omer/P4/SAI/inc/saibuffer.h: 589

_sai_fdb_entry_t = struct__sai_fdb_entry_t # /home/omer/P4/SAI/inc/saifdb.h: 77

_sai_fdb_event_notification_data_t = struct__sai_fdb_event_notification_data_t # /home/omer/P4/SAI/inc/saifdb.h: 264

_sai_fdb_api_t = struct__sai_fdb_api_t # /home/omer/P4/SAI/inc/saifdb.h: 351

_sai_hash_api_t = struct__sai_hash_api_t # /home/omer/P4/SAI/inc/saihash.h: 185

_sai_hostif_api_t = struct__sai_hostif_api_t # /home/omer/P4/SAI/inc/saihostif.h: 1066

_sai_lag_api_t = struct__sai_lag_api_t # /home/omer/P4/SAI/inc/sailag.h: 280

_sai_mirror_api_t = struct__sai_mirror_api_t # /home/omer/P4/SAI/inc/saimirror.h: 312

_sai_neighbor_entry_t = struct__sai_neighbor_entry_t # /home/omer/P4/SAI/inc/saineighbor.h: 130

_sai_neighbor_api_t = struct__sai_neighbor_api_t # /home/omer/P4/SAI/inc/saineighbor.h: 206

_sai_next_hop_group_api_t = struct__sai_next_hop_group_api_t # /home/omer/P4/SAI/inc/sainexthopgroup.h: 261

_sai_next_hop_api_t = struct__sai_next_hop_api_t # /home/omer/P4/SAI/inc/sainexthop.h: 175

_sai_mcast_fdb_entry_t = struct__sai_mcast_fdb_entry_t # ../../../inc/saimcastfdb.h: 54

_sai_mcast_fdb_api_t = struct__sai_mcast_fdb_api_t # ../../../inc/saimcastfdb.h: 170

_sai_l2mc_entry_t = struct__sai_l2mc_entry_t # ../../../inc/sail2mc.h: 82

_sai_l2mc_api_t = struct__sai_l2mc_api_t # ../../../inc/sail2mc.h: 190

_sai_ipmc_entry_t = struct__sai_ipmc_entry_t # ../../../inc/saiipmc.h: 76

_sai_ipmc_api_t = struct__sai_ipmc_api_t # ../../../inc/saiipmc.h: 195

_sai_route_entry_t = struct__sai_route_entry_t # ../../../inc/sairoute.h: 138

_sai_route_api_t = struct__sai_route_api_t # ../../../inc/sairoute.h: 204

_sai_object_key_t = struct__sai_object_key_t # /home/omer/P4/SAI/inc/saiobject.h: 61

_sai_policer_api_t = struct__sai_policer_api_t # /home/omer/P4/SAI/inc/saipolicer.h: 344

_sai_port_oper_status_notification_t = struct__sai_port_oper_status_notification_t # /home/omer/P4/SAI/inc/saiport.h: 95

_sai_port_api_t = struct__sai_port_api_t # /home/omer/P4/SAI/inc/saiport.h: 1583

_sai_qos_map_api_t = struct__sai_qos_map_api_t # /home/omer/P4/SAI/inc/saiqosmap.h: 182

_sai_queue_api_t = struct__sai_queue_api_t # /home/omer/P4/SAI/inc/saiqueue.h: 354

_sai_virtual_router_api_t = struct__sai_virtual_router_api_t # /home/omer/P4/SAI/inc/saivirtualrouter.h: 188

_sai_router_interface_api_t = struct__sai_router_interface_api_t # /home/omer/P4/SAI/inc/sairouterinterface.h: 282

_sai_samplepacket_api_t = struct__sai_samplepacket_api_t # /home/omer/P4/SAI/inc/saisamplepacket.h: 185

_sai_scheduler_group_api_t = struct__sai_scheduler_group_api_t # /home/omer/P4/SAI/inc/saischedulergroup.h: 183

_sai_scheduler_api_t = struct__sai_scheduler_api_t # /home/omer/P4/SAI/inc/saischeduler.h: 207

_sai_stp_api_t = struct__sai_stp_api_t # /home/omer/P4/SAI/inc/saistp.h: 259

_sai_switch_api_t = struct__sai_switch_api_t # /home/omer/P4/SAI/inc/saiswitch.h: 1468

_sai_tunnel_api_t = struct__sai_tunnel_api_t # /home/omer/P4/SAI/inc/saitunnel.h: 854

_sai_udf_api_t = struct__sai_udf_api_t # /home/omer/P4/SAI/inc/saiudf.h: 418

_sai_vlan_api_t = struct__sai_vlan_api_t # /home/omer/P4/SAI/inc/saivlan.h: 503

_sai_wred_api_t = struct__sai_wred_api_t # /home/omer/P4/SAI/inc/saiwred.h: 304

_sai_bridge_api_t = struct__sai_bridge_api_t # /home/omer/P4/SAI/inc/saibridge.h: 392

_sai_rpf_group_api_t = struct__sai_rpf_group_api_t # /home/omer/P4/SAI/inc/sairpfgroup.h: 231

_sai_l2mc_group_api_t = struct__sai_l2mc_group_api_t # /home/omer/P4/SAI/inc/sail2mcgroup.h: 231

_sai_ipmc_group_api_t = struct__sai_ipmc_group_api_t # /home/omer/P4/SAI/inc/saiipmcgroup.h: 231

_service_method_table_t = struct__service_method_table_t # /home/omer/P4/SAI/inc/sai.h: 170

# No inserted files

