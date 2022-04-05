#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::internal::movar
{
  template<class Arg>
  static constexpr auto impl::wrap (Arg&& arg)
  {
    using unqual = remove_cvref_t<Arg>;
    if constexpr (Variant<unqual>) {
      return std::forward<Arg> (arg);
    } else {
      return just (std::forward<Arg> (arg));
    }
  }

  template<class Fn, class... Args>
  static constexpr auto impl::wrap_invoke (Fn&& fn, Args&&... args)
  {
    using unqual = remove_cvref_t<std::invoke_result_t<Fn, Args...>>;
    if constexpr (Variant<unqual>) {
      return std::invoke (std::forward<Fn> (fn), std::forward<Args> (args)...);
    } else if constexpr (is_void_v<unqual>) {
      std::invoke (std::forward<Fn> (fn), std::forward<Args> (args)...);
      return nothing ();
    } else {
      return impl::wrap (std::invoke (std::forward<Fn> (fn), std::forward<Args> (args)...));
    }
  }
} // namespace ml::internal::movar