#pragma once
#include <ml/movar/internal/type/nothing.hpp>

namespace ml::movar
{

  /*! @defgroup LazyPipeline Lazy Pipelines
   */

  /*! @class ml::movar::filter
   * @ingroup LazyPipeline
   * @brief Forwards the argument if it satisfies a predicate, or returns ml::movar::nothing
   *
   * A filter is a lazy pipeline component that is constructed from a boolean predicate.
   *
   * The predicate is invoked with the piped argument.
   *
   * If the predicate returns true, the piped argument is forwarded to the next operation in the pipeline.
   *
   * Otherwise, ml::movar::nothing is returned.
   *
   * See [lazy-pipelines documentation](#lazy-pipelines) for example usage.
   */
  template<std::move_constructible Predicate>
  struct filter
  {
    Predicate _pred;

    constexpr filter (Predicate pred) //
      noexcept (std::is_nothrow_move_constructible_v<Predicate>)
      : _pred (std::move (pred))
    {}

    template<class Arg>
    [[nodiscard]] constexpr auto operator() (Arg&& arg) const        //
      noexcept (std::is_nothrow_invocable_v<Predicate const&, Arg&>) //
      requires (std::predicate<Predicate const&, Arg&>)              //
    {
      using ml::internal::movar::add_nothing;
      using ml::internal::movar::wrap_result;
      using result = add_nothing<wrap_result<Arg>>;
      namespace impl = ml::internal::movar::impl;
      if (std::invoke (_pred, arg))
        return result (impl::wrap (std::forward<Arg> (arg)));
      return result ();
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator>> (Other&& rhs) const&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_sequence (*this, std::forward<Other> (rhs));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator>> (Other&& rhs) &&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_sequence (std::move (*this), std::forward<Other> (rhs));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator| (Other&& rhs) const&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_fork (*this, std::forward<Other> (rhs));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator| (Other&& rhs) &&
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_fork (std::move (*this), std::forward<Other> (rhs));
    }
  };

  template<class Predicate>
  filter (Predicate) -> filter<Predicate>;

  /*! @class ml::movar::filter_type
   * @ingroup LazyPipeline
   * @brief Forwards the argument if it matches a given type, otherwise returns ml::movar::nothing.
   *
   * See [lazy-pipelines documentation](#lazy-pipelines) for example usage.
   */
  template<class Type>
  struct filter_type
  {
    template<class Arg>
    [[nodiscard]] constexpr auto operator() (Arg&&) const noexcept
    {
      return nothing ();
    }

    template<ml::internal::movar::SameUnqual<Type> Arg>
    [[nodiscard]] constexpr auto operator() (Arg&& arg) const //
    {
      namespace impl = ml::internal::movar::impl;
      return impl::wrap (std::forward<Arg> (arg));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator>> (Other&& other) const
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_sequence (*this, std::forward<Other> (other));
    }

    template<class Other>
    [[nodiscard]] constexpr auto operator| (Other&& other) const
    {
      namespace impl = ml::internal::movar::impl;
      return impl::pipe_fork (*this, std::forward<Other> (other));
    }
  };

} // namespace ml::movar