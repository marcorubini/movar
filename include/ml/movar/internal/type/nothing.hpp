#pragma once
#include <ml/movar/internal/core/core.hpp>

namespace ml::movar
{
  /*! @class ml::movar::nothing
   * @brief A variant with zero alternatives.
   * @ingroup Variant
   */
  struct nothing
  {
    //! @name Observers
    //! @{

    /*!
     * @return -1
     */
    [[nodiscard]] constexpr long index () const noexcept
    {
      return -1;
    }

    /*!
     * @return true
     */
    [[nodiscard]] constexpr bool is_nothing () const noexcept
    {
      return true;
    }

    /*!
     * @return false
     */
    [[nodiscard]] constexpr bool is_something () const noexcept
    {
      return false;
    }

    /*!
     * @return true
     * @tparam I must be -1
     */
    template<long I>
      requires (I == -1)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      return true;
    }

    /*!
     * @return true
     * @tparam T must be ml::movar::nothing
     */
    template<std::same_as<nothing> T>
    [[nodiscard]] constexpr bool is () const noexcept
    {
      return true;
    }

    //! @}
    //! @name Getters
    //! @{

    /*!
     * @brief deleted
     */
    template<long Index>
    [[nodiscard]] constexpr auto get () const noexcept = delete;

    /*!
     * @brief deleted
     */
    template<class T>
    [[nodiscard]] constexpr auto get () const noexcept = delete;

    //! @}
    //! @name Friends
    //! @{

    /*!
     * @return true
     * @tparam I must be -1
     */
    template<long I>
      requires (I == -1)
    [[nodiscard]] friend constexpr auto is (nothing const&) noexcept
    {
      return true;
    }

    /*!
     * @return true
     * @tparam T must be ml::movar::nothing
     */
    template<std::same_as<nothing> T>
    [[nodiscard]] friend constexpr auto is (nothing const&) noexcept
    {
      return true;
    }

    /*!
     * @brief deleted
     */
    template<long Index>
    [[nodiscard]] friend constexpr auto get (nothing const&) noexcept = delete;

    /*!
     * @brief deleted
     */
    template<class T>
    [[nodiscard]] friend constexpr auto get (nothing const&) noexcept = delete;

    //! @}
    //! @name Pipeline
    //! @{

    /*!
     * @brief see [pipelines documentation](#pipelines-map)
     */
    template<std::move_constructible Vis>
    [[nodiscard]] constexpr auto map (Vis) const
    {
      return nothing ();
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<nothing> Vis>
    constexpr auto match (Vis vis) const
    {
      return internal::movar::impl::wrap_invoke (std::move (vis), nothing ());
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<std::move_constructible Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) const
    {
      return def;
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<std::move_constructible Vis, internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto map_or_else (Vis vis, Default def) const
    {
      return internal::movar::impl::wrap_invoke (std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-or_else)
     */
    template<internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto or_else (Default def) const
    {
      return internal::movar::impl::wrap_invoke (std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-take)
     */
    [[nodiscard]] constexpr auto take () const noexcept
    {
      return nothing ();
    }

    //! @}

    /*!
     * @brief default equality
     */
    bool operator== (nothing const&) const = default;

    /*!
     * @brief default compare
     */
    auto operator<=> (nothing const&) const = default;
  };

} // namespace ml::movar
