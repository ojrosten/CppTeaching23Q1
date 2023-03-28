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
#include <variant>


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

int main()
{
    try
    {
      std::cout << sizeof(double) << '\n' << sizeof(std::variant<double, float, int>) << '\n';


      using namespace life2;

      std::vector<std::variant<whale, dog>> animals{ { whale{} },    { dog{} }     };
      //                                             ^^ variant^^   ^^variant^^  
      //                                           ^^
      //                                          initializing the vector

      for (const auto& a : animals)
      {
        std::visit(
          overloaded{
            [](const whale& w){ w.vocalize(); },
            [](const dog& d){ d.walk(); }
          },
          a);
        //std::visit(animal_actions{}, a);
        //         ^^^ the vistor - defines the action performed for each type 
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