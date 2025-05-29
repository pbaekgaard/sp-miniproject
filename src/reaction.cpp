#include "reaction.hpp"
#include "reactant.hpp"

// used for (A+B+C+D+E+F+G)
template <typename... Args> Reaction::Reaction(Args &&...reactants)
{
    (inputs.emplace_back(std::forward<Args>(reactants)), ...);
}

// Add a product
Reaction &Reaction::operator>>=(Reactant product)
{
    products.push_back(std::hash<std::string>{}("TODO: get name from product")); // Placeholder
    return *this;
}

// Add another reactant (member version)
Reaction Reaction::operator+(const Reactant &r)
{
    inputs.push_back(r);
}
