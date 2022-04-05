#pragma once
#include <ml/movar/internal/core/detect.hpp>
#include <boost/mp11/algorithm.hpp>

namespace ml::internal::movar
{
  template<Variant T>
  struct simplify_impl : type_identity<T>
  {};

  template<class T>
  struct simplify_impl<variant<T>> : type_identity<just<T>>
  {};

  template<class T1, class T2>
  struct simplify_impl<variant<T1, T2>> : type_identity<either<T1, T2>>
  {};

  template<class T>
  struct simplify_impl<maybe<T>> : type_identity<option<T>>
  {};

  template<Variant T>
  using simplify = typename simplify_impl<T>::type;

  template<bool maybe_null, class List>
  struct deduce_simplified_impl;

  template<>
  struct deduce_simplified_impl<true, mp_list<>>
  {
    using type = nothing;
  };

  template<class T>
  struct deduce_simplified_impl<true, mp_list<T>>
  {
    using type = option<T>;
  };

  template<class... Ts>
  struct deduce_simplified_impl<true, mp_list<Ts...>>
  {
    using type = maybe<Ts...>;
  };

  template<class T>
  struct deduce_simplified_impl<false, mp_list<T>>
  {
    using type = just<T>;
  };

  template<class T1, class T2>
  struct deduce_simplified_impl<false, mp_list<T1, T2>>
  {
    using type = either<T1, T2>;
  };

  template<class... Ts>
  struct deduce_simplified_impl<false, mp_list<Ts...>>
  {
    using type = variant<Ts...>;
  };

  template<class... Ts>
  using deduce_simplified = typename deduce_simplified_impl< //
    (same_as<Ts, nothing> || ...),
    mp_remove<mp_list<Ts...>, nothing>>::type;
} // namespace ml::internal::movar