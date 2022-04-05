#pragma once
#include <ml/movar/internal/algorithm/map.hpp>

namespace ml::internal::movar
{
  template<Variant Self, class Fn, class Default>
  using map_or_result_t = first_of<map_result_t<Self, Fn>, wrap_result<Default>>;

  template<class Var, class Fn, class Default>
  static constexpr auto impl::map_or (Var&& var, Fn&& fn, Default&& def)
  {
    using unqual = std::remove_cvref_t<Var>;
    using result_type = map_or_result_t<unqual, Fn, Default>;

    if constexpr (None<unqual>) {
      return result_type (impl::wrap (std::forward<Default> (def)));
    } else {
      if constexpr (Maybe<unqual>)
        if (var.is_nothing ())
          return result_type (impl::wrap (std::forward<Default> (def)));
      return impl::weak_visit_r<result_type> (std::forward<Fn> (fn), std::forward<Var> (var));
    }
  }
} // namespace ml::internal::movar