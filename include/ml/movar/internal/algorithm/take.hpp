#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::internal::movar
{
  template<class Var>
  static constexpr auto impl::take (Var&& var)
  {
    using unqual = remove_cvref_t<Var>;
    if constexpr (detect<unqual> == variant_tag::JUST) {
      return just (std::forward<Var> (var));
    } else if constexpr (detect<unqual> == variant_tag::EITHER) {
      return either (std::forward<Var> (var));
    } else if constexpr (detect<unqual> == variant_tag::VARIANT) {
      return variant (std::forward<Var> (var));
    } else if constexpr (detect<unqual> == variant_tag::OPTION) {
      return just (std::forward<Var> (var));
    } else {
      static_assert (detect<unqual> == variant_tag::MAYBE);
      if constexpr (size<unqual> == 1)
        return just (std::forward<Var> (var));
      else if constexpr (size<unqual> == 2)
        return either (std::forward<Var> (var));
      else
        return variant (std::forward<Var> (var));
    }
  }
} // namespace ml::internal::movar