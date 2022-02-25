`SAI` (*gensairpc.pl*)
======================

`SAI` is a group of sub-modules related to data collected from *SAI* headers.
Their only purpose is collecting all useful data about *SAI* headers, that may be useful
to generate another code and to provide some common methods.
Any information related to generating specific files should be contained in another
modules. For example, [`SAI::RPC`](SAI-RPC.md) module contains [roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) which contain useful functionalities to generate the RPC (*Thrift*) code, which are consumed by `SAI` modules:

##### `Function.pm`:

	with 'SAI::RPC::ThriftName', 'SAI::Utils::XMLLoader', 'SAI::RPC::Function';

It means that `SAI::Function` class consumes three roles: `SAI::RPC::ThriftName`
(enables Thrift naming convention, requires `name` field), `SAI::Utils::XMLLoader` (enables initialization from XML), and `SAI::RPC::Function`
(used by templates to determine the argument list exposed by RPC server or to determine the returned value).

##### `Struct.pm`:

	with 'SAI::RPC::ThriftName', 'SAI::Utils::XMLLoader';

In this case, there is no dedicated `SAI::RPC::Struct` role, because *SAI* structures looks very
similar to their RPC equivalents (only names are changed).
It means that templates do not need any special decisions or data manipulation for structures,
they just read the name and fields list.

