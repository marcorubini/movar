#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::internal::movar
{
  template<Variant Self, class Fn>
  static constexpr auto _deduce_map_result ()
  {
    if constexpr (None<Self>) {
      return simplify_impl<Self> ();
    } else if constexpr (Some<Self>) {
      using alts = alternatives<Self>;
      using possible_results = mp_transform<mp_bind_front<wrap_invoke_result, Fn>::template fn, alts>;
      return simplify_impl<mp_apply<join, possible_results>> ();
    } else {
      using alts = mp_remove<alternatives<Self>, nothing>;
      using possible_results = mp_transform<mp_bind_front<wrap_invoke_result, Fn>::template fn, alts>;
      return add_nothing_impl<mp_apply<join, possible_results>> ();
    }
  }

  template<Variant Self, class Fn>
  using map_result_t = typename decltype (_deduce_map_result<Self, Fn> ())::type;

  template<class Var, class Fn>
  static constexpr auto impl::map (Var&& var, Fn&& fn)
  {
    using unqual = remove_cvref_t<Var>;
    if constexpr (None<unqual>) {
      return nothing ();
    } else {
      using result_type = map_result_t<Var, Fn>;
      if constexpr (Maybe<unqual>)
        if (var.is_nothing ())
          return result_type (nothing ());
      return impl::weak_visit_r<result_type> (std::forward<Fn> (fn), std::forward<Var> (var));
    }
  }

} // namespace ml::internal::movar