#pragma once
#include <ml/movar/internal/core/simplify.hpp>

namespace ml::internal::movar
{
  template<Variant T>
  struct add_nothing_impl;

  template<>
  struct add_nothing_impl<nothing> : std::type_identity<nothing>
  {};

  template<class T>
  struct add_nothing_impl<just<T>> : std::type_identity<option<T>>
  {};

  template<class T>
  struct add_nothing_impl<option<T>> : std::type_identity<option<T>>
  {};

  template<class T1, class T2>
  struct add_nothing_impl<either<T1, T2>> : std::type_identity<maybe<T1, T2>>
  {};

  template<class... Ts>
  struct add_nothing_impl<maybe<Ts...>> : simplify_impl<maybe<Ts...>>
  {};

  template<class... Ts>
  struct add_nothing_impl<variant<Ts...>> : simplify_impl<maybe<Ts...>>
  {};

  template<Variant T>
  using add_nothing = typename add_nothing_impl<T>::type;
} // namespace ml::internal::movar