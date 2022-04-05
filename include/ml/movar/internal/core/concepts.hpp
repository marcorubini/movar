#pragma once
#include <ml/movar/internal/core/alternatives.hpp>
#include <ml/movar/internal/core/wrap.hpp>

namespace ml::internal::movar
{
  template<class Vis, class Var>
  constexpr inline bool weak_visitor = [] () -> bool {
    auto helper = []<class... Ts> (mp_list<Ts...>)
    {
      return (WrapInvocable<Vis, copy_quals<Var, Ts>> && ...);
    };
    return helper (alternatives<remove_cvref_t<Var>> {});
  }();

  template<class Vis, class Ret, class Var>
  constexpr inline bool weak_visitor_r = [] () -> bool {
    auto helper = []<class... Ts> (mp_list<Ts...>)
    {
      return (WrapInvocable<Vis, copy_quals<Var, Ts>> && ...) //
        && (is_invocable_r_v<Ret, Vis, copy_quals<Var, Ts>> && ...);
    };
    return helper (alternatives<remove_cvref_t<Var>> {});
  }();

  template<class Vis, class Var>
  constexpr inline bool visitor = [] () -> bool {
    constexpr bool maybe_null = None<remove_cvref_t<Var>> || Maybe<remove_cvref_t<Var>>;
    return weak_visitor<Vis, Var> && (maybe_null <= invocable<Vis, nothing>);
  }();

  template<class Vis, class Ret, class Var>
  constexpr inline bool visitor_r = [] () -> bool {
    constexpr bool maybe_null = None<remove_cvref_t<Var>> || Maybe<remove_cvref_t<Var>>;
    return weak_visitor_r<Vis, Ret, Var> && (maybe_null <= is_invocable_r_v<Ret, Vis, nothing>);
  }();

  template<class Vis, class Var>
  concept WeakVisitor = weak_visitor<Vis, Var>;

  template<class Vis, class Ret, class Var>
  concept WeakVisitorR = weak_visitor_r<Vis, Ret, Var>;

  template<class Vis, class Var>
  concept Visitor = visitor<Vis, Var>;

  template<class Vis, class Ret, class Var>
  concept VisitorR = visitor_r<Vis, Ret, Var>;

  template<class Fn>
  concept Lazy = WrapInvocable<Fn>;
} // namespace ml::internal::movar