Some data collected from *SAI* has not the same representation in the RPC interface. *Function.pm*
is a good example. There are some differences, related to *Thrift* capabilities and technologies used.
For example, arrays in C are being passed as the pair of pointer and the number of elements.
In C++ (RPC server) the [`std::vector`](https://en.cppreference.com/w/cpp/container/vector/vector)
is used instead. In Thrift it is a [`list`](https://thrift.apache.org/docs/types).
To provide such information, `SAI::RPC` roles are being consumed by `SAI` classes.

Note, that in case of many possible generation targets, such roles should be consumed more [dynamically](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod#ADDING-A-ROLE-TO-AN-OBJECT-INSTANCE), into objects instead of classes. Since the *gensairpc.pl* script is used to generate RPC code only, they are loaded statically, in all `SAI` sub-modules.

When reading the `SAI` modules code, all [`with`](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod#CONSUMING-ROLES)
and [`extends`](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Classes.pod#SUBCLASSING) keywords,
should be taken into account, because all fields and methods defined in roles and parent classes becomes part of the module that imports them.
Some of them also [changes](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/MethodModifiers.pod#BEFORE,-AFTER,-AND-AROUND) the behavior of inherited methods.


## `SAI` modules:

### `SAI::Attrs`
Based on `SAI::Enum`. Instead of `SAI::Enum::Element`s, contains `SAI::Attrs::Attribute`s.
It changes the creation process to obtain its elements.

It adds some methods related to obtaining the lists of attributes that are intended to be *get*, *set*, *create* (and *mandatory on create*) etc.
Note, that in order to simplify the interface, `mandatory()` method return the list of attributes
that are *mandatory on create*, and `create()` method returns other *create* attributes (**excluding** *mandatory* ones).

All of those methods are based on `filter_elements()` method of `SAI::Enum` (see the [`grep()`](https://metacpan.org/pod/Moose::Meta::Attribute::Native::Trait::Array) method).

### `SAI::Attrs::Attribute`
Based on `SAI::Enum::Element`. It changes the creation process to set its own fields. All of them
are related to *SAI attributes* properties (e.g, if it is *set* or *get* attribute, its type etc).

It has one field of special meaning. The `typename` is the name of corresponding element of **`sai_attribute_value_t`** structure. It is required to set or get attributes. Since the `SAI::Attrs::Attribute` has no list of existing structures (including `sai_attribute_value_t`), it needs to be set by external setter call. Perhaps in *gensairpc.pl*.

### `SAI::Enum`
Enum has the `name` and the list of its `SAI::Enum::Element`s.

`object()` method is used to obtain the name of *SAI* object, the Enum
is related to. E.g. for `sai_acl_entry_attr_t` it should return `acl_entry` as the *SAI* object name.

It also [handles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Delegation.pod)
some methods of the elements [`Array`](https://metacpan.org/pod/Moose::Meta::Attribute::Native::Trait::Array),
for example `filter_elements()` (which is actually the `grep()` method of elements `Array` trait).

### `SAI::Enum::Element`
The `SAI::Enum` element. It has the `name` and the numeric `value`.

`simple_name()` method is used to obtain its simplified name. For example, the simple name of `SAI_ACL_ENTRY_ATTR_TABLE_ID` is `table_id`.

### `SAI::Function`
`SAI` function has a `name`, is part of specific `api` and has the `return` value as well as `args` list.
It initializes its arguments, the return value is `SAI::Function::Argument` as well, with the default *retval* name.

`operation()` method returns the operation name the function performs (e.g. `create`, `remove`, `set` attributes etc). Basically, it is based on the function name. Sometimes it returns the empty string or `other` operation.
For example, the operation of `sai_create_acl_entry()` function is `create`.


`object()` method is used to obtain the name of *SAI* object, the function
is related to. For example, the object, the `sai_create_acl_entry()` function operates on is `acl_entry`.

### `SAI::Function::Argument`
The `SAI::Function` argument. Basically, it is a `SAI::Variable` with some additional properties (it can be `in`/`out`, it has its `pos`ition). It also stores an information about its `parent` (i.e. the `SAI::Function` the argument is part of).

Other properties:

`count` - if the attribute is a collection (in `SAI` it is usually an array pointer), it has also the another corresponding `SAI::Function::Attribute`, which stores the number of elements. It is an important information, because in the generated code, such additional variable may be redundant (e.g. when C++ `std::vector` is used instead).

`is_count` - defines if the attribute is the number of elements of corresponding collection `SAI::Function::Attribute`. 

`is_return()` - in the generated code, the *SAI* return value can be part of RPC function argument list and vice versa, the `out` *SAI* argument can be returned from the RPC function. This is why, the return value is the `SAI::Function::Argument` as well.

`is_retval` - replaced by `is_return()`, used to validate `is_return()` only. Possibly useful for debugging purposes.

`has_attributes()` - defines if the argument is the collection of *SAI* attributes.

### `SAI::Stats`
It is a `SAI::Enum`, with the only difference - it can be created from XML data if its name contains "*stat_t*" phrase.

### `SAI::Struct`
Struct has the `name` and the list of its `SAI::Struct::Member`s.

`canonical_name()` - returns the name without `sai_` prefix.

`short_name()` - returns the canonical name without `_t` suffix (i.e. the value [captured](https://perldoc.perl.org/perlre#Capture-groups)
from *`/^sai_(\w+)_t$/`* expression).

Note, that `SAI::Struct` is the only class within `SAI` module, which does not initialize its members list during the initialization. This is because of how _gensairpc.pl_ reads the XML files - it uses libraries from _SAI/meta_, and needs to call one of their function to obtain the Struct members list.
To avoid such dependency in `SAI` modules, the members list is simply created by _gensairpc.pl_ right after
struct creation. Dependency injection or simple function passing can be considered instead.

### `SAI::Struct::Member`
It is just a `SAI::Variable` initialized from struct member XML definition. Additionally, any pointer member is assumed to be a list member. Function pointers are not supported since *gensairpc.pl* handles them separately.

Note: `parse_xml_typedef()` ([`BUILDARGS()`](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Construction.pod#BUILDARGS) in fact) reads the `id` as part of struct member XML definition, but it is ignored by the [`BUILD()`](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Construction.pod#BUILD) method, since such
field is no longer required by templates. Their position in the members array is relevant.


### `SAI::Type`
The type representation:

`name` - the type name, without special signs (like `*` or `[]`), because lists or pointers may have different representation in generated languages (like Python, C++ or Thrift)

`ptr` - can be treated as Boolean "is\_pointer", but in fact this is the number of asterisks

`array` - can be treated as Boolean "is\_array", but in fact this is the number of elements

`subtype` - in case of array (or a pointer to the array), this is the type of array elements. Used to decide whether the type is a collection or not.
When set, the original type `name` becomes a debug information. For example, if the type is `sai_int`, then there is no subtype, and the `name` is `sai_int`.
However, if the original type is `sai_int *`, then initially the `name` is `sai_int` and `ptr` is `1`, but after calling `convert_to_list()`, the `subtype` is set, the `name` becomes invalid (`PTR=>sai_int`) and the `subtype` becomes the relevant value.
The same thing is done for arrays (e.g. `sai_int[5]`) with a different `name` (`ARRAY=>sai_int`).
The reason for this mechanism is:

1. In case of e.g. `sai_int[5]` the `sai_int` is not the full name of the type (see `name`). It suggest the simple `sai_int` type instead of an array. On the other hand, `sai_int[5]` as `name` may not be compatible with other languages.
1. It prevents bugs in the generated code. It is easy to forget to check if the type is pointer or not (which usually requires a special treatment), but the name like `PTR=>sai_int` won't compile.

`short_name()` - returns the name without `sai_` prefix and `_t` suffix (i.e. the value [captured](https://perldoc.perl.org/perlre#Capture-groups)
from *`/^sai_(\w+)_t$/`* expression).

`is_list()` - can be used to make sure that the type is a list of elements (like array or array pointer)

`is_attr()` - can be used to check if the type is the _SAI_ attribute

`convert_to_list()` - if we have a pointer, and we are sure that it it plays a role of list, then this method should be called. It is not called during the initialization, because it depends on the context. For arrays, the `subtype` is set during the initialization, so there is no need to call such function.

### `SAI::Typedef`
Typedef extends the `SAI::Type`. The `name` that comes from `SAI::Type` is the new type name.
It also has `SAI::Type` field (`type`) which is the original type.

### `SAI::Variable`
`SAI::Variable` is a composition of `SAI::Type` and the variable `name`. It is used as parent class of e.g. struct members or function arguments.

Sub-modules
===========

### [`SAI::Utils`](SAI-Utils.md)
It contains [roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) which are composed into classes from `SAI` sub-module.
They are adding functionalities related to loading XML data into `SAI` objects.

### [`SAI::RPC`](SAI-RPC.md)
It contains [roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) which are composed into classes from `SAI` sub-module.
They are adding functionalities that are useful for RPC (*Thrift*) auto-generation (e.g. new types, new names, decisions), basing on the collected data.
