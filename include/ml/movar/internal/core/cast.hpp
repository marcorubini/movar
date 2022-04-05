#pragma once
#include <ml/movar/internal/core/alternatives.hpp>

namespace ml::internal::movar
{
  template<class Source, Variant Dest>
    requires Variant<remove_cvref_t<Source>>
  constexpr inline bool can_explicit_cast = [] () -> bool {
    if constexpr (None<remove_cvref_t<Source>> || None<Dest>) {
      return false;
    } else {
      auto helper = []<class... Ts> (mp_list<Ts...>)
      {
        return (std::constructible_from<Dest, copy_quals<Source, Ts>> && ...);
      };
      return helper (alternatives<std::remove_cvref_t<Source>> {});
    }
  }();

  template<class Source, Variant Dest>
    requires Variant<remove_cvref_t<Source>>
  constexpr inline bool can_implicit_cast = can_explicit_cast<Source, Dest> //
    && (Maybe<remove_cvref_t<Source>> <= !Some<Dest>);
} // namespace ml::internal::movar