#include "Tennis.hpp"
#include "Probability.hpp"

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

int main()
{
    using namespace maths;

    try
    {
        probability<double> p{ 0.5 }, q{ 0.2 }, r{ q }, s{p + q};

        p /= q;

        probability<float> t{0.4f};

        std::cout << p.raw_value() << t.raw_value();
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