#pragma once

#include <stdexcept>
#include <concepts>

namespace maths
{
  [[nodiscard]]
  std::string make_out_of_bounds_error();

  // class template
  template<std::floating_point T>
  class probability
  {
  public:
    probability() = default;

    // p must be in range [0,1]
    constexpr explicit probability(T p) : m_Prob{ p }
    {
      if ((m_Prob < 0) || (m_Prob > 1))
        throw std::logic_error{ make_out_of_bounds_error()};
    }

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

  private:
    T m_Prob{};
  };

  // P(A) = P(A|B) * P(B) / P(B|A)

  template<std::floating_point T>
  [[nodiscard]]
  constexpr probability<T> bayes(probability<T> p_B, probability<T> p_AgB, probability<T> p_BgA)
  {
    return (p_B *= p_AgB) /= p_BgA;
  }
}