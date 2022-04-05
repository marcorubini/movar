#pragma once
#include <ml/movar/internal/core/add_nothing.hpp>

namespace ml::internal::movar
{
  template<Variant... Ts>
    requires (sizeof...(Ts) > 0)
  struct join_impl;

  template<Variant T>
  struct join_impl<T> : simplify_impl<T>
  {};

  template<Variant T, Variant... Ts>
  struct join_impl<T, Ts...> : join_impl<T, typename join_impl<Ts...>::type>
  {};

  template<Variant T1, Variant T2>
  struct join_impl<T1, T2>
  {
    static constexpr auto _deduce ()
    {
      if constexpr (None<T1>) {
        return add_nothing_impl<T2> {};
      } else if constexpr (None<T2>) {
        return add_nothing_impl<T1> {};
      } else if constexpr (Maybe<T1> || Maybe<T2>) {
        using result = mp_rename<mp_set_union<mp_list<>, T1, T2>, maybe>;
        return simplify_impl<result> {};
      } else {
        using result = mp_rename<mp_set_union<mp_list<>, T1, T2>, variant>;
        return simplify_impl<result> {};
      }
    }

    using type = typename decltype (_deduce ())::type;
  };

  template<Variant... Ts>
    requires (sizeof...(Ts) > 0)
  using join = typename join_impl<Ts...>::type;
} // namespace ml::internal::movar