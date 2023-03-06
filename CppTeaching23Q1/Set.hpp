#pragma once

#include <vector>
#include <ranges>
#include <algorithm>
#include <utility>

namespace containers
{
  template<class T>
  class set
  {
    using storage = std::vector<T>;
  public:
    using const_iterator = typename storage::const_iterator;

    struct insertion_result
    {
      const_iterator iter;
      bool is_inserted{};
    };


    set() = default;

    set(std::initializer_list<T> vals)
      : m_Vals{vals}
    {
      std::ranges::sort(m_Vals);
    }

    [[nodiscard]]
    const_iterator begin() const { return m_Vals.begin(); }

    [[nodiscard]]
    const_iterator end() const { return m_Vals.end(); }

    void erase(const_iterator it)
    {
      m_Vals.erase(it);
    }

    insertion_result insert(const T& t)
    {
      auto found{std::ranges::lower_bound(m_Vals, t)};
      if((found == end()) || (*found != t))
        return {m_Vals.insert(found, t), true};

      return {found, false};
    }
  private:
    storage m_Vals;
  };
}
