#pragma once
#include <ml/movar/internal/core/fwd.hpp>

namespace ml::internal::movar
{
  enum class variant_tag
  {
    UNKNOWN,
    NOTHING,
    JUST,
    EITHER,
    VARIANT,
    OPTION,
    MAYBE
  };

  template<class T>
  constexpr inline variant_tag detect = variant_tag::UNKNOWN;

  template<>
  constexpr inline variant_tag detect<nothing> = variant_tag::NOTHING;

  template<class T>
  constexpr inline variant_tag detect<just<T>> = variant_tag::JUST;

  template<class T1, class T2>
  constexpr inline variant_tag detect<either<T1, T2>> = variant_tag::EITHER;

  template<class... Ts>
  constexpr inline variant_tag detect<variant<Ts...>> = variant_tag::VARIANT;

  template<class T>
  constexpr inline variant_tag detect<option<T>> = variant_tag::OPTION;

  template<class... Ts>
  constexpr inline variant_tag detect<maybe<Ts...>> = variant_tag::MAYBE;

  template<class T>
  constexpr inline bool detect_nothing = detect<T> == variant_tag::NOTHING;

  template<class T>
  constexpr inline bool detect_some = (detect<T> == variant_tag::JUST) //
    || (detect<T> == variant_tag::EITHER)                              //
    || (detect<T> == variant_tag::VARIANT);

  template<class T>
  constexpr inline bool detect_maybe = (detect<T> == variant_tag::OPTION) //
    || (detect<T> == variant_tag::MAYBE);
} // namespace ml::internal::movar

namespace ml::movar
{
  template<class T>
  concept Variant = (internal::movar::detect<T> != internal::movar::variant_tag::UNKNOWN);

  template<class T>
  concept Some = (Variant<T> && internal::movar::detect_some<T>);

  template<class T>
  concept None = (Variant<T> && internal::movar::detect_nothing<T>);

  template<class T>
  concept Maybe = (Variant<T> && internal::movar::detect_maybe<T>);
} // namespace ml::movar

namespace ml::internal::movar
{
  using ml::movar::Maybe;
  using ml::movar::None;
  using ml::movar::Some;
  using ml::movar::Variant;
} // namespace ml::internal::movar