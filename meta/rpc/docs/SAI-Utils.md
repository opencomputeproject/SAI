`SAI::Utils` (*gensairpc.pl*)
=============================

`SAI::Utils` is a group of sub-modules which provide useful functionalities for working with `SAI` modules.

### `SAI::Utils::XMLLoader`

When initializing a *Moose* object, all its attributes should be passed as [named parameters](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Construction.pod#OBJECT-CONSTRUCTION-AND-ATTRIBUTES).
For example, when *gensairpc.pl* initialized `SAI::Function`, it should pass its name, return value, the list of arguments etc.
So before creating each object, *gensairpc.pl* should read an XML file internals itself.
However, *Moose* provides a [`BUILDARG`](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Construction.pod#BUILDARGS) method, which allows to take different parameters and create the argument list from them. This is how `SAI` classes works. The way of how XML internals should be transformed into object fields is implemented within each class separately.

Because all of them would have their own, very similar `BUILDARG` methods, `SAI::Utils::XMLLoader` provides the standardised way of
doing it.
`BUILDARG` defined by `SAI::Utils::XMLLoader` requires **`xml_typedef`** argument, which is the structure read from XML file. If not specified, then the standard initialization with its specified fields is performed. Otherwise, it **verifies** provided XML definition and then **parses** them.
Thus, the `SAI::Utils::XMLLoader` [role](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/Roles.pod) requires two methods defined by its consumers:

1. `validate_xml_typedef()`
1. `parse_xml_typedef()`

When specified, the class does not have to specify its own `BUILDARG`. When needed, the `SAI::Utils::XMLLoader` `BUILDARG` behavior can be modified with [method modifiers](https://metacpan.org/pod/distribution/Moose/lib/Moose/Manual/MethodModifiers.pod).
