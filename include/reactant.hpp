#pragma once
#include <string>

class Reaction;

class Reactant
{
  public:
    Reactant();
    Reactant(Reactant &&) = default;
    Reactant(const Reactant &) = default;
    Reactant &operator=(Reactant &&) = default;
    Reactant &operator=(const Reactant &) = default;
    Reaction operator>>(size_t); // for creating a reaction with just one reactant
    Reaction operator+(const Reactant &other) const;
    ~Reactant();

  private:
    size_t quantity{0};
    std::string name;
};
