#pragma once
#include <ml/movar/internal/core/core.hpp>

namespace ml::internal::movar
{
  template<class Lhs, class Rhs>
  static constexpr auto impl::pipe_sequence (Lhs&& lhs, Rhs&& rhs)
  {
    return ml::movar::sequence (std::forward<Lhs> (lhs), std::forward<Rhs> (rhs));
  }

  template<class Lhs, class Rhs>
  static constexpr auto impl::pipe_fork (Lhs&& lhs, Rhs&& rhs)
  {
    return ml::movar::fork (std::forward<Lhs> (lhs), std::forward<Rhs> (rhs));
  }
} // namespace ml::internal::movar