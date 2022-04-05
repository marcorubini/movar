#pragma once
#include <ml/movar/internal/type/nothing.hpp>
#include <optional>
#include <variant>

namespace ml::movar
{
  /*! @class ml::movar::variant
   * @brief A variant with many alternatives.
   * @ingroup Variant
   */
  template<class... Ts>
    requires (sizeof...(Ts) > 0)
  struct variant
  {
    static_assert ((std::move_constructible<Ts> && ...));
    static_assert ((!std::is_const_v<Ts> && ...));
    static_assert ((!std::is_volatile_v<Ts> && ...));
    static_assert ((std::is_object_v<Ts> && ...));

    std::variant<Ts...> _value {};

    /*!
     * @brief default constructor
     */
    variant () = default;

    /*!
     * @brief Move-construct the alternative
     */
    template<class Init>
      requires (std::same_as<Init, Ts> || ...)
    constexpr variant (Init value) //
      noexcept (std::is_nothrow_move_constructible_v<Init>)
      : _value (std::move (value))
    {}

    template<internal::movar::DiffUnqual<variant> Other>
      requires (internal::movar::can_explicit_cast<Other, variant>)
    explicit(!internal::movar::can_implicit_cast<Other, variant>) constexpr variant (Other other) //
      noexcept (noexcept (variant (internal::movar::impl::cast<variant> (std::move (other)))))    //
      : variant (internal::movar::impl::cast<variant> (std::move (other)))
    {}

    //! @name Observers
    //! @{

    /*!
     * @return the active index
     */
    [[nodiscard]] constexpr long index () const noexcept
    {
      return _value.index ();
    }

    /*!
     * @return false
     */
    [[nodiscard]] constexpr bool is_nothing () const noexcept
    {
      return false;
    }

    /*!
     * @return true
     */
    [[nodiscard]] constexpr bool is_something () const noexcept
    {
      return true;
    }

    /*!
     * @return true if @a Index equals the active index
     * @tparam Index must be in [-1, size)
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<variant, Index> || Index == -1)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      if constexpr (Index == -1) {
        return is_nothing ();
      } else {
        return _value.index () == Index;
      }
    }

    /*!
     * @return true if @a T equals the active type
     * @tparam T must be an alternative or ml::movar::nothing
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<variant, T> || std::same_as<T, nothing>)
    [[nodiscard]] constexpr bool is () const noexcept
    {
      if constexpr (std::same_as<T, nothing>) {
        return is_nothing ();
      } else {
        return std::holds_alternative<T> (_value);
      }
    }

    //! @}
    //! @name Getters
    //! @{

    /*!
     * @brief getter given index (overload 1)
     * @tparam Index must be in [0, size)
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<variant, Index> || Index == -1)
    [[nodiscard]] constexpr auto const& get () const& noexcept
    {
      return *std::get_if<Index> (&_value);
    }

    /*!
     * @brief getter given index (overload 2)
     * @tparam Index must be in [0, size)
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<variant, Index> || Index == -1)
    [[nodiscard]] constexpr auto& get () & noexcept
    {
      return *std::get_if<Index> (&_value);
    }

    /*!
     * @brief getter given index (overload 3)
     * @tparam Index must be in [0, size)
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<variant, Index> || Index == -1)
    [[nodiscard]] constexpr auto get () &&
    {
      return std::move (*std::get_if<Index> (&_value));
    }

    /*!
     * @brief getter given type (overload 1)
     * @tparam T must be an alternative
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<variant, T>)
    [[nodiscard]] constexpr T const& get () const& noexcept
    {
      return *std::get_if<T> (&_value);
    }

    /*!
     * @brief getter given type (overload 2)
     * @tparam T must be an alternative
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<variant, T>)
    [[nodiscard]] constexpr T& get () & noexcept
    {
      return *std::get_if<T> (&_value);
    }

    /*!
     * @brief getter given type (overload 3)
     * @tparam T must be an alternative
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<variant, T>)
    [[nodiscard]] constexpr T get () &&
    {
      return std::move (*std::get_if<T> (&_value));
    }

    //! @}
    //! @name Friends
    //! @{

    /*!
     * @return true if @a Index equals the active index
     * @tparam Index must be in [-1, size)
     */
    template<long Index>
      requires (internal::movar::ContainsIndex<variant, Index> || Index == -1)
    [[nodiscard]] friend constexpr bool is (variant const& self) noexcept
    {
      return self.template is<Index> ();
    }

    /*!
     * @return true if @a T equals the active type
     * @tparam T must be an alternative or ml::movar::nothing
     */
    template<class T>
      requires (internal::movar::ContainsAlternative<variant, T> || std::same_as<T, nothing>)
    [[nodiscard]] friend constexpr bool is (variant const& self) noexcept
    {
      return self.template is<T> ();
    }

    /*!
     * @brief getter given index
     * @tparam Index must be in [0, size)
     */
    template<long Index, internal::movar::SameUnqual<variant> Self>
      requires (internal::movar::ContainsIndex<variant, Index>)
    [[nodiscard]] friend constexpr decltype (auto) get (Self&& self) //
      noexcept (noexcept (std::forward<Self> (self).template get<Index> ()))
    {
      return std::forward<Self> (self).template get<Index> ();
    }

    /*!
     * @brief getter given type
     * @tparam T must be an alternative
     */

    template<class T, internal::movar::SameUnqual<variant> Self>
      requires (internal::movar::ContainsAlternative<variant, T>)
    [[nodiscard]] friend constexpr decltype (auto) get (Self&& self) //
      noexcept (noexcept (std::forward<Self> (self).template get<T> ()))
    {
      return std::forward<Self> (self).template get<T> ();
    }

    //! @}
    //! @name Pipeline
    //! @{

    /*!
     * @brief see [pipelines documentation](#pipelines-map  )
     */
    template<internal::movar::WeakVisitor<variant const&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) const&
    {
      return internal::movar::impl::map (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map  )
     */
    template<internal::movar::WeakVisitor<variant&&> Vis>
    [[nodiscard]] constexpr auto map (Vis vis) &&
    {
      return internal::movar::impl::map (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<variant const&> Vis>
    constexpr auto match (Vis vis) const&
    {
      return internal::movar::impl::match (*this, std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-match)
     */
    template<internal::movar::Visitor<variant&&> Vis>
    constexpr auto match (Vis vis) &&
    {
      return internal::movar::impl::match (std::move (*this), std::move (vis));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<variant const&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or)
     */
    template<internal::movar::WeakVisitor<variant&&> Vis, std::move_constructible Default>
    [[nodiscard]] constexpr auto map_or (Vis vis, Default def) &&
    {
      return internal::movar::impl::map_or (std::move (*this), std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<variant const&> Vis, internal::movar::Lazy Default>
    [[nodiscard]] constexpr auto map_or_else (Vis vis, Default def) const&
    {
      return internal::movar::impl::map_or_else (*this, std::move (vis), std::move (def));
    }

    /*!
     * @brief see [pipelines documentation](#pipelines-map_or_else)
     */
    template<internal::movar::WeakVisitor<variant&&> Vis, internal::movar::Lazy Default>
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
    bool operator== (variant const&) const = default;

    /*!
     * @brief default compare
     */
    auto operator<=> (variant const&) const = default;
  };

  template<class T>
  variant (just<T>) -> variant<T>;

  template<class T1, class T2>
  variant (either<T1, T2>) -> variant<T1, T2>;

  template<class... Ts>
  variant (maybe<Ts...>) -> variant<Ts...>;

  template<class T>
  variant (option<T>) -> variant<T>;

} // namespace ml::movar