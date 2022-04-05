#pragma once
#include <ml/movar/internal/algorithm/map.hpp>

namespace ml::internal::movar
{
  template<Variant Self, class Fn, class LazyFn>
  using map_or_else_result_t = first_of<map_result_t<Self, Fn>, wrap_invoke_result<LazyFn>>;

  template<class Var, class Fn, class Default>
  static constexpr auto impl::map_or_else (Var&& var, Fn&& fn, Default&& lazy)
  {
    using unqual = std::remove_cvref_t<Var>;
    using result_type = map_or_else_result_t<unqual, Fn, Default>;

    if constexpr (None<unqual>) {
      return result_type (impl::wrap_invoke (std::forward<Default> (lazy)));
    } else {
      if constexpr (Maybe<unqual>)
        if (var.is_nothing ())
          return result_type (impl::wrap_invoke (std::forward<Default> (lazy)));
      return impl::weak_visit_r<result_type> (std::forward<Fn> (fn), std::forward<Var> (var));
    }
  }
} // namespace ml::internal::movar