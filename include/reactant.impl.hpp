// reactant.impl.hpp
#pragma once
#include "reactant.hpp"
#include "reaction.hpp" // 👈 bring full definition of Reaction

template <std::integral T> Reaction Reactant::operator>>(T rate) const
{
    auto reaction = Reaction(*this);
    reaction >> rate;
    return reaction;
}

template <std::floating_point T> Reaction Reactant::operator>>(T rate) const
{
    auto reaction = Reaction(*this);
    reaction >> rate;
    return reaction;
}
