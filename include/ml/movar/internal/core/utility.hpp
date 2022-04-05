#pragma once
#pragma once
#include <concepts>
#include <functional>

#include <boost/mp11/algorithm.hpp>
#include <boost/mp11/bind.hpp>
#include <boost/mp11/list.hpp>

/*! @namespace ml
 * @brief Decs
 */
namespace ml
{}

/*! @namespace ml::movar
 * @brief Decs
 */
namespace ml::movar
{}

namespace ml::internal::movar
{
  using boost::mp11::mp_apply;
  using boost::mp11::mp_at_c;
  using boost::mp11::mp_bind_front;
  using boost::mp11::mp_contains;
  using boost::mp11::mp_list;
  using boost::mp11::mp_push_front;
  using boost::mp11::mp_remove;
  using boost::mp11::mp_rename;
  using boost::mp11::mp_set_union;
  using boost::mp11::mp_size;
  using boost::mp11::mp_transform;
  using std::add_const_t;
  using std::add_lvalue_reference_t;
  using std::add_rvalue_reference_t;
  using std::conditional_t;
  using std::identity;
  using std::invocable;
  using std::invoke_result_t;
  using std::is_const_v;
  using std::is_invocable_r_v;
  using std::is_lvalue_reference_v;
  using std::is_rvalue_reference_v;
  using std::is_same_v;
  using std::is_void_v;
  using std::move_constructible;
  using std::remove_cvref_t;
  using std::remove_reference_t;
  using std::same_as;
  using std::type_identity;

  template<class T1, class T2>
  constexpr inline bool same_unqual = is_same_v<std::remove_cvref_t<T1>, remove_cvref_t<T2>>;

  template<class T1, class T2>
  concept SameUnqual = same_unqual<T1, T2>;

  template<class T1, class T2>
  constexpr inline bool diff_unqual = not is_same_v<remove_cvref_t<T1>, remove_cvref_t<T2>>;

  template<class T1, class T2>
  concept DiffUnqual = diff_unqual<T1, T2>;

  template<class T, class U>
  struct copy_quals_impl
  {
    static constexpr bool is_const = is_const_v<remove_reference_t<T>>;
    static constexpr bool is_lref = is_lvalue_reference_v<T&&>;
    static constexpr bool is_rref = is_rvalue_reference_v<T&&>;

    using type1 = remove_cvref_t<U>;
    using type2 = conditional_t<is_const, add_const_t<type1>, type1>;
    using type3 = conditional_t<is_lref, add_lvalue_reference_t<type2>, type2>;
    using type4 = conditional_t<is_rref, add_rvalue_reference_t<type3>, type3>;
    using type = type4;
  };

  template<class T, class U>
  using copy_quals = typename copy_quals_impl<T, U>::type;
} // namespace ml::internal::movar