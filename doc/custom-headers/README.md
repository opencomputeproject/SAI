Proposal for SAI Custom Headers Directory
==============================================

SAI/custom is designed to hold custom headers provided by vendors.

By default this directory is empty, and no custom headers are present.  If
vendor want to provide custom headers, it can just simply copy custom headers
to that directory, and automatically that directory will be used by meta parser
to generate metadata for custom headers as well.

There is requirement that custom headers must still follow style convention
as regular headers.

This directory SAI/doc/custom-headers contains sample custom headers and how
they should look like. Base file is saicustom.h file which must be present if
vendor wants to add custom headers.

Custom headers, can extend existing object type, api, stats, attributes and
provide new apis and totally new enums.

Examing provided examples in this directory. If there will be problem with
custom headers, meta parser will provide error/warning message.
