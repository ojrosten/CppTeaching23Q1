#include "Tennis.hpp"
#include "Probability.hpp"
#include "Set.hpp"

#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <array>
#include <ranges>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>
#include <functional>
#include <format>
#include <sstream>

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

auto pred(int e) -> bool { return e > 60; }

struct moon_pred
{
private:
  int maxMoons{};
public:

  moon_pred(int m) : maxMoons{m} {};

  bool operator() (int e) const { return e > maxMoons; }
};

namespace life
{
  class animal
  {
  public:
    virtual ~animal() = default;

    virtual void vocalize() = 0;

    virtual bool is_dog() const { return false; }
    virtual bool is_cat() const { return false; }
    virtual bool is_whale() const { return false; }
  };


  class dog : public animal
  {
  public:
    void walk()
    {
      std::cout << "Dog walking\n";
    }

    void vocalize() override
    {
      std::cout << "Dog noises\n";
    }

    bool is_dog() const override { return true; }
  };

  class cat : public animal
  {
  public:
    void walk()
    {
      std::cout << "Cat walking\n";
    }

    void vocalize() override
    {
      std::cout << "Cat noises\n";
    }

    bool is_cat() const override { return true; }
  };

  class whale final : public animal
  {
    std::vector<int> m_Bones;
  public:

    void vocalize() override
    {
      std::cout << "Whale noises\n";
    }

    bool is_whale() const final { return true; }
  };

  class factory
  {
  public:
    using product = std::unique_ptr<animal>;
    using map_elements = std::pair<const std::string, std::function<product()>>;

    factory(std::initializer_list<map_elements> init) 
      : m_Map{init}
    { }

    product make(const std::string& name) const
    {
      auto found{m_Map.find(name)};
      if(found == m_Map.end()) throw std::runtime_error{"Type not found"};

      return found->second();
    }
  private:
    std::map<std::string, std::function<product()>> m_Map;
  };
}

namespace life2
{
  class whale
  {
  public:
    void vocalize() const
    {
      std::cout << "Whale noises\n";
    }
  };

  class dog
  {
  public:
    void walk() const
    {
      std::cout << "Dog walking\n";
    }

    void vocalize() const
    {
      std::cout << "Dog noises\n";
    }
  };

  template<class T>
  constexpr bool can_vocalize{
    requires(const T& t) { t.vocalize(); }
  };

  template<class T>
  concept is_animal = can_vocalize<T>;

  class animal
  {
  public:
    template<is_animal T>
    animal(T t)
      : m_pActualAnimal{std::make_unique<essence<T>>(t)}
    {}

    animal(const animal& other)
      : m_pActualAnimal{ other.m_pActualAnimal->clone() }
    {}

    animal(animal&&) noexcept = default;

    void vocalize() const
    {
      if (!m_pActualAnimal)
        throw std::runtime_error{"Null pointer! using moved-from object"};

      m_pActualAnimal->vocalize();
    }

    template<class T>
    const T* animal_cast()
    {
      return dynamic_cast<const T*>(m_pActualAnimal.get());
    }
  private:
    struct soul
    {
      virtual ~soul() = default;

      virtual std::unique_ptr<soul> clone() = 0;

      virtual void vocalize() const = 0;
    };

    template<class T>
    struct essence final : soul
    {
      essence(T t) : m_ActualAnimal{ t } {}

      void vocalize() const final
      {
        m_ActualAnimal.vocalize();
      }

      std::unique_ptr<soul> clone()
      {
        return std::make_unique<essence<T>>(m_ActualAnimal);
      }

      T m_ActualAnimal;
    };

    std::unique_ptr<soul> m_pActualAnimal{};
  };
}

template<class R, class... Args>
class function
{
public:
  template<class T>
  function(T t)
    : m_pInvocable{std::make_unique<essence<T>>(t)}
  {
  }

  R operator()(Args... args) const
  {
    return (*m_pInvocable)(args...);
  }

  // Feed it functions, or function objects (lambdas)
private:
  struct soul
  {
    virtual ~soul() = default;

    virtual R operator()(Args...) const = 0;
  };

  template<class T>
  struct essence final : soul
  {
    essence(T t) : m_Val{t} {}

    R operator()(Args... args) const final
    {
      return m_Val(args...);
    }

    T m_Val;
  };


  std::unique_ptr<soul> m_pInvocable;
};

int main()
{
    try
    {
      using namespace life2;

      std::vector<animal> v{ whale{}, dog{}, whale{}, whale{} };

      for (auto& a : v)
      {
        a.vocalize();
      }
    }
    catch(const std::out_of_range& e)
    {
      std::cout << "Out of range Error: " << e.what();
    }
    catch (const std::logic_error& e)
    {
        std::cout << "Logic Error: " << e.what();
    }
    catch(const std::runtime_error& e)
    {
      std::cout << e.what();
    }
    catch(const std::exception& e)
    {
      std::cout << "Exception Error: " << e.what();
    }
    catch (...)
    {
        std::cout << "Unrecognized exception\n";
    }

}