Lazy Pipelines                   {#lazy-pipelines}
==============

Examples
========

Sequence
--------

@code{.cpp}
using ml::movar::sequence;
using ml::movar::option;
using ml::movar::nothing;

auto add1 = [](int x) { return x+1; };
auto mul2 = [](int x) { return x*2; };
auto pipe = sequence() >> add1 >> mul2;

// deduced to option<int>
// value is 22
auto result1 = option(10).match(pipe);

// deduced to option<int>
// value is nothing
auto result2 = option<int>().match(pipe);
@endcode

Filter and fork
---------------

@code{.cpp}
using ml::movar::sequence;
using ml::movar::option;
using ml::movar::filter;

auto add1 = [](int x) { return x+1; };
auto iszero = [] (int x) { return x == 0; };
auto pipe = (filter(iszero) >> add1)
  | (sequence() >> add1 >> add1);

// deduced to option<int>
// result is 12 (enter the filter)
auto result1 = option(10).match(pipe);

// deduced to option<int>
// result is 1 (does not enter the filter)
auto result2 = option(1).match(pipe);
@endcode

Filter type and fork
--------------------

@code{.cpp}
using ml::movar::sequence;
using ml::movar::option;
using ml::movar::maybe;
using ml::movar::filter_type;

auto add1 = [](int x) { return x+1; };
auto pipe = (filter_type<int>() >> add1)
  | (filter_type<double>() >> add1 >> add1);

static_assert(option(10).match(pipe) == 11);
static_assert(option(10.).match(pipe) == 12);

static_assert (maybe<int, double> (10).match (pipe) == 11);
static_assert (maybe<int, double> (10.0).match (pipe) == 12);
@endcode