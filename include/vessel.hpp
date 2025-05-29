#pragma once
#include "environment.hpp"
#include "reaction.hpp"
#include <string>

class Vessel
{
  public:
    Vessel(std::string name);
    Vessel(Vessel &&) = default;
    Vessel(const Vessel &) = default;
    Vessel &operator=(Vessel &&) = default;
    Vessel &operator=(const Vessel &) = default;
    std::size_t add(std::string item, std::size_t rate); // Adds to the symbol table
    void add(Reaction reaction_specification);           // Adds to the symbol table
    Environment environment()
    {
        return Environment();
    };

  private:
    std::string name;
};
