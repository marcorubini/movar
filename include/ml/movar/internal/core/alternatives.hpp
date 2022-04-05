#pragma once
#include <ml/movar/internal/core/simplify.hpp>

namespace ml::internal::movar
{
  template<Variant T>
  struct alternatives_impl;

  template<>
  struct alternatives_impl<nothing> : type_identity<mp_list<>>
  {};

  template<class T>
  struct alternatives_impl<just<T>> : type_identity<mp_list<T>>
  {};

  template<class T>
  struct alternatives_impl<option<T>> : type_identity<mp_list<T>>
  {};

  template<class T1, class T2>
  struct alternatives_impl<either<T1, T2>> : type_identity<mp_list<T1, T2>>
  {};

  template<class... Ts>
  struct alternatives_impl<maybe<Ts...>> : type_identity<mp_list<Ts...>>
  {};

  template<class... Ts>
  struct alternatives_impl<variant<Ts...>> : type_identity<mp_list<Ts...>>
  {};

  template<Variant T>
  using alternatives = typename alternatives_impl<T>::type;

  template<Variant T>
  constexpr inline long size = mp_size<T>::value;

  template<>
  constexpr inline long size<nothing> = 0;

  template<Variant T, long I>
  constexpr inline bool contains_index = I >= 0 && I < size<T>;

  template<class T, long I>
  concept ContainsIndex = Variant<T> && contains_index<T, I>;

  template<Variant T, class E>
  constexpr inline bool contains_alternative = mp_contains<T, E>::value;

  template<class T, class E>
  concept ContainsAlternative = Variant<T> && contains_alternative<T, E>;

  template<Variant T, long I>
    requires ContainsIndex<T, I>
  using alternative = mp_at_c<T, I>;

} // namespace ml::internal::movar