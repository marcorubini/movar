#pragma once
#include <ml/movar/internal/core/simplify.hpp>

namespace ml::internal::movar
{
  template<Variant... Ts>
    requires (sizeof...(Ts) > 0)
  struct first_of_impl;

  template<Variant T1, Variant... Ts>
  struct first_of_impl<T1, Ts...> : first_of_impl<T1, typename first_of_impl<Ts...>::type>
  {};

  template<Variant T>
  struct first_of_impl<T> : simplify_impl<T>
  {};

  template<Variant T1, Variant T2>
  struct first_of_impl<T1, T2>
  {
    static constexpr auto _deduce ()
    {
      if constexpr (None<T1>) {
        return simplify_impl<T2> {};
      } else if constexpr (None<T2>) {
        return simplify_impl<T1> {};
      } else if constexpr (Some<T1>) {
        return simplify_impl<T1> {};
      } else if constexpr (Some<T2>) {
        using result = mp_rename<mp_set_union<mp_list<>, T1, T2>, variant>;
        return simplify_impl<result> {};
      } else {
        using result = mp_rename<mp_set_union<mp_list<>, T1, T2>, maybe>;
        return simplify_impl<result> {};
      }
    }

    using type = typename decltype (_deduce ())::type;
  };

  template<Variant... Ts>
    requires (sizeof...(Ts) > 0)
  using first_of = typename first_of_impl<Ts...>::type;
} // namespace ml::internal::movar