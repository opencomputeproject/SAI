*gensairpc.pl* architecture
===========================

*gensairpc.pl* performs the scan of *SAI* headers, and basing on them generates:

* *sai.thrift*
* *sai_adapter.py*
* *sai_rpc_server.cpp*

All of this work is done in the following steps:

1. **Parsing**
   1. Generate *Doxygen* XML files from *SAI* headers, using *SAI/meta* tools (*Makefile*)
   1. Use *SAI/meta* utils to obtain data
   1. Create Perl objects with all interesting data stored in their fields
1. **Generation**
   1. Generate *sai.thrift* using *Template Toolkit* (*TT*)
   1. Use *Thrift* to generate *sai\_rpc\_server.skeleton.cpp*
   1. Convert the skeleton into *sai\_rpc\_server.cpp.tt* template
   1. Generate *sai\_rpc\_server.cpp* using Template Toolkit (*TT*)
   1. Generate *sai\_adapter.py* using Template Toolkit (*TT*)
1. Perform some optional steps like **formatting**
1. **Cleanup**

## Data objects
All parsed data is being collected in Perl objects. Each useful *SAI* code element has its
object representation. Classes are defined in *perl/SAI* directory (see [`SAI`](SAI.md)):

* Attribute lists and attributes themselves
* Enums and their elements
* Functions and their arguments
* Statistics
* Structures and their members
* Typedefs
* Types
* Variables
 
Some of them inherit from others (e.g. function arguments from variables),
other are related by composition (e.g. structure has elements).
However, it is not enough to generate the code. To do this, templates needs
some additional information and decision making mechanism.
All such information needed by templates is composed into these classes in using
[roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) (see [`SAI::RPC`](SAI-RPC.md)).


Since created objects keeps only parsed data with no additional information, all
collected data can be previewed in *sai_dbg.dump* file (use ```-d```).

