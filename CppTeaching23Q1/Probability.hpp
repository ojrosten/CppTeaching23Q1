#pragma once

#include <stdexcept>
#include <concepts>
#include <iostream>
#include <algorithm>
#include <string>

namespace maths
{
  template<class Stream, class T>
  constexpr bool is_serializable_to{
    requires(Stream& s, T& t) {
      {s << t} -> std::same_as<Stream&>;
    }
  };

  template<class Stream, class T>
  constexpr bool is_deserializable_to{
    requires(Stream & s, T & t) {
      {s >> t} -> std::same_as<Stream&>;
    }
  };

  [[nodiscard]]
  std::string make_out_of_bounds_error();

  struct throw_on_range_error
  {
    template<std::floating_point T>
    static T check(T t)
    {
      if ((t < 0) || (t > 1))
        throw std::out_of_range{ make_out_of_bounds_error() };

      return t;
    }

    [[nodiscard]]
    friend constexpr auto operator<=>(const throw_on_range_error&, const throw_on_range_error&) noexcept = default;
  protected:
    throw_on_range_error() = default;
    ~throw_on_range_error() = default;
  };

  /*template<class Policy, class T>
  constexpr bool has_check_for{
    requires (const T& t) {
      Policy::check(t);
    }
  };*/

 /* static_assert(has_check_for<throw_on_range_error, float>);
  static_assert(has_check_for<throw_on_range_error, double>);
  static_assert(!has_check_for<throw_on_range_error, int>);
  static_assert(!has_check_for<int, int>);*/

  template<std::floating_point T>
  class clamp_on_range_error
  {
  public:
    using value_type = T;

    [[nodiscard]]
    T get_error() const { return m_Error; }

    [[nodiscard]]
    friend constexpr auto operator<=>(const clamp_on_range_error&, const clamp_on_range_error&) noexcept = default;

    constexpr static T tolerance{1e-4f};
  protected:
    clamp_on_range_error() = default;

    clamp_on_range_error(const clamp_on_range_error&) = default;
    clamp_on_range_error& operator=(const clamp_on_range_error&) = default;

    clamp_on_range_error(clamp_on_range_error&&) noexcept = default;
    clamp_on_range_error& operator=(clamp_on_range_error&&) noexcept = default;

    ~clamp_on_range_error() = default;
  protected:
    [[nodiscard]]
    T check(T t)
    {
      if (t > T(1) + tolerance)
        throw std::out_of_range{ make_out_of_bounds_error() };

      auto p{ std::clamp(t, T{}, T{ 1 }) };
      m_Error = t - p;
      return p;
    }
  private:
    T m_Error;
  };

  namespace impl
  {
    template<class T>
    struct aggregator{};

    template<class T>
      requires (!std::is_fundamental_v<T>)
    struct aggregator<T> : T
    {
      aggregator() = default;
      using T::check;
    };
  }

  template<class Policy, class T>
  constexpr bool has_check_for{
       std::regular<impl::aggregator<Policy>>
    &&
       requires (impl::aggregator<Policy>& p, const T & t) {
         { p.check(t) } -> std::same_as<T>;
       }
  };

  static_assert(has_check_for<throw_on_range_error, float>);
  static_assert(has_check_for<clamp_on_range_error<float>, float>);
  static_assert(has_check_for<throw_on_range_error, double>);
  static_assert(!has_check_for<throw_on_range_error, int>);
  //static_assert(!has_check_for<std::string, int>);
  static_assert(!has_check_for<int, int>);

  static_assert(std::regular<impl::aggregator<clamp_on_range_error<float>>>);


  // class template
  template<std::floating_point T, class RangeErrorPolicy=throw_on_range_error>
      requires has_check_for<RangeErrorPolicy, T>
  class probability : public RangeErrorPolicy
  {
    T m_Prob{};
  public:
    probability() = default;

    // p must be in range [0,1]
    constexpr explicit probability(T p) : m_Prob{ this->check(p) }
    {}

    constexpr probability(const probability&) = default;

    constexpr probability& operator=(const probability&) = default;

    constexpr probability& operator+=(const probability& p)
    {
      probability q{ m_Prob + p.m_Prob };
      *this = q;

      return *this;
    }

    constexpr probability& operator/=(const probability& p)
    {
      probability q{ m_Prob / p.m_Prob };
      *this = q;

      return *this;
    }

    constexpr probability& operator*=(const probability& p)
    {
      probability q{ m_Prob * p.m_Prob };
      *this = q;

      return *this;
    }

    // operator-=, operator*=, operator/=

    [[nodiscard]]
    constexpr T raw_value() const noexcept
    {
      return m_Prob;
    }

    [[nodiscard]]
    friend constexpr probability operator+(probability lhs, probability rhs)
    {
      return lhs += rhs;
    }

    // three-way comparison / spaceship
    [[nodiscard]]
    friend constexpr auto operator<=>(const probability&, const probability&) noexcept = default;

    template<class Stream>
      requires is_deserializable_to<Stream, T>
    friend Stream& operator>>(Stream& s, probability& p)
    {
      T t{};
      s >> t;
      p = probability{t};

      return s;
    }

    template<class Stream>
      requires is_serializable_to<Stream, T>
    friend Stream& operator<<(Stream& s, probability& p)
    {
      return s << p.raw_value();
    }
  };

  // P(A) = P(A|B) * P(B) / P(B|A)

  template<std::floating_point T>
  [[nodiscard]]
  constexpr probability<T> bayes(probability<T> p_B, probability<T> p_AgB, probability<T> p_BgA)
  {
    return (p_B *= p_AgB) /= p_BgA;
  }
}