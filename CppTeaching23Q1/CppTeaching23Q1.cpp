#include "Tennis.hpp"
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
#include <variant>
#include <optional>
#include <filesystem>
#include <any>
#include <iterator>
#include <numbers>
#include <source_location>

import mathematics;

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

  struct animal_actions
  {
    void operator()(const whale& a) const
    {
      a.vocalize();
    }

    void operator()(const dog& a) const
    {
      a.vocalize();
      a.walk();
    }
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


template<class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};

namespace tennis2
{

  class game 
  {
  };

  class tiebreak
  {
  };

  class set
  {
    std::vector<game> games;
    std::optional<tiebreak> tb;
  };
}

void file_access(const std::optional<std::filesystem::path>& p)
{
  if (p.has_value())
  {
    if (std::filesystem::exists(p.value()))
    {
      std::cout << "hello, file!\n";
    }
    else
    {
      throw std::runtime_error{ "File not found" };
    }
  }

  std::cout << "No file supplied\n";
}

namespace punning
{
  int foo(int* pi, float* pf)
  {
    *pi = 1;
    *pf = 0;

    return *pi;
  }
}


namespace temp
{

  struct shape
  {
    virtual ~shape() = default;

    virtual bool is_circle() const { return false; }
    virtual bool is_square() const { return false; }
  };

  struct circle : shape
  {
    double radius() { return {}; }

    bool is_circle() const override { return true; }
  };

  struct square : shape
  {
    bool is_square() const override { return true; }
  };
}

std::string plural(std::string_view word, std::source_location loc = std::source_location::current())
{
    return std::string{ word.empty() ? "" : std::string{word} += "s " }.append(std::to_string(loc.line()));
}

#define GET_SET(TYPE_NAME_, PROP_NAME_, VAR_NAME_)\
void Set##PROP_NAME_(const TYPE_NAME_ val) { VAR_NAME_ = val; }\
const TYPE_NAME_ Get##PROP_NAME_() const { return (const TYPE_NAME_)VAR_NAME_; }

class baz
{
    double m_Radius{};
public:
    GET_SET(double, Radius, m_Radius)
};

template<class T>
class counting_allocator
{
public:
  using value_type = T;

  counting_allocator() = default;

  T* allocate(std::size_t n)
  {
    if (n)
    {
      ++(*m_Count);
      return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    return nullptr;
  }

  void deallocate(T* p, std::size_t)
  {
    ::operator delete(p);
  }

  [[nodiscard]]
  int count() const noexcept { return *m_Count; }

#ifdef _MSC_VER
#if _ITERATOR_DEBUG_LEVEL != 0
  template<class U>
  explicit operator counting_allocator<U>() const
  {
    return counting_allocator<U>{m_Count};
  }
#endif
#endif

private:
  template<class U>
  friend class counting_allocator;

  explicit counting_allocator(std::shared_ptr<int> count) : m_Count{ count } {}

  std::shared_ptr<int> m_Count{std::make_shared<int>()};
};

namespace example
{
  struct baz
  {
    void swap(baz& rhs)
    {
      std::cout << "Member-swap!\n";
    }

    friend void swap(baz& lhs, baz& rhs)
    {
      std::cout << "User-defined swap\n";
      lhs.swap(rhs);
    }
  };
}

namespace algos
{
  template<class T>
  concept has_adl_swap = requires(T & lhs, T & rhs){
    { swap(lhs, rhs) };
  };

  struct swapper
  {
    template<class T>
      requires (!has_adl_swap<T>)
    void operator()(T& lhs, T& rhs) const
    {
      std::swap(lhs, rhs);
    }

    template<class T>
     requires has_adl_swap<T>
    void operator()(T& lhs, T& rhs) const
    {
      swap(lhs, rhs);
    }
  };

  constexpr swapper swap{};
}

int main()
{
    try
    {
      std::vector<int, counting_allocator<int>> v{};

      std::cout << v.get_allocator().count() << '\n';

      example::baz a{}, b{};

      algos::swap(a, b);
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