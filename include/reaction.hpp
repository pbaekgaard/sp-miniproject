#pragma once
#include "reactant.hpp"
#include <vector>

class Reaction
{
  public:
    template <typename... Args> Reaction(Args &&...reactants);
    Reaction(Reaction &&) = default;
    Reaction(const Reaction &) = default;
    Reaction &operator=(Reaction &&) = default;
    Reaction &operator=(const Reaction &) = default;
    Reaction operator+(const Reactant &r);   // For chaining (A + B + C)
    Reaction &operator>>=(Reactant product); // For adding the product to the reaction
    ~Reaction();

  private:
    std::vector<Reactant> inputs{};
    std::vector<size_t> products{};
    size_t delay{0};
    size_t rate{0};
};
