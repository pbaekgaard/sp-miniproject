#pragma once
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
    ~Vessel();
    size_t add(std::string item, size_t rate); // Adds to the symbol table
    void add(Reaction reaction_specification); // Adds to the symbol table
    size_t environment();

  private:
    std::string name;
};
