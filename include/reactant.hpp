#pragma once
#include "concepts.hpp"
#include <string>

class Reaction;

class Reactant
{
  public:
    Reactant(std::string reactantName, std::size_t quantity);
    Reactant(Reactant &&) = default;
    Reactant(const Reactant &) = default;
    Reactant &operator=(Reactant &&) = default;
    bool operator==(const Reactant &other) const;
    Reactant &operator=(const Reactant &) = default;
    template <RateVal T> Reaction operator>>(T rate) const;
    Reaction operator+(const Reactant &other) const;
    std::size_t quantity{0};
    std::string name;
    friend std::ostream &operator<<(std::ostream &os, const Reactant &r);
};

template <typename T>
concept IsReactant = std::is_same_v<std::remove_cvref_t<T>, Reactant>;
