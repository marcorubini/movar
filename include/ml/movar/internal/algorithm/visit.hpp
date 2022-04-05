#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::internal::movar
{
  template<class Vis, class Var>
  static constexpr auto impl::weak_visit (Vis&& vis, Var&& var)
  {
    using unqual = std::remove_cvref_t<Var>;
    return boost::mp11::mp_with_index<size<unqual>> (var.index (), [&] (auto I) {
      return impl::wrap_invoke (std::forward<Vis> (vis), get<I> (std::forward<Var> (var)));
    });
  }

  template<class R, class Vis, class Var>
  static constexpr auto impl::weak_visit_r (Vis&& vis, Var&& var)
  {
    using unqual = remove_cvref_t<Var>;
    return boost::mp11::mp_with_index<size<unqual>> (var.index (), [&] (auto I) -> R {
      return R (impl::wrap_invoke (std::forward<Vis> (vis), get<I> (std::forward<Var> (var))));
    });
  }

  template<class Vis, class Var>
  static constexpr auto impl::visit (Vis&& vis, Var&& var)
  {
    using unqual = remove_cvref_t<Var>;
    if constexpr (None<unqual>) {
      return impl::wrap_invoke (std::forward<Vis> (vis), nothing ());
    } else {
      if constexpr (Maybe<unqual>)
        if (var.is_nothing ())
          return impl::wrap_invoke (std::forward<Vis> (vis), nothing ());
      return impl::weak_visit (std::forward<Vis> (vis), std::forward<Var> (var));
    }
  }

  template<class R, class Vis, class Var>
  static constexpr auto impl::visit_r (Vis&& vis, Var&& var)
  {
    using unqual = remove_cvref_t<Var>;
    if constexpr (None<unqual>) {
      return R (impl::wrap_invoke (std::forward<Vis> (vis), nothing ()));
    } else {
      if constexpr (Maybe<unqual>)
        if (var.is_nothing ())
          return R (impl::wrap_invoke (std::forward<Vis> (vis), nothing ()));
      return impl::weak_visit_r<R> (std::forward<Vis> (vis), std::forward<Var> (var));
    }
  }
} // namespace ml::internal::movar