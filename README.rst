####
bulb
####

This library provides OpenFlow types used in C++.

These types are extracted from `allium <https://github.com/amedama41/allium>`_.
These are available without allium.

This library's design goals:

- Ease of use: The library should minimize the complexity of OpenFlow protocol.
  Some types of OpenFlow protocol have numerous fields. The library should show only fields a 
  library user is interested.
- Safety: The library should make a library user harder to create OpenFlow messages violating 
  the protocol requirements.
  Nevertheless, the library should manage also the violation message in consideration of the 
  real problem.
- Extensibility: The library should support not only experiment types but also user original types.
- Efficiency: The library should avoid the unnecessary validations.

*******
Install
*******

Through the path to the directory. ``/path/to/include/canard``

This library is a header only library.

.. todo:: You can use this library as a linked library.

This library depends on Boost library (Tested with Boost 1.59).

**************
Design Concept
**************

Kind of types
=============

This library defines 2 kind of types, OpenFlow type and utility type.

OpenFlow type is the set of types which correspond to structured types OpenFlow protocol defines.
In some types defined in OpenFlow protocol, a type may corresponds to multiple types in OpenFlow type
(e.g. ofp_flow_mod corresponds to flow_add, flow_modify, flow_delete, and so on).

Utility type is the set of non-OpenFlow types, and these types have several roles.

- Type erasure type: Contains a different type object (e.g. any_action, any_oxm_match_field).
- Container type: Contains multiple objects (e.g. action_list, action_set).
- Entry type: Sorts out the objects in terms of the OpenFlow protocol meaning (e.g. flow_entry, group_entry).

Object equality
===============

**Current implementation does not follow this section yet.**

For all types defined in this library, two objects of a type are equal only if the object's each fields, 
including padding, are equal.
Although two objects are semantically same, the objects are not equal.
The reason including padding fields is that paddings are observable when the object are encoding to binaries.
If two objects are equal, then the binaries from the objects should be also equal.

If you want to know whether two object semantically equal,
use a member function ``regulate``, or a non-member function ``equivalent``.

Object generation
=================

Objects of any OpenFlow type can be created by below 3 means, 

- call a static function ``create``, or,
- call a static function ``decode`` which creates an object from binary data, or,
- call constructors.

In addition, some of the type provides useful static member functions as factory functions.

Any ``create`` validates the new object's value in conformity with OpenFlow protocol requirements.
For instance, ``ouput::create`` check whether the output port number is larger than zero, and is 
enable to be used as an output port.

A function ``decode`` creates an object without any value checking, except length field checking
(The detail of length field checking is showed in the section of encoding and decoding function concept).
This is because there are some cases we want to permit a little violation of the requirements.
In fact, some switch's messages may not satisfy the protocol.

Object validations can be performed by a function ``validate``, so that we can examine decoded 
objects any time.

Constructors validate their values as much as ``decode`` s.
If you can ensure the object's value must be correct, you can avoid the validation costs
by creating object through a constructor.
However, usually to use ``create`` or other static factory functions is recommended.

Constructors for types consisting of other type objects will check the total length of the objects.
If the length is larger than the maximum value for the type of length field, the construction will fail.

.. todo::

  Some types offer other factory static functions in order to enhance correctness and readability.
  By default, these functions check the value as well as ``create``.
  You can change validation level, none, default, or recursively by the function's final parameter.

Recursive validation
====================

Several types are composed by other types.
But the validation of a factory functions does not check the composing values recursively.
There is a reason that we suppose that the parameters passed to the factory function are 
validated by their each factories.

However, sometimes recursive checking is important for some objects which are created by ``decode`` 
and so on.
Therefore, ``validate``'s are also configurable. You can switch to recursive checking anytime.

Encoding and decoding function concept
======================================

Any type has two member functions for translation between an object and binary data.

Non-static member function ``encode`` s convert to binaries. These functions take an 
OutputIterator as an argument to which is written the binaries.
The number of bytes which the ``encode`` writes is equal to the value the function ``length`` returns.

Static member function ``decode`` s create an object from binary data.
The binary data is passed as a range of InputIterator to the functions.
The ``decode`` first InputIterator parameter, which is the beginning of the range, shall be a reference.
After returning from the function, the iterator will point to the last byte used to create the object.

.. todo:: The iterator will advance when the function ends by exception

All ``decode`` does not check the length of binary specified by two InputIterators.
Therefore you must ensure the length is larger than the minimum length of the decoding type.
If the length is smaller, the behavior is undefined.

Note that although most of OpenFlow types have a length field, ``decode`` s for some types 
do not check the field.
Which type checks depends on a kind of the type.

**Unique type** is a type which corresponds to an OpenFlow structured type with no type field 
(e.g. ofp_packet_queue, ofp_flow_stats).
This type does not check the length field.

**Variant type** is a type which corresponds to an OpenFlow structured type with type field 
(e.g. ofp_output, ofp_apply_actions, oxm_header).
``decode`` of this type does not check the length field.
This is because in order to satisfy above binary length requirements, the caller of ``decode`` 
shall already check the length field in advance. (Copy binary to a common structured type 
object, and then check the length field and compare the field to the length of binary).

**********
Copyrights
**********

Copyright (c) 2016 amedama41.

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

