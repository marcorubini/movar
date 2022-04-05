Pipelines                   {#pipelines}
=========

Pipelines allow to describe a sequence of operations on a variant type.

Every pipeline operation returns an instance of one of:

1. ml::movar::nothing
2. ml::movar::just
3. ml::movar::either
4. ml::movar::variant
5. ml::movar::option
6. ml::movar::maybe

All operations visit the variant so they must be invocable with every variant alternative.

map                         {#pipelines-map}
---
    
@code{.cpp}
auto map(unary-invocable fn);
@endcode

__fn__: a transformation to apply to the active alternative.

    auto add_one = [](int x) { return x+1; };
    auto result = option(10).map(add_one);
    // the result type is deduced to be ml::movar::option<int>
    // the result value is 11

If the variant has value __x__, returns __fn__( __x__ )
Otherwise, returns ml::movar::nothing.

map_or                      {#pipelines-map_or}
------

@code{.cpp}
auto map_or(unary-invocable fn, move-constructible default);
@endcode

__fn__:       a transformation to apply to the active alternative.

__default__:  a default value

If the variant has value __x__, returns __fn__( __x__ )
Otherwise, returns __default__ 

@code{.cpp}
auto add_one = [](int x) { return x+1; };
auto result = option<int>().map_or(add_one, 42);
// the result type is deduced to be ml::movar::just<int>
// the result value is 42
@endcode

map_or_else                 {#pipelines-map_or_else}
-----------

@code{.cpp}
auto map_or_else(unary-invocable fn, invocable default);
@endcode

__fn__:       a transformation to apply to the active alternative.

__default__:  a lazy default value

If the variant has value __x__, returns __fn__( __x__ )
Otherwise, returns __defaut__()

This overload is similar to __map_or__, but the default value is computed lazily
by invoking a function object.

@code{.cpp}
auto add_one = [](int x) { return x+1; };
auto lazy_42 = []() { return 42; };
auto result = option<int>().map_or_else(add_one, lazy_42);
// the result type is deduced to be ml::movar::just<int>
// the result value is 42
@endcode

or_else                     {#pipelines-or_else}
-------

    auto or_else(invocable default);

__default__: a lazy default value


If the variant has a value __x__, returns __x__
Otherwise, returns __default__()


@code{.cpp}
auto lazy_42 = []() { return 42; };
auto result = option<int>().or_else(lazy_42);
// the result type is deduced to be ml::movar::just<int>
// the result value is 42
@endcode

take                        {#pipelines-take}
----

    auto take();

This operation moves out the active variant alternative, leaving the variant in an undeterminate state.

Use __take__ to transform a variant that can be empty (like ml::movar::option or ml::movar::maybe)
to a variant that is never empty.

@code{.cpp}
// result1 is deduced to ml::movar::just<int>
auto resul1 = option(10).take(); 

// result2 is deduced to ml::movar::variant<int, double>
auto resul2 = maybe<int, double>(10.).take() 
@endcode

match                       {#pipelines-match}
-----

@code{.cpp}
auto match(unary-invocable fn);
@endcode


__fn__: a function object to invoke with the active alternative or ml::movar::nothing

__match__ invokes fn with the active alternative and returns the result.


Unlike __map__, __match__ requires __fn__ to handle the ml::movar::nothing case.
