#pragma once
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
    Reaction operator>>(std::size_t); // for creating a reaction with just one reactant
    Reaction operator+(const Reactant &other) const;
    ~Reactant();

  private:
    std::size_t quantity{0};
    std::string name;
};

template <typename T>
concept IsReactant = std::is_same_v<std::remove_cvref_t<T>, Reactant>;
