#pragma once
#include <ml/movar/internal/type/nothing.hpp>
#include <stdexcept>

#ifdef __GNUC__
#  define ML_MOVAR_UNREACHABLE __builtin_unreachable ()
#else
#  define ML_MOVAR_UNREACHABLE __assume (false)
#endif

namespace ml::internal::movar
{
  template<class To, int..., class From>
  static constexpr To impl::cast (From&& from)
  {
    static_assert (!None<To>);
    static_assert (!None<remove_cvref_t<From>>);

    using unqual_from = remove_cvref_t<From>;
    constexpr bool can_fail = (Maybe<unqual_from> && Some<To>);

    auto const visitor = []<class X> (X&& value) -> To {
      constexpr bool is_none = None<remove_cvref_t<X>>;
      if constexpr (is_none && can_fail) {
        if constexpr (ML_MOVAR_THROWING_CAST)
          throw std::runtime_error ("Bad variant cast");
        ML_MOVAR_UNREACHABLE;
      } else {
        return To (std::forward<X> (value));
      }
    };
    return impl::visit (visitor, std::forward<From> (from));
  }
} // namespace ml::internal::movar