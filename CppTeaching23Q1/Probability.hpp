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
    probability(T p) : m_Prob{ p }
    {
      if ((m_Prob < 0) || (m_Prob > 1))
        throw std::logic_error{ make_out_of_bounds_error()};
    }

    probability(const probability&) = default;

    probability& operator=(const probability&) = default;

    probability& operator+=(const probability& p)
    {
      probability q{ m_Prob + p.m_Prob };
      *this = q;

      return *this;
    }

    probability& operator/=(const probability& p)
    {
      probability q{ m_Prob / p.m_Prob };
      *this = q;

      return *this;
    }

    // operator-=, operator*=, operator/=

    [[nodiscard]]
    T raw_value() const
    {
      return m_Prob;
    }

    [[nodiscard]]
    friend probability operator+(probability lhs, probability rhs)
    {
      return lhs += rhs;
    }

    // three-way comparison / spaceship
    [[nodiscard]]
    friend auto operator<=>(const probability&, const probability&) = default;

  private:
    T m_Prob{};
  };
}