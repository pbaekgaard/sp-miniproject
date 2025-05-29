#include "reactant.hpp"
#include "reaction.hpp"

Reaction Reactant::operator>>(size_t rate)
{
    return Reaction(*this, rate);
}

Reaction Reactant::operator+(const Reactant &other) const
{
    return Reaction(*this, other);
}
