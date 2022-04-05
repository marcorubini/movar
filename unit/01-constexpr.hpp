#pragma once
#include <ml/movar/movar.hpp>
#include <doctest/doctest.h>

TEST_CASE ("constexpr")
{
  using namespace ml::movar;

  SUBCASE ("just")
  {
    static_assert (just (10) == 10);
    static_assert (just (10).get () == 10);
    static_assert (just (10).get<0> () == 10);
    static_assert (just (10).index () == 0);
    static_assert (just (10).is_nothing () == false);
    static_assert (just (10).is_something () == true);
    static_assert (just (10).is<0> () == true);
    static_assert (just (10).is<-1> () == false);
    static_assert (just (10).is<nothing> () == false);
    static_assert (just (10).is<int> () == true);
  }

  SUBCASE ("option")
  {
    static_assert (option (10) == 10);
    static_assert (option (10).get () == 10);
    static_assert (option (10).get<0> () == 10);
    static_assert (option (10).index () == 0);
    static_assert (option (10).is_nothing () == false);
    static_assert (option (10).is_something () == true);
    static_assert (option (10).is<0> () == true);
    static_assert (option (10).is<-1> () == false);
    static_assert (option (10).is<nothing> () == false);
    static_assert (option (10).is<int> () == true);

    static_assert (option<int> () == nothing ());
    static_assert (option<int> ().index () == -1);
    static_assert (option<int> ().is_nothing () == true);
    static_assert (option<int> ().is_something () == false);
    static_assert (option<int> ().is<0> () == false);
    static_assert (option<int> ().is<-1> () == true);
    static_assert (option<int> ().is<nothing> () == true);
    static_assert (option<int> ().is<int> () == false);
  }

  SUBCASE ("either")
  {
    using T = either<int, double>;
    static_assert (T (10).get<0> () == 10);
    static_assert (T (10).index () == 0);
    static_assert (T (10).is<0> () == true);
    static_assert (T (10).is<1> () == false);
    static_assert (T (10).is<-1> () == false);
    static_assert (T (10).is<int> () == true);
    static_assert (T (10).is<double> () == false);
    static_assert (T (10).is<nothing> () == false);

    static_assert (T (10.).get<1> () == 10);
    static_assert (T (10.).index () == 1);
    static_assert (T (10.).is<0> () == false);
    static_assert (T (10.).is<1> () == true);
    static_assert (T (10.).is<-1> () == false);
    static_assert (T (10.).is<int> () == false);
    static_assert (T (10.).is<double> () == true);
    static_assert (T (10.).is<nothing> () == false);
  }

  SUBCASE ("maybe")
  {
    using T = maybe<int, double>;
    static_assert (T ().is<-1> () == true);
    static_assert (T ().index () == -1);
    static_assert (T ().is_nothing ());
    static_assert (T ().is_something () == false);
    static_assert (T ().is<nothing> ());

    static_assert (T (10).is<0> ());
    static_assert (T (10).is<-1> () == false);
    static_assert (T (10).is<1> () == false);
    static_assert (T (10).is<int> ());
    static_assert (T (10).is<nothing> () == false);
    static_assert (T (10).get<0> () == 10);
  }

  SUBCASE ("variant")
  {
    using T = variant<int, double>;
    static_assert (T (10).is<-1> () == false);
    static_assert (T (10).is<0> ());
    static_assert (T (10).is<1> () == false);
    static_assert (T (10).is<int> () == true);
    static_assert (T (10).is<double> () == false);
    static_assert (T (10).is<nothing> () == false);
    static_assert (T (10).get<0> () == 10);
    static_assert (T (10).get<int> () == 10);
  }

  SUBCASE ("conversion")
  {
    static_assert (just (option (10)) == 10);
    static_assert (just (variant<int> (10)) == 10);
    static_assert (just (maybe<int> (10)) == 10);

    static_assert (either (maybe<int, double> (10)).get<0> () == 10);
    static_assert (either (variant<int, double> (10)).get<0> () == 10);

    static_assert (variant (just (10)).get<0> () == 10);
    static_assert (variant (option (10)).get<0> () == 10);
    static_assert (variant (either<int, double> (100.)).get<1> () == 100);
    static_assert (variant (maybe<int> (10)).get<0> () == 10);

    static_assert (option (just (10)) == 10);
    static_assert (option (maybe<int> (10)) == 10);
    static_assert (option (variant<int> (10)) == 10);

    static_assert (maybe (just (42)) == 42);
    static_assert (maybe (option (42)) == 42);
    static_assert (maybe (variant<int> (42)) == 42);
    static_assert (maybe (either<int, double> (42)) == 42);
    static_assert (maybe (variant<int, double> (42)) == 42);
  }

  SUBCASE ("take")
  {
    static_assert (option (10).take () == 10);
  }

  SUBCASE ("map")
  {
    auto const mul2 = [] (int x) {
      return x * 2;
    };
    static_assert (option (10).map (mul2) == 20);
    static_assert (option (10).map (mul2).map (mul2) == 40);
    static_assert (option<int> ().map (mul2) == nothing ());
    static_assert (just (10).map (mul2) == 20);
    static_assert (either<int, double> (10.).map (mul2) == 20.);
    static_assert (variant<int> (10).map (mul2) == 20);
    static_assert (maybe<int> (10).map (mul2) == 20);
  }

  SUBCASE ("map_or")
  {
    auto const mul2 = [] (int x) {
      return x * 2;
    };
    static_assert (option (10).map_or (mul2, 42) == 20);
    static_assert (option<int> ().map_or (mul2, 42) == 42);
    static_assert (nothing ().map_or (mul2, 42) == 42);
  }

  SUBCASE ("map_or_else")
  {
    auto const mul2 = [] (int x) {
      return x * 2;
    };
    auto const const42 = [] () {
      return 42;
    };
    static_assert (option (10).map_or_else (mul2, const42) == 20);
    static_assert (option<int> ().map_or_else (mul2, const42) == 42);
  }

  SUBCASE ("or_else")
  {
    auto const const42 = [] {
      return 42;
    };
    static_assert (option (10).or_else (const42) == 10);
    static_assert (option<int> ().or_else (const42) == 42);
  }

  SUBCASE ("sequence")
  {
    constexpr auto add1 = [] (int x) {
      return x + 1;
    };

    auto const ifzero = [] (int x) -> option<int> {
      if (x == 0)
        return 0;
      return {};
    };

    constexpr auto s = sequence () >> add1 >> add1;
    constexpr auto w = sequence () >> ifzero >> add1;

    static_assert (s (0) == 2);
    static_assert (sequence (add1, add1) (0) == 2);
    static_assert (sequence (add1) (0) == 1);
    static_assert (sequence () (0) == 0);

    static_assert (w (1) == nothing ());
    static_assert (w (0) == 1);

    constexpr auto a = s >> w;
    constexpr auto b = w >> s;

    static_assert (a (0) == nothing ());
    static_assert (b (0) == 3);
    static_assert (b (1) == nothing ());
  }

  SUBCASE ("fork")
  {
    constexpr auto add1 = [] (int x) {
      return x + 1;
    };
    constexpr auto ifzero = [] (int x) -> option<int> {
      if (x == 0)
        return x;
      return {};
    };
    constexpr auto ifone = [] (int x) -> option<int> {
      if (x == 1)
        return x;
      return {};
    };

    constexpr auto s1 = (sequence () >> ifone >> add1) //
      | (sequence () >> ifzero >> add1 >> add1);

    static_assert (s1 (0) == 2);
    static_assert (s1 (1) == 2);
  }

  SUBCASE ("fork-overload")
  {
    struct e1
    {};
    struct e2
    {};
    struct e3
    {};
    struct e4
    {};

    constexpr auto handle1 = [] (e1) {
      return 0;
    };
    constexpr auto handle2 = [] (e2) {
      return 1;
    };
    constexpr auto handle3 = [] (e3) {
      return 2;
    };

    using ml::movar::fork;
    constexpr auto f = ((sequence () >> handle1) //
      | (sequence () >> handle2)                 //
      | (sequence () >> handle3));

    static_assert (f (e1 {}) == 0);
    static_assert (f (e2 {}) == 1);
    static_assert (f (e3 {}) == 2);

    // must not compile
    // static_assert (f (e4 {}) == 0);
  }

  SUBCASE ("filter")
  {
    constexpr auto iszero = [] (int x) {
      return x == 0;
    };
    constexpr auto isone = [] (int x) {
      return x == 1;
    };
    constexpr auto add1 = [] (int x) {
      return x + 1;
    };

    constexpr auto s = (sequence () >> filter (iszero) >> add1) //
      | (sequence () >> filter (isone) >> add1 >> add1);
    static_assert (s (0) == 1);
    static_assert (s (1) == 3);
    static_assert (s (2) == nothing ());

    constexpr auto x = (filter_type<int> () >> add1) //
      | (filter_type<double> () >> add1 >> add1)     //
      | (filter (iszero) >> add1 >> add1 >> add1);
    static_assert (x (0) == 1);
    static_assert (x (.0) == 2);
    static_assert (x (.0f) == 3);
    static_assert (x (1u) == nothing ());
  }

  SUBCASE ("examples")
  {
    using ml::movar::sequence;
    constexpr auto add1 = [] (int x) {
      return x + 1;
    };
    constexpr auto mul2 = [] (int x) {
      return x * 2;
    };
    constexpr auto pipe = sequence () >> add1 >> mul2;

    static_assert (ml::movar::option (10).match (pipe) == 22);
    static_assert (ml::movar::option<int> ().match (pipe) == ml::movar::nothing ());

    using ml::movar::filter_type;
    using ml::movar::maybe;

    constexpr auto pipe2 = (filter_type<int> () >> add1) //
      | (filter_type<double> () >> add1 >> add1);

    static_assert (pipe2 (10) == 11);
    static_assert (pipe2 (10.) == 12);

    static_assert (maybe<int, double> (10).match (pipe2) == 11);
    static_assert (maybe<int, double> (10.0).match (pipe2) == 12);
  }
}