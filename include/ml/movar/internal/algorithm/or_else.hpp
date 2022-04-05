#pragma once
#include <ml/movar/internal/algorithm/map.hpp>

namespace ml::internal::movar
{
  template<Variant Self, class LazyFn>
  using or_else_result_t = first_of<Self, wrap_invoke_result<LazyFn>>;

  template<class Var, class Default>
  static constexpr auto impl::or_else (Var&& var, Default&& lazy)
  {
    using unqual = remove_cvref_t<Var>;
    using result_type = or_else_result_t<unqual, Default>;

    if constexpr (None<unqual>) {
      return result_type (impl::wrap_invoke (std::forward<Default> (lazy)));
    } else {
      if constexpr (Maybe<unqual>)
        if (var.is_nothing ())
          return result_type (impl::wrap_invoke (std::forward<Default> (lazy)));
      return result_type (std::forward<Var> (var));
    }
  }
} // namespace ml::internal::movar