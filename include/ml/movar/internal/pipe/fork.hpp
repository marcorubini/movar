#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::movar
{
  /*! @class ml::movar::fork
   * @ingroup LazyPipeline
   * @brief Returns the first result that is not ml::movar::nothing, or ml::movar::nothing.
   *
   * See [lazy-pipelines documentation](#lazy-pipelines) for example usage.
   */
  template<std::move_constructible... Ts>
    requires (sizeof...(Ts) > 0)
  struct fork;

  template<std::move_constructible T1>
  struct fork<T1>
  {
    T1 _fn;

    constexpr fork (T1 value) //
      noexcept (std::is_nothrow_move_constructible_v<T1>)
      : _fn (std::move (value))
    {}

    // ---

    template<class Arg>
    [[nodiscard]] constexpr auto operator() (Arg&& arg) const       //
      requires (ml::internal::movar::WrapInvocable<T1 const&, Arg>) //
    {
      namespace impl = ml::internal::movar::impl;
      return impl::wrap_invoke (_fn, std::forward<Arg> (arg));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator>> (Other&& other) const&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_sequence (*this, std::forward<Other> (other));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator>> (Other&& other) &&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_sequence (std::move (*this), std::forward<Other> (other));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator| (Other&& other) const&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_fork (*this, std::forward<Other> (other));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator| (Other&& other) &&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_fork (std::move (*this), std::forward<Other> (other));
    }
  };

  template<std::move_constructible T1, std::move_constructible T2>
  struct fork<T1, T2>
  {
    T1 _first;
    T2 _second;

    constexpr fork (T1 first, T2 second)                                                             //
      noexcept (std::is_nothrow_move_constructible_v<T1>&& std::is_nothrow_move_constructible_v<T2>) //
      : _first (std::move (first))
      , _second (std::move (second))
    {}

    template<class Arg>
    [[nodiscard]] constexpr auto operator() (Arg&& arg) const       //
      requires (ml::internal::movar::WrapInvocable<T1 const&, Arg&> //
        || ml::internal::movar::WrapInvocable<T2 const&, Arg&>)     //
    {
      using ml::internal::movar::WrapInvocable;
      namespace impl = ml::internal::movar::impl;
      if constexpr (WrapInvocable<T1 const&, Arg&> && WrapInvocable<T2 const&, Arg&>) {
        return impl::wrap_invoke (_first, arg) //
          .or_else (std::bind_front (_second, std::forward<Arg> (arg)));
      } else if constexpr (WrapInvocable<T1 const&, Arg>) {
        return impl::wrap_invoke (_first, std::forward<Arg> (arg));
      } else if constexpr (WrapInvocable<T2 const&, Arg>) {
        return impl::wrap_invoke (_second, std::forward<Arg> (arg));
      } else {
        return nothing ();
      }
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator>> (Other&& other) const&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_sequence (*this, std::forward<Other> (other));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator>> (Other&& other) &&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_sequence (std::move (*this), std::forward<Other> (other));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator| (Other&& other) const&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_fork (*this, std::forward<Other> (other));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator| (Other&& other) &&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_fork (std::move (*this), std::forward<Other> (other));
    }
  };

  template<class T>
  fork (T) -> fork<T>;

  template<class T1, class T2>
  fork (T1, T2) -> fork<T1, T2>;

} // namespace ml::movar