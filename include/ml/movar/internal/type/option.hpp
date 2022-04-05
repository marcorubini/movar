#pragma once
#include <ml/movar/internal/type/nothing.hpp>
#include <optional>

namespace ml::movar
{
  /*! @class ml::movar::option
   * @brief A variant with one alternative that can also be empty.
   * @ingroup Variant
   */
  template<class ValueType>
  struct option
  {
    static_assert (std::move_constructible<ValueType>);
    static_assert (!std::is_const_v<ValueType>);
    static_assert (!std::is_volatile_v<ValueType>);
    static_assert (std::is_object_v<ValueType>);

    using value_type = ValueType;
    using reference = value_type&;
    using const_reference = value_type const&;

    std::optional<ValueType> _value {};

    /*!
     * @brief default constructor
     */
    option () = default;

    /*!
     * @brief Move-constructs ValueType
     */
    constexpr option (ValueType value) noexcept
      : _value (std::move (value))
    {}

    /*!
     * @brief Leaves the option empty
     */
    constexpr option (nothing) noexcept
      : _value ()
    {}

    /*!
     * @brief Leaves the option empty
     */
    constexpr option (std::nullopt_t) noexcept
      : _value ()
    {}

    template<internal::movar::DiffUnqual<option> Other>
      requires (internal::movar::can_explicit_cast<Other, option>)
    explicit(!internal::movar::can_implicit_cast<Other, option>) constexpr option (Other other) //
      noexcept (noexcept (option (internal::movar::impl::cast<option> (std::move (other)))))    //
      : option (internal::movar::impl::cast<option> (std::move (other)))
    {}

    //! @name Observers
    //! @{

    /*!
     * @return the active index, 0 or -1
     */
    [[nodiscard]] constexpr long index () const noexcept
    {
      return static_cast<long> (_value.has_value ()) - 1;
    }

    /*!
     * @return true if the option is empty
     */
    [[nodiscard]] constexpr bool is_nothing () const noexcept
    {
      return _value.has_value () == false;
    }

    /*!
     * @return false if the option is empty
     */
    [[nodiscard]] constexpr bool is_something () const noexcept
    {
      return _value.has_value () == true;
    }

    /*!
     * @return true if @a Index equals the active index
     * @tparam Index can be 0 or -1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<option, Index> || Index == -1)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      if constexpr (Index == -1) {
        return is_nothing ();
      } else {
        return is_something ();
      }
    }

    /*!
     * @return true if @a T equals the active type
     * @tparam T can be @a ValueType or ml::movar::nothing
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<option, T> || std::same_as<T, nothing>)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      if constexpr (std::same_as<T, nothing>) {
        return is_nothing ();
      } else {
        return is_something ();
      }
    }

    //! @}
    //! @name Getters
    //! @{

    /*!
     * @brief getter given index (overload 1)
     * @tparam Index must be 0
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<option, Index>)
    [[nodiscard]] constexpr const_reference get () const& noexcept
    {
      return *_value;
    }

    /*!
     * @brief getter given index (overload 2)
     * @tparam Index must be 0
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<option, Index>)
    [[nodiscard]] constexpr reference get () & noexcept
    {
      return *_value;
    }

    /*!
     * @brief getter given index (overload 3)
     * @tparam Index must be 0
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<option, Index>)
    [[nodiscard]] constexpr value_type get () &&                  //
      noexcept (std::is_nothrow_move_constructible_v<value_type>) //
    {
      return std::move (*_value);
    }

    /*!
     * @brief getter given type (overload 1)
     * @tparam T must be @a ValueType
     */
    template<class T = value_type>
      requires (internal::movar::ContainsAlternative<option, T>)
    [[nodiscard]] constexpr const_reference get () const& noexcept
    {
      return *_value;
    }

    /*!
     * @brief getter given type (overload 1)
     * @tparam T must be @a ValueType
     */
    template<class T = value_type>
      requires (internal::movar::ContainsAlternative<option, T>)
    [[nodiscard]] constexpr reference get () & noexcept
    {
      return *_value;
    }

    /*!
     * @brief getter given type (overload 1)
     * @tparam T must be @a ValueType
     */
    template<class T = value_type>
      requires (internal::movar::ContainsAlternative<option, T>)
    [[nodiscard]] constexpr value_type get () &&                  //
      noexcept (std::is_nothrow_move_constructible_v<value_type>) //
    {
      return std::move (*_value);
    }

    //! @}
    //! @name Friends
    //! @{

    /*!
     * @return true if @a Index equals the active index
     * @tparam Index must be 0 or -1
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<option, Index> || Index == -1)
    [[nodiscard]] friend constexpr bool is (option const& self) noexcept
    {
      return self.template is<Index> ();
    }

    /*!
     * @return true if @a T equals the active type
     * @tparam T must be @a ValueType or ml::movar::nothing
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<option, T> || std::same_as<T, nothing>)
    [[nodiscard]] friend constexpr bool is (option const& self) noexcept
    {
      return self.template is<T> ();
    }

    /*!
     * @brief getter given index
     * @tparam Index must be 0
     * @tparam Self must be an instance of ml::movar::option
     */
    template<long Index, internal::movar::SameUnqual<option> Self>
      requires (internal::movar::ContainsIndex<option, Index>)
    [[nodiscard]] friend constexpr decltype (auto) get (Self&& self) //
      noexcept (noexcept (std::forward<Self> (self).template get<Index> ()))
    {
      return std::forward<Self> (self).template get<Index> ();
    }

    /*!
     * @brief getter given type
     * @tparam T must be @a ValueType
     * @tparam Self must be an instance of ml::movar::option
     */
    template<class T, internal::movar::SameUnqual<option> Self>
      requires (internal::movar::ContainsAlternative<option, T>)
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
    template<internal::movar::WeakVisitor<option const&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) const&
    {
      return internal::movar::impl::map (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map)
     */
    template<internal::movar::WeakVisitor<option&&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) &&
    {
      return internal::movar::impl::map (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<option const&> Vis>
    constexpr auto match (Vis vis) const&
    {
      return internal::movar::impl::match (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<option&&> Vis>
    constexpr auto match (Vis vis) &&
    {
      return internal::movar::impl::match (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<option const&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<option&&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) &&
    {
      return internal::movar::impl::map_or (std::move (*this), std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<option const&> Vis, internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto map_or_else (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or_else (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<option&&> Vis, internal::movar::Lazy Default>
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
     * @brief defautl equality
     */
    bool operator== (option const&) const = default;

    /*!
     * @brief default compare
     */
    auto operator<=> (option const&) const = default;
  };

  template<class T>
  option (T) -> option<T>;

  template<class T>
  option (just<T>) -> option<T>;

  template<class T>
  option (maybe<T>) -> option<T>;

  template<class T>
  option (variant<T>) -> option<T>;

} // namespace ml::movar
