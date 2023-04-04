#pragma once

#include <stdexcept>
#include <concepts>
#include <iostream>
#include <algorithm>

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
  protected:
    throw_on_range_error() = default;
    ~throw_on_range_error() = default;
  };


  template<std::floating_point T>
  class clamp_on_range_error
  {
  public:
    using value_type = T;

    [[nodiscard]]
    T get_error() const { return m_Error; }
  protected:
    clamp_on_range_error() = default;

    clamp_on_range_error(const clamp_on_range_error&) = default;
    clamp_on_range_error& operator=(const clamp_on_range_error&) = default;

    clamp_on_range_error(clamp_on_range_error&&) noexcept = default;
    clamp_on_range_error& operator=(clamp_on_range_error&&) noexcept = default;

    ~clamp_on_range_error() = default;

    [[nodiscard]]
    T check(T t, T tol)
    {
      if (t > T(1) + tol)
        throw std::out_of_range{ make_out_of_bounds_error() };

      auto p{ std::clamp(t, T{}, T{ 1 }) };
      m_Error = t - p;
      return p;
    }
  private:
    T m_Error;
  };

  template<class Policy>
  constexpr bool is_range_error_policy{
    requires(Policy& policy){
      &policy.check;
    }
  };

  static_assert(is_range_error_policy<throw_on_range_error>);

  template<class T>
  struct is_floating_point
  {
    static constexpr bool value{ false };
  };

  template<>
  struct is_floating_point<float>
  {
    static constexpr bool value{ true };
  };

  template<>
  struct is_floating_point<double>
  {
    static constexpr bool value{ true };
  };

  static_assert(!is_floating_point<int>::value);
  static_assert(is_floating_point<float>::value);

  template<class T>
  concept floating_point = is_floating_point<T>::value;

  // class template
  template<floating_point T, class RangeErrorPolicy=throw_on_range_error>
  class probability : public RangeErrorPolicy
  {
    T m_Prob{};
  public:
    probability() = default;

    template<class... Args>
    // p must be in range [0,1]
    constexpr explicit(sizeof...(Args) == 1) probability(T p, Args&&... args) : m_Prob{ this->check(p, std::forward<Args>(args)...) }
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