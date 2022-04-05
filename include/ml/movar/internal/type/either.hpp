#pragma once
#include <ml/movar/internal/core/core.hpp>
#include <variant>

namespace ml::movar
{
  /*! @defgroup Variant Variant
   */

  /*!
   * @class ml::movar::either
   * @brief A variant type with two alternatives
   * @ingroup Variant
   * @nosubgrouping
   *
   * ml::movar::either is a Variant of size 2 that always has a value.
   */
  template<class T1, class T2>
  struct either
  {
    static_assert (std::move_constructible<T1> && std::move_constructible<T2>);
    static_assert (std::is_object_v<T1> && std::is_object_v<T2>);
    static_assert (!std::is_const_v<T1> && !std::is_const_v<T2>);
    static_assert (!std::is_volatile_v<T1> && !std::is_volatile_v<T2>);

    using first_type = T1;
    using second_type = T2;

    std::variant<T1, T2> _value;

    /*!
     * @brief default constructor
     */
    constexpr either () = default;

    /*!
     * @brief Move-constructs an instance of @a T1
     */
    constexpr either (T1 value) //
      noexcept (std::is_nothrow_move_constructible_v<T1>)
      : _value (std::move (value))
    {}

    /*!
     * @brief Move-constructs an instance of @a T2
     */
    constexpr either (T2 value) //
      noexcept (std::is_nothrow_move_constructible_v<T2>)
      : _value (std::move (value))
    {}

    template<internal::movar::DiffUnqual<either> Other>
      requires (internal::movar::can_explicit_cast<Other, either>)
    explicit(!internal::movar::can_implicit_cast<Other, either>) constexpr either (Other other) //
      noexcept (noexcept (either (internal::movar::impl::cast<either> (std::move (other)))))    //
      : either (internal::movar::impl::cast<either> (std::move (other)))
    {}

    //! @name Observers
    //! @{

    /*!
     * @brief Returns the active index
     */
    [[nodiscard]] constexpr long index () const noexcept
    {
      return _value.index ();
    }

    /*!
     * @brief Returns false
     */
    [[nodiscard]] constexpr bool is_nothing () const noexcept
    {
      return false;
    }

    /*!
     * @brief Returns true
     */
    [[nodiscard]] constexpr bool is_something () const noexcept
    {
      return true;
    }

