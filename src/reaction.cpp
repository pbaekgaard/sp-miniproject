#include "reaction.hpp"
#include "environment.hpp"
#include "reactant.hpp"

// Add a product to the reaction // R1
Reaction &Reaction::operator>>=(const Reaction &product)
{
    products.emplace_back(product);
    return *this;
}

Reaction &Reaction::operator>>=(const Environment &env)
{
    products.emplace_back(env);
    return *this;
}

bool Reaction::operator==(const Reaction &other) const
{
    return products == other.products && inputs == other.inputs && delay == other.delay && rate == other.rate;
}

// used for (A+B+C+D+E+F+G)
// Add another reactant to the reaction // R1
Reaction &Reaction::operator+(const Reactant &r)
{
    inputs.push_back(r);
    return *this;
}

// Add a rate to the reaction // R1
Reaction &Reaction::operator>>(size_t reaction_rate)
{
    rate = reaction_rate;
    return *this;
}
