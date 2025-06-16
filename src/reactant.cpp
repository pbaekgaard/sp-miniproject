#include "reactant.hpp"
#include "reaction.hpp"
#include <ostream>

// Used for creating a reaction with 2 reactants // R1
Reaction Reactant::operator+(const Reactant &other) const
{
    return Reaction(*this, other);
}
//R1
bool Reactant::operator==(const Reactant &other) const
{
    return name == other.name && quantity == other.quantity;
}

Reactant::Reactant(std::string reactantName, std::size_t quantity)
{
    name = reactantName;
    this->quantity = quantity;
}

std::ostream &operator<<(std::ostream &os, const Reactant &r) // R2 + R1
{
    os << r.name << "(" << r.quantity << ")";
    return os;
}
