#pragma once

#include <vector>
#include <ranges>
#include <algorithm>

namespace containers
{
  template<class T>
  class set
  {
    using storage = std::vector<T>;
  public:
    using const_iterator = typename storage::const_iterator;

    set() = default;

    set(std::initializer_list<T> vals)
      : m_Vals{vals}
    {
      std::ranges::sort(m_Vals);
    }

    auto begin() const { return m_Vals.begin(); }

    auto end() const { return m_Vals.end(); }

    void erase(const_iterator it)
    {
      m_Vals.erase(it);
    }

    void insert(const T& t)
    {
      auto found{std::ranges::lower_bound(m_Vals, t)};
      if((found == end()) || (*found != t))
        m_Vals.insert(found, t);
    }
  private:
    storage m_Vals;
  };
}
