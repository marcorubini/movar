#pragma once
#include <ml/movar/internal/core/utility.hpp>

namespace ml::internal::movar
{
  template<long I, class T>
  inline void get (T&&) = delete;

  template<class U, class T>
  inline void get (T&&) = delete;

  // hide from ADL
  namespace impl
  {
    template<class To, int..., class From>
    static constexpr To cast (From&& from);

    template<class Arg>
    static constexpr auto wrap (Arg&& arg);

    template<class Fn, class... Args>
    static constexpr auto wrap_invoke (Fn&& fn, Args&&... args);

    template<class Vis, class Var>
    static constexpr auto weak_visit (Vis&& vis, Var&& var);

    template<class R, class Vis, class Var>
    static constexpr auto weak_visit_r (Vis&& vis, Var&& var);

    template<class Vis, class Var>
    static constexpr auto visit (Vis&& vis, Var&& var);

    template<class R, class Vis, class Var>
    static constexpr auto visit_r (Vis&& vis, Var&& var);

    template<class Var, class Fn>
    static constexpr auto map (Var&& var, Fn&& fn);

    template<class Var, class Fn>
    static constexpr auto match (Var&& var, Fn&& fn);

    template<class Var, class Fn, class Default>
    static constexpr auto map_or (Var&& var, Fn&& fn, Default&& def);

    template<class Var, class Fn, class Default>
    static constexpr auto map_or_else (Var&& var, Fn&& fn, Default&& lazy);

    template<class Var, class Default>
    static constexpr auto or_else (Var&& var, Default&& lazy);

    template<class Var>
    static constexpr auto take (Var&&);

    template<class Lhs, class Rhs>
    static constexpr auto pipe_sequence (Lhs&& lhs, Rhs&& rhs);

    template<class Lhs, class Rhs>
    static constexpr auto pipe_fork (Lhs&& lhs, Rhs&& rhs);
  } // namespace impl
} // namespace ml::internal::movar

namespace ml::movar
{
  struct nothing;

  template<class T>
  struct just;

  template<class T>
  struct option;

  template<class T1, class T2>
  struct either;

  template<class... Ts>
    requires (sizeof...(Ts) > 0)
  struct variant;

  template<class... Ts>
    requires (sizeof...(Ts) > 0)
  struct maybe;

  template<std::move_constructible... Ts>
  struct sequence;

  template<std::move_constructible... Ts>
    requires (sizeof...(Ts) > 0)
  struct fork;

  template<std::move_constructible T>
  struct filter;

  template<class T>
  struct filter_on_type;
} // namespace ml::movar

namespace ml::internal::movar
{
  using ml::movar::either;
  using ml::movar::just;
  using ml::movar::maybe;
  using ml::movar::nothing;
  using ml::movar::option;
  using ml::movar::variant;

  using ml::movar::filter;
  using ml::movar::filter_on_type;
  using ml::movar::fork;
  using ml::movar::sequence;
} // namespace ml::internal::movar