#include "reaction.hpp"
#include "hash.hpp"
#include "reactant.hpp"
#include <functional>
#include <iomanip>
#include <sstream>

// used for (A+B+C+D+E+F+G)

// Add a product to the reaction // R1
Reaction &Reaction::operator>>=(const Reaction &product)
{
    // If product has multiple inputs and no sub-products, split them:
    if (!product.inputs.empty() && product.products.empty())
    {
        for (const auto &reactant : product.inputs)
        {
            products.emplace_back(Reaction(reactant)); // Add each as a separate product Reaction
        }
    }
    else
    {
        // Otherwise just add the whole product reaction as before
        products.emplace_back(product);
    }
    return *this;
}
//R1
Reaction &Reaction::operator>>=(const Reactant &product)
{
    products.emplace_back(Reaction(product));
    return *this;
}
//R1
bool Reaction::operator==(const Reaction &other) const
{
    return products == other.products && inputs == other.inputs && delay == other.delay && rate == other.rate;
}

// Add another reactant to the reaction // R1
Reaction &Reaction::operator+(const Reactant &r)
{
    inputs.push_back(r);
    return *this;
}

// R2 + R1
std::ostream &operator<<(std::ostream &os, const Reaction &reaction)
{
    os << "Inputs: ";
    for (size_t i = 0; i < reaction.inputs.size(); ++i)
    {
        os << reaction.inputs[i];
        if (i + 1 < reaction.inputs.size())
            os << " + ";
    }

    os << "\nProducts: ";
    for (size_t i = 0; i < reaction.products.size(); ++i)
    {
        const auto &prod = reaction.products[i];

        // If product has no inputs, treat it as env
        if (prod.inputs.empty())
        {
            os << "[env]{" << prod.incrementAmount << "}";
        }
        else
        {
            for (size_t j = 0; j < prod.inputs.size(); ++j)
            {
                os << prod.inputs[j];
                if (j + 1 < prod.inputs.size())
                    os << " + ";
            }
            os << "{" << prod.incrementAmount << "}";
        }

        if (i + 1 < reaction.products.size())
            os << " + ";
    }

    os << "\nRate: " << reaction.rate;
    os << "\nDelay: " << reaction.delay;
    return os;
}
std::string Reaction::hash() const
{
    size_t raw_hash = std::hash<Reaction>{}(*this);
    std::stringstream ss;
    ss << std::hex << std::setw(sizeof(size_t) * 2) << std::setfill('0') << raw_hash;
    return ss.str();
}
