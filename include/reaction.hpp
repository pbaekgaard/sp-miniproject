#pragma once
#include "reactant.hpp"
#include <concepts>
#include <vector>

class Environment;
class Reaction
{
  public:
    template <typename... Args>
        requires(IsReactant<Args> && ...)
    Reaction(Args &&...reactants)
    {
        (inputs.push_back(std::forward<Args>(reactants)), ...);
    }
    Reaction &operator=(Reaction &&) = default;
    Reaction(const Reaction &other) = default;
    bool operator==(const Reaction &other) const;
    Reaction &operator=(const Reaction &) = default;
    Reaction &operator+(const Reactant &r); // For chaining (A + B + C)
    Reaction &operator>>(size_t rate);
    Reaction &operator>>=(const Reaction &product); // For adding the product to the reaction
    Reaction &operator>>=(const Environment &env);  // For adding the product to the reaction
    std::vector<Reactant> inputs{};
    std::vector<Reaction> products{};

    size_t delay{0};
    size_t rate{0};
};
