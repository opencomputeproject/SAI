`SAI::RPC` (*gensairpc.pl*)
===========================

While `SAI` [classes](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Classes.pod) are responsible for data collection,
`SAI::RPC` [roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) extends their capabilities to be useful
for RPC interface generation (including server, client and *Thrift* header).

## `sai_thrift_attribute_list_t` type
`sai_thrift_attribute_list_t` is the special struct type that replaces the list of attributes in the `Thrift` interface.
Used for return values only.

## `SAI::RPC` modules:

### `SAI::RPC::Function`
This role extends the `SAI::Function` with some methods, useful mostly in templates.
Most of them are wrappers of `SAI` arguments filters.
It also replaces the returned list of attributes with `sai_thrift_attribute_list_t`.

`rpc_return()` - determines what is returned by generated function. It is not always the same value
as the original return value. E.g. some `out` SAI function arguments, are returned by the RPC function directly.

`rpc_args()` - similar to the previous one, but related to arguments. For details, see `SAI::RPC::Function::Argument`.

`adapter_args()` - Python adapter (RPC client in fact) may take different arguments than C++ server. For details, see `SAI::RPC::Function::Argument`.

`preprocessed_args()` - arguments that need to be preprocessed on the server side, before passing into _SAI_. For details, see `SAI::RPC::Function::Argument`.

`declared_args()` - *SAI* arguments that were not passed into RPC server function, so they need to be declared first. For details, see `SAI::RPC::Function::Argument`.

`adapter_preprocessed_args()` - arguments that need to be preprocessed on the client side, before passing into server. For details, see `SAI::RPC::Function::Argument`.

### `SAI::RPC::Function::Argument`
Note, that this role extends the `SAI` argument. It means that each argument is the *SAI* argument, not necessarily the adapter or server argument.

`is_rpc_return()` - determines if `SAI` argument is returned by RPC interface.

`is_cpp_return()` - determines if `SAI` argument is returned by RPC server. Basically, it should be the same as the one expected to be the RPC return value. However, sometimes the server interface differs from the RPC one, what is internally handled by _Thrift_. For example, the `list` returned by RPC must be returned by *out* argument on the server side (then the server returns void instead).

`is_adapter_arg()` - determines if `SAI` argument is also a Python adapter function argument. Adapter can
expose only RPC arguments, but some of them are handled in the different way on the Python side. For example, if an argument is the attribute list, then the adapter does not expose this argument. Instead, it defines separate argument for each attribute, and composes the list internally so that it can call RPC function.

`is_rpc_arg()` - determines if `SAI` argument is exposed by RPC. For example, if the *SAI* argument is being returned by RPC interface function, then it is no longer on the argument list.

`requires_vector()` - determines if `SAI` argument requires additional `std::vector` allocated on the server side. This is usually true, if the *SAI* argument is a list, but is not passed into the server as `std::vector` (for example attribute list is passed as `sai_thrift_attribute_list_t`).

`requires_malloc()` - determines if `SAI` argument requires additional malloc on the server side. This is true for all lists, because the server operates on `std::vector` before passing lists into _SAI_.

`requires_counter_parsing()` - determines if `SAI` argument requires parsing of counters. True for counter lists.

`requires_parsing()` - determines if `SAI` argument requires additional parsing before passing into _SAI_. True for all complex types. It means, that before calling _SAI_ function, additional one is called for the specified argument to transform some data.

`requires_preprocessing()` - determines if `SAI` argument requires additional preprocessing stage before calling the _SAI_ function. It includes calling a memory allocation and parsing.

`requires_casting()` - determines if `SAI` argument requires casting during the _SAI_ function call. It is true for all types that were not "preprocessed" before (memory allocation or parser function already return the variables of type required by _SAI_).

`requires_address()` - determines if `SAI` argument requires '&' during the _SAI_ function call. True for all pointer arguments, unless the memory was allocated (then we already have a pointer).

`requires_declaration()` - determines if `SAI` argument needs to be declared on a server side. True for arguments that were not passed into server via RPC.

`requires_adapter_preprocessing()` - determines if `SAI` argument requires additional preprocessing stage before calling the RPC function. It is related to arguments that are part of RPC interface, but not taken by Python function. It includes e.g. lists which are usually not taken on Python side, but required by RPC function.

### `SAI::RPC::ThriftName`
Extends `SAI` classes with the naming convention used on the *Thrift* side.
Some of name modifications are related to the convention (e.g. `sai_` prefix is replaced with `sai_thrift_`), other are related
to the way *Thrift* works (e.g. *Thrift* keywords are not valid names). If a class that consumes this role has
a type, its name can be used to determine the new name (e.g. "`set`" name can be replaced with part of type, otherwise it is replaced by "`accessor`").

`thrift_name()` - returns the name used in RPC interface. Operates on `name` and optionally on `type`.

### `SAI::RPC::ThriftName::Type`
This role extends the `SAI::RPC::ThriftName` so that it can be consumed by `SAI::Type`.

`thrift_name()` - the extension of the original `thrift_name()` method. It replaces the type names
with ones valid on the _Thrift_ interface definition. For example, it replaces lists (arrays, pointers etc.) with "`list<subtype>`" version.

`cpp_name()` - since some "_Thrift_ names" are not valid types in C++, this `thrift_name()` wrapper can be used on the server side. Example: `std::vector<subtype>` instead of `list<subtype>` should be used.

`python_name()` - since some "_Thrift_ names" are not valid types in Python, this `thrift_name()` wrapper can be used on the client side. Example: `List[subtype]` instead of `list<subtype>` should be used (see [type annotation in Python](https://docs.python.org/3/library/typing.html)).

### `SAI::RPC::ThriftName::Variable`
This role extends the `SAI::RPC::ThriftName` so that it can be consumed by `SAI::Variable`.

`thrift_name()` - the extension of the original `thrift_name()` method. The only thing this wrapper does, is prefix disablement.

### `SAI::RPC::Type`
This role extends the `SAI::Type` functionalities, by adding the support to replace lists of attributes with `sai_thrift_attribute_list_t`.

`is_attr_list` - determines if the type is the attribute lists (i.e. `sai_thrift_attribute_list_t`)

`convert_to_attr_list()` - validates if the type is the list of attributes and, if so, it sets `is_attr_list` field. Used by `SAI::RPC::Function`.
