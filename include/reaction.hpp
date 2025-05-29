#pragma once
#include "reactant.hpp"
#include <concepts>
#include <vector>

class Reaction
{
  public:
    template <typename... Args>
        requires(IsReactant<Args> && ...)
    Reaction(Args &&...reactants);

    Reaction &operator=(Reaction &&) = default;
    Reaction(const Reaction &other) = default;
    Reaction &operator=(const Reaction &) = default;
    Reaction &operator+(const Reactant &r); // For chaining (A + B + C)
    void operator>>(size_t rate);
    Reaction &operator>>=(Reactant product); // For adding the product to the reaction
    ~Reaction();
    void set_rate(size_t rate);
    std::vector<Reactant> inputs{};
    std::vector<size_t> products{};

  private:
    size_t delay{0};
    size_t rate{0};
};
