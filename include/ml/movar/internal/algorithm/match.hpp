#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::internal::movar
{
  template<Variant Self, class Fn>
  static constexpr auto _deduce_match_result ()
  {
    using alts =
      conditional_t<Maybe<Self> || None<Self>, mp_push_front<alternatives<Self>, nothing>, alternatives<Self>>;
    using possible_results = mp_transform<mp_bind_front<wrap_invoke_result, Fn>::template fn, alts>;
    return simplify_impl<mp_apply<join, possible_results>> ();
  }

  template<Variant Self, class Fn>
  using match_result_t = typename decltype (_deduce_match_result<Self, Fn> ())::type;

  template<class Var, class Fn>
  static constexpr auto impl::match (Var&& var, Fn&& fn)
  {
    using unqual = std::remove_cvref_t<Var>;
    using result_type = match_result_t<Var, Fn>;
    return impl::visit_r<result_type> (std::forward<Fn> (fn), std::forward<Var> (var));
  }

} // namespace ml::internal::movar