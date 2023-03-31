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
    T check(T t)
    {
      auto p{ std::clamp(t, T{}, T{ 1 }) };
      m_Error = t - p;
      return p;
    }

    T get_error() const { return m_Error; }
  protected:
    ~clamp_on_range_error() = default;
  private:
    T m_Error;
  };

  // class template
  template<std::floating_point T, class RangeErrorPolicy=throw_on_range_error>
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