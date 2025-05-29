#include "reaction.hpp"
#include "reactant.hpp"

// used for (A+B+C+D+E+F+G)

// Add a product to the reaction // R1
Reaction &Reaction::operator>>=(Reactant product)
{
    products.push_back(std::hash<std::string>{}("TODO: get name from product")); // Placeholder
    return *this;
}

// Add another reactant to the reaction // R1
Reaction &Reaction::operator+(const Reactant &r)
{
    inputs.push_back(r);
}

// Add a rate to the reaction // R1
void Reaction::operator>>(size_t rate)
{
    rate = rate;
}

template <typename... Args>
    requires(IsReactant<Args> && ...)
Reaction::Reaction(Args &&...reactants)
{
    (inputs.push_back(std::forward<Args>(reactants)), ...);
}