    /*!
     * @brief Returns true if @a Index equals the active index
     * @tparam Index can be 0, 1 or -1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<either, Index> || Index == -1)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      if constexpr (Index == -1) {
        return false;
      } else {
        return Index == _value.index ();
      }
    }

    /*!
     * @brief Returns true if @a T equals the active type
     * @tparam T can be T1, T2 or ml::movar::nothing
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<either, T> || std::same_as<T, nothing>)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      if constexpr (std::same_as<T, T1>) {
        return index () == 0;
      } else if constexpr (std::same_as<T, T2>) {
        return index () == 1;
      } else {
        return false;
      }
    }

    //! @}
    //! @name Getters
    //! @{

    /*!
     * @brief get with index (overload 1)
     * @tparam Index can be 0 or 1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<either, Index>)
    [[nodiscard]] constexpr auto const& get () const& noexcept
    {
      return *std::get_if<Index> (&_value);
    }

    /*!
     * @brief get with index (overload 2)
     * @tparam Index can be 0 or 1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<either, Index>)
    [[nodiscard]] constexpr auto& get () & noexcept
    {
      return *std::get_if<Index> (&_value);
    }

    /*!
     * @brief get with index (overload 3)
     * @tparam Index can be 0 or 1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<either, Index>)
    [[nodiscard]] constexpr auto get () &&                                                           //
      noexcept (std::is_nothrow_move_constructible_v<T1>&& std::is_nothrow_move_constructible_v<T2>) //
    {
      return std::move (*std::get_if<Index> (&_value));
    }

    /*!
     * @brief get with type (overload 1)
     * @tparam T can be @a T1 or @a T2
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<either, T>)
    [[nodiscard]] constexpr T const& get () const& noexcept
    {
      return *std::get_if<T> (&_value);
    }

    /*!
     * @brief get with type (overload 2)
     * @tparam T can be @a T1 or @a T2
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<either, T>)
    [[nodiscard]] constexpr T& get () & noexcept
    {
      return *std::get_if<T> (&_value);
    }

    /*!
     * @brief get with type (overload 3)
     * @tparam T can be @a T1 or @a T2
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<either, T>)
    [[nodiscard]] constexpr T get () &&                                                              //
      noexcept (std::is_nothrow_move_constructible_v<T1>&& std::is_nothrow_move_constructible_v<T2>) //
    {
      return std::move (*std::get_if<T> (&_value));
    }

    //! @}
    //! @name Friends
    //! @{

    /*!
     * @brief Returns true if @a Index equals the active index
     * @tparam Index can be 0, 1 or -1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<either, Index> || Index == -1)
    [[nodiscard]] friend constexpr bool is (either const& self) noexcept
    {
      return self.template is<Index> ();
    }

    /*!
     * @brief Returns true if @a T equals the active type
     * @tparam T can be @a T1, @a T2 or ml::movar::nothing
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<either, T> || std::same_as<T, nothing>)
    [[nodiscard]] friend constexpr bool is (either const& self) noexcept
    {
      return self.template is<T> ();
    }

    /*!
     * @brief get with Index
     * @tparam Index can be 0, 1
     * @tparam Self
     */
    template<long Index, internal::movar::SameUnqual<either> Self>
      requires (internal::movar::ContainsIndex<either, Index>)
    [[nodiscard]] friend constexpr decltype (auto) get (Self&& self) //
      noexcept (noexcept (std::forward<Self> (self).template get<Index> ()))
    {
      return std::forward<Self> (self).template get<Index> ();
    }

    /*!
     * @brief get with type
     * @tparam T can be @a T1 or @a T2
     * @tparam Self
     */
    template<class T, internal::movar::SameUnqual<either> Self>
      requires (internal::movar::ContainsAlternative<either, T>)
    [[nodiscard]] friend constexpr decltype (auto) get (Self&& self) //
      noexcept (noexcept (std::forward<Self> (self).template get<T> ()))
    {
      return std::forward<Self> (self).template get<T> ();
    }

    //! @}
    //! @name Pipeline
    //! @{

    /*!
     * @brief see [pipelines documentation](#pipelines-map)
     */
    template<internal::movar::WeakVisitor<either const&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) const&
    {
      return internal::movar::impl::map (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map)
     */
    template<internal::movar::WeakVisitor<either&&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) &&
    {
      return internal::movar::impl::map (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<either const&> Vis>
    constexpr auto match (Vis vis) const&
    {
      return internal::movar::impl::match (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<either&&> Vis>
    constexpr auto match (Vis vis) &&
    {
      return internal::movar::impl::match (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<either const&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<either&&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) &&
    {
      return internal::movar::impl::map_or (std::move (*this), std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<either const&> Vis, internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto map_or_else (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or_else (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<either&&> Vis, internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto map_or_else (Vis vis, Default def) &&
    {
      return internal::movar::impl::map_or_else (std::move (*this), std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-or_else)
     */
    template<internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto or_else (Default def) const&
    {
      return internal::movar::impl::or_else (*this, std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-or_else)
     */
    template<internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto or_else (Default def) &&
    {
      return internal::movar::impl::or_else (std::move (*this), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-take)
     */
    [[nodiscard]] constexpr auto take ()
    {
      return internal::movar::impl::take (std::move (*this));
    }

    //! @}

    /*!
     * @brief defaulf equality
     */
    bool operator== (either const&) const = default;

    /*!
     * @brief default comparison
     */
    auto operator<=> (either const&) const = default;
  };

  template<class T1, class T2>
  either (T1, T2) -> either<T1, T2>;

  template<class T1, class T2>
  either (variant<T1, T2>) -> either<T1, T2>;

  template<class T1, class T2>
  either (maybe<T1, T2>) -> either<T1, T2>;

} // namespace ml::movar