Stub SAI implementation
============================

This folder contains SAI stub implementation.

Compilation is done by :
  1. ./autogen.sh
  2. ./configure --enable-debug --prefix=/usr
  3.  make
  4.  make install

The output result is SAI library, called libsai
User applications can then link with this library, in order to use the SAI stub implementation.

The implementation contains most of the attributes, as where in master branch of github on May 26, except :
  1. few new attributes are missing for switch API
  2. only few attributes are implemented for host interface 
  3. acl and qos are not implemented at all

The output is written to syslog USER facility
Verbose output is written for every implemented attribute

Most of the get attributes calls return default values
On create objects, an increasing static counter per object is used to return increasing object IDs.
Next hop group contains an almost full implementation in memory

Extensive parameter checking is done. It includes :
  1. Checking the attribute is valid for the feature API
  2. Checking the operation is valid for the attribute (for example, doesn't allow set for RO attribute)
  3. Checking all mandatory attributes are passed to create
  4. Checking an attribute doesn't appear twice in attribute list
  5. Checking the value of attributes of type list is not NULL
  6. Additional specific checks per function

All the stub sources are under Apache license

Future enhancments :
  1. Use SAI schema for parameter checking (developed by MS)
  2. Have a storage layer to store the created object. Right now, the attributes of an object is not store. The GET/SET calls are either noop or return predefined values.
