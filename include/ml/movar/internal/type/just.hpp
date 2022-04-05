#pragma once
#include <ml/movar/internal/core/core.hpp>

namespace ml::movar
{

  /*!
   * @class ml::movar::just
   * @brief A variant type with only one alternative
   * @ingroup Variant
   * @nosubgrouping
   *
   * ml::movar::just is a Variant of size 1 that always has a value.
   */
  template<class ValueType>
  struct just
  {
    static_assert (std::move_constructible<ValueType>);
    static_assert (!std::is_const_v<ValueType>);
    static_assert (!std::is_volatile_v<ValueType>);
    static_assert (std::is_object_v<ValueType>);

    using value_type = ValueType;
    using reference = value_type&;
    using const_reference = value_type const&;

    ValueType _value;

    /*!
     * @brief default constructor
     */
    constexpr just () = default;

    /*!
     * @brief Move-constructs ValueType
     */
    constexpr just (ValueType value) //
      noexcept (std::is_nothrow_move_constructible_v<value_type>)
      : _value (std::move (value))
    {}

    template<internal::movar::DiffUnqual<just> Other>
      requires (internal::movar::can_explicit_cast<Other, just>)
    explicit(!internal::movar::can_implicit_cast<Other, just>) constexpr just (Other other) //
      noexcept (noexcept (just (internal::movar::impl::cast<just> (std::move (other)))))    //
      : just (internal::movar::impl::cast<just> (std::move (other)))
    {}

    /*!
     * @brief Explicit const reference conversion
     */
    [[nodiscard]] explicit constexpr operator const_reference () const& noexcept
    {
      return _value;
    }

    /*!
     * @brief Explicit reference conversion
     */
    [[nodiscard]] explicit constexpr operator reference () & noexcept
    {
      return _value;
    }

    /*!
     * @brief Explicit value conversion
     */
    [[nodiscard]] explicit constexpr operator value_type () && //
      noexcept (std::is_nothrow_move_constructible_v<value_type>)
    {
      return std::move (_value);
    }

    //! @name Observers
    //! @{

    /*!
     * @brief Returns 0
     */
    [[nodiscard]] constexpr long index () const noexcept
    {
      return 0;
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
     * @tparam Index can be 0 or -1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<just, Index> || Index == -1)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      return Index == 0;
    }

    /*!
     * @brief Returns true if @a T equals the active type
     * @tparam T can be ValueType or nothing
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<just, T> || std::same_as<T, nothing>)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      return std::same_as<T, ValueType>;
    }

    //! @}
    //! @name Getters
    //! @{

    /*!
     * @brief get with index (overload 1)
     * @tparam Index must be 0
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<just, Index>)
    [[nodiscard]] constexpr const_reference get () const& noexcept
    {
      return _value;
    }

    /*!
     * @brief get with index (overload 2)
     * @tparam Index must be 0
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<just, Index>)
    [[nodiscard]] constexpr reference get () & noexcept
    {
      return _value;
    }

    /*!
     * @brief get with index (overload 3)
     * @tparam Index must be 0
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<just, Index>)
    [[nodiscard]] constexpr value_type get () && //
      noexcept (std::is_nothrow_move_constructible_v<value_type>)
    {
      return std::move (_value);
    }

    /*!
     * @brief get with type (overload 1)
     * @tparam T must be ValueType
     */
    template<class T = value_type>
      requires (ml::internal::movar::ContainsAlternative<just, T>)
    [[nodiscard]] constexpr const_reference get () const& noexcept
    {
      return _value;
    }

    /*!
     * @brief get with type (overload 2)
     * @tparam T must be ValueType
     */
    template<class T = value_type>
      requires (ml::internal::movar::ContainsAlternative<just, T>)
    [[nodiscard]] constexpr reference get () & noexcept
    {
      return _value;
    }

    /*!
     * @brief get with type (overload 3)
     * @tparam T must be ValueType
     */
    template<class T = value_type>
      requires (ml::internal::movar::ContainsAlternative<just, T>)
    [[nodiscard]] constexpr value_type get () && //
      noexcept (std::is_nothrow_move_constructible_v<value_type>)
    {
      return std::move (_value);
    }

    //! @}
    //! @name Friends
    //! @{

    /*!
     * @brief Returns true if @a Index equals the active index.
     * @tparam Index
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<just, Index> || Index == -1)
    [[nodiscard]] friend constexpr bool is (just const& self) noexcept
    {
      return Index == 0;
    }

    /*!
     * @brief Returns true if @a T equals the active type.
     * @tparam T
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<just, T> || std::same_as<T, nothing>)
    [[nodiscard]] friend constexpr bool is (just const& self) noexcept
    {
      return std::same_as<T, ValueType>;
    }

    /*!
     * @brief get with Index
     * @tparam Index must be 0
     * @tparam Self
     */
    template<long Index, internal::movar::SameUnqual<just> Self>
      requires (internal::movar::ContainsIndex<just, Index>)
    [[nodiscard]] friend constexpr decltype (auto) get (Self&& self) //
      noexcept (noexcept (std::forward<Self> (self).template get<Index> ()))
    {
      return std::forward<Self> (self).template get<Index> ();
    }

    /*!
     * @brief get with type
     * @tparam T must be ValueType
     * @tparam Self
     */
    template<class T, internal::movar::SameUnqual<just> Self>
      requires (internal::movar::ContainsAlternative<just, T>)
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
    template<internal::movar::WeakVisitor<just const&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) const&
    {
      return internal::movar::impl::map (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map)
     */
    template<internal::movar::WeakVisitor<just&&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) &&
    {
      return internal::movar::impl::map (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<just const&> Vis>
    constexpr auto match (Vis vis) const&
    {
      return internal::movar::impl::match (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<just&&> Vis>
    constexpr auto match (Vis vis) &&
    {
      return internal::movar::impl::match (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<just const&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<just&&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) &&
    {
      return internal::movar::impl::map_or (std::move (*this), std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<just const&> Vis, internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto map_or_else (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or_else (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<just&&> Vis, internal::movar::Lazy Default>
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
     * @brief default equality
     */
    bool operator== (just const&) const = default;

    /*!
     * @brief default compare
     */
    auto operator<=> (just const&) const = default;
  };

  template<class T>
    requires (!Variant<T>)
  just (T)
  ->just<T>;

  template<class T>
  just (option<T>) -> just<T>;

  template<class T>
  just (variant<T>) -> just<T>;

  template<class T>
  just (maybe<T>) -> just<T>;

} // namespace ml::movar

#undef ML_MOVAR_NOEXCEPT