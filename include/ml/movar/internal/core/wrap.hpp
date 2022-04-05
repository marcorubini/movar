#pragma once
#include <ml/movar/internal/core/detect.hpp>

namespace ml::internal::movar
{
  template<class Fn, class... Args>
  constexpr inline bool wrap_invocable = [] () {
    if constexpr (!invocable<Fn, Args...>) {
      return false;
    } else {
      using result = remove_cvref_t<invoke_result_t<Fn, Args...>>;
      return Variant<result> || is_void_v<result> || move_constructible<result>;
    }
  }();

  template<class Fn, class... Args>
  concept WrapInvocable = wrap_invocable<Fn, Args...>;

  template<class T>
  concept Wrappable = WrapInvocable<identity, T>;

  template<class Fn, class... Args>
    requires WrapInvocable<Fn, Args...>
  using wrap_invoke_result = decltype (impl::wrap_invoke (std::declval<Fn> (), std::declval<Args> ()...));

  template<Wrappable T>
  using wrap_result = wrap_invoke_result<identity, T>;
} // namespace ml::internal::movar