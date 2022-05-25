*gensairpc.pl* templates
========================

Templates are stored in *templates* directory and are written using [*Template Toolkit*](http://www.template-toolkit.org/).
They use data collected during the parsing ([`SAI`](SAI.md) module) and methods defined for all objects ([`SAI::RPC`](SAI-RPC.md)).
Almost all decisions are taken inside those methods.

## *sai.thrift.tt*
The main function is at the end of the file. Its goal is to:
1. define typedefs
1. define structures
1. define functions

All of those functionalities are implemented in separate blocks.
They define `common` API parts (usually required by remaining ones),
and then iterate over other APIs.
There are also some exceptions, like workarounds (related to the ordering, caused by
dependencies between types) and other manually written parts. For example `define_structs()` defines
`object` structures before `common` as part of workaround, and after defining remaining
structures it also defines manually written one (`sai_thrift_attribute_list_t`).

## *sai_adapter.py.tt*
The main loop is at the end of the file. It iterates over all APIs and defines
all their functions.
The function is composed from header, docstring and body itself, and all of them
are defined in separate block.

The generated code meets pep8 and pylint requirements (with some exceptions).
If the function is not supported (the regex for unsupported functions is at the beginning
of the file), then all arguments are not used, thus need to be `unset`.

### *sai_adapter_utils.tt*
This is not a standalone template. It is included by *sai_adapter.py.tt*, and optionally (see `--dev-utils`) used
to generate additional utilities, not related to the RPC client itself. Contains manually
written code.

## *sai_rpc_server.cpp.tt*
This file does not have the main loop. Since this template is generated, all functions
has their own declaration in the template. It is generated from *sai_rpc_server.skeleton.cpp*, which is
also generated (by Thrift) from *sai.thrift*, which is generated before from [*sai.thrift.tt*](#saithrifttt). The template itself is
a result of skeleton transformation performed by [`generate_server_template_from_skeleton()`](README.md#sub-generate_server_template_from_skeleton) function.

This file includes also *sai_rpc_server_functions.tt* to define functions bodies and
*sai_rpc_server_helper_functions.tt* to define helper functions (like *parse* or *deparse* functions).

### *sai_rpc_server_functions.tt*
This is not a standalone template. It is included by *sai_rpc_server.cpp.tt*, and used to define
functions bodies. The logic is defined in [`SAI::RPC`](SAI-RPC.md) module.

Some functions are not supported because of their complexity (the regex for unsupported functions is at the beginning of the file).

### *sai_rpc_server_helper_functions.tt*
This is not a standalone template. It is included by *sai_rpc_server.cpp.tt*, to define helper functions (like *parse* or *deparse* functions).

Some functions are not supported because of their complexity (the regular expressions for supported functions and structures is at the beginning of the file).
Note, that functions internals are manually written in *sai_rpc_frontend.cpp* file. In order to add support for some complex structure processing,
the sub-functions should be manually implemented first, and then added to the list of supported structures or removed from the list of unsupported elements.
