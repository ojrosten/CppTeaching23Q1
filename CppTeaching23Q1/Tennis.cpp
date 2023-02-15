#include "Tennis.hpp"

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
}