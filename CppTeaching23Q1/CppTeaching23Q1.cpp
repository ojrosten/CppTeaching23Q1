#include "Tennis.hpp"
#include "Probability.hpp"

#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <array>
#include <ranges>

namespace rugby
{
    enum class score {
        rugby_try = 5,
        conversion = 2,
        penalty = 3
    };
}

enum emotions { love};


enum class gl_state { none=0, depth_test=1, stencil_test=2, face_culling=4};

[[nodiscard]]
gl_state operator|(gl_state lhs, gl_state rhs)
{
    return gl_state{static_cast<int>(lhs) | static_cast<int>(rhs)};
}


//
// 001
// 010
// 011
// 100

// RAII
// Resource Acquisition Is Initialization
class wrapper
{
public:
  wrapper() = default;

  wrapper(int x)
    : m_P{new int{x}}
  {
    std::cout << "Constructor\n";
  }

  wrapper(wrapper const&) = delete;

  /*wrapper(wrapper const& other)
    : m_P{new int{*(other.m_P)}}
  {
    std::cout << "Copy Constructor\n";
  }*/

  wrapper(wrapper&& other) noexcept
    : m_P{std::exchange(other.m_P, nullptr)}
  {
    std::cout << "Move Constructor\n";
  }

  wrapper& operator=(wrapper const&) = delete;

  wrapper& operator=(wrapper&& other) noexcept
  {
    if(&other != this)
    {
      m_P = std::exchange(other.m_P, nullptr);
    }
    return *this;
  }

  ~wrapper()
  {
    std::cout << "Destructor\n";
    delete m_P;
  }

  [[nodiscard]]
  int value() const {
    if(m_P == nullptr)
      throw std::runtime_error{"null pointer!"};

    return *m_P;
  }

private:
  int* m_P{};
};

wrapper bar(int x) {
  wrapper w{x};
  return w;
}

class foo
{
public:
  foo(int x, int y)
  {}
private:
};

template<class T>
void swap(T& lhs, T& rhs)
{
  auto temp{std::move(lhs)}; // Construction
  lhs = std::move(rhs);      // assignment
  rhs = std::move(temp);     // assignment
}

template<class T>
constexpr bool has_empty_fn = requires(const T& t){
  t.empty();
};


int main()
{

    try
    {


      int x[8]{0,0,1,2,60,62, 27, 14};

      /*for(int i{}; i < 8; ++i)
      {
        std::cout << x[i] << '\n';
      }*/

      //int* begin{x};// , * end{begin + 8};
      //for(; begin != begin + 8; ++begin)
      //{
      //  std::cout << *begin << '\n';
      //}

      std::vector<int> y{0,0,1,2,60,62, 27, 14};

      /*for(int i{}; i < y.size(); ++i)
      {
        std::cout << y[i] << '\n';
      }*/


      //using it = std::array<int, 9>::const_iterator;
      for(auto i{y.begin()}; i != y.end(); ++i)
      {
        std::cout << *i << '\n';
      }


      for(auto e : y)
      {
        std::cout << e << '\n';
      }

      std::cout << "The size of my container is: " << y.size() << '\n';
;    }
    catch (const std::logic_error& e)
    {
        std::cout << e.what();
    }
    catch(const std::runtime_error& e)
    {
      std::cout << e.what();
    }
    catch (...)
    {
        std::cout << "Unrecognized exception\n";
    }

}