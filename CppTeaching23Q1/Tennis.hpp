#ifndef TENNIS_HPP
#define TENNIS_HPP

#include <string>
#include <stdexcept>
#include <vector>

namespace maths
{
  template<std::floating_point T>
  class probability;
}

namespace tennis
{
    //namespace tennis
    //{
    enum class score {
        love = 0,
        fifteen = 15,
        thirty = 30
    };


    // s must be sensible
    std::string to_string(score s);
    /*{
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

        throw std::logic_error{ "Illegal value!\n" };
    }*/
    //}

    class game
    {
        int m_PlayerA{}, m_PlayerB{};
    public:
        std::pair<score, score> get_score() const { return {}; }

        maths::probability<float>* m_Prob;
    };

    enum class player_designation { A, B};

    player_designation serve(maths::probability<float> p);
}
#endif