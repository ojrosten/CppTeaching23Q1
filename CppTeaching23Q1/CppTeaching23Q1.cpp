#include "Tennis.hpp"
#include "Probability.hpp"

#include <iostream>
#include <vector>
#include <limits>
#include <utility>

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

  ~wrapper()
  {
    std::cout << "Destructor\n";
    delete m_P;
  }

private:
  int* m_P{};
};

wrapper bar(int x) {
  wrapper w{x};
  std::cout << "inside bar\n";

  return w;
}

class foo
{
public:
  foo(int x) : m_W{x}
  {}
private:
  wrapper m_W;
};

int main()
{

    try
    {
      wrapper y{bar(7)};
    }
    catch (const std::logic_error& e)
    {
        std::cout << e.what();
    }
    catch (...)
    {
        std::cout << "Unrecognized exception\n";
    }

}