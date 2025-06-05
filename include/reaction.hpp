#pragma once
#include "reactant.hpp"
#include <concepts>
#include <functional>
#include <iostream>
#include <optional>
#include <ostream>
#include <vector>

class Reaction
{
  public:
    // Without increment
    template <typename... Args>
        requires(IsReactant<std::decay_t<Args>> && ...)
    Reaction(Args &&...reactants)
    {
        (inputs.push_back(std::forward<Args>(reactants)), ...);
    }

    // used for environments
    Reaction(int incrementAmount)
    {
        this->incrementAmount = incrementAmount;
        inputs = {};
        products = {};
    };

    Reaction &operator=(Reaction &&) = default;
    Reaction(const Reaction &other) = default;
    bool operator==(const Reaction &other) const;
    Reaction &operator=(const Reaction &) = default;
    Reaction &operator+(const Reactant &r);         // For chaining (A + B + C)
    Reaction &operator>>(size_t rate);              // For adding the rate of the reaction
    Reaction &operator>>=(const Reaction &product); // For adding the product to the reaction
    Reaction &operator>>=(const Reactant &product); // For adding the product to the reaction
    std::vector<Reaction>
        products{}; // Maybe this should be a symbol table? that is generic so we can use Environment in this
    std::vector<Reactant> inputs{}; // Wont hurt to do the same here

    size_t delay{0};
    size_t rate{0};
    int incrementAmount = 1;
    friend std::ostream &operator<<(std::ostream &os, const Reaction &r);
    std::string hash() const;
    void print() const
    {
        std::cout << *this << "\n";
    }
};
