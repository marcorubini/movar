#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::movar
{
  /*! @class ml::movar::sequence
   * @ingroup LazyPipeline
   * @brief Returns the result of a sequence of operation, or ml::movar::nothing if one of them fails.
   *
   * See [lazy-pipelines documentation](#lazy-pipelines) for example usage.
   */
  template<std::move_constructible... Ts>
  struct sequence;

  template<>
  struct sequence<>
  {
    template<class T>
    [[nodiscard]] constexpr auto operator() (T&& value) const //
    {
      namespace impl = ml::internal::movar::impl;
      return impl::wrap (std::forward<T> (value));
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

  template<std::move_constructible T1>
  struct sequence<T1>
  {
    T1 _fn;

    constexpr sequence (T1 value) //
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
  struct sequence<T1, T2>
  {
    T1 _fn1;
    T2 _fn2;

    [[nodiscard]] constexpr sequence (T1 t1, T2 t2)      //
      noexcept (std::is_nothrow_move_constructible_v<T1> //
          && std::is_nothrow_constructible_v<T2>)        //
      : _fn1 (std::move (t1))
      , _fn2 (std::move (t2))
    {}

    // ---

    template<class Arg>
    [[nodiscard]] constexpr auto operator() (Arg&& arg) const&     //
      requires (ml::internal::movar::WrapInvocable<T1 const&, Arg> //
          && ml::internal::movar::WeakVisitor<T2 const&, ml::internal::movar::wrap_invoke_result<T1 const&, Arg>>) //
    {
      namespace impl = ml::internal::movar::impl;
      return impl::wrap_invoke (_fn1, std::forward<Arg> (arg)) //
        .map (_fn2);
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

  sequence ()->sequence<>;

  template<class T>
  sequence (T) -> sequence<T>;

  template<class T1, class T2>
  sequence (T1, T2) -> sequence<T1, T2>;

} // namespace ml::movar