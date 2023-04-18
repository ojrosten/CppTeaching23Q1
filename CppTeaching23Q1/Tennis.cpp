#include "Tennis.hpp"

#include "Probability.hpp"

#include <random>

namespace tennis
{
    std::string to_string(score s)
    {
        using enum score;
        switch (s)
        {
        case love:
            return "love";
        case fifteen:
            return "fifteen";
        case thirty:
            return "thirty";
        }

        throw std::logic_error{ "Illegal value!" };
    }

    player_designation serve(maths::probability<float> p)
    {
      std::random_device rd{};
      std::mt19937 gen{rd()};
      std::uniform_real_distribution<float> dist;

      return dist(gen) < p.raw_value() ? player_designation::A : player_designation::B;
    }
}