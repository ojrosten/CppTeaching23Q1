#include "Tennis.hpp"

#include <iostream>
#include <vector>
#include <limits>

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

namespace maths
{
    class probability
    {
    public:
        probability() = default;

        // p must be in range [0,1]
        probability(double p) : m_Prob{ p }
        {
            if ((m_Prob < 0) || (m_Prob > 1))
                throw std::logic_error{"Invalid probability"};
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
        double raw_value() const
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
        double m_Prob{}, m_Error{};
    };

    /*probability operator+(probability lhs, probability rhs)
    {
        return lhs += rhs;
    }*/
}

int main()
{
    using namespace maths;

    try
    {
        probability p{ 0.5 }, q{ 0.2 }, r{ q }, s{p + q};

        p /= q;

        
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