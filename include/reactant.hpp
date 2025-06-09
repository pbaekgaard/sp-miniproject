#pragma once
#include <string>

class Reaction; // still needed for declaration

class Reactant
{
  public:
    Reactant(std::string reactantName, std::size_t quantity);
    Reactant(Reactant &&) = default;
    Reactant(const Reactant &) = default;
    Reactant &operator=(Reactant &&) = default;
    bool operator==(const Reactant &other) const;
    Reactant &operator=(const Reactant &) = default;

    // Only declarations here
    template <std::floating_point T> Reaction operator>>(T rate) const;
    template <std::integral T> Reaction operator>>(T rate) const;

    Reaction operator+(const Reactant &other) const;
    std::size_t quantity{0};
    std::string name;

    friend std::ostream &operator<<(std::ostream &os, const Reactant &r);
};

template <typename T>
concept IsReactant = std::is_same_v<std::remove_cvref_t<T>, Reactant>;

// NO: #include "reactant.impl.hpp"