To make sure, what decision has been made during the generation, basing on collected data,
use ```--dbg``` flag and print any additional information in comments of the generated code.
Some template modification may be required:

	struct [% struct.thrift_name %] {
	    [%- id = 1; FOREACH member IN struct.members %]
	        [%- IF dbg %]
	  // [% member.type.name %] [% member.name %];
	        [%- END -%]

Note, that the code still should compile and meet *pylint*/*pep8* requirements if possible.

### Moose
All classes are defined using [Moose](https://metacpan.org/pod/Moose) OOP library.
It is recommended to get familiar with its basics before working with *gensairpc.pl*.

It is used to:

* Define fields for each important data related to the object
* Define methods used by templates to perform any decision
  * Traits ([Moose roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod))
    are used to mix in such functionalities

It is much [more convenient](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Unsweetened.pod)
to use *Moose* instead of plain Perl classes.

### Object construction
Almost each class has some helper methods defined:

* `parse_xml_typedef()`
* `validate_xml_typedef()`

Both of them are required by *SAI::Utils::XMLLoader*. *XMLLoader* is a Role, which is composed
into almost all classes that obtains their information from XML. If both of these functions
are defined, the *XMLLoader* Role knows what to read from XML data during the object creation.
Internally, *XMLLoader* uses *[BUILDARGS](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Construction.pod)* to prepare all arguments before the object construction.

In short, Moose objects require the complete list of arguments (fields) during the construction.
Instead of passing all of them (e.g. name and element list for Enum) into constructor, we just pass the XML data, and the class itself has information which data to read from it and how.

For more information see [`SAI::Utils`](SAI-Utils.md).

### Using the collected object data
During the construction, only the basic information is being stored. Nor conclusions neither additional information are created.
This is the main role of classes stored in *perl/SAI* directory.

However, during SAI RPC files generation, a lot of decisions need to be taken basing on those information.
For example, the RPC function sometimes has to return different value then the original function and the
arguments list differs as well. Some of arguments require a prepossessing before the original function call,
some of them need to be dynamically created (the number of elements is stored in C++ `std::vector`,
but the C function need such counter explicitly). Names or types are another example.

All this stuff is defined in *perl/SAI/RPC* directory ([`SAI::RPC`](SAI-RPC.md)). It contains roles, that are consumed by SAI classes. Example from *SAI/Function.pm*:


	with 'SAI::RPC::ThriftName', 'SAI::Utils::XMLLoader', 'SAI::RPC::Function';


It means, that `SAI::Function` plays the `SAI::RPC::Function` role as well, it can be constructed from XML and it supports naming convention used in *Thrift* files.

Moose roles (imported using *with* keyword) should be interpreted as interfaces from some OOP languages, but instead of implementing them, classes composes their functions (and need to meet their requirements).

### Template Toolkit
All templates are stored in *templates* directory and written in the language defined by *Template Toolkit* (similar to *Jinja* etc).

Note, that templates should be as simple as possible. Any additional information obtained from the object or any additional decision should be implemented in *perl/SAI/RPC* roles directory ([`SAI::RPC`](SAI-RPC.md)).

For details see [`Templates`](Templates.md).

*gensairpc.pl* internals
========================

### `sub generate_server_template_from_skeleton()`
While almost all generation is performed using the templates (*TT*), this function is an exception.
It is a pure-perl function which converts *Thrift's* *sai_rpc_server.skeleton.cpp*
into the *TT* template *sai_rpc_server.cpp.tt*. It is composed from regular expressions, which
decides whether the line from *sai_rpc_server.skeleton.cpp* should be modified before printing (if printed),
or some additional lines need to be printed as well. Note, that the order of expressions may be relevant.

This function uses also [flip-flop](https://perldoc.perl.org/perlop#Range-Operators) operator (in scalar context). Understanding how it works may be useful to understand some conditionals.

It uses also `given-when` construction. It should be verified if it can be used with *Perl 7*, if not then `if-elsif-else` should be used instead.


### `sub get_api_name()`
This functions extracts the API name from the original header
(contrary to the data obtained from XML files).

To avoid a [bottle-neck](https://metacpan.org/release/Devel-NYTProf) here, the API names are being cached by this function.

It also replaces some API names by `common` API. This is used to resolve some *sai.thrift* dependencies (`common` API is processed first, and usually required by other APIs). `types` API is an example of API that becomes part of `common` API.

### `sub get_definitions()`
The main parser loop. Populates the tree of all parsed objects. It effect can be observed in *sai_dbg.dump* file.

### `sub assign_attr_types()`
Each *SAI* attribute has not only name, properties or type. It has also the corresponding field in `sai_attribute_value_t` structure. Since there is no information, which field should be used, its name need to be taken basing on the type of the attribute and type and `sai_attribute_value_t` types.

This function:

1. Looks for `sai_attribute_value_t` in the list of structures.
1. Calls `get_attr_type()` to obtain the correct `sai_attribute_value_t` field name.
1. For each `SAI::Attrs::Attribute` sets its `typename`.

Generated files
===============

### *sai.thrift*
This file is the simplest one to generate, since it is similar to C headers. It requires no
formatting.

The most problematic are dependencies between types.
Those dependencies require a very strict order of structures declaration.
Workarounds not only manually change the default definition order, but also types of
some structure fields.

### *sai\_adapter.py*
The Python file is generated from scratch, but must apply the Thrift interface used by
the server. It  can be automatically formatted by external tools (`--python-format`),
but those tools are very slow, so the file is manually formatted (i.e. in the template).

### *sai\_rpc\_server.cpp*
RPC server is a C++ code and is the most challenging part.
Its generation depends on *Thrift* and *sai.thrift* file.
While *sai_rpc_server.cpp.tt* is generated by *Thrift* and *gensairpc.pl* itself,
it includes and uses manually written *sai_rpc_server_functions.tt* and *sai_rpc_server_helper_functions.tt*.
This file is always formatted using external tools (unless `--no-format` is used),
but the not-formatted version should be as close to the formatted one as possible.

For details see [`Templates`](Templates.md).

### Exceptions
In case of error, the *sai\_rpc\_server.cpp* throws an exception. When exception is thrown,
no other data can be returned (unless it is part of the exception itself).
Depending on *sai\_adapter.CATCH\_EXCEPTIONS* value, exceptions are being handled by *sai\_adapter.py*
(*True*) or thrown to the caller (*False*).

Generating other code than RPC for SAI
======================================

The limitation is not the generation itself, but the parsing method.

Since the *gensairpc.pl* does not scan the code itself, but XML files generated by *Doxygen*, and uses *SAI/meta* tools to do so, it does not support
parsing anything else. To achieve this, the *gensairpc.pl* should get rid of *SAI* dependencies, scan the code (or headers) and `SAI` classes should
be able to be initialized from such data (e.g. code lines) instead of XML data. It is not impossible, but requires a lot of changes.

`SAI` classes themselves should be splitted into those related to `SAI` and generic ones.

`SAI::RPC` roles should be splitted into `SAI::RPC` (if still needed) and `RPC` roles. 

Any additional roles or templates can be added without deep changes, so generating something else (for *SAI*) should be relatively easy. To support more
then just RPC-related roles, they should be consumed more [dynamically](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod#ADDING-A-ROLE-TO-AN-OBJECT-INSTANCE), into objects instead of classes. For now, they are loaded statically, in all `SAI` sub-modules.


Sub-modules
===========

### [`SAI`](SAI.md)
It contains classes related to *SAI* data collected from XML. They works rather as data structures than real classes.

### [`SAI::Utils`](SAI-Utils.md)
It contains [roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) which are composed into classes from `SAI` sub-module.
They are adding functionalities related to loading XML data into `SAI` objects.

### [`SAI::RPC`](SAI-RPC.md)
It contains [roles](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) which are composed into classes from `SAI` sub-module.
They are adding functionalities that are useful for RPC (*Thrift*) auto-generation (e.g. new types, new names, decisions), basing on the collected data.

### `Utils`
Utils related to _gensairpc.pl_ itself, like code formatting.
